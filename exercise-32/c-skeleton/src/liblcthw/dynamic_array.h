#ifndef _lcthw_dynamic_array_h
#define _lcthw_dynamic_array_h

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_EXPAND_RATE 10

typedef struct DArray {
    // current length of the array
    // @FIXME some functions assume length as size_t (also watch int overflow!)
    int length;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DArray;

DArray *DArray_create(size_t element_size, int length);
void DArray_destroy(DArray *array);

void *DArray_get(DArray *array, int index);
void DArray_set(DArray *array, int index, void *el);

int DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

void *DArray_remove(DArray *array, int index);

#endif
