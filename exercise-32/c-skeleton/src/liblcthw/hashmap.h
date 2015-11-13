#ifndef _lcthw_hashmap_h
#define _lcthw_hashmap_h

#include <stdint.h>

#include <liblcthw/dynamic_array.h>


#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare)(void *a, void *b);
typedef uint32_t (*Hashmap_hash)(void *key);

typedef struct HashmapBucket {
    uint32_t hash;
    void *key;
    void *data;
    uint32_t next;
    int is_defined;
} HashmapBucket;

typedef struct Hashmap {
    Hashmap_compare compare;
    Hashmap_hash hash;
    HashmapBucket *arData;
    uint32_t *arHash;
    int size;
} Hashmap;

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash);
void Hashmap_destroy(Hashmap *map);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);
void *Hashmap_remove(Hashmap *map, void *key);

int Hashmap_iterator_next(Hashmap *map, int *index, void **key_out, void **data_out);
int Hashmap_iterator_prev(Hashmap *map, int *index, void **key_out, void **data_out);

int Hashmap_contains(Hashmap *map, void *key);

void Hashmap_debug_dump(Hashmap *map);

#endif
