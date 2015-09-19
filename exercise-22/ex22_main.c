#include <stdio.h>
#include "ex22.h"

const char *MY_NAME = "Zed A. Shaw";

void scope_demo(int count)
{
    printf("count is %d\n", count);
    
    if (count > 10) {
        int count = 100; // BAD! BUGS!
        
        printf("count in this scope is %d\n", count);
    }
    
    printf("count is at exit: %d\n", count);
    
    count = 3000;
    
    printf("count after assign: %d\n", count);
}

int main(int argc, char *argv[])
{
    // test out THE_AGE accessors
    printf("My name: %s, age: %d\n", MY_NAME, get_age());

    set_age(100);
    
    printf("My age is now: %d\n", get_age());
    
    // test out THE_SIZE extern
    printf("THE_SIZE is: %d\n", THE_SIZE);
    
    THE_SIZE = 9;
    
    printf("THE SIZE is now: %d\n", THE_SIZE);
    print_size();
    
   // test the ratio function static
    printf("Ratio at first: %f\n", update_ratio(2.0));
    printf("Ratio again: %f\n", update_ratio(10.0));
    printf("Ratio once more: %f\n", update_ratio(300.0));
    
    // test the scope demo
    int count = 4;
    scope_demo(count);
    scope_demo(count * 20);

    printf("count after calling scope_demo: %d\n", count);
    
    
    return 0;
}
