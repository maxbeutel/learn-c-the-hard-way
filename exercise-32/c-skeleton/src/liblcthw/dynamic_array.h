#ifndef _lcthw_dynamic_array_h
#define _lcthw_dynamic_array_h

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_EXPAND_RATE 10

typedef struct DArray {
    // @FIXME watch int overflows here!
    int size; // logical size, number of elements
    int capacity; // physical size
    int expand_rate;
    int* dirty_indexes;
    size_t element_size;
    void **contents;
} DArray;

DArray *DArray_create(size_t element_size, int initial_capacity);
void DArray_destroy(DArray *array);

void *DArray_get(DArray *array, int index);
void DArray_set(DArray *array, int index, void *el);

void DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

void *DArray_remove(DArray *array, int index);

#endif
