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

char *test_create()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);
    
    assert(array != NULL && "DArray_create failed.");
    assert(array->contents != NULL && "contents are wrong in darray");
    assert(array->end == 0 && "end isn't at the right spot");
    assert(array->element_size == sizeof(intptr_t) && "element size is wrong.");
    assert(array->max == 100 && "wrong max length on initial size");
    
    DArray_destroy(array);
    
    return NULL;
}

char *test_destroy_emptyArray()
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

char *test_destroy_arrayContainingMallocedValues()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);
    
    DArray_destroy(array);

    return NULL;
}

char *test_get_set()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);

    intptr_t value;
    
    DArray_set(array, 1, (void *) (intptr_t) 15);
    value = (intptr_t) DArray_get(array, 1);
    assert(value == 15 && "Did not get expected value at index 1.");
    
    DArray_set(array, 20, (void *) (intptr_t) 9789);
    value = (intptr_t) DArray_get(array, 20);
    assert(value == 9789 && "Did not get expected value at index 20.");

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

    DArray_set(array, 20, (void *) (intptr_t) 9789);
    DArray_remove(array, 20);
    
    assert(DArray_end(array) == 20 && "Wrong array->end found.");
    
    void *value = DArray_get(array, 20);
    assert(value == NULL && "Value at index 20 was not removed.");
    
    // @TODO count + end sind buggy (fixit!)
    // @FIXME DArray_last benutzt end direkt
    
    //assert(DArray_count(array) == 0);
    //assert(DArray_end(array) == 0 && "array->end was not reset.");

    DArray_destroy(array);
    
    return NULL;
}

char *test_push_pop()
{
    DArray *array = DArray_create(sizeof(intptr_t), 100);
    
    DArray_push(array, (void *) (intptr_t) 10);
    DArray_push(array, (void *) (intptr_t) 20);
    DArray_push(array, (void *) (intptr_t) 30);
    
    intptr_t value = (intptr_t) DArray_pop(array);
    assert(value == 30 && "pop'ed wrong value from array.");

    value = (intptr_t) DArray_pop(array);
    assert(value == 20 && "pop'ed wrong value from array.");
    
    value = (intptr_t) DArray_pop(array);
    assert(value == 10 && "pop'ed wrong value from array.");
    
    DArray_push(array, (void *) (intptr_t) 60);
    value = (intptr_t) DArray_get(array, 0);
    assert(value == 60 && "Getting value by index after pop'ing all values from array failed.");

    DArray_destroy(array);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_destroy_emptyArray);
    mu_run_test(test_destroy_arrayContainingMallocedValues);
    mu_run_test(test_get_set);
    mu_run_test(test_get_set_undefinedIndex);
    mu_run_test(test_remove);
    //mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    //mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests)
