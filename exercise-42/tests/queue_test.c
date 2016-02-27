#include <assert.h>
#include <queue.h>

// DEBUG
#include <stdio.h>

#include "minunit.h"

#define TEST_ELEMENT_1 "test1 data"
#define TEST_ELEMENT_2 "test2 data"
#define TEST_ELEMENT_3 "test3 data"
#define TEST_ELEMENT_4 "test4 data"
#define TEST_ELEMENT_5 "test5 data"
#define TEST_ELEMENT_6 "test6 data"
#define TEST_ELEMENT_7 "test7 data"

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

    // send
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

char *test_sendOverwritesExistingElementWhenFull()
{
    Queue *queue = Queue_create(3);

    Queue_send(queue, TEST_ELEMENT_1);
    Queue_send(queue, TEST_ELEMENT_2);
    Queue_send(queue, TEST_ELEMENT_3);

    // at this point queue is full
    assert(Queue_peek(queue) == TEST_ELEMENT_1 && "Expected first added element to be returned on peek.");

    // now send one more which overwrites the first element that was previously added
    Queue_send(queue, TEST_ELEMENT_4);
    assert(Queue_peek(queue) == TEST_ELEMENT_4 && "Expected first added element to be overwritten by most recent added element.");

    assert(queue->size == 3 && "Expected queue size to remain constant after adding element to full queue.");

    Queue_destroy(queue);

    return NULL;
}

char *test_receiveOverwritingValueFromQueue()
{
    Queue *queue = Queue_create(3);

    Queue_send(queue, TEST_ELEMENT_1);
    Queue_send(queue, TEST_ELEMENT_2);
    Queue_send(queue, TEST_ELEMENT_3);

    Queue_send(queue, TEST_ELEMENT_4);
    Queue_send(queue, TEST_ELEMENT_5);
    Queue_send(queue, TEST_ELEMENT_6);

    Queue_send(queue, TEST_ELEMENT_7);

    char *element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_7 && "Wrong element on receive.");

    element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_5 && "Wrong element on receive.");

    element = Queue_receive(queue);
    assert(element == TEST_ELEMENT_6 && "Wrong element on receive.");

    Queue_destroy(queue);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_sendReceive);
    mu_run_test(test_sendOverwritesExistingElementWhenFull);
    mu_run_test(test_receiveOverwritingValueFromQueue);

    return NULL;
}

RUN_TESTS(all_tests)
