#ifndef _lcthw_queue_h
#define _lcthw_queue_h

typedef struct Queue {
    int capacity;
    int size;
    int front_element_index;
    void **elements;
} Queue;

Queue *Queue_create(int size);
void Queue_destroy(Queue *queue);

void Queue_send(Queue *queue, void *element);

void *Queue_peek(Queue *queue);
void *Queue_receive(Queue *queue);

#endif
