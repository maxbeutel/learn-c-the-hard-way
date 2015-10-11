#include <time.h>
#include <libradixmap/radixmap.h>

#include "minunit.h"

char *test_search()
{
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_search);

    return NULL;
}

RUN_TESTS(all_tests)
