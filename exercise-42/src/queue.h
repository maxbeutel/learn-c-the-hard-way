#ifndef _lcthw_queue_h
#define _lcthw_queue_h

typedef struct Queue {
    int size; // @TODO do we need this?
} Queue;

Queue *Queue_create();
void Queue_destroy(Queue *queue);

void Queue_send(Queue *queue, void *value);

void *Queue_peek(Queue *queue);
void *Queue_receive(Queue *queue);

int Queue_count(Queue *queue);

#endif
