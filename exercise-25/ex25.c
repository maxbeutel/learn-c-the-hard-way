#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

// @TODO funktionen auslagern
// @TODO when implementing better error handling, use return codes again :-)

#define MAX_DATA 100

static void strip_newline(char *str)
{
    assert(str != NULL);
    
    char *ptr = strchr(str, '\n');
    
    if (ptr == NULL) {
        return;
    }
    
    *ptr = '\0';
}

// @see http://www.gnu.org/software/libc/manual/html_node/Parsing-of-Integers.html
static int convert_str_to_long(char *str, long *out)
{
    assert(str != NULL);
    assert(out != NULL);

    char *tail = NULL;
    
    errno = 0;
    int return_code = 0;
    long converted = strtol(str, &tail, 0);
    
    if (errno) {
        return_code = -1;
        goto cleanup;
    }
    
    if (tail != NULL && *tail != '\0') {
        return_code = -1;
        goto cleanup;
    }

    *out = converted;

    cleanup:
        errno = 0;

    return return_code;
}

int read_string(char **out_string, int max_buffer)
{
    assert(*out_string != NULL);
    assert(max_buffer > 0);

    int return_code = 0;

    char *result = fgets(*out_string, max_buffer, stdin);
    
    if (result == NULL) {
        return_code = -1;
        goto cleanup;
    }

    strip_newline(*out_string);
    
    cleanup:
    
    return return_code;
}

int read_int(int *out_int)
{
    assert(out_int != NULL);
    
    int return_code = 0;
    const int max_int_string_len = 12; // string length of min int + \0
    
    char *input = malloc(sizeof(char) * max_int_string_len); 
    assert(input != NULL);
    
    int read_string_rc = read_string(&input, max_int_string_len);
    if (read_string_rc != 0) {
        return_code = -1;
        goto cleanup;
    }
    
    long converted = 0;
    int convert_str_to_long_rc = convert_str_to_long(input, &converted);
    if (convert_str_to_long_rc != 0) {
        return_code = -1;
        goto cleanup;
    }

    assert(converted <= INT_MAX);
    assert(converted >= INT_MIN);
    
    // at this point we can be sure that "long converted" fits into an int
    *out_int = (int) converted;

    cleanup:
        free(input);

    return return_code;
}

typedef enum {
    KEEP_NEWLINE = 0,
    CHOMP_NEWLINE,
} READ_CHAR_OPTIONS;

int read_char(char *out_char, READ_CHAR_OPTIONS option)
{
    assert(out_char != NULL);

    int return_code = 0;
    
    errno = 0;
    int input = fgetc(stdin);

    if (input == EOF) {
        // errno is now set
        return_code = -1;
        goto cleanup;
    }

    *out_char = (char) input;

    if (option == CHOMP_NEWLINE) {
        errno = 0;
        int newline_chomp_char = fgetc(stdin);
    
        if (newline_chomp_char == EOF) {
            // errno is now set
            return_code = -1;
            goto cleanup;
        }        
    }
    
    cleanup:
    
    return return_code;
}

int read_scan(const char *fmt, ...)
{
    assert(fmt != NULL);
    
    int return_code = 0;
    
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    
    int max_buffer = 0;
    
    va_list argp;
    va_start(argp, fmt);
    
    // @TODO better error handling everywhere (goto error)
    // @TODO buffer overflow check & limit checking
    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            i++;
            
            switch (fmt[i]) {
                case '\0':
                    printf("Invalid format, you ended with %%.\n");
                    break;
                
                case 'd':
                    out_int = va_arg(argp, int *);
                    
                    int read_int_rc = read_int(out_int);
                    
                    if (read_int_rc != 0) {
                        return_code = -1;
                        goto cleanup;
                    }
                    
                    break;
                
                case 'c':
                    out_char = va_arg(argp, char *);
                    
                    int read_char_rc = read_char(out_char, CHOMP_NEWLINE);
                    
                    if (read_char_rc != 0) {
                        return_code = -1;
                        goto cleanup;
                    }
                    
                    break;

                case 's':
                    out_string = va_arg(argp, char **);
                    max_buffer = va_arg(argp, int);
                    
                    if (out_string == NULL) {
                        return_code = -1;
                        goto cleanup;
                    }
                    
                    if (max_buffer <= 0) {
                        return_code = -1;
                        goto cleanup;
                    }

                    int read_string_rc = read_string(out_string, max_buffer);
                    
                    if (read_string_rc != 0) {
                        return_code = -1;
                        goto cleanup;
                    }

                    break;
                
                default:
                    printf("Invalid format.\n");

                    break;
            }
        }
        
        // @TODO hier sollte nur feof geprüft werden
        // ferror() eigentlich nach jedem read?
        assert(!feof(stdin) && !ferror(stdin));
    }
    
    cleanup:
        errno = 0;
        va_end(argp);
    
    return return_code;
}

int main(int argc, char *argv[])
{
    int return_code = 0;

    char *first_name = malloc(sizeof(char) * MAX_DATA);
    assert(first_name != NULL);

    char *last_name = malloc(sizeof(char) * MAX_DATA);
    assert(last_name != NULL);

    char initial = '\0';
    
    int age = 0;

    int read_scan_rc = false;
    
    printf("What's your first name? ");
    read_scan_rc = read_scan("%s", &first_name, MAX_DATA);
    if (read_scan_rc != 0) {
        return_code = -1;
        goto cleanup;
    }
    

    printf("What's your initial (single char)? ");
    read_scan_rc = read_scan("%c",&initial);
    if (read_scan_rc != 0) {
        return_code = -1;
        goto cleanup;
    }

    printf("What's your last name? ");
    read_scan_rc = read_scan("%s", &last_name, MAX_DATA);
    if (read_scan_rc != 0) {
        return_code = -1;
        goto cleanup;
    }

    printf("How old are you? ");
    read_scan_rc = read_scan("%d", &age);
    if (read_scan_rc != 0) {
        return_code = -1;
        goto cleanup;
    }

    printf("---- RESULTS ----\n");
    printf("First Name: %s\n", first_name);
    printf("Initial: %c\n", initial);
    printf("Last Name: %s\n", last_name);
    printf("Age: %d\n", age);

    cleanup:
        free(first_name);
        free(last_name);
        
    if (return_code != 0) {
        printf("An error occurred ... aborting ...\n");
    }
    
    return return_code;
}







