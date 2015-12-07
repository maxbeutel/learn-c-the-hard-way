#include <string.h>
#include <assert.h>

#include <queue.h>

#include "minunit.h"

#define TEST_VALUE_1 "test value 1"
#define TEST_VALUE_1_LEN strlen(TEST_VALUE_1)

#define TEST_VALUE_2 "test value 2"
#define TEST_VALUE_2_LEN strlen(TEST_VALUE_2)

#define TEST_VALUE_3 "test value 3"
#define TEST_VALUE_3_LEN strlen(TEST_VALUE_3)

char *test_create()
{
    Queue *queue = Queue_create(3);
    assert(queue != NULL);

    assert(queue->size == 0);
    assert(queue->capacity == 3);

    Queue_destroy(queue);

    return NULL;
}

char *test_enqueueDequeue()
{
    Queue *queue = Queue_create(3);

    Queue_enqueue(queue, TEST_VALUE_1);
    Queue_enqueue(queue, TEST_VALUE_1);
    Queue_enqueue(queue, TEST_VALUE_1);

    char *value = Queue_dequeue(queue);
    assert(value == TEST_VALUE_1 && "Got unexpected value when dequeuing item.");

    Queue_destroy(queue);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_enqueueDequeue);

    return NULL;
}

RUN_TESTS(all_tests)
