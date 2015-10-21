#ifndef _lcthw_dynamic_array_h
#define _lcthw_dynamic_array_h

#include <stdlib.h>
#include <assert.h>

#define DARRAY_DEFAULT_EXPAND_RATE 10

typedef struct DArray {
    int size; // logical size, number of elements
    int capacity; // physical size
    int expand_rate;
    int* dirty_indexes;
    size_t element_size;
    void **contents;
} DArray;

typedef int (*DArray_predicate)(void *search_element, void *current_element, int index);

DArray *DArray_create(size_t element_size, int initial_capacity);
void DArray_destroy(DArray *array);

void *DArray_get(DArray *array, int index);
void DArray_set(DArray *array, int index, void *element);

void DArray_push(DArray *array, void *element);
void *DArray_pop(DArray *array);

void *DArray_remove(DArray *array, int index);

int DArray_contains(DArray *array, void *search_element, DArray_predicate predicate);

int DArray_iterator_next(DArray *array, int *index);
int DArray_iterator_prev(DArray *array, int *index);

#endif
