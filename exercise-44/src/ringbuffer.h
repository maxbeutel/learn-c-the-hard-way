#ifndef _lcthw_ringbuffer_h
#define _lcthw_ringbuffer_h

typedef struct {
    char *buffer;
    int length;
    int start;
    int end;
} Ringbuffer;

Ringbuffer *Ringbuffer_create(int length);
void Ringbuffer_destroy(Ringbuffer *buffer);

int Ringbuffer_read(Ringbuffer *buffer, char *target, int amount);
int Ringbuffer_write(Ringbuffer *buffer, char *data, int length);

int Ringbuffer_empty(Ringbuffer *buffer);
int Ringbuffer_full(Ringbuffer *buffer);

int Ringbuffer_available_data(Ringbuffer *buffer);
int Ringbuffer_available_space(Ringbuffer *buffer);

char *Ringbuffer_gets(Ringbuffer *buffer, int amount);

// @TODO add macros

#endif
