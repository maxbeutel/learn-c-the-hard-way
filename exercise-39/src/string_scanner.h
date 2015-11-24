#ifndef _string_scanner_h
#define _string_scanner_h

#include <limits.h>
#include <string.h>

typedef struct StringScanner {
    bstring in;
    const unsigned char *haystack;
    ssize_t nlen;
    size_t skip_chars[UCHAR_MAX + 1];
} StringScanner;

int String_find(bstring in, bstring what);

StringScanner *StringScanner_create(bstring in);

int StringScanner_scan(StringScanner *scan, bstring tofind);

void StringScanner_destroy(StringScanner *scan);

#endif
