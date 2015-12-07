#ifndef _queue_h
#define _queue_h

#include <stdlib.h>

typedef struct Queue {
    size_t size; // logical size, number of elements
    size_t capacity; // physical size

    void **items;
} Queue;

Queue *Queue_create(size_t capacity);
void Queue_destroy(Queue *queue);

int Queue_enqueue(Queue *queue, void *item);
void *Queue_dequeue(Queue *queue);

#endif
