#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

void read_string(char **out_string, int max_buffer)
{
    assert(*out_string != NULL);
    
    char *result = fgets(*out_string, max_buffer, stdin);
    assert(result != NULL);
}

void read_fmt(const char *fmt, ...) 
{
    va_list argp;
    va_start(argp, fmt);
    
    char **out_str = va_arg(argp, char **);
    int max_buffer = va_arg(argp, int);

    read_string(out_str, max_buffer);
}

int main(void)
{
    int max_str_len = 5;
    char *str = malloc(sizeof(char) * max_str_len);
    assert(str != NULL);
    
    read_fmt("%s", &str, max_str_len);
    printf("%s\n", str);
    
    free(str);
    
    return 0;
}
