#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <liblcthw/bstrlib.h>

// DEBUG
#include <stdio.h>

#include "hashmap.h"

static int default_compare(void *key_a, void *key_b)
{
    return bstrcmp((bstring) key_a, (bstring) key_b);
}

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t jenkins_one_at_a_time_hash(char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return (hash >> 0);
}

static uint32_t default_hash(void *key)
{
    assert(key != NULL);

    size_t len = blength((bstring) key);
    char *raw_key = bdata((bstring) key);

    return jenkins_one_at_a_time_hash(raw_key, len);
}

// @TODO make this nicer
#define HASHMAP_INITIAL_SIZE 8
#define HASHMAP_INVALID_INDEX ((uint32_t) -1)

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
    Hashmap *map = calloc(1, sizeof(Hashmap));
    assert(map != NULL);
    // @TODO proper error handling for all calloc in this function

    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash  == NULL ? default_hash : hash;

    // @TODO optimization is to allocate one chunk of memory for both
    map->arData = calloc(HASHMAP_INITIAL_SIZE, sizeof(HashmapBucket));
    assert(map->arData != NULL);

    /* map->arHash = calloc(HASHMAP_INITIAL_SIZE, sizeof(uint32_t)); */
    /* assert(map->arHash != NULL); */
    map->arHash = malloc(sizeof(uint32_t) * HASHMAP_INITIAL_SIZE);
    assert(map->arHash != NULL);
    memset(map->arHash, HASHMAP_INVALID_INDEX, sizeof(uint32_t) * HASHMAP_INITIAL_SIZE);

    map->size = 0;

    return map;
}

void Hashmap_destroy(Hashmap *map)
{
    if (map) {
        free(map->arData);
        free(map->arHash);
    }

    free(map);
}

int Hashmap_set(Hashmap *map, void *key, void *data)
{
    assert(map != NULL);
    assert(key != NULL);
    //    assert(map->size < INT_MAX);
    assert(map->size < HASHMAP_INITIAL_SIZE);

    uint32_t hash = map->hash(key);
    printf("hash => %u\n", hash);

    // save index of actual data
    // @TODO HASHMAP_INITIAL_SIZE should be the current physical size of map->arData
    uint32_t hash_index = hash % HASHMAP_INITIAL_SIZE;
    assert(hash_index >= 0);
    assert(hash_index < HASHMAP_INITIAL_SIZE);

    uint32_t bucket_index = map->arHash[hash_index];
    printf("hash index %u\n", hash_index);
    printf("hash index int %d\n", hash_index);
    printf("bucket index %u\n", bucket_index);


    uint32_t current_value_next = HASHMAP_INVALID_INDEX;


    // found a collision
    // the bucket this hash would go to is already defined
    //map->arData[bucket_index].is_defined && map->arData[bucket_index].hash == hash
    if (map->size != 0 && bucket_index != HASHMAP_INVALID_INDEX) {
        printf("Already defined\n");

        uint32_t next_bucket_index = map->arData[bucket_index].next;

        if (next_bucket_index != HASHMAP_INVALID_INDEX) {
            current_value_next = next_bucket_index;
        }

         map->arData[bucket_index].next = map->size;
    // completely new entry
    } else {
        printf("Not yet defined, setting new entry in arHash\n");

        map->arHash[hash_index] = map->size;
    }

    // assign data to always growing arData
    map->arData[map->size].hash = hash;
    map->arData[map->size].key = key;
    map->arData[map->size].data = data;
    map->arData[map->size].next = current_value_next;
    map->arData[map->size].is_defined = 1;

    map->size++;

    return 0;
}

void *Hashmap_get(Hashmap *map, void *key)
{
    assert(map != NULL);
    assert(key != NULL);

    uint32_t hash = map->hash(key);
    uint32_t hash_index = hash & (HASHMAP_INITIAL_SIZE - 1);

    uint32_t data_index = map->arHash[hash_index];

    while (data_index != HASHMAP_INVALID_INDEX) {
        printf("iterating at %u\n", data_index);
        HashmapBucket bucket = map->arData[data_index];

        /* if (hash == bucket.hash) printf("hash matches bucket hash\n"); */
        /* else printf("hash and bucket hash not matching\n"); */

        /* if (map->compare(key, bucket.key) == 0) printf("key and bucket key matching\n"); */
        /* else printf("key and bucket key not matching\n"); */

        if (hash == bucket.hash && map->compare(key, bucket.key) == 0) {
            return bucket.data;
        }

        data_index = bucket.next;
    }

    return NULL;
}

void *Hashmap_remove(Hashmap *map, void *key)
{
    assert(map != NULL);
    assert(key != NULL);

    uint32_t hash = map->hash(key);
    uint32_t bucket_n = hash % HASHMAP_INITIAL_SIZE; // @TODO this should be the current physical size
    assert(bucket_n >= 0);

    int data_index = map->arHash[bucket_n];
    assert(data_index >= 0);
    assert(data_index < map->size);

    void *node_data = map->arData[data_index].data;

    map->arData[data_index].hash = 0;
    map->arData[data_index].key = NULL;
    map->arData[data_index].data = NULL;
    map->arData[data_index].is_defined = 0;

    map->arHash[bucket_n] = data_index;

    map->size++;

    return node_data;
}

void Hashmap_debug_dump(Hashmap *map)
{
    assert(map != 0);

    for (int i = 0; i < HASHMAP_INITIAL_SIZE; i++) {
        HashmapBucket bucket = map->arData[i];

        printf("IDX %d\n", i);

        if (!bucket.is_defined) {
            printf("\tundefined\n");
            continue;
        }

        printf("\thash => '%u'\n", bucket.hash);
        printf("\tnext => '%u'\n", bucket.next);
    }

    printf("\n");

    for (int i = 0; i < HASHMAP_INITIAL_SIZE; i++) {
        printf("arHash[%d] = dataIndex '%u'\n", i, map->arHash[i]);
    }

    printf("==================\n");
}
