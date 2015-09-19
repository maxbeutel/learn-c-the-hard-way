#include <stdio.h>
#include <ctype.h>

int print_a_message(const char *message)
{
    printf("A STRING: %s\n", message);
    
    return 0;
}

int uppercase(const char *message)
{
    for (int i = 0; message[i] != '\0'; i++) {
        printf("%c", toupper(message[i]));
    }
    
    printf("\n");
    
    return 0;
}

int lowercase(const char *message)
{
    for (int i = 0; message[i] != '\0'; i++) {
        printf("%c", tolower(message[i]));
    }
    
    printf("\n");
    
    return 0;
}

int fail_on_purpose(const char *message)
{
    printf("A STRING: %s\n", message);
    
    return 1;
}
