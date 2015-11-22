#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include <liblcthw/bstrlib.h>
#include <liblcthw/hashmap.h>
#include <liblcthw/hashmap_algorithms.h>
#include <liblcthw/dynamic_array.h>

#define UNUSED(x) (void)(x)

static int fillWithRandomKeys(DArray *keys)
{
    assert(keys != NULL);

    errno = 0;
    FILE *urandfilePtr = fopen("/dev/urandom", "r");

    if (urandfilePtr == NULL) {
        return 1;
    }

    int rc = 0;

    for (int i = 0; i < keys->capacity; i++) {
        char str[10 + 1] = {0};
        int bytesRead = fread(str, sizeof(char), 10, urandfilePtr);

        if (bytesRead != 10) {
            rc = 1;
            goto end;
        }

        bstring key = bfromcstr(str);
        assert(key != NULL);

        DArray_push(keys, key);
    }

    assert(keys->size > 0);
    assert(keys->size == keys->capacity);

end:
    errno = 0;
    int closeSuccess = fclose(urandfilePtr);

    if (closeSuccess != 0) {
        return 2;
    }

    return rc;
}

static void distributeKeysOnBuckets(DArray *keys, int num_buckets, Hashmap_hash hash_func, int *statistics)
{
    assert(keys != NULL);
    assert(num_buckets > 0);
    assert(hash_func != NULL);
    assert(statistics != NULL);

    int index = -1;

    while (DArray_iterator_next(keys, &index)) {
        bstring value = (bstring) DArray_get(keys, index);
        uint32_t hash = hash_func(value);

        int index = hash % num_buckets;

        statistics[index]++;
    }
}

static void dumpStatistics(FILE *outputFilePtr, char **statisticsLabels, int *statistics, int numRows, int numCols)
{
    assert(outputFilePtr != NULL);

    for (int i = 0; i < numRows; i++) {
        printf("%s\t", *(statisticsLabels + i));

        for (int j = 0; j < numCols; j++) {
            int *value = statistics + (i * numCols) + j;
            fprintf(outputFilePtr, "%d\t", *value);
        }

        fprintf(outputFilePtr, "\n");
    }
}

static void freeRandomKeys(DArray *keys)
{
    int index = -1;

    while (DArray_iterator_next(keys, &index)) {
        bstring key = (bstring) DArray_get(keys, index);
        bdestroy(key);
    }
}

int main(void)
{
    int numKeys = 100 /* * 10*/;
    int numBuckets = 100;

    char *statisticsLabels[3] = {"FNV", "A32", "DJB"};
    int *statistics = calloc(3 * numBuckets, sizeof(int));
    assert(statistics != NULL);

    DArray *keys = DArray_create(1, numKeys);
    assert(keys != NULL);

    int rc = 0;

    if (fillWithRandomKeys(keys) != 0) {
        rc = 1;
        goto end;
    }

    distributeKeysOnBuckets(keys, numBuckets, Hashmap_fnv1a_hash, statistics);
    distributeKeysOnBuckets(keys, numBuckets, Hashmap_adler32_hash, statistics + numBuckets);
    distributeKeysOnBuckets(keys, numBuckets, Hashmap_djb_hash, statistics + (2 * numBuckets));

    dumpStatistics(stdout, statisticsLabels, statistics, 3, numBuckets);

end:
    if (keys != NULL) {
        freeRandomKeys(keys);

        DArray_destroy(keys);
    }

    if (statistics != NULL) {
        free(statistics);
    }

    return rc;
}
