#include <stdio.h>

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5};
    char chars[] = {'a', 'i'};
    char name[] = "Max";
    
    name[1] = chars[1];
    
    printf("name (before) = %s\n", name);
    
    name[1] = numbers[1];
    
    printf("name (after) = %s\n", name);
    
    return 0;
}