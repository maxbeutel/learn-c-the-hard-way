#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *buf = malloc(sizeof(char) * 10);
    
    printf("Enter data: ");
    fgets(buf, 10, stdin);
    
    //printf("len => %lu\n", strlen(buf));
    printf("%s\n", buf);
    
    free(buf);
    
    return 0;
}
