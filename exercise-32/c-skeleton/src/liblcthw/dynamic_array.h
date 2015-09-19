#ifndef _lcthw_dynamic_array_h
#define _lcthw_dynamic_array_h

#include <stdlib.h>
#include <assert.h>

typedef struct DArray {
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(DArray *array);

int DArray_expand(DArray *array);
int DArray_contract(DArray *array);

int DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_end(A) ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A) ((A)->max)

#define DEFAULT_EXPAND_RATE 300

// @FIXME why does this not expand?
static inline void DArray_set(DArray *array, int i, void *el)
{
    assert(i < array->max && "DArray attempt to get past max.");
    
    if (i > array->end) {
        array->end = i;
    }
    
    array->contents[i] = el;
}

static inline void *DArray_get(DArray *array, int i)
{
    assert(i < array->max && "DArray attempt to get past max.");
    
    return array->contents[i];
}

static inline void *DArray_remove(DArray *array, int i)
{
    assert(i < array->max && "DArray attempt to get past max.");
    
    void *el = array->contents[i];
    
    array->contents[i] = NULL;
    
    return el;
}

// why do we actually need this?
// #define DArray_free(E) free((E))
/*static inline void *DArray_new(DArray *array)
{
    assert(array->element_size > 0 && "Can't use DArray_new on 0 size darrays.");
    
    return calloc(1, array->element_size);
}*/

#endif
