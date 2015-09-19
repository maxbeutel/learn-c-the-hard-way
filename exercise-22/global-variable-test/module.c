#include <stdio.h>
#include "module.h"

int GLOBAL_VARIABLE = 0;

void increase_global_variable()
{
    GLOBAL_VARIABLE++;
    printf("GLOBAL_VARIABLE in module = %d\n", GLOBAL_VARIABLE);
}
