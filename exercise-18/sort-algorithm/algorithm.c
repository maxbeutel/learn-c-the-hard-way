#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare(const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int* quicksort(int *numbers, int count)
{
    assert(numbers != NULL);

    int *numbers_sorted = malloc(sizeof(int) * count);
    assert(numbers_sorted != NULL);

    memcpy(numbers_sorted, numbers, sizeof(int) * count);

    qsort(numbers_sorted, count, sizeof(int), compare);
    
    return numbers_sorted;
}


int main(void)
{
    int data_count = 5;
    int *data = malloc(sizeof(int) * data_count);
    assert(data != NULL);
    
    int *data_2 = NULL;
    
    data[0] = 5;
    data[1] = 99;
    data[2] = 1;
    data[3] = 47;
    data[4] = 18;
    
    printf("before: ");
    
    // übung: zu pointer iteration umschreiben
    for (int i = 0; i < data_count; i++) {
        printf("%d ", data[i]);
    }
    
    printf("\n");
    
    data_2 = quicksort(data, data_count);
    
    printf("after: ");
    
    // übung: zu pointer iteration umschreiben
    for (int i = 0; i < data_count; i++) {
        printf("%d ", data_2[i]);
    }
    
    printf("\n");
    
    free(data_2);
    free(data);
    
    return 0;
}

