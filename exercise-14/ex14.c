#include <stdio.h>
#include <ctype.h>
#include <string.h>


void print_letters(char arg[], size_t arg_len);

void print_arguments(int argc, char *argv[])
{
    int i = 0;
    
    for (i = 0; i < argc; i++) {
        char *arg = argv[i];
        size_t arg_len = strlen(arg);

        print_letters(argv[i], arg_len);
    }
}

void print_letters(char arg[], size_t arg_len) 
{
    size_t i = 0;
    
    for (i = 0; i < arg_len; i++) {
        char ch = arg[i];
        
        if (isalpha(ch) || isblank(ch)) {
            printf("'%c' == %d ", ch, ch);
        }
    }
    
    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}