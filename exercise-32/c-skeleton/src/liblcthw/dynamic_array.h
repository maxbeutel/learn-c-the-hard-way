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

int DArray_expand(DArray *array);
int DArray_contract(DArray *array);

int DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

#define DEFAULT_EXPAND_RATE 300

// @FIXME why does this not expand?
static inline void DArray_set(DArray *array, int index, void *el)
{
    assert(index < array->length && "DArray attempt to set undefined index.");
    
    array->contents[index] = el;
}

static inline void *DArray_get(DArray *array, int index)
{
    assert(index < array->max && "DArray attempt to get undefined index.");

    return array->contents[i];
}

static inline void *DArray_remove(DArray *array, int index)
{
    assert(index < array->max && "DArray attempt to remove undefined index.");
    
    void *el = array->contents[index];
    
    array->contents[index] = NULL;
    
    return el;
}

#endif
