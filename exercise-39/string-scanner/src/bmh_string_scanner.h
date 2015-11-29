#ifndef _bmh_string_scanner_h
#define _bmh_string_scanner_h

#include <string.h>

#define BMH_STRING_SCANNER_NOT_FOUND -1

typedef struct BMHStringScanner {
    unsigned char *haystack;
    size_t haystack_len;
} BMHStringScanner_t;

BMHStringScanner_t *BMHStringScanner_create(unsigned char *haystack, size_t haystack_len);

ssize_t BMHStringScanner_scan(BMHStringScanner_t *scanner, unsigned char *needle, size_t needle_len);

void BMHStringScanner_destroy(BMHStringScanner_t *scanner);

#endif
