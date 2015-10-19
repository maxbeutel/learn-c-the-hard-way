#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include <liblcthw/dynamic_array.h>

#include "minunit.h"

#define TEST_STR_1 "hallo"
#define TEST_STR_1_LEN strlen(TEST_STR_1)

#define TEST_STR_2 "welt"
#define TEST_STR_2_LEN strlen(TEST_STR_2)

#define UNUSED(x) (void)(x)

char *test_create()
{
    DArray *array = DArray_create(sizeof(intptr_t), 5);
    assert(array != NULL && "DArray_create failed.");
    assert(array->size == 0 && "Logical size does not match.");
    assert(array->capacity == 5 && "Physical capacity does not match.");
    assert(array->element_size == sizeof(intptr_t) && "element_size does not match.");
    assert(array->expand_rate == DEFAULT_EXPAND_RATE && "expand_rate must be set to DEFAULT_EXPAND_RATE.");
    assert(array->contents != NULL && "contents must not be NULL.");

    DArray_destroy(array);

    return NULL;
}

char *test_destroy_arrayContainingMallocedValues()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);

    char *test_str_1 = malloc(sizeof(char) * TEST_STR_1_LEN);
    strncpy(test_str_1, TEST_STR_1, TEST_STR_1_LEN);

    char *test_str_2 = malloc(sizeof(char) * TEST_STR_2_LEN);
    strncpy(test_str_2, TEST_STR_2, TEST_STR_2_LEN);

    DArray_push(array, test_str_1);
    DArray_push(array, test_str_2);

    DArray_destroy(array);

    free(test_str_1);
    free(test_str_2);

    return NULL;
}

char *test_destroy_emptyArray()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);

    DArray_destroy(array);

    return NULL;
}

char *test_get_set()
{
    DArray *array = DArray_create(sizeof(intptr_t), 5);

    intptr_t value;

    DArray_set(array, 1, (void *) (intptr_t) 15);
    value = (intptr_t) DArray_get(array, 1);
    assert(value == 15 && "Did not get expected value at index 1.");
    assert(array->size == 1 && "Unexpected array size after adding one element at index 1.");

    DArray_set(array, 4, (void *) (intptr_t) 9789);
    value = (intptr_t) DArray_get(array, 4);
    assert(value == 9789 && "Did not get expected value at index 4.");
    assert(array->size == 2 && "Unexpected array size after adding one element at index 4.");

    DArray_destroy(array);

    return NULL;
}

char *test_get_set_undefinedIndex()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);

    void *value_at_undefined_index = DArray_get(array, 1);
    assert(value_at_undefined_index == NULL && "Getting value at undefined index is not NULL.");

    DArray_destroy(array);

    return NULL;
}

char *test_remove()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);
    DArray_remove(array, 66);
    assert(array->size == 0 && "Unexpected array size - must be 0 as no elements were added.");

    DArray_set(array, 20, (void *) (intptr_t) 9789);
    DArray_remove(array, 20);
    assert(array->size == 0 && "Unexpected array size - must be 0 again after removing element.");

    void *value = DArray_get(array, 20);
    assert(value == NULL && "Value at index 20 was not removed.");

    DArray_destroy(array);

    return NULL;
}

