#include <stdlib.h>
#include <assert.h>

// debug:
#include <stdio.h>

#include "liblcthw/dynamic_array.h"

// debug, in order to avoid compiler warnings for not yet implemented functions
#define UNUSED(x) (void)(x)

DArray *DArray_create(size_t element_size, size_t length)
{
    assert(length > 0 && "You must set a length > 0.");

    DArray *array = malloc(sizeof(DArray));
    assert(array != NULL);
    array->length = length;
    array->elements_added = 0;

    array->contents = calloc(length, sizeof(void *));
    assert(array->contents != NULL); // @FIXME goto und free(array)

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

// @FIXME why does this not expand?
// @FIXME this must expand if elements_added == length
int DArray_push(DArray *array, void *el)
{
    assert(array != NULL);

    array->contents[array->length] = el;

    return 0; // does a return value make sense here?
}

// @FIXME this must then resize (contract) ?
void *DArray_pop(DArray *array)
{
    assert(array != NULL);

    void *value = array->contents[(array->length - 1)];
    
    return value;
}
