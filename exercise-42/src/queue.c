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
    queue->front_element_index = -1;

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
    assert(queue->size < queue->capacity);

    queue->elements[queue->size] = element;

    if (queue->size == 0) {
        queue->front_element_index = queue->size;
    }

    queue->size++;
}

void *Queue_peek(Queue *queue)
{
    assert(queue != NULL);

    if (queue->size == 0) {
        return NULL;
    }

    return queue->elements[queue->front_element_index];
}

void *Queue_receive(Queue *queue)
{
    assert(queue != NULL);

    if (queue->size == 0) {
        return 0;
    }

    void *element = queue->elements[queue->front_element_index];
    queue->front_element_index++;

    queue->size--;

    return element;
}
