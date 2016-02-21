#include <assert.h>
#include <queue.h>

// DEBUG
#include <stdio.h>

#include "minunit.h"

#define TEST_ELEMENT_1 "test1 data"
#define TEST_ELEMENT_2 "test2 data"
#define TEST_ELEMENT_3 "test3 data"

char *test_create()
{
    Queue *queue = Queue_create(1);
    assert(queue != NULL && "Pointer to queue was not created successfully.");

    Queue_destroy(queue);

    return NULL;
}

char *test_sendReceive()
{
    Queue *queue = Queue_create(3);

    // receive
    Queue_send(queue, TEST_ELEMENT_1);
    assert(Queue_peek(queue) == TEST_ELEMENT_1 && "Expected first added element to be returned on peek.");
    assert(queue->size == 1 && "Wrong queue count after sending element.");

    Queue_send(queue, TEST_ELEMENT_2);
    assert(Queue_peek(queue) == TEST_ELEMENT_1 && "Expected first added element to be returned on peek.");
    assert(queue->size == 2 && "Wrong queue count after sending element.");

    Queue_send(queue, TEST_ELEMENT_3);
    assert(Queue_peek(queue) == TEST_ELEMENT_1 && "Expected first added element to be returned on peek.");
    assert(queue->size == 3 && "Wrong queue count after sending element.");

    // receive
    char *element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_1 && "Wrong element on receive.");
    assert(queue->size == 2 && "Wront queue count after receiving element.");
    assert(Queue_peek(queue) == TEST_ELEMENT_2 && "Expected next element to be returned on peek.");

    element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_2 && "Wrong element on receive.");
    assert(queue->size == 1 && "Wront queue count after receiving element.");
    assert(Queue_peek(queue) == TEST_ELEMENT_3 && "Expected next element to be returned on peek.");

    element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_3 && "Wrong element on receive.");
    assert(queue->size == 0 && "Wront queue count after receiving element.");
    assert(Queue_peek(queue) == NULL && "Expected no more elements to be returned on peek.");

    Queue_destroy(queue);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_sendReceive);

    return NULL;
}

RUN_TESTS(all_tests)