char *test_expand_setUndefinedIndexExpandsArray()
{
    int initial_capacity = 2;
    DArray *array = DArray_create(sizeof(intptr_t), initial_capacity);

    DArray_set(array, 0, (void *) (intptr_t) 9789);
    DArray_set(array, 1, (void *) (intptr_t) 747);
    assert(array->size == 2 && "Unexpected array size after adding 2 elements");
    assert(array->capacity == initial_capacity && "Unexpected array capacity - should match initial capacity.");

    DArray_set(array, 2, (void *) (intptr_t) 8776);
    assert(array->size == 3 && "Unexpected array size - after adding one more element.");
    assert(array->capacity == (initial_capacity + DEFAULT_EXPAND_RATE) && "Unexpected array capacity after expanding.");

    intptr_t value = (intptr_t) DArray_get(array, 0);
    assert(value == 9789 && "Did not get expected value at index 0.");

    value = (intptr_t) DArray_get(array, 1);
    assert(value == 747 && "Did not get expected value at index 1.");

    value = (intptr_t) DArray_get(array, 2);
    assert(value == 8776 && "Did not get expected value at index 2.");

    // index 0 1 2 are set, all others should be NULL
    for (int i = initial_capacity + 1;
         i < initial_capacity + DEFAULT_EXPAND_RATE;
         i++) {
        void *null_value = DArray_get(array, i);

        assert(null_value == NULL && "Expected remaining indexes to be NULL");
    }

    DArray_destroy(array);

    return NULL;
}

char *test_contract()
{
    int initial_size = 2;
    DArray *array = DArray_create(sizeof(intptr_t), initial_size);

    DArray_set(array, 0, (void *) (intptr_t) 1);
    DArray_set(array, 1, (void *) (intptr_t) 2);

    // start expanding, first time
    DArray_set(array, 2, (void *) (intptr_t) 3);
    DArray_set(array, 3, (void *) (intptr_t) 4);
    DArray_set(array, 4, (void *) (intptr_t) 5);
    DArray_set(array, 5, (void *) (intptr_t) 6);
    DArray_set(array, 6, (void *) (intptr_t) 7);
    DArray_set(array, 7, (void *) (intptr_t) 8);
    DArray_set(array, 8, (void *) (intptr_t) 9);
    DArray_set(array, 9, (void *) (intptr_t) 10);
    DArray_set(array, 10, (void *) (intptr_t) 11);
    DArray_set(array, 11, (void *) (intptr_t) 12);

    assert(array->size == 12 && "Unexpected array size - expected initial size + one time expansion.");

    // start expanding, second time
    DArray_set(array, 12, (void *) (intptr_t) 13);
    DArray_set(array, 13, (void *) (intptr_t) 14);
    DArray_set(array, 14, (void *) (intptr_t) 15);
    DArray_set(array, 15, (void *) (intptr_t) 16);
    DArray_set(array, 16, (void *) (intptr_t) 17);
    DArray_set(array, 17, (void *) (intptr_t) 18);
    DArray_set(array, 18, (void *) (intptr_t) 19);
    DArray_set(array, 19, (void *) (intptr_t) 20);
    DArray_set(array, 20, (void *) (intptr_t) 21);
    DArray_set(array, 21, (void *) (intptr_t) 22);

    assert(array->size == 22 && "Unexpected array size - expected initial size + one time expansion.");

    DArray_destroy(array);

    return NULL;
}

 char *test_push_pop()
 {
     DArray *array = DArray_create(sizeof(intptr_t), 5);

     DArray_push(array, (void *) (intptr_t) 30);
     DArray_push(array, (void *) (intptr_t) 20);
     DArray_push(array, (void *) (intptr_t) 10);

     intptr_t value = (intptr_t) DArray_pop(array);
     assert(value == 10 && "pop'ed wrong value from array.");

     value = (intptr_t) DArray_pop(array);
     assert(value == 20 && "pop'ed wrong value from array.");

     value = (intptr_t) DArray_pop(array);
     assert(value == 30 && "pop'ed wrong value from array.");

     DArray_push(array, (void *) (intptr_t) 60);
     value = (intptr_t) DArray_get(array, 0);
     assert(value == 60 && "Getting value by index after pop'ing all values from array failed.");

     DArray_destroy(array);

     return NULL;
}

int _test_predicate(void *search_element, void *current_element, int index)
{
    UNUSED(index);

    intptr_t search_element_int = (intptr_t) search_element;
    intptr_t current_element_int = (intptr_t) current_element;

    if (search_element_int == current_element_int) {
        return 1;
    }

    return 0;
}

