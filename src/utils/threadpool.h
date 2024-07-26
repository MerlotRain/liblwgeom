/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/


#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

typedef struct _intthreadpool se_intthreadpool;

/* create thread pool
 * min: min thread count
 * max: max thread count
 * queue_size: task queue size
 */
se_intthreadpool *create_threadpool(int min, int max, int queue_size);

/* destroy thread pool */
int destroy_threadpool(se_intthreadpool *pool);

/* add task to thread pool */
void threadpool_add_task(se_intthreadpool *pool, void (*func)(void *), void *arg);

/* exit thread pool */
void threadpool_exit(se_intthreadpool *pool);

/* get busy count */
int threadpool_busy_count(se_intthreadpool *pool);

/* get alive count */
int threadpool_alive_count(se_intthreadpool *pool);

#endif