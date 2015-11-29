#ifndef _brute_force_string_scanner_h
#define _brute_force_string_scanner_h

#include <string.h>

#define BRUTE_FORCE_STRING_SCANNER_NOT_FOUND -1

typedef struct BruteForceStringScanner {
    char *haystack;
    size_t haystack_len;
} BruteForceStringScanner_t;

BruteForceStringScanner_t *BruteForceStringScanner_create(char *haystack, size_t haystack_len);

ssize_t BruteForceStringScanner_scan(BruteForceStringScanner_t *scanner, char *needle, size_t needle_len);

void BruteForceStringScanner_destroy(BruteForceStringScanner_t *scanner);

#endif