char *test_contains()
{
    DArray *array = DArray_create(sizeof(intptr_t), 3);

    DArray_push(array, (void *) (intptr_t) 60);
    DArray_push(array, (void *) (intptr_t) 90);

    assert(DArray_contains(array, (void *) (intptr_t) 90, _test_predicate) == 1 && "Expected to find value in array.");
    assert(DArray_contains(array, (void *) (intptr_t) 60, _test_predicate) == 1 && "Expected to find value in array.");
    assert(DArray_contains(array, (void *) (intptr_t) 100, _test_predicate) == 0 && "Expected to not find value in array.");
    assert(DArray_contains(array, NULL, _test_predicate) == 0 && "Expected to not find value in array.");

    DArray_destroy(array);

    return NULL;
}

char *test_iteration_arrayContainingContinousElements()
{
    DArray *array = DArray_create(sizeof(intptr_t), 3);

    DArray_push(array, (void *) (intptr_t) 60);
    DArray_push(array, (void *) (intptr_t) 670);
    DArray_push(array, (void *) (intptr_t) 90);

    intptr_t collected_elements[3] = {0};

    DArray_iterator iterator = DARRAY_ITERATOR_EMPTY;
    int index = 0;

    while ((index = DArray_iterator_next(array, &iterator)) != DARRAY_ITERATOR_END) {
        intptr_t element = (intptr_t) DArray_get(array, index);

        collected_elements[index] = element;
    }

    assert(collected_elements[0] == 60 && "Expected value at index not found.");
    assert(collected_elements[1] == 670 && "Expected value at index not found.");
    assert(collected_elements[2] == 90 && "Expected value at index not found.");

    return NULL;
}

char *test_iteration_arrayContainingHoles_1()
{
    DArray *array = DArray_create(sizeof(intptr_t), 3);

    DArray_set(array, 0, (void *) (intptr_t) 10);
    DArray_set(array, 2, (void *) (intptr_t) 30);

    intptr_t collected_elements[3] = {0};

    DArray_iterator iterator = DARRAY_ITERATOR_EMPTY;
    int index = 0;

    while ((index = DArray_iterator_next(array, &iterator)) != DARRAY_ITERATOR_END) {
        intptr_t element = (intptr_t) DArray_get(array, index);

        collected_elements[index] = element;
    }

    assert(collected_elements[0] == 10 && "Expected value at index not found.");
    assert(collected_elements[2] == 30 && "Expected value at index not found.");

    return NULL;
}

char *test_iteration_arrayContainingHoles_2()
{
    DArray *array = DArray_create(sizeof(intptr_t), 3);

    DArray_set(array, 0, (void *) (intptr_t) 10);
    DArray_set(array, 1, (void *) (intptr_t) 20);
    DArray_set(array, 2, (void *) (intptr_t) 30);

    DArray_remove(array, 0);

    intptr_t collected_elements[3] = {0};

    DArray_iterator iterator = DARRAY_ITERATOR_EMPTY;
    int index = 0;

    while ((index = DArray_iterator_next(array, &iterator)) != DARRAY_ITERATOR_END) {
        intptr_t element = (intptr_t) DArray_get(array, index);

        collected_elements[index] = element;
    }

    assert(collected_elements[1] == 20 && "Expected value at index not found.");
    assert(collected_elements[2] == 30 && "Expected value at index not found.");

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_destroy_emptyArray);
    mu_run_test(test_destroy_arrayContainingMallocedValues);
    mu_run_test(test_get_set);
    mu_run_test(test_get_set_undefinedIndex);
    mu_run_test(test_expand_setUndefinedIndexExpandsArray);
    mu_run_test(test_remove);
    mu_run_test(test_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_contains);
    mu_run_test(test_iteration_arrayContainingContinousElements);
    mu_run_test(test_iteration_arrayContainingHoles_1);
    mu_run_test(test_iteration_arrayContainingHoles_2);

    return NULL;
}

RUN_TESTS(all_tests)
