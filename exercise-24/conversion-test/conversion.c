#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>

static bool strip_trailing_newline(char *str)
{
    assert(str != NULL);
    
    while (*str != '\0') {
        if (*str == '\n') {
            *str = '\0';
            return true;
        }
        
        str++;
    }
    
    return false;
}

// @see http://www.gnu.org/software/libc/manual/html_node/Parsing-of-Integers.html
static bool convert_str_to_long(char *str, long *out)
{
    long converted = 0;
    char *tail = NULL;
    
    errno = 0;
    converted = strtol(str, &tail, 0);
    
    if (errno) {
        goto error;
    }
    
    if (tail != NULL && *tail != '\0') {
        goto error;
    }
    
    *out = converted;
    return true;
    
error:
    errno = 0;
    return false;
}

int main(void)
{
    char buf[50] = {0};

    fgets(buf, 50, stdin);
    assert(buf != NULL);
    
    assert(strip_trailing_newline(buf) == true);

    long converted = 0;
    
    if (convert_str_to_long(buf, &converted)) {
        printf("conv  : %ld\n", converted);
    } else {
        printf("ERR: conversion failed - input: '%s'\n", buf);
    }

    return 0;
}
