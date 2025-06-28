#include "thread_pool.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define THREADPOOL_FULL -2
#define THREADPOOL_ERR -1
#define THREADPOOL_OK 0

static void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;

    for(;;) {
        pthread_mutex_lock(&(pool->lock));

        // Wait for the task or close the notification
        while(pool->count == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        // Close immediately
        if(pool->shutdown == 1) {
            break;
        }

        // Close after completing the remaining tasks
        if(pool->shutdown == 2 && pool->count == 0) {
            break;
        }

        // Obtain the task
        task.function = pool->queue[pool->head].function;
        task.argument = pool->queue[pool->head].argument;
        pool->head = (pool->head + 1) % pool->queue_size;
        pool->count -= 1;

        pthread_mutex_unlock(&(pool->lock));

        // run the task
        (*(task.function))(task.argument);
    }

    pool->thread_count--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return NULL;
}

int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument)
{
    if(pool == NULL || function == NULL) {
        return THREADPOOL_ERR;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return THREADPOOL_ERR;
    }

    int next = (pool->tail + 1) % pool->queue_size;
    
    // queue is full
    if(pool->count == pool->queue_size) {
        pthread_mutex_unlock(&(pool->lock));
        return THREADPOOL_FULL;
    }

    // closing
    if(pool->shutdown) {
        pthread_mutex_unlock(&(pool->lock));
        return THREADPOOL_ERR;
    }

    // add task to queue
    pool->queue[pool->tail].function = function;
    pool->queue[pool->tail].argument = argument;
    pool->tail = next;
    pool->count += 1;

    // Notify a waiting queue
    if(pthread_cond_signal(&(pool->notify)) != 0) {
        pthread_mutex_unlock(&(pool->lock));
        return THREADPOOL_ERR;
    }

    pthread_mutex_unlock(&(pool->lock));
    return THREADPOOL_OK;
}

threadpool_t *threadpool_create(int thread_count, int queue_size)
{
    if(thread_count <= 0 || queue_size <= 0) {
        return NULL;
    }

    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
    if(pool == NULL) {
        return NULL;
    }

    // init
    pool->thread_count = 0;
    pool->queue_size = queue_size;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = 0;

    // alloc memory for thread and queue.
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    pool->queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_size);

    // init mutex and cond
    if(pthread_mutex_init(&(pool->lock), NULL) != 0 ||
       pthread_cond_init(&(pool->notify), NULL) != 0 ||
       pool->threads == NULL || pool->queue == NULL) {
        if(pool) {
            threadpool_destroy(pool, 0);
        }
        return NULL;
    }

    // create work thread
    for(int i = 0; i < thread_count; i++) {
        if(pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void*)pool) != 0) {
            threadpool_destroy(pool, 0);
            return NULL;
        }
        pool->thread_count++;
    }

    return pool;
}

int threadpool_destroy(threadpool_t *pool, int flags)
{
    if(pool == NULL) {
        return THREADPOOL_ERR;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return THREADPOOL_ERR;
    }

    int err = 0;

    // closed
    if(pool->shutdown) {
        err = THREADPOOL_ERR;
    } else {
        pool->shutdown = (flags & 1) ? 1 : 2;

        // Wake up all threads
        if((pthread_cond_broadcast(&(pool->notify)) != 0) ||
           (pthread_mutex_unlock(&(pool->lock)) != 0)) {
            err = THREADPOOL_ERR;
        }

        // Wait for all threads to complete
        for(int i = 0; i < pool->thread_count; i++) {
            if(pthread_join(pool->threads[i], NULL) != 0) {
                err = THREADPOOL_ERR;
            }
        }
    }

    // release resource
    if(!err) {
        if(pool->threads) {
            free(pool->threads);
            pthread_mutex_destroy(&(pool->lock));
            pthread_cond_destroy(&(pool->notify));
        }
        if(pool->queue) {
            free(pool->queue);
        }
        free(pool);
    }
    
    return err;
}

#if 0
void example_task(void *arg) {
    int *num = (int *)arg;
    printf("Task %d started\n", *num);
    sleep(1);
    printf("Task %d finished\n", *num);
}

int main() {
    threadpool_t *pool = threadpool_create(4, 100);
    if(pool == NULL) {
        printf("Failed to create thread pool\n");
        return 1;
    }

    int task_args[20];
    for(int i = 0; i < 20; i++) {
        task_args[i] = i;
        if(threadpool_add(pool, example_task, (void *)&task_args[i]) != 0) {
            printf("Failed to add task %d\n", i);
        }
    }

    sleep(5);

    if(threadpool_destroy(pool, 0) != 0) {
        printf("Failed to destroy thread pool\n");
    }

    return 0;
}
#endif