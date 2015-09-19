#include <stdio.h>
#include <assert.h>

int sample_someFunc(char *input)
{
    assert(input != NULL);
    
    printf("Input is: %s\n", input);
    
    return 1;
}
