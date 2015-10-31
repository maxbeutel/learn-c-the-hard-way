#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <liblcthw/bstrlib.h>

#include "hashmap.h"

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t default_hash(void *a)
{
    size_t len = blength((bstring) a);
    char *key = bdata((bstring) a);

    uint32_t hash = 0;

    for (size_t i = 0; i < len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
    Hashmap *map = calloc(1, sizeof(Hashmap));
    assert(map != NULL);
    // @TODO proper error handling
    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash  == NULL ? default_hash : hash;
    map->buckets = DArray_create(sizeof(DArray *), DEFAULT_NUMBER_OF_BUCKETS);
    // "fake out expanding it" - what does that do?!
    assert(map->buckets != NULL);

    return map;
}

void Hashmap_destroy(Hashmap *map)
{
    if (map) {
        int i = -1;

        while (DArray_iterator_next(map->buckets, &i)) {
            DArray *bucket = DArray_get(map->buckets, i);
            int j = -1;

            while (DArray_iterator_next(bucket, &j)) {
                HashmapNode *node = DArray_get(bucket, j);
                free(node);
            }

            DArray_destroy(bucket);
        }

        DArray_destroy(map->buckets);
    }

    free(map);
}

static inline HashmapNode *HashmapNode_create(int hash, void *key, void *data)
{
    HashmapNode *node = calloc(1, sizeof(HashmapNode));
    assert(node != NULL);
    // @TODO proper error handling

    node->hash = hash;
    node->key = key;
    node->data = data;

    return node;
}

static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash_out)
{
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    assert(bucket_n >= 0);
    *hash_out = hash; // store it for the return so the caller can use it

    DArray *bucket = DArray_get(map->buckets, bucket_n);

    if (!bucket && create) {
        bucket = DArray_create(sizeof(void *), DEFAULT_NUMBER_OF_BUCKETS);
        assert(bucket != NULL);
        // @TODO proper error handling

        DArray_set(map->buckets, bucket_n, bucket);
    }

    return bucket;
}

int Hashmap_set(Hashmap *map, void *key, void *data)
{
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
    assert(bucket != NULL);

    HashmapNode *node = HashmapNode_create(hash, key, data);
    assert(node != NULL);

    DArray_push(bucket, node);

    return 0;
}

static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, void *key)
{
    int i = -1;

    while (DArray_iterator_next(bucket, &i)) {
        HashmapNode *node = DArray_get(bucket, i);

        if (node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }

    return -1;
}

void *Hashmap_get(Hashmap *map, void *key)
{
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);

    if (!bucket) {
        return NULL;
    }

    int i = Hashmap_get_node(map, hash, bucket, key);

    if (i == -1) {
        return NULL;
    }

    HashmapNode *node = DArray_get(bucket, i);
    assert(node != NULL);

    return node->data;
}

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb)
{
    int i = -1;

    while (DArray_iterator_next(map->buckets, &i)) {
        DArray *bucket = DArray_get(map->buckets, i);

        if (bucket) {
            int j = -1;

            while (DArray_iterator_next(bucket, &j)) {
                HashmapNode *node = DArray_get(bucket, j);

                int rc = traverse_cb(node); // why does the external traverse_cb get to know the internal Node structure?

                if (rc != 0) {
                    return rc; // ??? why?
                }
            }
        }
    }

    return 0;
}

void *Hashmap_delete(Hashmap *map, void *key)
{
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);

    if (!bucket) {
        return NULL;
    }

    int i = Hashmap_get_node(map, hash, bucket, key);

    if (i == -1) {
        return NULL;
    }

    HashmapNode *node = DArray_get(bucket, i);
    assert(node != NULL);

    void *data = node->data;

    // why is this needed?
    /* HashmapNode *ending = DArray_pop(bucket); */

    /* if(ending != node) { */
    /*     // alright looks like it's not the last one, swap it */
    /*     DArray_set(bucket, i, ending); */
    /* } */


    return data;
}
