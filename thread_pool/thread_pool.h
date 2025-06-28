#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    void (*function)(void *);
    void *argument;
} threadpool_task_t;

typedef struct {
    pthread_mutex_t lock;     // Mutex for thread synchronization
    pthread_cond_t notify;    // Condition variable for task notification
    pthread_t *threads;       // Array of worker threads
    threadpool_task_t *queue; // Task queue (circular buffer)
    
    int thread_count;         // Number of worker threads
    int queue_size;           // Maximum capacity of task queue
    int head;                 // Index of first element in queue
    int tail;                 // Index of next available slot in queue
    int count;                // Current number of pending tasks
    int shutdown;             // Flag indicating shutdown status:
                              // 0 = running, 1 = immediate shutdown, 
                              // 2 = graceful shutdown
} threadpool_t;

threadpool_t *threadpool_create(int thread_count, int queue_size);
int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument);
int threadpool_destroy(threadpool_t *pool, int flags);

#ifdef __cplusplus
}
#endif

#endif // THREAD_POOL_H