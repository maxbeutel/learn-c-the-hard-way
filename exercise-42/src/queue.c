#include <stdlib.h>
#include <assert.h>

// debug
#include <stdio.h>
#define UNUSED(x) (void)(x)

#include "queue.h"

Queue *Queue_create(int capacity)
{
    assert(capacity > 0);

    Queue *queue = calloc(1, sizeof(Queue));
    assert(queue != NULL);

    queue->capacity = capacity;
    queue->size = 0;
    queue->nextFreeIndex = 0;
    queue->frontElementIndex = -1;

    queue->elements = calloc(capacity, sizeof(void *));
    assert(queue->elements != NULL);

    return queue;
}

void Queue_destroy(Queue *queue)
{
    if (queue != NULL) {
        free(queue->elements);
    }

    free(queue);
}


void Queue_send(Queue *queue, void *element)
{
    assert(queue != NULL);
    assert(element != NULL);

    assert(queue->nextFreeIndex >= 0);
    assert(queue->nextFreeIndex < queue->capacity);

    queue->elements[queue->nextFreeIndex] = element;
    queue->size++;

    // we now have a first entry to which we can point
    if (queue->size == 1) {
        queue->frontElementIndex = 0;
    }

    // queue is full, next send call will start overwriting from beginning
    if (queue->size >= queue->capacity) {
        queue->nextFreeIndex = 0;
    } else {
        queue->nextFreeIndex++;
    }
}

void *Queue_peek(Queue *queue)
{
    assert(queue != NULL);

    if (queue->size == 0) {
        return NULL;
    }

    return queue->elements[queue->frontElementIndex];
}

void *Queue_receive(Queue *queue)
{
    assert(queue != NULL);

    if (queue->size == 0) {
        return 0;
    }

    void *element = queue->elements[queue->frontElementIndex];
    queue->frontElementIndex++;

    queue->size--;

    return element;
}
