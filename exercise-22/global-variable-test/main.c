#include <stdio.h>
#include "main.h"
#include "module.h"

int GLOBAL_VARIABLE = 0;

int main(int argc, char *argv[])
{
    GLOBAL_VARIABLE++;
    printf("GLOBAL_VARIABLE in main = %d\n", GLOBAL_VARIABLE);
    
    increase_global_variable();
    
    printf("===========\n");
    
    GLOBAL_VARIABLE++;
    printf("GLOBAL_VARIABLE in main = %d\n", GLOBAL_VARIABLE);
    
    
    increase_global_variable();
    
    printf("===========\n");
    
    
    return 0;
}
