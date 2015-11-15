#ifndef _lcthw_hashmap_algorithms_h
#define _lcthw_hashmap_algorithms_h

#include <stdint.h>

uint32_t Hashmap_fnv1a_hash(void *data);

uint32_t Hashmap_adler32_hash(void *data);

uint32_t Hashmap_djb_hash(void *data);

#endif
