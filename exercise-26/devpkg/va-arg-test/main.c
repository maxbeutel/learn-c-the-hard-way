#include <stdarg.h>
#include <stdio.h>

void some_func(char *str, ...)
{
    va_list argp;
    va_start(argp, str); 
    
    const char *key = NULL;
    const char *arg = NULL;
    
    for (key = va_arg(argp, const char *);
         key != NULL;
         key = va_arg(argp, const char *)
    ) {
        arg = va_arg(argp, const char *);
        
        printf("%s => %s\n", key, arg);
    }
}

int main(void)
{   
    some_func(
        "rand",
    
        "key1",
        "val1",
        
        "key2",
        "val2",
        
        NULL
    );
    
    return 0;
}
