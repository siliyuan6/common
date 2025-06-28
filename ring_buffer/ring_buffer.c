#include "ring_buffer.h"


// 初始化队列
ring_buffer* ring_buffer_create(size_t capacity)
{
    ring_buffer *queue = malloc(sizeof(ring_buffer));
    if (!queue) return NULL;
    
    queue->buffer = malloc(capacity);
    if (!queue->buffer) {
        free(queue);
        return NULL;
    }
    
    queue->capacity = capacity;
    queue->head = 0;
    queue->tail = 0;
    queue->is_full = false;
    return queue;
}

// 释放队列
void ring_buffer_destroy(ring_buffer *queue)
{
    if (queue) {
        free(queue->buffer);
        free(queue);
    }
}

// 检查队列是否为空
bool ring_buffer_is_empty(const ring_buffer *queue)
{
    return (queue->head == queue->tail) && !queue->is_full;
}

// 检查队列是否已满
bool ring_buffer_is_full(const ring_buffer *queue)
{
    return queue->is_full;
}

// 计算可用空间
size_t ring_buffer_available(const ring_buffer *queue)
{
    if (queue->is_full) return 0;
    
    if (queue->head <= queue->tail) {
        return queue->capacity - (queue->tail - queue->head);
    } else {
        return queue->head - queue->tail;
    }
}

// 入队操作
bool ring_buffer_enqueue(ring_buffer *queue, const void *data, size_t data_len)
{
    if (!queue || !data || data_len == 0) return false;
    
    // 检查是否有足够空间
    if (data_len + sizeof(size_t) > ring_buffer_available(queue)) {
        return false;
    }
    
    // 先写入数据长度
    size_t *len_ptr = (size_t *)((char *)queue->buffer + queue->tail);
    *len_ptr = data_len;
    queue->tail += sizeof(size_t);
    if (queue->tail >= queue->capacity) {
        queue->tail -= queue->capacity;
    }
    
    // 写入实际数据
    size_t space_to_end = queue->capacity - queue->tail;
    if (data_len <= space_to_end) {
        memcpy((char *)queue->buffer + queue->tail, data, data_len);
        queue->tail += data_len;
    } else {
        memcpy((char *)queue->buffer + queue->tail, data, space_to_end);
        memcpy(queue->buffer, (char *)data + space_to_end, data_len - space_to_end);
        queue->tail = data_len - space_to_end;
    }
    
    if (queue->tail == queue->head) {
        queue->is_full = true;
    }
    
    return true;
}

// 出队操作
bool ring_buffer_dequeue(ring_buffer *queue, void *data, size_t *data_len)
{
    if (!queue || ring_buffer_is_empty(queue)) return false;
    
    // 读取数据长度
    size_t len;
    size_t *len_ptr = (size_t *)((char *)queue->buffer + queue->head);
    len = *len_ptr;
    queue->head += sizeof(size_t);
    if (queue->head >= queue->capacity) {
        queue->head -= queue->capacity;
    }
    
    // 如果提供了data_len指针，返回数据长度
    if (data_len) {
        *data_len = len;
    }
    
    // 如果提供了data缓冲区，复制数据
    if (data) {
        size_t space_to_end = queue->capacity - queue->head;
        if (len <= space_to_end) {
            memcpy(data, (char *)queue->buffer + queue->head, len);
            queue->head += len;
        } else {
            memcpy(data, (char *)queue->buffer + queue->head, space_to_end);
            memcpy((char *)data + space_to_end, queue->buffer, len - space_to_end);
            queue->head = len - space_to_end;
        }
    } else {
        // 不复制数据，只移动指针
        queue->head += len;
        if (queue->head >= queue->capacity) {
            queue->head -= queue->capacity;
        }
    }
    
    queue->is_full = false;
    return true;
}
