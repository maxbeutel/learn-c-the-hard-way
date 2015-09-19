#include <stdlib.h>
#include <assert.h>

// debug:
#include <stdio.h>

#include "liblcthw/dynamic_array.h"

#define UNUSED(x) (void)(x)

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    assert(initial_max > 0 && "You must set an initial_max > 0.");

    DArray *array = malloc(sizeof(DArray));
    assert(array != NULL);
    array->max = initial_max;

    array->contents = calloc(initial_max, sizeof(void *));
    assert(array->contents != NULL); // @FIXME goto und free(array)
    
    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;

    return array;
}

void DArray_destroy(DArray *array)
{
    if (array) {
        free(array->contents);
    }
    
    free(array);
}

int DArray_expand(DArray *array) 
{
    UNUSED(array);
    
    return -1;
}

int DArray_contract(DArray *array)
{
    UNUSED(array);
    
    return -1;
}

int DArray_push(DArray *array, void *el)
{
    assert(array != NULL);

    array->contents[array->end] = el;
    array->end++;

    return 0;
}

void *DArray_pop(DArray *array)
{
    void *value = array->contents[(array->end - 1)];
    array->end--;
    
    return value;
}
