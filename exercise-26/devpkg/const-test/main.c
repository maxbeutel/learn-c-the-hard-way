#include <stdio.h>
#include <stdlib.h>

static void someFunc(const char *str)
{
    //error: assignment of read-only location ‘*str’
    //str[0] = 't';
    
    str = "rara";
    
    printf("inside someFunc: %s\n", str);
}

int main(void)
{
    int i = 5;
    int j = 10;
    
    const int *i_ptr = &i;
    
    printf("i_ptr = %d\n", *i_ptr);
    
    i_ptr = &j;
    
    printf("i_ptr = %d\n", *i_ptr);
    
    // error: assignment of read-only location ‘*i_ptr’
    //*i_ptr = 999;
    

    char str[] = "foo";
    
    //str[0] = 'z';
    
    printf("before - str ist %s\n", str);
    
    str[0] = 'z';
    
    printf("after - str ist %s\n", str);
    
    const char *str_const = str;
    
    printf("before - str_const = %s\n", str_const);

    // error: assignment of read-only location ‘*str_const’
    //str_const[0] = 'z';
    
    const char **str_ptr_const = &str_const;
    
    char *somestr = "boooooobar";
    
    *str_ptr_const = somestr;
    
    printf("after 2 - str ist %s\n", str);
    printf("after 2 - str_const ist %s\n", str_const);
    printf("after 2 - str_ptr_const ist %s\n", *str_ptr_const);
    
    
    printf("before someFunc - str is %s\n", str);
    someFunc(str);
    printf("after someFunc - str is %s\n", str);

    
    return 0;
}


// did it on em
