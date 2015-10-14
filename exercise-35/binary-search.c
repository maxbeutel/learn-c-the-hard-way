#include <stdio.h>
#include <math.h>
#include <assert.h>

static inline void do_binary_search(int value, int *array, int min, int max, int *index_of_value)
{
    assert(array != NULL);
    assert(index_of_value != NULL);

    if (min > max) {
        return;
    }

    int midpoint = floor(((max - min) / 2) + min);
    int current_value = array[midpoint];

    if (current_value > value) {
        do_binary_search(value, array, min, midpoint - 1, index_of_value);
    } else if (current_value < value) {
        do_binary_search(value, array, midpoint + 1, max, index_of_value);
    } else {
        *index_of_value = midpoint;
    }
}

static int binary_search(int value, int *array, int array_length)
{
    assert(array != NULL);

    int index_of_value = -1;

    if (array_length > 0) {
        int min = 0;
        int max = array_length - 1;

        do_binary_search(value, array, min, max, &index_of_value);
    }

    return index_of_value;
}

int main(void)
{
    int array[10] = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
    };
    int array_length = 10;

    {
        int value = 8;
        int index = binary_search(value, array, array_length);

        printf("found value \"%d\" at index \"%d\".\n", value, index);
    }

    {
        int value = 11;
        int index = binary_search(value, array, array_length);

        printf("found value \"%d\" at index \"%d\".\n", value, index);
    }

    {
        int value = 0;
        int index = binary_search(value, array, array_length);

        printf("found value \"%d\" at index \"%d\".\n", value, index);
    }

    return 0;
}
