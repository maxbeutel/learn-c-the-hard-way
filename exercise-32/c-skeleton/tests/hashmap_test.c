#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <liblcthw/hashmap.h>
#include <liblcthw/bstrlib.h>

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

static uint32_t test_strlen_is_hash(void *key)
{
    assert(key != NULL);

    size_t len = blength((bstring) key);
    return len;
}

char *test_create() {
    Hashmap *map = Hashmap_create(NULL, NULL);
    assert(map != NULL && "Failed to create map.");
    assert(map->size == 0 && "Got unexpected initial size of map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_get_set()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

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

char *test_get_set_keyCollisions()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    int rc = Hashmap_set(map, &test_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    // using a key that is different then key_1, but returns the same hash!
    rc = Hashmap_set(map, &test_key_4, &test_value_2);
    assert(rc == 0 && "Failed to set value in map.");

    // using a key that is different then key_1, but returns the same hash!
    rc = Hashmap_set(map, &test_key_5, &test_value_3);
    assert(rc == 0 && "Failed to set value in map.");

    bstring result = Hashmap_get(map, &test_key_1);
    assert(&test_value_1 == result && "Got wrong value for key from map.");

    result = Hashmap_get(map, &test_key_4);
    assert(&test_value_2 == result && "Got wrong value for key from map.");

    /* result = Hashmap_get(map, &test_key_3); */
    /* assert(&test_value_3 == result && "Got wrong value for key from map."); */

    Hashmap_destroy(map);

    return NULL;
}

char *test_set_differentValueSameKey()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    int rc = Hashmap_set(map, &test_key_1, &test_value_1);
    assert(rc == 0 && "Failed to set value in map.");

    Hashmap_set(map, &test_key_1, &test_value_2);
    assert(rc == 0 && "Failed to set value in map.");

    bstring result = Hashmap_get(map, &test_key_1);
    assert(&test_value_2 == result && "Got wrong value for key from map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove_mapIsEmpty()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    bstring result = Hashmap_remove(map, &test_key_1);
    assert(result == NULL && "Removing non-existing key returned non-NULL result.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    bstring result = Hashmap_remove(map, &test_key_2);
    assert(&test_value_2 == result && "Removing returned unexpected result.");

    result = Hashmap_remove(map, &test_key_3);
    assert(&test_value_3 == result && "Removing returned unexpected result.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove_keyCollisions_1()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_4, &test_value_1);
    Hashmap_set(map, &test_key_5, &test_value_3);
    Hashmap_set(map, &test_key_2, &test_value_2);

    bstring result = Hashmap_remove(map, &test_key_5);
    assert(&test_value_3 == result && "Removing returned unexpected result.");
    assert(map->size == 2 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_2);
    assert(&test_value_2 == result && "Removing returned unexpected result.");
    assert(map->size == 1 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_4);
    assert(&test_value_1 == result && "Removing returned unexpected result.");
    assert(map->size == 0 && "Failed to assert map size after removing key.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove_keyCollisions_2()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_4, &test_value_2);
    Hashmap_set(map, &test_key_5, &test_value_3);

    bstring result = Hashmap_remove(map, &test_key_5);
    assert(&test_value_3 == result && "Removing returned unexpected result.");
    assert(map->size == 2 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_4);
    assert(&test_value_2 == result && "Removing returned unexpected result.");
    assert(map->size == 1 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_1);
    assert(&test_value_1 == result && "Removing returned unexpected result.");
    assert(map->size == 0 && "Failed to assert map size after removing key.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove_keyCollisions_3()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_4, &test_value_2);
    Hashmap_set(map, &test_key_5, &test_value_3);

    bstring result = Hashmap_remove(map, &test_key_1);
    assert(&test_value_1 == result && "Removing returned unexpected result.");
    assert(map->size == 2 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_4);
    assert(&test_value_2 == result && "Removing returned unexpected result.");
    assert(map->size == 1 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_5);
    assert(&test_value_3 == result && "Removing returned unexpected result.");
    assert(map->size == 0 && "Failed to assert map size after removing key.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_remove_4()
{
    Hashmap *map = Hashmap_create(NULL, NULL);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_4, &test_value_2);
    Hashmap_set(map, &test_key_5, &test_value_3);

    bstring result = Hashmap_remove(map, &test_key_1);
    assert(&test_value_1 == result && "Removing returned unexpected result.");
    assert(map->size == 2 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_4);
    assert(&test_value_2 == result && "Removing returned unexpected result.");
    assert(map->size == 1 && "Failed to assert map size after removing key.");

    result = Hashmap_remove(map, &test_key_5);
    assert(&test_value_3 == result && "Removing returned unexpected result.");
    assert(map->size == 0 && "Failed to assert map size after removing key.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_iterationForward()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    bstring key_out = NULL;
    bstring data_out = NULL;
    int index = -1;

    assert(
        Hashmap_iterator_next(map, &index, (void**) (&key_out), (void**) (&data_out)) == 1
        && "Expected to continue to next element."
    );
    assert(&test_key_1 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_1 == data_out && "Got wrong data when iterating map.");

    assert(
        Hashmap_iterator_next(map, &index, (void**) (&key_out), (void**) (&data_out)) == 1
        && "Expected to continue to next element."
    );
    assert(&test_key_2 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_2 == data_out && "Got wrong data when iterating map.");

    assert(
        Hashmap_iterator_next(map, &index, (void**) (&key_out), (void**) (&data_out)) == 0
        && "Expected to end iteration."
    );
    assert(&test_key_3 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_3 == data_out && "Got wrong data when iterating map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_iterationBackward()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_2, &test_value_2);
    Hashmap_set(map, &test_key_3, &test_value_3);

    bstring key_out = NULL;
    bstring data_out = NULL;
    int index = -1;

    assert(
        Hashmap_iterator_prev(map, &index, (void**) (&key_out), (void**) (&data_out)) == 1
        && "Expected to continue to prev element."
    );
    assert(&test_key_3 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_3 == data_out && "Got wrong data when iterating map.");

    assert(
        Hashmap_iterator_prev(map, &index, (void**) (&key_out), (void**) (&data_out)) == 1
        && "Expected to continue to prev element."
    );
    assert(&test_key_2 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_2 == data_out && "Got wrong data when iterating map.");

    assert(
        Hashmap_iterator_prev(map, &index, (void**) (&key_out), (void**) (&data_out)) == 0
        && "Expected to end iteration."
    );
    assert(&test_key_1 == key_out && "Got wrong key when iterating map.");
    assert(&test_value_1 == data_out && "Got wrong data when iterating map.");

    Hashmap_destroy(map);

    return NULL;
}

char *test_contains()
{
    Hashmap *map = Hashmap_create(NULL, test_strlen_is_hash);

    assert(Hashmap_contains(map, &test_key_1) == 0 && "Map was not expected to contain key.");

    Hashmap_set(map, &test_key_1, &test_value_1);
    Hashmap_set(map, &test_key_2, &test_value_2);

    assert(Hashmap_contains(map, &test_key_1) == 1 && "Map was expected to contain key.");
    assert(Hashmap_contains(map, &test_key_2) == 1 && "Map was expected to contain key.");

    Hashmap_destroy(map);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_get_set_keyCollisions);
    mu_run_test(test_set_differentValueSameKey);
    mu_run_test(test_iterationForward);
    mu_run_test(test_iterationBackward);
    mu_run_test(test_remove_mapIsEmpty);
    mu_run_test(test_remove);
    mu_run_test(test_remove_keyCollisions_1);
    mu_run_test(test_remove_keyCollisions_2);
    mu_run_test(test_remove_keyCollisions_3);
    mu_run_test(test_remove_4);
    mu_run_test(test_contains);

    return NULL;
}

RUN_TESTS(all_tests)
