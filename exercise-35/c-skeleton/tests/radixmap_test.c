#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <libradixmap/radixmap.h>

#include "minunit.h"


static void make_random(RadixMap *map)
{
    for (size_t i = 0; i < map->max - 1; i++) {
        uint32_t key = (uint32_t) (rand() | (rand() << 16));
        assert(RadixMap_add(map, key, i) == 0 && "Failed to add data to map.");
    }
}

static int check_order(RadixMap *map)
{
    RMElement d1, d2;

    // only signal errors (which should not be)
    for (unsigned int i = 0; map->end > 0 && i < map->end -1; i++) {
        d1 = map->contents[i];
        d2 = map->contents[i + 1];

        if (d1.data.key > d2.data.key) {
            printf(
                "FAIL:i=%u, key: %u, value: %u, equals max? %d\n",
                i,
                d1.data.key,
                d1.data.value,
                d2.data.key == UINT32_MAX
            );

            return 0;
        }
    }

    return 1;
}

static void test_search(RadixMap *map)
{
    RMElement *d = NULL;
    RMElement *found = NULL;

    for (unsigned int i = map->end / 2; i < map->end; i++) {
        d = &map->contents[i];
        found = RadixMap_find(map, d->data.key);

        assert(found != NULL && "Key not found in radixmap.");
        assert(found->data.key == d->data.key && "Wrong result returned from find operation.");
    }
}

static char *test_operations()
{
    size_t N = 200;

    RadixMap *map = RadixMap_create(N);
    assert(map != NULL);
    make_random(map);

    RadixMap_sort(map);
    assert(check_order(map) && "Failed to properly sort the RadixMap.");

    test_search(map);
    assert(check_order(map) && "RadixMap failed to staty sorted after search.");

    while (map->end > 0) {
        RMElement *el = RadixMap_find(map, map->contents[map->end / 2].data.key);
        assert(el != NULL && "Faield to get result from RadixMap_find.");

        size_t old_end = map->end;
        assert(RadixMap_delete(map, el) == 0 && "Failed to delete element from RadixMap.");
        assert(old_end - 1 == map->end && "Wrong size after deleting element from RadixMap.");
        assert(check_order(map) && "RadixMap failed to staty sorted after deleting element.");
    }

    RadixMap_destroy(map);

    return NULL;
}

char *all_tests()
{
    srand(time(NULL));

    mu_suite_start();

    mu_run_test(test_operations);

    return NULL;
}

RUN_TESTS(all_tests)
