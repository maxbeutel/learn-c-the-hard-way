#include <stdio.h>

int main(int argc, char *argv[])
{
    // go through each string in argv

    int i = 0;
    while(i < argc) {
        printf("arg %d: %s\n", i, argv[i]);
        i++;
    }
    
    
    char *my_data[2] = {NULL};
    
    my_data[0] = argv[0]; 
    my_data[1] = argv[1];
    
    printf("argv 0: %s\n", my_data[0]);
    printf("argv 1: %s\n", my_data[1]);

    /*// let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;
    i = 0;  // watch for this
    while(i < num_states) {
        printf("state %d: %s\n", i, states[i]);
        i++;
    }*/

    return 0;
}
