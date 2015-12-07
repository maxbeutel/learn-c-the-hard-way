#include <assert.h>

#include "queue.h"

// debug
#define UNUSED(x) (void)(x)

Queue *Queue_create(size_t capacity)
{
    Queue *queue = calloc(1, sizeof(Queue));
    assert(queue != NULL);

    queue->size = 0;
    queue->capacity = capacity;

    queue->items = malloc(sizeof(void *) * capacity);
    assert(queue->items != NULL);

    return queue;
}

void Queue_destroy(Queue *queue)
{
    if (queue != NULL) {
        free(queue->items);
    }

    free(queue);
}

int Queue_enqueue(Queue *queue, void *item)
{
    assert(queue != NULL);
    assert(queue->size < queue->capacity);

    queue->items[queue->size] = item;
    queue->size++;

    return -1;
}

void *Queue_dequeue(Queue *queue)
{
    assert(queue != NULL);
    assert(queue->size > 0);
    assert(queue->size <= queue->capacity);

    void *item = queue->items[queue->size - 1];
    queue->size--;

    return item;
}
