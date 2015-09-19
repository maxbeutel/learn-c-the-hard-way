#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(void)
{
    char *format = "Room %lu somedata\n";
    int len = snprintf(0, 0, format, LONG_MAX);

    printf("%ld\n", LONG_MIN);    
    printf("%d\n", len);
  
    /*
    char *format = "Room %lu somedata\n";
    char *description = malloc(sizeof(char) * strlen(format) + 1);

    sprintf(description, format, LONG_MAX);

    puts(description);
*/
    return 0;
}

