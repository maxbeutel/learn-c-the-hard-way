#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_DATA 100

typedef enum EyeColor {
    BLUE_EYES = 0,
    GREEN_EYES,
    BROWN_EYES,
    BLACK_EYES,
    OTHER_EYES,

    EYE_COLOR_COUNT
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
    "Blue", 
    "Green",
    "Brown",
    "Black",
    "Other",
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;  
} Person;

int main(int argc, char *argv[])
{
    Person you = { .age = 0 };
    int rc = 0;
    char *in = NULL;
    
    printf("What's your First Name? ");
    //in = gets(you.first_name);
    in = fgets(you.first_name, MAX_DATA, stdin);
    //rc = fscanf(stdin, "%50s", you.first_name);
    assert(in != NULL);
    
    printf("What's your Last Name? ");
    //in = gets(you.last_name);
    in = fgets(you.last_name, MAX_DATA, stdin);
    //rc = fscanf(stdin, "%50s", you.last_name);
    assert(in != NULL);
    
    printf("How old are you? ");
    rc = fscanf(stdin, "%d", &you.age);
    assert(rc > 0);
    
    printf("What color are your eyes?\n");
    for (int i = 0; i < EYE_COLOR_COUNT; i++) {
        printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
    }
    printf("> ");
    
    int eyes = -1;
    rc = fscanf(stdin, "%d", &eyes);
    
    you.eyes = eyes - 1;
    //assert(you.eyes < EYE_COLOR_COUNT && you.eyes >= 0);
    
    printf("How much do you make an hour? ");
    rc = fscanf(stdin, "%f", &you.income);
    assert(rc > 0);
    
    printf("----- RESULTS -----\n");

    printf("First Name: %s", you.first_name);
    printf("Last Name: %s", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
    printf("Income: %f\n", you.income);
    
    return 0;
}
