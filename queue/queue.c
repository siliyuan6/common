#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* queue_create(int capacity)
{
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    
    queue->items = (void**)malloc(sizeof(void*) * capacity);
    if (queue->items == NULL) {
        free(queue);
        return NULL;
    }
    
    queue->front = 0;
    queue->rear = -1;
    queue->capacity = capacity;
    queue->size = 0;
    
    return queue;
}

void queue_destroy(Queue *queue)
{
    if (queue != NULL) {
        free(queue->items);
        free(queue);
    }
}

bool queue_enqueue(Queue *queue, void *item)
{
    if (queue_is_full(queue)) {
        return false;
    }
    
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->items[queue->rear] = item;
    queue->size++;
    
    return true;
}

void* queue_dequeue(Queue *queue)
{
    if (queue_is_empty(queue)) {
        return NULL;
    }
    
    void *item = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    return item;
}

void* queue_peek(Queue *queue)
{
    if (queue_is_empty(queue)) {
        return NULL;
    }
    
    return queue->items[queue->front];
}
bool queue_is_empty(Queue *queue)
{
    return queue->size == 0;
}

bool queue_is_full(Queue *queue)
{
    return queue->size == queue->capacity;
}

int queue_size(Queue *queue)
{
    return queue->size;
}