#include <stdio.h>
#include <ctype.h>
#include <assert.h>

int print_a_message(const char *message)
{
    assert(message != NULL);
    
    printf("A STRING: %s\n", message);
    
    return 0;
}

int uppercase(const char *message, int message_len)
{
    assert(message != NULL);
    
    for (int i = 0; i < message_len; i++) {
        printf("%c", toupper(message[i]));
    }
    
    printf("\n");
    
    return 0;
}

int lowercase(const char *message, int message_len)
{
    assert(message != NULL);
    
    for (int i = 0; i < message_len; i++) {
        printf("%c", tolower(message[i]));
    }
    
    printf("\n");
    
    return 0;
}

int fail_on_purpose(const char *message)
{
    assert(message != NULL);
    
    printf("A STRING: %s\n", message);
    
    return 1;
}
