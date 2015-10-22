#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t default_hash(char *key)
{
    size_t len = strlen(key);

    uint32_t hash = 0;

    for (size_t i = 0; i < len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

void trim_string(char *str)
{
    assert(str != NULL);

    char *newline = strchr(str, '\n');

    if (newline != NULL) {
        *newline = '\0';
    }
}

int main(void)
{
    char *dictionary_file_path = "/usr/share/dict/web2";

    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    stream = fopen(dictionary_file_path, "r");
    assert(stream != NULL);

    while ((read = getline(&line, &len, stream)) != -1) {
        trim_string(line);

        if (line == '\0') {
            continue;
        }

        printf("%s %d\n", line, default_hash(line));
    }

    free(line);
    fclose(stream);


    /* uint32_t hash = default_hash("some key"); */
    /* printf("hash = %d\n", hash); */

    /* hash = default_hash("some key other key"); */
    /* printf("hash = %d\n", hash); */

    /* int number_of_buckets = 100; */


    /* int bucket_number = hash % number_of_buckets; */
    /* printf("bucket_number = %d\n", bucket_number); */

    /* bucket_number = 100 % number_of_buckets; */
    /* printf("bucket_number = %d\n", bucket_number); */

    /* bucket_number = 0 % number_of_buckets; */
    /* printf("bucket_number = %d\n", bucket_number); */

    return 0;
}
