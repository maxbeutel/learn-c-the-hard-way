#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <brute_force_string_scanner.h>

#include "minunit.h"

char *test_singleOccurence()
{
    char *haystack = "IAMANANTELOPE";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "ANT";
    ssize_t needle_len = strlen(needle);

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == 5);

    BruteForceStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleIsNull()
{
    char *haystack = "IAMANANTELOPE";
    ssize_t haystack_len = strlen(haystack);

    char *needle = NULL;
    ssize_t needle_len = 0;

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == BRUTE_FORCE_STRING_SCANNER_NOT_FOUND);

    BruteForceStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleIsEmptyString()
{
    char *haystack = "IAMANANTELOPE";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "";
    ssize_t needle_len = strlen(needle);

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == BRUTE_FORCE_STRING_SCANNER_NOT_FOUND);

    BruteForceStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleNotFound()
{
    char *haystack = "IAMANANTELOPE";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "bear";
    ssize_t needle_len = strlen(needle);

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == BRUTE_FORCE_STRING_SCANNER_NOT_FOUND);

    BruteForceStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleAtEndOfString()
{
    char *haystack = "IAMANANT";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "ANT";
    ssize_t needle_len = strlen(needle);

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == 5);

    BruteForceStringScanner_destroy(scanner);

    return NULL;
}

char *test_needleLongerThenHaystack()
{
    char *haystack = "ANT";
    ssize_t haystack_len = strlen(haystack);

    char *needle = "IAMANANT";
    ssize_t needle_len = strlen(needle);

    BruteForceStringScanner_t *scanner = BruteForceStringScanner_create(haystack, haystack_len);

    assert(BruteForceStringScanner_scan(scanner, needle, needle_len) == BRUTE_FORCE_STRING_SCANNER_NOT_FOUND);

    BruteForceStringScanner_destroy(scanner);

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
    mu_run_test(test_needleLongerThenHaystack);

    return NULL;
}

RUN_TESTS(all_tests)
