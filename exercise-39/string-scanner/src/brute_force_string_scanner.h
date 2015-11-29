#ifndef _brute_force_string_scanner_h
#define _brute_force_string_scanner_h

#include <string.h>

typedef struct BruteForceStringScanner {
    char *haystack;
    ssize_t haystack_len;
} BruteForceStringScanner_t;

BruteForceStringScanner_t *BruteForceStringScanner_create(char *haystack, ssize_t haystack_len);

int BruteForceStringScanner_scan(BruteForceStringScanner_t *scanner, char *needle, ssize_t needle_len);

void BruteForceStringScanner_destroy(BruteForceStringScanner_t *scanner);

#endif
