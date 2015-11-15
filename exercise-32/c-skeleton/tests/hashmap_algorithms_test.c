#include <stdio.h>
#include <assert.h>

#include <liblcthw/bstrlib.h>
#include <liblcthw/hashmap.h>
#include <liblcthw/hashmap_algorithms.h>
#include <liblcthw/dynamic_array.h>

#include "minunit.h"

#define UNUSED(x) (void)(x)

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

#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 1000

enum { ALGO_FNV1A = 0, ALGO_ADLER32, ALGO_DJB  };

void gen_keys(DArray *keys, int num_keys)
{
    FILE *urand = fopen("/dev/urandom", "r");
    assert(urand != NULL);

    struct bStream *stream = bsopen((bNread) fread, urand);
    assert(stream != NULL);

    bstring key = bfromcstr("");
    int rc = 0;

    // FNV1a histogram
    for (int i = 0; i < num_keys; i++) {
        rc = bsread(key, stream, BUFFER_LEN);
        assert(rc >= 0);

        DArray_push(keys, bstrcpy(key));
    }

    bsclose(stream);
    fclose(urand);
}

void destroy_keys(DArray *keys)
{
    int index = -1;

    while (DArray_iterator_next(keys, &index)) {
        bdestroy(DArray_get(keys, index));
    }

    DArray_destroy(keys);
}

void fill_distribution(int *stats, DArray *keys, Hashmap_hash hash_func)
{
    int index = -1;

    while (DArray_iterator_next(keys, &index)) {
        uint32_t hash = hash_func(DArray_get(keys, index));
        stats[hash % BUCKETS] += 1;
    }
}

char *test_distributions()
{
    int stats[3][BUCKETS] = {{ 0 }};

    // @TODO refactor DArray_create and remove first (and internally unused) argument of DArray_create()
    DArray *keys = DArray_create(1, NUM_KEYS);
    assert(keys != NULL);

    gen_keys(keys, NUM_KEYS);

    fill_distribution(stats[ALGO_FNV1A], keys, Hashmap_fnv1a_hash);
    fill_distribution(stats[ALGO_ADLER32], keys, Hashmap_adler32_hash);
    fill_distribution(stats[ALGO_DJB], keys, Hashmap_djb_hash);

    fprintf(stderr, "FNV\tA32\tDJB\n");

    for (int i = 0; i < BUCKETS; i++) {
        fprintf(
            stderr,
            "%d\t%d\t%d\n",
            stats[ALGO_FNV1A][i],
            stats[ALGO_ADLER32][i],
            stats[ALGO_DJB][i]
        );
    }

    destroy_keys(keys);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_fnv1a);
    mu_run_test(test_adler32);
    mu_run_test(test_djb);
    mu_run_test(test_distributions);

    return NULL;
}

RUN_TESTS(all_tests)
