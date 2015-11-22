#include <stdio.h>
#include <assert.h>

#include <liblcthw/bstrlib.h>
#include <liblcthw/hashmap.h>
#include <liblcthw/hashmap_algorithms.h>
#include <liblcthw/dynamic_array.h>

#include "minunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

char *test_fnv1a()
{
    uint32_t hash = Hashmap_fnv1a_hash(&test1);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_fnv1a_hash(&test2);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_fnv1a_hash(&test3);
    assert(hash != 0 && "Bad hash.");

    return NULL;
}

char *test_adler32()
{
    uint32_t hash = Hashmap_adler32_hash(&test1);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_adler32_hash(&test2);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_adler32_hash(&test3);
    assert(hash != 0 && "Bad hash.");

    return NULL;
}

char *test_djb()
{
    uint32_t hash = Hashmap_djb_hash(&test1);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_djb_hash(&test2);
    assert(hash != 0 && "Bad hash.");

    hash = Hashmap_djb_hash(&test3);
    assert(hash != 0 && "Bad hash.");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_fnv1a);
    mu_run_test(test_adler32);
    mu_run_test(test_djb);

    return NULL;
}

RUN_TESTS(all_tests)
