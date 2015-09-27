#ifndef _lcthw_dynamic_array_h
#define _lcthw_dynamic_array_h

#include <stdlib.h>
#include <assert.h>

typedef struct DArray {
    // current length of the array
    int length;
    // number of elements that have been added so far (@FIXME possible buffer overflow)
    int elements_added;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t length);
void DArray_destroy(DArray *array);

// should be internal @TODO prefix underscore
int DArray_expand(DArray *array);
// should be internal @TODO prefix underscore
int DArray_contract(DArray *array);

int DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

#define DEFAULT_EXPAND_RATE 10

static inline void DArray_set(DArray *array, int index, void *el)
{
    assert(array != NULL);
    assert(index >= 0 && "DArray attempt to get negative index");

    if (index >= array->length) {
        DArray_expand(array);
    }

    assert(index < array->length && "DArray attempt to set undefined index.");

    array->contents[index] = el;
}

static inline void *DArray_get(DArray *array, int index)
{
    assert(index < array->length && "DArray attempt to get undefined index.");

    return array->contents[index];
}

static inline void *DArray_remove(DArray *array, int index)
{
    assert(index < array->length && "DArray attempt to remove undefined index.");

    void *el = array->contents[index];

    array->contents[index] = NULL;

    return el;
}

#endif
