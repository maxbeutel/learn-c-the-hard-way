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

    for (hash = i = 0; i < len; ++i) {
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

    map->arData = calloc(HASHMAP_INITIAL_SIZE, sizeof(HashmapBucket));
    assert(map->arData != NULL);

    map->arHash = malloc(sizeof(uint32_t) * HASHMAP_INITIAL_SIZE);
    assert(map->arHash != NULL);
    memset(map->arHash, HASHMAP_INVALID_INDEX, sizeof(uint32_t) * HASHMAP_INITIAL_SIZE);

    map->size = 0;
    map->capacity = HASHMAP_INITIAL_SIZE;

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

static int Hashmap_isFull(Hashmap *map)
{
    return map->capacity > (2 ^ 30);
}

static int Hashmap_computeHashIndex(Hashmap *map, uint32_t hash)
{
    assert(map != NULL);

    int hash_index = hash % map->capacity;
    assert(hash_index >= 0);
    assert(hash_index < map->capacity);

    return hash_index;
}

static int Hashmap_remapKeys(Hashmap *map, int old_capacity)
{
    assert(map != NULL);

    // we assume that map is already resized at this moment,
    // so we only traverse half of it
    for (int hash_index = 0; hash_index < old_capacity; hash_index++) {
        uint32_t data_index = map->arHash[hash_index];
        uint32_t previous_data_index = HASHMAP_INVALID_INDEX;

        int i = 0;

        while (data_index != HASHMAP_INVALID_INDEX) {
            HashmapBucket *bucket = &map->arData[data_index];

            // printf("> at data_index %d\n", data_index);
            // printf("> previous: %d\n", previous_data_index);

            // when a bucket is not defined, we assume it doesn't have any data or siblings
            if (!bucket->is_defined) {
                // printf("bucket not defined, ending traversal\n");
                break;
            }

            uint32_t next_data_index = bucket->next;
            int remapped_hash_index = Hashmap_computeHashIndex(map, bucket->hash);

            if (remapped_hash_index != hash_index) {
                // printf("> need to put bucket in %d, was %d\n", remapped_hash_index, hash_index);

                // update old arHash if this bucket is current head of collision list
                // as this bucket will be moved, .next of this bucket is now head of
                // collision list
                if (map->arHash[hash_index] == data_index) {
                    map->arHash[hash_index] = bucket->next;
                }

                // if current bucket has a previous bucket in the collision list,
                // (meaning its not the head of the collision list)
                // we need to update the previous entry to not point to this bucket anymore
                // but only if the previous bucket still points to current bucket!!!
                if (previous_data_index != HASHMAP_INVALID_INDEX && map->arData[previous_data_index].next == data_index) {
                    // printf("> setting %d.next to %d (was %d)\n", previous_data_index, bucket->next, map->arData[previous_data_index].next);
                    map->arData[previous_data_index].next = bucket->next;
                }

                uint32_t data_index_in_remapped_bucket = map->arHash[remapped_hash_index];
                int collision_found = (data_index_in_remapped_bucket != HASHMAP_INVALID_INDEX);

                // update arHash, with index of newly created arData entry
                map->arHash[remapped_hash_index] = data_index;

                if (collision_found) {
                    // printf("> collision found, already data in remapped bucket: %d\n", data_index_in_remapped_bucket);
                    bucket->next = data_index_in_remapped_bucket;
                } else {
                    // printf("> no collision found data_index %d is now head of collision list at %d\n", data_index, remapped_hash_index);
                    bucket->next = HASHMAP_INVALID_INDEX;
                }
            }

            // printf("-------------------\n");

            previous_data_index = data_index;
            data_index = next_data_index;

            i++;
        }
    }

    return 0;
}

static int Hashmap_doExpand(Hashmap *map, int old_capacity, int expanded_capacity)
{
    assert(map != NULL);

    HashmapBucket *arData_expanded = realloc(map->arData, sizeof(HashmapBucket) * expanded_capacity);
    assert(arData_expanded != NULL);

    uint32_t *arHash_expanded = realloc(map->arHash, sizeof(uint32_t) * expanded_capacity);
    assert(arHash_expanded != NULL);

    memset(
        arHash_expanded + old_capacity, // leave first half of new allocated memory alone
        HASHMAP_INVALID_INDEX,
        sizeof(uint32_t) * (expanded_capacity - old_capacity) // ... and set second half of allocated memory to HASHMAP_INVALID_INDEX
    );

    map->arData = arData_expanded;
    map->arHash = arHash_expanded;

    map->capacity = expanded_capacity;

    return 0;
}

static int Hashmap_expand(Hashmap *map)
{
    assert(map != NULL);

    if (Hashmap_isFull(map)) {
        return 1;
    }

    int old_capacity = map->capacity;
    int expanded_capacity = map->capacity * 2;

    // @TODO check and handle return values
    Hashmap_doExpand(map, old_capacity, expanded_capacity);
    Hashmap_remapKeys(map, old_capacity);

    return 0;
}

static int Hashmap_mustExpandForNewElement(Hashmap *map)
{
    assert(map != NULL);

    if (map->size == map->capacity) {
        return 1;
    }

    return 0;
}

int Hashmap_set(Hashmap *map, void *key, void *data)
{
    assert(map != NULL);
    assert(key != NULL);

    if (Hashmap_mustExpandForNewElement(map)) {
        assert(Hashmap_expand(map) == 0);
    }

    assert(map->size < map->capacity);

    uint32_t hash = map->hash(key);

    // save index of actual data
    int hash_index = Hashmap_computeHashIndex(map, hash);

    // @TODO change this to int
    uint32_t data_index = map->arHash[hash_index];

    int collision_found = (data_index != HASHMAP_INVALID_INDEX);

    // assign data to always expanding arData
    HashmapBucket *bucket = &map->arData[map->size];

    bucket->hash = hash;
    bucket->key = key;
    bucket->data = data;
    bucket->next = (collision_found ? data_index : HASHMAP_INVALID_INDEX);
    bucket->is_defined = 1;

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
    int hash_index = Hashmap_computeHashIndex(map, hash);

    uint32_t data_index = map->arHash[hash_index];

    while (data_index != HASHMAP_INVALID_INDEX) {
        HashmapBucket *bucket = &map->arData[data_index];

        if (hash == bucket->hash && map->compare(key, bucket->key) == 0) {
            return bucket->data;
        }

        data_index = bucket->next;
    }

    return NULL;
}

void *Hashmap_remove(Hashmap *map, void *key)
{
    assert(map != NULL);
    assert(key != NULL);

    uint32_t hash = map->hash(key);
    int hash_index = Hashmap_computeHashIndex(map, hash);

    uint32_t data_index = map->arHash[hash_index];
    int i = 0;
    uint32_t previous_data_index = HASHMAP_INVALID_INDEX;

    while (data_index != HASHMAP_INVALID_INDEX) {
        HashmapBucket *bucket = &map->arData[data_index];

        if (hash == bucket->hash && map->compare(key, bucket->key) == 0) {
            int is_collision_list_head = (i == 0);

            // current entry is head of collision list, we need to update that
            if (is_collision_list_head) {
                map->arHash[hash_index] = bucket->next;
            }

            // keep collision linked list in tact
            if (previous_data_index != HASHMAP_INVALID_INDEX) {
                assert(map->arData[previous_data_index].next == data_index);

                map->arData[previous_data_index].next = bucket->next;
            }

            void *bucket_data = bucket->data;

            bucket->hash = 0;
            bucket->key = NULL;
            bucket->data = NULL;
            bucket->next = HASHMAP_INVALID_INDEX;
            bucket->is_defined = 0;

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
    int hash_index = Hashmap_computeHashIndex(map, hash);

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

    for (int i = 0; i < map->capacity; i++) {
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

    for (int i = 0; i < map->capacity; i++) {
        printf("arHash[%d] = dataIndex '%u'\n", i, map->arHash[i]);
    }

    printf("==================\n");
}
