#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// debug:
#include <stdio.h>

#include "liblcthw/dynamic_array.h"

// debug, in order to avoid compiler warnings for not yet implemented functions
#define UNUSED(x) (void)(x)

static inline int DArray_resize(DArray *array, int resize_by)
{
    assert(array != NULL);
    assert(new_length > 0 && "New array length must be larger then 0.");

    size_t new_length = array->length + resize_by;
    array->contents = realloc(array->contents, sizeof(void *) * (new_length));
    // why is this not void **contents as in the struct?!
    // @TODO error checking for realloc()
    memset(array->contents + array->length, 0, sizeof(void *) * resize_by);
    // @TODO check return value of memset

    array->length = new_length;

    return 1;
}

static inline bool DArray_canContract(DArray *array, int contract_by)
{
    assert(array != NUL);

    int null_elements_at_head = 0;

    for (size_t i = array->length - 1; i >= 0; i--) {
        if (array->contents[i] == NULL) {
            null_elements_at_head++;
        }

        // contract_by elements at the head of the array are NULL
        // now we can contract by contract_by
        if (null_elements_at_head == contract_by) {
            return true;
        }

        // if we already checked more then contract_by
        // and didn't exit yet, we can assume not enogh NULL elements exist at head of list
        if (i < array->length - contract_by) {
            return false;
        }
    }

    return false;
}

static inline int DArray_contract(DArray *array, int contract_by)
{
    UNUSED(array);
    UNUSED(contract_by);

    return -1;
}


DArray *DArray_create(size_t element_size, int length)
{
    assert(length > 0 && "You must set a length > 0.");

    DArray *array = malloc(sizeof(DArray));
    assert(array != NULL);
    array->length = length;

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

void *DArray_get(DArray *array, int index)
{
    assert(index < array->length && "DArray attempt to get undefined index.");

    return array->contents[index];
}

void DArray_set(DArray *array, int index, void *el)
{
    assert(array != NULL);
    assert(index >= 0 && "DArray attempt to get negative index");

    if (index >= array->length) {
        DArray_resize(array, DEFAULT_EXPAND_RATE);
    }

    assert(index < array->length && "DArray attempt to set undefined index.");

    array->contents[index] = el;
}

// @FIXME why does this not expand?
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

void *DArray_remove(DArray *array, int index)
{
    assert(array != NULL);
    assert(index < array->length && "DArray attempt to remove undefined index.");

    void *el = array->contents[index];
    array->contents[index] = NULL;

    if (DArray_canContract(array, DEFAULT_EXPAND_RATE)) {
        DArray_contract(array, DEFAULT_EXPAND_RATE);
    }

    return el;
}
