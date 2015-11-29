#include <stdlib.h>
#include <assert.h>

#include "brute_force_string_scanner.h"

// debug
#include <stdio.h>
#define UNUSED(x) (void)(x)

BruteForceStringScanner_t *BruteForceStringScanner_create(char *haystack, size_t haystack_len)
{
    BruteForceStringScanner_t *scanner = calloc(1, sizeof(BruteForceStringScanner_t));
    assert(scanner != NULL);

    scanner->haystack = haystack;
    scanner->haystack_len = haystack_len;

    return scanner;
}

ssize_t BruteForceStringScanner_scan(BruteForceStringScanner_t *scanner, char *needle, size_t needle_len)
{
    assert(scanner != NULL);

    if (needle == NULL) {
        return BRUTE_FORCE_STRING_SCANNER_NOT_FOUND;
    }

    if (needle_len == 0) {
        return BRUTE_FORCE_STRING_SCANNER_NOT_FOUND;
    }

    if (needle_len > scanner->haystack_len) {
        return BRUTE_FORCE_STRING_SCANNER_NOT_FOUND;
    }

    // @TODO implement multiple scans (vorher vllt. den algorithmus implementieren?
    for (size_t i = 0; i < scanner->haystack_len; i++) {
        /* printf("at index %zd\n", i); */

        for (size_t j = 0; j < needle_len && i + j < scanner->haystack_len; j++) {
            /* printf("## looking ahead %zd + %zd\n", i, j); */

            if (scanner->haystack[i + j] != needle[j]) {
                /* printf("## abort\n"); */
                break;
            }

            if (j == needle_len - 1) {
                /* printf("## found\n"); */
                return i;
            }
        }
    }

    return BRUTE_FORCE_STRING_SCANNER_NOT_FOUND;
}

void BruteForceStringScanner_destroy(BruteForceStringScanner_t *scanner)
{
    free(scanner);
}
