#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <bmh_string_scanner.h>

#include "minunit.h"

char *test_singleOccurence()
{
    char *haystack = "TRUSTHARDTOOTHBRUSHES";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "TOOTH";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == 9);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleIsNull()
{
    char *haystack = "TRUSTHARDTOOTHBRUSHES";
    ssize_t haystack_len = strlen(haystack);

    char *needle = NULL;
    ssize_t needle_len = 0;

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == BMH_STRING_SCANNER_NOT_FOUND);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleIsEmptyString()
{
    char *haystack = "TRUSTHARDTOOTHBRUSHES";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == BMH_STRING_SCANNER_NOT_FOUND);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleNotFound()
{
    char *haystack = "TRUSTHARDTOOTHBRUSHES";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "bear";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == BMH_STRING_SCANNER_NOT_FOUND);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleAtEndOfString()
{
    char *haystack = "XABCD";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "CD";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == 3);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleEqualsHaystack()
{
    char *haystack = "CD";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "CD";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == 0);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleLongerThenHaystack()
{
    char *haystack = "TOOTH";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "TRUSTHARDTOOTHBRUSHES";
    ssize_t needle_len = strlen(needle);

    BMHStringScanner_t *scanner = BMHStringScanner_create((unsigned char *) haystack, haystack_len);

    assert(BMHStringScanner_scan(scanner, (unsigned char *) needle, needle_len) == BMH_STRING_SCANNER_NOT_FOUND);

    BMHStringScanner_destroy(scanner);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_singleOccurence);
    mu_run_test(test_needleIsNull);
    mu_run_test(test_needleIsEmptyString);
    mu_run_test(test_needleNotFound);
    mu_run_test(test_needleAtEndOfString);
    mu_run_test(test_needleEqualsHaystack);
    mu_run_test(test_needleLongerThenHaystack);

    return NULL;
}

RUN_TESTS(all_tests)
