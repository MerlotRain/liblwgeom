/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "threadpool.h"
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int THREAD_POOL_NUMBER = 2;

struct _singletask
{
  void (*function) (void *arg);
  void *arg;
};

struct _intthreadpool
{
  struct _singletask *task_queue; /* task queue */
  int queue_capacity;             /* task queue capacity */
  int queue_size;                 /* task queue size */
  int queue_front;                /* task queue front */
  int queue_rear;                 /* task queue rear */
  pthread_t manager_id;           /* manager thread id */
  pthread_t *thread_ids;          /* task thread ids */
  int min_num;                    /* min thread num */
  int max_num;                    /* max thread num */
  int busy_num;                   /* busy thread num */
  int live_num;                   /* live thread num */
  int exit_num;                   /* destroy thread num */
  pthread_mutex_t mutex_pool;     /* lock mutex */
  pthread_mutex_t mutex_busy;     /* lock busy_num */
  pthread_cond_t not_full;        /* Check if the task queue is full */
  pthread_cond_t not_empty;       /* Check if the task queue is empty */

  int shutdown; /* Whether to destroy the thread pool, 1: destroy. 0: Do not
                   destroy*/
};

static void *
worker (void *arg)
{
  se_intthreadpool *pool = (se_intthreadpool *)arg;

  while (1)
    {
      pthread_mutex_lock (&pool->mutex_pool);
      while (pool->queue_size == 0 && !pool->shutdown)
        {
          pthread_cond_wait (&pool->not_empty, &pool->mutex_pool);
          if (pool->exit_num > 0)
            {
              pool->exit_num--;
              if (pool->live_num > pool->min_num)
                {
                  pool->live_num--;
                  pthread_mutex_unlock (&pool->mutex_pool);
                  thread_pool_exit (pool);
                }
            }
        }

      if (pool->shutdown)
        {
          pthread_mutex_unlock (&pool->mutex_pool);
          thread_pool_exit (pool);
        }

      struct _singletask task;
      task.function = pool->task_queue[pool->queue_front].function;
      task.arg = pool->task_queue[pool->queue_front].arg;

      pool->queue_front = (pool->queue_front + 1) % pool->queue_capacity;
      pool->queue_size--;

      pthread_cond_signal (&pool->not_full);
      pthread_mutex_unlock (&pool->mutex_pool);

      pthread_mutex_lock (&pool->mutex_busy);
      pool->busy_num++;
      pthread_mutex_unlock (&pool->mutex_busy);
      task.function (task.arg);
      free (task.arg);
      task.arg = NULL;

      pthread_mutex_lock (&pool->mutex_busy);
      pool->busy_num--;
      pthread_mutex_unlock (&pool->mutex_busy);
    }
  return NULL;
}

static void *
manager (void *arg)
{
  se_intthreadpool *pool = (se_intthreadpool *)arg;
  while (!pool->shutdown)
    {
      sleep (3);

      pthread_mutex_lock (&pool->mutex_pool);
      int queueSize = pool->queue_size;
      int liveNum = pool->live_num;
      pthread_mutex_unlock (&pool->mutex_pool);

      pthread_mutex_lock (&pool->mutex_busy);
      int busyNum = pool->busy_num;
      pthread_mutex_unlock (&pool->mutex_busy);

      if (queueSize > liveNum && liveNum < pool->max_num)
        {
          pthread_mutex_lock (&pool->mutex_pool);
          int counter = 0;
          for (int i = 0; i < pool->max_num && counter < THREAD_POOL_NUMBER
                          && pool->live_num < pool->max_num;
               ++i)
            {
              if (pool->thread_ids[i] == 0)
                {
                  pthread_create (&pool->thread_ids[i], NULL, worker, pool);
                  counter++;
                  pool->live_num++;
                }
            }
          pthread_mutex_unlock (&pool->mutex_pool);
        }

      if (busyNum * 2 < liveNum && liveNum > pool->min_num)
        {
          pthread_mutex_lock (&pool->mutex_pool);
          pool->exit_num = THREAD_POOL_NUMBER;
          pthread_mutex_unlock (&pool->mutex_pool);
          for (int i = 0; i < THREAD_POOL_NUMBER; ++i)
            {
              pthread_cond_signal (&pool->not_empty);
            }
        }
    }
  return NULL;
}

