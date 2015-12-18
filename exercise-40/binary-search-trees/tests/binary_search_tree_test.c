#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include <binary_search_tree.h>
#include <bstrlib.h>

#include "minunit.h"

#define UNUSED(x) (void)(x)

int test_key_1 = 1;
struct tagbstring test_value_1 = bsStatic("test value 1");

int test_key_2 = 2;
struct tagbstring test_value_2 = bsStatic("test value 2");

int test_key_3 = 3;
struct tagbstring test_value_3 = bsStatic("test value 3");

int test_key_4 = 4;
int test_key_5 = 5;

static int intKey_compare(void *a, void *b)
{
    int a_int = (int) a;
    int b_int = (int) b;

    return a_int - b_int;
}

char *test_createEmpty()
{
    BSTree *map = BSTree_create(intKey_compare);

    assert(map != NULL);
    assert(map->size == 0);
    assert(map->compare != NULL);

    BSTree_destroy(map);

    return NULL;
}

char *test_setRootNodeOnly()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_replaceExistingKeyWithNewValue()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_2);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_getSetMultipleNodes()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_2, &test_value_2);
    assert(map->size == 2 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_3, &test_value_3);
    assert(map->size == 3 && "Wrong count after adding value to map.");

    bstring value = BSTree_get(map, (void *) (intptr_t) test_key_1);
    assert(&test_value_1 == value && "Got unexpected value from map.");

    value = BSTree_get(map, (void *) (intptr_t) test_key_2);
    assert(&test_value_2 == value && "Got unexpected value from map.");

    value = BSTree_get(map, (void *) (intptr_t) test_key_3);
    assert(&test_value_3 == value && "Got unexpected value from map.");

    BSTree_destroy(map);

    return NULL;
}

/* char *test_deleteRootNodeOnly() */
/* { */
/*     BSTree *map = BSTree_create(intKey_compare); */

/*     BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1); */
/*     assert(map->size == 1 && "Wrong count after adding value to map."); */

/*     BSTree_delete(map, (void *) (intptr_t) test_key_1); */
/*     assert(map->size == 0 && "Wrong count after deleting value from map."); */

/*     BSTree_destroy(map); */

/*     return NULL; */
/* } */

char *test_deleteSingleChildOfRoot()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_2, &test_value_2);
    assert(map->size == 2 && "Wrong count after adding value to map.");

    BSTree_delete(map, (void *) (intptr_t) test_key_2);
    assert(map->size == 1 && "Wrong count after deleting value from map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_unknownKeyDoesNotDeleteAnyNode()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_2, &test_value_2);
    assert(map->size == 2 && "Wrong count after adding value to map.");

    BSTree_delete(map, (void *) (intptr_t) test_key_3);
    assert(map->size == 2 && "Wrong count after deleting value from map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_deleteNodeWithSingleChildLeft()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_2, &test_value_2);
    assert(map->size == 2 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_3, &test_value_3);
    assert(map->size == 3 && "Wrong count after adding value to map.");

    BSTree_delete(map, (void *) (intptr_t) test_key_2);
    assert(map->size == 2 && "Wrong count after deleting value from map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_deleteNodeWithSingleChildRight()
{
    BSTree *map = BSTree_create(intKey_compare);

    BSTree_set(map, (void *) (intptr_t) test_key_3, &test_value_3);
    assert(map->size == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_2, &test_value_2);
    assert(map->size == 2 && "Wrong count after adding value to map.");

    BSTree_set(map, (void *) (intptr_t) test_key_1, &test_value_1);
    assert(map->size == 3 && "Wrong count after adding value to map.");

    BSTree_delete(map, (void *) (intptr_t) test_key_2);
    assert(map->size == 2 && "Wrong count after deleting value from map.");

    BSTree_destroy(map);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_createEmpty);
    mu_run_test(test_setRootNodeOnly);
    mu_run_test(test_getSetMultipleNodes);
    mu_run_test(test_deleteSingleChildOfRoot);
    mu_run_test(test_unknownKeyDoesNotDeleteAnyNode);
    mu_run_test(test_replaceExistingKeyWithNewValue);
    mu_run_test(test_deleteNodeWithSingleChildLeft);
    mu_run_test(test_deleteNodeWithSingleChildRight);

    /* mu_run_test(test_deleteRootNodeOnly); */

    return NULL;
}

RUN_TESTS(all_tests)
