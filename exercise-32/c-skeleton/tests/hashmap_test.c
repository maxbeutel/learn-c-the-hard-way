#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <liblcthw/hashmap.h>
#include <liblcthw/bstrlib.h>

#include "minunit.h"

#define UNUSED(x) (void)(x)

struct tagbstring test_key_1 = bsStatic("test key 1");
struct tagbstring test_value_1 = bsStatic("test value 1");

struct tagbstring test_key_2 = bsStatic("test key 2");
struct tagbstring test_value_2 = bsStatic("test value 2");

struct tagbstring test_key_3 = bsStatic("test key 3");
struct tagbstring test_value_3 = bsStatic("test value 3");

// -1037811338
struct tagbstring test_collision_key_1 = bsStatic("Exobasidiaceae");
struct tagbstring test_collision_key_2 = bsStatic("gangster");

// -1125762254
struct tagbstring test_collision_key_3 = bsStatic("knavishness");
struct tagbstring test_collision_key_4 = bsStatic("undiminishable");


static int traverse_called = 0;

static int traverse_good_cb(HashmapNode *node)
{
    UNUSED(node);

    traverse_called++;

    return 0;
}

static int traverse_interruptingTraversal_cb(HashmapNode *node)
{
    UNUSED(node);

    traverse_called++;

    if (traverse_called == 2) {
        return 1;
    }

    return 0;
}

char *test_create() {
    Hashmap *map = Hashmap_create(NULL, NULL);
    assert(map != NULL && "Failed to create map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_get_set()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    int rc = Hashmap_set(map, &test_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    rc = Hashmap_set(map, &test_key_2, &test_value_2);
    assert(rc == 0 && "Failed to set value in map.");

    rc = Hashmap_set(map, &test_key_3, &test_value_3);
    assert(rc == 0 && "Failed to set value in map.");


    bstring result = Hashmap_get(map, &test_key_1);
    assert(&test_value_1 == result && "Got wrong value for key from map.");

    result = Hashmap_get(map, &test_key_2);
    assert(&test_value_2 == result && "Got wrong value for key from map.");

    result = Hashmap_get(map, &test_key_3);
    assert(&test_value_3 == result && "Got wrong value for key from map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_set_sameKeyTwice()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    int rc = Hashmap_set(map, &test_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    Hashmap_set(map, &test_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_get_set_keysCollide()
{
    // @FIXME this test should actually fail because keys collide?!
    Hashmap *map = Hashmap_create(NULL, NULL);

    int rc = Hashmap_set(map, &test_collision_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    Hashmap_set(map, &test_collision_key_2, &test_value_2);
    assert(rc == 0 && "Failed to set value in map.");

    bstring result = Hashmap_get(map, &test_collision_key_1);
    assert(&test_value_1 == result);

    result = Hashmap_get(map, &test_collision_key_2);
    assert(&test_value_2 == result);

    Hashmap_destroy(map);

    return NULL;
}

char *test_delete_mapIsEmpty()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    bstring result = Hashmap_delete(map, &test_key_1);
    assert(result == NULL && "Deleting non-existing key returned non-NULL result.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_delete()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    bstring result = Hashmap_delete(map, &test_key_2);
    assert(&test_value_2 == result && "Deleting returned unexpected result.");

    result = Hashmap_delete(map, &test_key_3);
    assert(&test_value_3 == result && "Deleting returned unexpected result.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_traverse_mapIsEmpty()
{
    traverse_called = 0;

    Hashmap *map = Hashmap_create(NULL, NULL);

    int rc = Hashmap_traverse(map, traverse_good_cb);
    assert(rc == 0 && "Traversing of map failed.");
    assert(traverse_called == 0 && "Expected 0 calls to traverse callback.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_traverse()
{
    traverse_called = 0;

    Hashmap *map = Hashmap_create(NULL, NULL);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    int rc = Hashmap_traverse(map, traverse_good_cb);
    assert(rc == 0 && "Traversing of map failed.");
    assert(traverse_called == 3 && "Expected traverse callback to be called.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_traverse_callbackInterruptsTraversal()
{
    traverse_called = 0;

    Hashmap *map = Hashmap_create(NULL, NULL);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    int rc = Hashmap_traverse(map, traverse_interruptingTraversal_cb);
    assert(rc == 1 && "Expected traversing of map to be interruped.");
    assert(traverse_called == 2 && "Expected callback to interrupt traversal.");

    Hashmap_destroy(map);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_delete_mapIsEmpty);
    mu_run_test(test_delete);
    mu_run_test(test_traverse_mapIsEmpty);
    mu_run_test(test_traverse);
    mu_run_test(test_traverse_callbackInterruptsTraversal);
    mu_run_test(test_set_sameKeyTwice);
    mu_run_test(test_get_set_keysCollide);

    return NULL;
}

RUN_TESTS(all_tests)
