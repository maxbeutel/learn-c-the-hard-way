#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    char *ptr = malloc(sizeof(char) * 3);
    assert(ptr != NULL);

    ptr[0] = 'a';
    ptr[1] = 'b';

    printf("%s\n", ptr);

    ptr = realloc(ptr, sizeof(char) * 5); // + 2
    assert(ptr != NULL);

    ptr[2] = 'c';
    ptr[3] = 'd';

    printf("%s\n", ptr);

    memset(ptr + 2, 0, 1);
    printf("%s\n", ptr);

    free(ptr);


//    {
//        char *ptr = malloc(sizeof(char) * 800);
//        printf("%s\n", ptr);
//        free(ptr);
//    }

    return 0;
}
