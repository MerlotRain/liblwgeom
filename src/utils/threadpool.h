#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct se_thread_pool thread_pool_t;

/* create thread pool
 * min: min thread count
 * max: max thread count
 * queue_size: task queue size
 */
thread_pool_t *create_thread_pool(int min, int max, int queue_size);

/* destroy thread pool */
int destroy_thread_pool(thread_pool_t *pool);

/* add task to thread pool */
void thread_pool_add_task(thread_pool_t *pool, void (*func)(void *), void *arg);

/* exit thread pool */
void thread_pool_exit(thread_pool_t *pool);

/* get busy count */
int thread_pool_busy_count(thread_pool_t *pool);

/* get alive count */
int thread_pool_alive_count(thread_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif //__THREADPOOL_H__