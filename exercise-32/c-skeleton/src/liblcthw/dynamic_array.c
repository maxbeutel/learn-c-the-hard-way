#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#include "dynamic_array.h"

static int DArray_resize(DArray *array, int new_capacity)
{
    assert(array != NULL);
    assert(new_capacity > 0 && "New array capacity must be larger then 0.");
    assert(new_capacity != array->capacity && "Cowardly refusing to resize array to existing capacity.");

    array->contents = realloc(array->contents, sizeof(void *) * new_capacity);
    if (!array->contents) {
        goto error;
    }

    array->dirty_indexes = realloc(array->dirty_indexes, sizeof(int) * new_capacity);
    if (!array->dirty_indexes) {
        goto error;
    }

    int old_capacity = array->capacity;

    // on when expanding - nullify all new allocated memory from current head onwards
    // when contracting, this is not necessary
    if (new_capacity > old_capacity) {
        int capacity_increase = new_capacity - old_capacity;
        void *new_memory_start = NULL;

        new_memory_start = array->contents + old_capacity;
        memset(new_memory_start, 0, sizeof(void *) * capacity_increase);

        new_memory_start = array->dirty_indexes + old_capacity;
        memset(new_memory_start, 0, sizeof(int) * capacity_increase);
    }

    array->capacity = new_capacity;

    return 0;

error:
    free(array->contents);
    free(array->dirty_indexes);

    return -1;
}

static int DArray_canContract(DArray *array)
{
    assert(array != NULL);

    if (array->size == 0) {
        return false;
    }

    if (array->capacity <= array->expand_rate) {
        return false;
    }

    int clean_elements_at_head = 0;

    for (int i = array->capacity - 1; i >= 0; i--) {
        if (array->dirty_indexes[i]) {
            return false;
        } else {
            clean_elements_at_head++;
        }

        if (clean_elements_at_head == array->expand_rate) {
            return true;
        }
    }

    return false;
}

// @FIXME why do i need element_size ??? it seems like its nowhere used
DArray *DArray_create(size_t element_size, int initial_capacity)
{
    assert(element_size > 0 && "You must provide an element size > 0.");
    assert(initial_capacity > 0 && "You must set a initial_capacity > 0.");

    DArray *array = malloc(sizeof(DArray));
    if (!array) {
        goto error;
    }

    array->size = 0;
    array->capacity = initial_capacity;

    array->contents = calloc(initial_capacity, sizeof(void *));
    if (!array->contents) {
        goto error;
    }

    array->dirty_indexes = calloc(initial_capacity, sizeof(int));
    if (!array->dirty_indexes) {
        goto error;
    }

    array->element_size = element_size;
    array->expand_rate = DARRAY_DEFAULT_EXPAND_RATE;

    return array;

error:
    free(array);

    if (array) {
        free(array->contents);
        free(array->dirty_indexes);
    }

    return NULL;
}

void DArray_destroy(DArray *array)
{
    if (array) {
        free(array->contents);
        free(array->dirty_indexes);
    }

    free(array);
}

void *DArray_get(DArray *array, int index)
{
    assert(array != NULL);
    assert(index >= 0 && "DArray attempt to get negative index");
    assert(index < array->capacity && "DArray attempt to get undefined index.");

    return array->contents[index];
}

void DArray_set(DArray *array, int index, void *element)
{
    assert(array != NULL);
    assert(index >= 0 && "DArray attempt to get negative index");

    if (index >= array->capacity) {
        assert(DArray_resize(array, array->capacity + array->expand_rate) == 0);
    }

    array->size++;
    array->contents[index] = element;
    array->dirty_indexes[index] = 1;
}

void DArray_push(DArray *array, void *element)
{
    assert(array != NULL);
    assert(array->size < INT_MAX - 1 && "Maximum array capacity of INT_MAX reached.");

    if (array->size == 0) {
        DArray_set(array, 0, element);
        return;
    }

    for (int i = array->capacity - 1; i >= 0; i--) {
        if (array->dirty_indexes[i]) {
            DArray_set(array, i + 1, element);
            break;
        }
    }
}

void *DArray_pop(DArray *array)
{
    assert(array != NULL);

    if (array->size == 0) {
        return NULL;
    }

    for (int i = array->capacity - 1; i >= 0; i--) {
        if (array->dirty_indexes[i]) {
            void *element = DArray_get(array, i);
            DArray_remove(array, i);

            return element;
        }
    }

    return NULL;
}

void *DArray_remove(DArray *array, int index)
{
    assert(array != NULL);
    assert(index >= 0 && "DArray attempt to get negative index");
    assert(index < array->capacity && "DArray attempt to remove undefined index.");

    if (array->size == 0) {
        return NULL;
    }

    if (!array->dirty_indexes[index]) {
        return NULL;
    }

    void *element = array->contents[index];
    array->size--;
    array->contents[index] = NULL;
    array->dirty_indexes[index] = 0;

    if (DArray_canContract(array)) {
        assert(DArray_resize(array, array->capacity - array->expand_rate) == 0);
    }

    return element;
}

int DArray_contains(DArray *array, void *search_element, DArray_predicate predicate)
{
    assert(array != NULL);
    assert(predicate != NULL);

    for (int i = 0; i < array->capacity; i++) {
        if (array->dirty_indexes[i]) {
            void *element = DArray_get(array, i);

            if (predicate(search_element, element, i) == 1) {
                return 1;
            }
        }
    }

    return 0;
}

int DArray_iterator_next(DArray *array, int *index)
{
    assert(array != NULL);
    assert(index != NULL);
    assert(*index <= INT_MAX - 1);

    if (array->size == 0) {
        return 0;
    }

    // initially find first dirty index
    if (*index == -1) {
        for (int i = 0; i < array->capacity; i++) {
            if (array->dirty_indexes[i]) {
                *index = i;
                return 1;
            }
        }

        return 0;
    } else {
        assert(*index >= 0);
        assert(*index < array->capacity);
    }

    for (int i = *index + 1; i < array->capacity; i++) {
        if (array->dirty_indexes[i]) {
            *index = i;
            return 1;
        }
    }

    return 0;
}

int DArray_iterator_prev(DArray *array, int *index)
{
    assert(array != NULL);
    assert(index != NULL);

    if (array->size == 0) {
        return 0;
    }

    // initially find last dirty index
    if (*index == -1) {
        for (int i = array->capacity - 1; i >= 0; i--) {
            if (array->dirty_indexes[i]) {
                *index = i;
                return 1;
            }
        }

        return 0;
    } else {
        assert(*index >= 0);
        assert(*index < array->capacity);
    }

    for (int i = *index - 1; i >= 0; i--) {
        if (array->dirty_indexes[i]) {
            *index = i;
            return 1;
        }
    }

    return 0;
}
