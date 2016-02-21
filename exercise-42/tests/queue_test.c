#include <assert.h>
#include <queue.h>

// DEBUG
#include <stdio.h>

#include "minunit.h"

char *tests[] = {"test1 data", "test2 data", "test3 data"};
#define NUM_TESTS 3

char *test_create()
{
    Queue *queue = Queue_create(1);
    assert(queue != NULL && "Pointer to queue was not created successfully.");

    Queue_destroy(queue);

    return NULL;
}

char *test_sendReceive()
{
    Queue *queue = Queue_create(NUM_TESTS);

    // send
    for (int i = 0; i < NUM_TESTS; i++) {
        Queue_send(queue, tests[i]);
        assert(Queue_peek(queue) == tests[0] && "Expected first added element to be peekable.");
    }

    assert(queue->size == NUM_TESTS && "Wrong queue count after sending items.");

    /* QUEUE_FOREACH(queue, cur) { */
    /*     printf("VAL: %s\n", (char *) cur->value); */
    /* } */

    // receive
    for (int i = 0; i < NUM_TESTS; i++) {
        char *val = Queue_receive(queue);
        assert(val == tests[i] && "Wrong element on receive.");
    }

    assert(queue->size == 0 && "Wrong queue count after receiving items.");

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
