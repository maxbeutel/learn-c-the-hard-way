#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person
{
    char *name;
    int age;
    int height;
    int weight;    
} Person_t;

Person_t *Person_create(char *name, int age, int height, int weight)
{
    Person_t *who = malloc(sizeof(Person_t));
    assert(who != NULL);
    
    who->name = malloc(strlen(name) + 1);
    assert(who->name != NULL);
    strcpy(who->name, name);

    who->age = age;
    who->height = height;
    who->weight = weight;
    
    return who;
}

void Person_destroy(Person_t *who)
{
    assert(who != NULL);
    
    free(who->name);
    free(who);
}

void Person_print(Person_t *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);    
}

int main(int argc, char *argv[])
{
    Person_t *joe = Person_create("Joe Alex", 32, 64, 140);
    Person_t *frank = Person_create("Frank Blank", 20, 72, 180);
    
    printf("Size of struct Person: %zd\n", sizeof(Person_t));
    
    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);
    

    // make everyone age 20 years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    // destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);
    
    return 0;
}
