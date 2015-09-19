#include <stdio.h>
#include <assert.h>

int main(void)
{
    char c = '\0';
    char str[50];
    
    printf("Write single char:\n");
    c = fgetc(stdin);
    fgetc(stdin); // read newline
    
    printf("Write string:\n");
    fgets(str, 50, stdin);
    
    
    printf("Data: '%c' & '%s'\n", c, str);
    
    return 0;
}
