#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int some_int = 70;
    void *ptr = (void *) (intptr_t) some_int;

    int the_int = (intptr_t) ptr;

    printf("the_int = %d\n", the_int);

    return 0;
}
