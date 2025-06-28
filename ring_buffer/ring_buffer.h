#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    void *buffer;       // 队列存储区
    size_t capacity;    // 队列总容量(字节)
    size_t head;        // 头部位置(字节偏移)
    size_t tail;        // 尾部位置(字节偏移)
    bool is_full;       // 队列是否已满标志
} ring_buffer;

ring_buffer* ring_buffer_create(size_t capacity);
void ring_buffer_destroy(ring_buffer *queue);
bool ring_buffer_is_empty(const ring_buffer *queue);
bool ring_buffer_is_full(const ring_buffer *queue);
size_t ring_buffer_available(const ring_buffer *queue);
bool ring_buffer_enqueue(ring_buffer *queue, const void *data, size_t data_len);
bool ring_buffer_dequeue(ring_buffer *queue, void *data, size_t *data_len);

#ifdef __cplusplus
}
#endif

#endif // RING_BUFFER_H