#include <stdlib.h>
#include <assert.h>

// debug
#include <stdio.h>
#define UNUSED(x) (void)(x)

#include "queue.h"

Queue *Queue_create()
{
    return NULL;
}

void Queue_destroy(Queue *queue)
{
    UNUSED(queue);
}


void Queue_send(Queue *queue, void *value)
{
    UNUSED(queue);
    UNUSED(value);
}

void *Queue_peek(Queue *queue)
{
    UNUSED(queue);

    return NULL;
}

void *Queue_receive(Queue *queue)
{
    UNUSED(queue);

    return NULL;
}

int Queue_count(Queue *queue)
{
    UNUSED(queue);

    return -1;
}
