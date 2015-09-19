#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    
    exit(1);
}

typedef int (*compare_cb)(const void *a, const void *b);
typedef int* (*algorithm_cb)(int *numbers, int count, compare_cb cb);

int *bubblesort(int *numbers, int count, compare_cb cmp)
{
    assert(numbers != NULL);
    assert(cmp != NULL);
    
    int tmp = 0;

    int *target = malloc(count * sizeof(int));
    assert(target != NULL);
    
    memcpy(target, numbers, count * sizeof(int));
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count - 1; j++) {
            if (cmp(&target[j], &target[j + 1]) > 0) {
                tmp = target[j + 1];
                target[j + 1] = target[j];
                target[j] = tmp;
            }
        }
    }
    
    return target;
}

int* quicksort(int *numbers, int count, compare_cb cmp)
{
    assert(numbers != NULL);
    assert(cmp != NULL);

    int *numbers_sorted = malloc(sizeof(int) * count);
    assert(numbers_sorted != NULL);

    memcpy(numbers_sorted, numbers, sizeof(int) * count);

    qsort(numbers_sorted, count, sizeof(int), cmp);
    
    return numbers_sorted;
}


int sorted_order(const void *a, const void *b)
{
    return *((int *) a) - *((int *) b);
}

int reverse_order(const void *a, const void *b)
{
    return *((int *) b) - *((int *) a);
}

void test_sorting(int *numbers, int count, algorithm_cb algo, compare_cb cmp)
{
    assert(numbers != NULL);
    assert(algo != NULL);
    assert(cmp != NULL);
    
    int *sorted = algo(numbers, count, cmp);
    
    for (int i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    
    printf("\n");
    
    free(sorted);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        die("USAGE: ex18 4 3 1 5 6");
    }
    
    int count = argc - 1;
    char **inputs = argv + 1;
    
    int *numbers = malloc(count * sizeof(int));
    assert(numbers != NULL); 
    
    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, quicksort, sorted_order);
    test_sorting(numbers, count, quicksort, reverse_order);

    test_sorting(numbers, count, bubblesort, sorted_order);
    test_sorting(numbers, count, bubblesort, reverse_order);

    free(numbers);
    
    return 0;
}
