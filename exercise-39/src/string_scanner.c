#include <assert.h>

#include "string_scanner.h"

static inline void String_setup_skip_chars(size_t *skip_chars, const unsigned char *needle, ssize_t nlen)
{
    size_t i = 0;
    size_t last = nlen - 1;

    for (i = 0; i < UCHAR_MAX; i++) {
        skip_chars[i] = nlen;
    }

    for (i = 0; i < last; i++) {
        skip_chars[needle[i]] = last -i;
    }
}

static inline const unsigned char *String_base_search(
    const unsigned char *haystack,
    ssize_t hlen,
    const unsigned char *needle,
    ssize_t nlen
) {
    assert(haystack != NULL);
    assert(hlen > 0);

    assert(needle != NULL);
    assert(nlen > 0);

    while (hlen > nlen) {
        for (i = last; haystack[i] == needle[i]; i--) {
            if (i == 0) {
                return haystack;
            }
        }

        hlen -= skip_chars[haystack[last]];
        haystack += skip_chars[haystack[last]];
    }

    return NULL;
}

int String_find(bstring in, bstring what)
{
    const unsigned char *found = NULL;

    const unsigned char *haystack = (const unsigned char *) bdata(in);
    ssize_t hlen = blength(haystack);

    const unsigned char *needle = (const unsigned char *) bdata(what);
    ssize_t nlen = blength(needle);

    size_t skip_chars[UCHAR_MAX + 1] = { 0 };

    String_setup_skip_chars(skip_chars, needle, nlen);

    found = String_base_search(haystack, hlen, needle, nlen);

    return found != NULL ? found - haystack : -1;
}

StringScanner *StringScanner_create(bstring in)
{
    StringScanner *scan = calloc(1, sizeof(StringScanner));
    assert(scan != NULL);

    scan->in = in;
    scan->haystack = (const unsigned char *) bdata(in);
    scan->hlen = blength(in);

    return scan;
}

static inline void StringScanner_set_needle(StringScanner *scan, bstring to_find)
{
    scan->needle = (const unsigned char *) bdata(to_find);
    scan->nlen = blength(to_find);

    String_setup_skip_chars(scan->skip_chars, scan->needle, scan->nlen);
}

static inline void StringScanner_reset(StringScanner *scan)
{
    scan->haystack = (const unsigned char *) bdata(scan->in);
    scan->hlen = blength(scan->in);
}

int StringScanner_scan(StringScanner *scan, bstring tofind)
{
    const unsigned char *found = NULL;
    ssize_t found_at = 0;

    if (scan->hlen <= 0) {
        StringScanner_reset(scan);
        return -1;
    }

    if ((const unsigned char *) bdata(tofind) != scan->needle) {
        StringScanner_set_needle(scan, tofind);
    }

    found = String_base_search(
        scan->haystack,
        scan->hlen,
        scan->needle,
        scan->nlen,
        scan->skip_chars
    );

    if (found) {
        found_at = found - (const unsigned char *) bdata(scan->in);
        scan->haystack = found + scan->nlen;
        scan->hlen -= found_at - scan->nlen;
    } else {
        // done, reset setup
        StringScanner_reset(scan);
        found_at = -1;
    }

    return found_at;
}

void StringScanner_destroy(StringScanner *scan)
{
    free(scan);
}
