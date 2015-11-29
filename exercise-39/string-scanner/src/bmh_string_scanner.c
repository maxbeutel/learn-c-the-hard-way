#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "bmh_string_scanner.h"

// debug
#include <stdio.h>
#define UNUSED(x) (void)(x)

BMHStringScanner_t *BMHStringScanner_create(unsigned char *haystack, size_t haystack_len)
{
    BMHStringScanner_t *scanner = calloc(1, sizeof(BMHStringScanner_t));
    assert(scanner != NULL);

    scanner->haystack = haystack;
    scanner->haystack_len = haystack_len;

    return scanner;
}

static void constructBadMatchTable(size_t *badMatches, size_t badMatches_len, unsigned char *needle, size_t needle_len)
{
    for (size_t i = 0; i < badMatches_len; i++) {
        badMatches[i] = needle_len;
    }

    // debug
    /* for (size_t i = 0; i < badMatches_len; i++) { */
    /*     printf("%zd ", badMatches[i]); */
    /* } */
    // debug

    //printf("\n");

    for (size_t i = 0; i < needle_len; i++) {
        // last letter, if already defined, leave, otherwise length
        if (i == needle_len - 1) {
            // already defined, its someting else then needle_len
            if (badMatches[(int) needle[i]] < needle_len) {
                continue;
            }

            // otherwise leave it at needle_len
            // @TODO write test for this
        } else {
            badMatches[(int) needle[i]] = needle_len - i - 1;
        }
    }

    // debug
    /* for (size_t i = 0; i < badMatches_len; i++) { */
    /*     if (badMatches[i] != needle_len) { */
    /*         printf("%c=%zd ", (unsigned int) i, badMatches[i]); */
    /*     } */
    /* } */
    // debug


    /* UNUSED(needle); */
    /* printf("\n"); */
}

ssize_t BMHStringScanner_scan(BMHStringScanner_t *scanner, unsigned char *needle, size_t needle_len)
{
    assert(scanner != NULL);

    if (needle == NULL) {
        return BMH_STRING_SCANNER_NOT_FOUND;
    }

    if (needle_len == 0) {
        return BMH_STRING_SCANNER_NOT_FOUND;
    }

    size_t badMatches[UCHAR_MAX];
    constructBadMatchTable(badMatches, UCHAR_MAX, needle, needle_len);

    size_t skip = 0;

    while (skip < scanner->haystack_len) {
        size_t i = needle_len - 1; // start at last char in needle

        /* printf("comparing %c and %c\n", scanner->haystack[skip + i], needle[i]); */

        while (needle[i] == scanner->haystack[skip + i]) {
            i--; // check "next" (= previous) character

            if (i == 0) {
                return skip;
            }
        }

        skip += badMatches[scanner->haystack[skip + needle_len - 1]];
        /* printf("skip is now %zd\n", skip); */
    }

    return BMH_STRING_SCANNER_NOT_FOUND;
}

void BMHStringScanner_destroy(BMHStringScanner_t *scanner)
{
    free(scanner);
}
