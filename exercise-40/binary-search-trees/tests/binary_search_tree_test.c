#include <stdlib.h>
#include <assert.h>

#include <binary_search_tree.h>
#include <bstrlib.h>

#include "minunit.h"

#define UNUSED(x) (void)(x)

struct tagbstring test_key_1 = bsStatic("abcdefgh");
struct tagbstring test_value_1 = bsStatic("test value 1");

struct tagbstring test_key_2 = bsStatic("abcdefghi");
struct tagbstring test_value_2 = bsStatic("test value 2");

struct tagbstring test_key_3 = bsStatic("abcdefghij");
struct tagbstring test_value_3 = bsStatic("test value 3");

struct tagbstring test_key_4 = bsStatic("xxxxxxxx");
struct tagbstring test_key_5 = bsStatic("yyyyyyyy");

char *test_createEmpty()
{
    BSTree *map = BSTree_create(NULL);

    assert(map != NULL);
    assert(map->count == 0);
    assert(map->compare != NULL);

    BSTree_destroy(map);

    return NULL;
}

char *test_setRootNodeOnly()
{
    BSTree *map = BSTree_create(NULL);

    BSTree_set(map, &test_key_1, &test_value_1);
    assert(map->count == 1 && "Wrong count after adding value to map.");

    BSTree_destroy(map);

    return NULL;
}

char *test_addMultipleNodes()
{
    BSTree *map = BSTree_create(NULL);

    BSTree_set(map, &test_key_1, &test_value_1);
    assert(map->count == 1 && "Wrong count after adding value to map.");

    BSTree_set(map, &test_key_2, &test_value_2);
    assert(map->count == 2 && "Wrong count after adding value to map.");

    BSTree_set(map, &test_key_3, &test_value_3);
    assert(map->count == 3 && "Wrong count after adding value to map.");

    BSTree_destroy(map);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_createEmpty);
    mu_run_test(test_setRootNodeOnly);
    mu_run_test(test_addMultipleNodes);

    return NULL;
}

RUN_TESTS(all_tests)