se_intthreadpool *
create_threadpool (int min, int max, int queueSize)
{
  se_intthreadpool *pool
      = (se_intthreadpool *)malloc (sizeof (se_intthreadpool));
  do
    {
      if (pool == NULL)
        {
          printf ("malloc se_intthreadpool fail...\n");
          break;
        }

      pool->thread_ids = (pthread_t *)malloc (sizeof (pthread_t) * max);
      if (pool->thread_ids == NULL)
        {
          printf ("malloc thread_ids fail...\n");
          break;
        }
      memset (pool->thread_ids, 0, sizeof (pthread_t) * max);
      pool->min_num = min;
      pool->max_num = max;
      pool->busy_num = 0;
      pool->live_num = min;
      pool->exit_num = 0;

      if (pthread_mutex_init (&pool->mutex_pool, NULL) != 0
          || pthread_mutex_init (&pool->mutex_busy, NULL) != 0
          || pthread_cond_init (&pool->not_empty, NULL) != 0
          || pthread_cond_init (&pool->not_full, NULL) != 0)
        {
          printf ("mutex or condition init fail...\n");
          break;
        }

      pool->task_queue = (struct _singletask *)malloc (
          sizeof (struct _singletask) * queueSize);
      pool->queue_capacity = queueSize;
      pool->queue_size = 0;
      pool->queue_front = 0;
      pool->queue_rear = 0;

      pool->shutdown = 0;
      pthread_create (&pool->manager_id, NULL, manager, pool);
      for (int i = 0; i < min; ++i)
        {
          pthread_create (&pool->thread_ids[i], NULL, worker, pool);
        }
      return pool;
    }
  while (0);

  if (pool && pool->thread_ids)
    free (pool->thread_ids);
  if (pool && pool->task_queue)
    free (pool->task_queue);
  if (pool)
    free (pool);

  return NULL;
}

int
destroy_threadpool (se_intthreadpool *pool)
{
  if (pool == NULL)
    {
      return -1;
    }

  pool->shutdown = 1;

  pthread_join (pool->manager_id, NULL);
  for (int i = 0; i < pool->live_num; ++i)
    {
      pthread_cond_signal (&pool->not_empty);
    }

  if (pool->task_queue)
    {
      free (pool->task_queue);
    }
  if (pool->thread_ids)
    {
      free (pool->thread_ids);
    }

  pthread_mutex_destroy (&pool->mutex_pool);
  pthread_mutex_destroy (&pool->mutex_busy);
  pthread_cond_destroy (&pool->not_empty);
  pthread_cond_destroy (&pool->not_full);

  free (pool);
  pool = NULL;

  return 0;
}

void
threadpool_add_task (se_intthreadpool *pool, void (*func) (void *), void *arg)
{
  pthread_mutex_lock (&pool->mutex_pool);
  while (pool->queue_size == pool->queue_capacity && !pool->shutdown)
    {
      pthread_cond_wait (&pool->not_full, &pool->mutex_pool);
    }
  if (pool->shutdown)
    {
      pthread_mutex_unlock (&pool->mutex_pool);
      return;
    }

  pool->task_queue[pool->queue_rear].function = func;
  pool->task_queue[pool->queue_rear].arg = arg;
  pool->queue_rear = (pool->queue_rear + 1) % pool->queue_capacity;
  pool->queue_size++;

  pthread_cond_signal (&pool->not_empty);
  pthread_mutex_unlock (&pool->mutex_pool);
}

int
threadpool_busy_count (se_intthreadpool *pool)
{
  pthread_mutex_lock (&pool->mutex_busy);
  int busyNum = pool->busy_num;
  pthread_mutex_unlock (&pool->mutex_busy);
  return busyNum;
}

int
threadpool_alive_count (se_intthreadpool *pool)
{
  pthread_mutex_lock (&pool->mutex_pool);
  int aliveNum = pool->live_num;
  pthread_mutex_unlock (&pool->mutex_pool);
  return aliveNum;
}

void
threadpool_exit (se_intthreadpool *pool)
{
  pthread_t tid = pthread_self ();
  for (int i = 0; i < pool->max_num; ++i)
    {
      if (pool->thread_ids[i] == tid)
        {
          pool->thread_ids[i] = 0;
          break;
        }
    }
  pthread_exit (NULL);
}