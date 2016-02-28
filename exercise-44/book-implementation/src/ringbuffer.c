#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ringbuffer.h"

Ringbuffer *Ringbuffer_create(int length)
{
    Ringbuffer *buffer = calloc(1, sizeof(Ringbuffer));
    assert(buffer != NULL);

    buffer->length = length + 1;
    buffer->start = 0;
    buffer->end = 0;

    buffer->buffer = calloc(buffer->length, 1); // @FIXME should "1" be "sizeof(char)" ???
    assert(buffer->buffer != NULL);

    return buffer;
}

void Ringbuffer_destroy(Ringbuffer *buffer)
{
    if (buffer) {
        free(buffer->buffer);
    }

    free(buffer);
}


int Ringbuffer_read(Ringbuffer *buffer, char *target, int amount)
{
    assert(buffer != NULL);
    assert(target != NULL);
    assert(amount > 0);

    if (amount > Ringbuffer_available_data(buffer)) {
        printf(
            "Not enough in the buffer: has %d, needs %d",
            Ringbuffer_available_data(buffer),
            amount
        );

        return -1;
    }

    void *result = memcpy(target, Ringbuffer_starts_at(buffer), amount);
    assert(result != NULL && "Failed to write buffer into data.");

    Ringbuffer_commit_read(buffer, amount);

    if (buffer->end == buffer->start) {
        buffer->start = 0;
        buffer->end = 0;
    }

    return amount;
}

int Ringbuffer_write(Ringbuffer *buffer, char *data, int length)
{
    assert(buffer != NULL);
    assert(data != NULL);
    assert(length > 0);

    if (Ringbuffer_available_data(buffer) == 0) {
        buffer->start = 0;
        buffer->end = 0;
    }

    if (length > Ringbuffer_available_space(buffer)) {
        printf(
            "Not enough space: %d request, %d available",
            length,
            Ringbuffer_available_data(buffer)
        );

        return -1;
    }

    void *result = memcpy(Ringbuffer_ends_at(buffer), data, length);
    assert(result != NULL && "Failed to write data into buffer.");

    Ringbuffer_commit_write(buffer, length);

    return length;
}

/* char *Ringbuffer_gets(Ringbuffer *buffer, int amount) */
/* { */
/*     // @TODO implement function */
/*     return NULL; */
/* } */
