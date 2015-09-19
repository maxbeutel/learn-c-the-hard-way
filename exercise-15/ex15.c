#include <stdio.h>

int main(int argc, char *argv[])
{
    // create two arrays we care about
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
    
    // safely get the size of ages
    int count = sizeof(ages) / sizeof(int);
    /*int i = 0;*/
    
    // first way using indexing
    int *ages_ptr = ages;
    int *ages_end_ptr = ages_ptr + count;
    
    char **names_ptr = names;
    char **names_end_ptr = names_ptr + count;
    
    while (ages_ptr < ages_end_ptr && names_ptr < names_end_ptr) {
        printf("(1) %s is %d years old", *names_ptr, *ages_ptr);
        printf(" names_ptr '%p', ages_ptr '%p'", names_ptr, ages_ptr);
        printf("\n");
        
        ages_ptr++;
        names_ptr++;
    }
    
    printf("---\n");

    // setup the pointers to the start of the arrays
    int *cur_age = ages;
    char **cur_name = names;

    // second way using pointers
    for (i = 0; i < count; i++) {
        printf("(2) %s is %d years old\n", *(cur_name + i), *(cur_age + i));
    }
    
    printf("---\n");
    /*
    // third way, pointers are just arrays
    for (i = 0; i < count; i++) {
        printf("(3) %s is %d years old\n", cur_name[i], cur_age[i]);
    }
    
    printf("---\n");
    
    // fourth way with pointers in a stupid complex way
    for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++) {
        printf("(4) %s is %d years old\n", *cur_name, *cur_age);
    }
*/
    return 0;
}