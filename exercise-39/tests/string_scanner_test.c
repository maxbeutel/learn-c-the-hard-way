#include <time.h>

#include <string_scanner.h>
#include <bstrlib.h>

#include "minunit.h"

struct tagbstring IN_STR = bsStatic("I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");

const int TEST_TIME = 1;

char *test_find_and_scan()
{
    StringScanner *scan = StringScanner_create(&IN_STR);
    assert(scan != NULL && "Failed to create the scanner");

    int find_i = String_find(&IN_STR, &ALPHA);
    assert(find_i > 0 && "Failed to find 'ALPHA' in test string.");

    int scan_i = StringScanner_scan(&ALPHA);
    assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");
    assert(scan_i == find_i, "find and scan don't match");

    scan_i = StringScanner_scan(scan, &ALPHA);
    assert(scan_i > find_i, "should find another ALPHA after the first");

    scan_i = StringScanner_scan(scan, &ALPHA);
    assert(scan_i > find_i, "should find another ALPHA after the first");

    assert(StringScanner_scan(scan, &ALPHA) == -1, "shouldn't find it");

    StringScanner_destroy(scan);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_find_and_scan);

    return NULL;
}

RUN_TESTS(all_tests);
