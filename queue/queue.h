#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>

// Queue structure definition
typedef struct Queue {
    void **items;      // Array to store queue elements (using void* for generic type)
    int front;         // Index of the front element
    int rear;          // Index of the rear element
    int capacity;      // Maximum capacity of the queue
    int size;          // Current number of elements in the queue
} Queue;

/* Function declarations */
Queue* queue_create(int capacity);
void queue_destroy(Queue *queue);
bool queue_enqueue(Queue *queue, void *item);
void* queue_dequeue(Queue *queue);
void* queue_peek(Queue *queue);
bool queue_is_empty(Queue *queue);
bool queue_is_full(Queue *queue);
int queue_size(Queue *queue);

#ifdef __cplusplus
}
#endif

#endif // QUEUE_H