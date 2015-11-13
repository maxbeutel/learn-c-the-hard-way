#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <liblcthw/bstrlib.h>

#include "hashmap.h"

// DEBUG
#include <stdio.h>
#define UNUSED(x) (void)(x)

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

    // save index of actual data
    // @TODO HASHMAP_INITIAL_SIZE should be the current physical size of map->arData
    uint32_t hash_index = hash % HASHMAP_INITIAL_SIZE;
    assert(hash_index >= 0);
    assert(hash_index < HASHMAP_INITIAL_SIZE);

    uint32_t data_index = map->arHash[hash_index];

    int collision_found = (data_index != HASHMAP_INVALID_INDEX);

    // assign data to always growing arData
    map->arData[map->size].hash = hash;
    map->arData[map->size].key = key;
    map->arData[map->size].data = data;
    map->arData[map->size].next = (collision_found ? data_index : HASHMAP_INVALID_INDEX);
    map->arData[map->size].is_defined = 1;

    // update arHash, with index of newly created arData entry
    map->arHash[hash_index] = map->size;

    map->size++;

    return 0;
}

void *Hashmap_get(Hashmap *map, void *key)
{
    assert(map != NULL);
    assert(key != NULL);

    uint32_t hash = map->hash(key);
    uint32_t hash_index = hash % HASHMAP_INITIAL_SIZE;

    uint32_t data_index = map->arHash[hash_index];

    while (data_index != HASHMAP_INVALID_INDEX) {
        HashmapBucket bucket = map->arData[data_index];

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
    uint32_t hash_index = hash % HASHMAP_INITIAL_SIZE; // @TODO this should be the current physical size

    uint32_t data_index = map->arHash[hash_index];
    int i = 0;
    uint32_t previous_data_index = HASHMAP_INVALID_INDEX;

    while (data_index != HASHMAP_INVALID_INDEX) {
        int is_collision_list_head = (i == 0);

        if (hash == map->arData[data_index].hash && map->compare(key, map->arData[data_index].key) == 0) {
            // current entry is head of collision list, we need to update that
            if (is_collision_list_head) {
                map->arHash[hash_index] = map->arData[data_index].next;
            }

            // keep collision linked list in tact
            if (previous_data_index != HASHMAP_INVALID_INDEX) {
                assert(map->arData[previous_data_index].next == data_index);

                map->arData[previous_data_index].next = map->arData[data_index].next;
            }

            void *bucket_data = map->arData[data_index].data;

            map->arData[data_index].hash = 0;
            map->arData[data_index].key = NULL;
            map->arData[data_index].data = NULL;
            map->arData[data_index].next = HASHMAP_INVALID_INDEX;
            map->arData[data_index].is_defined = 0;

            map->size--;

            return bucket_data;
        }

        previous_data_index = data_index;
        data_index = map->arData[data_index].next;

        i++;
    }

    return NULL;
}

int Hashmap_iterator_next(Hashmap *map, int *index, void **key_out, void **data_out)
{
    assert(map != NULL);
    assert(index != NULL);
    assert(*index < INT_MAX - 1);

    if (map->size == 0) {
        return 0;
    }

    if (*index < -1) {
        return 0;
    }

    for (int i = *index + 1; i < map->size; i++) {
        HashmapBucket bucket = map->arData[i];

        if (bucket.is_defined) {
            *index = i;

            *key_out = bucket.key;
            *data_out = bucket.data;

            // next iteration would be already out of bounds, so tell caller
            // to end iteration
            return (i == map->size - 1 ? 0 : 1);
        }
    }

    return 0;
}

int Hashmap_iterator_prev(Hashmap *map, int *index, void **key_out, void **data_out)
{
    assert(map != NULL);
    assert(index != NULL);
    assert(*index < INT_MAX - 1);

    if (map->size == 0) {
        return 0;
    }

    if (*index < -1) {
        return 0;
    }

    for (int i = (*index == -1 ? map->size - 1 : *index - 1); i >= 0; i--) {
        HashmapBucket bucket = map->arData[i];

        if (bucket.is_defined) {
            *index = i;

            *key_out = bucket.key;
            *data_out = bucket.data;

            // next iteration would be already out of bounds, so tell caller
            // to end iteration
            return (*index - 1 == -1 ? 0 : 1);
        }
    }

    return 0;
}

int Hashmap_contains(Hashmap *map, void *key)
{
    assert(map != NULL);
    assert(key != NULL);

    UNUSED(map);
    UNUSED(key);

    if (map->size == 0) {
        return 0;
    }

    uint32_t hash = map->hash(key);
    uint32_t hash_index = hash % HASHMAP_INITIAL_SIZE;

    uint32_t data_index = map->arHash[hash_index];

    while (data_index != HASHMAP_INVALID_INDEX) {
        HashmapBucket bucket = map->arData[data_index];

        if (hash == bucket.hash && map->compare(key, bucket.key) == 0) {
            return 1;
        }

        data_index = bucket.next;
    }

    return 0;
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
