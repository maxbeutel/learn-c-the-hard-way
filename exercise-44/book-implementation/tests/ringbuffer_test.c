#include <assert.h>
#include <ringbuffer.h>

#include <string.h>

// DEBUG
#include <stdio.h>

#include "minunit.h"

#define TEST_STR_1 "ABCDEF"
#define TEST_STR_1_LEN strlen(TEST_STR_1)

#define TEST_STR_2 "XX"
#define TEST_STR_2_LEN strlen(TEST_STR_2)

char *test_create()
{
    Ringbuffer *buffer = Ringbuffer_create(10);
    assert(buffer != NULL && "Expected initialized buffer to be created.");

    Ringbuffer_destroy(buffer);

    return NULL;
}

char *test_readWrite_bufferNotFull()
{
    Ringbuffer *buffer = Ringbuffer_create(10);

    int rc = Ringbuffer_write(buffer, TEST_STR_1, TEST_STR_1_LEN);
    assert(rc == TEST_STR_1_LEN && "Expected number of bytes to be returned on write.");

    rc = Ringbuffer_write(buffer, TEST_STR_2, TEST_STR_2_LEN);
    assert(rc == TEST_STR_2_LEN && "Expected number of bytes written to be returned on write.");

    char out_1[7] = {0};
    assert(Ringbuffer_read(buffer, out_1, TEST_STR_1_LEN) == TEST_STR_1_LEN && "Expected number of bytes to be returned on read.");
    assert(strcmp(out_1, TEST_STR_1) == 0 && "Got invalid value when reading string from buffer.");
    assert(out_1[TEST_STR_1_LEN] == '\0' && "Expected read string to be valid 0-terminated C string.");

    char out_2[3] = {0};
    assert(Ringbuffer_read(buffer, out_2, TEST_STR_2_LEN) == TEST_STR_2_LEN && "Expected number of bytes to be returned on read.");
    assert(strcmp(out_2, TEST_STR_2) == 0 && "Got invalid value when reading string from buffer.");
    assert(out_2[TEST_STR_2_LEN] == '\0' && "Expected read string to be valid 0-terminated C string.");

    Ringbuffer_destroy(buffer);

    return NULL;
}

char *test_readWrite_bufferFull()
{
    Ringbuffer *buffer = Ringbuffer_create(8);

    int rc = Ringbuffer_write(buffer, TEST_STR_1, TEST_STR_1_LEN);
    assert(rc == TEST_STR_1_LEN && "Expected number of bytes to be returned on write.");

    rc = Ringbuffer_write(buffer, TEST_STR_2, TEST_STR_2_LEN);
    assert(rc == TEST_STR_2_LEN && "Expected number of bytes written to be returned on write.");

    char out[7] = {0};
    assert(Ringbuffer_read(buffer, out, TEST_STR_1_LEN) == TEST_STR_1_LEN && "Expected number of bytes to be returned on read.");
    assert(strcmp(out, TEST_STR_1) == 0 && "Got invalid value when reading string from buffer.");
    assert(out[TEST_STR_1_LEN] == '\0' && "Expected read string to be valid 0-terminated C string.");

    Ringbuffer_destroy(buffer);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_readWrite_bufferNotFull);

    return NULL;
}

RUN_TESTS(all_tests)
