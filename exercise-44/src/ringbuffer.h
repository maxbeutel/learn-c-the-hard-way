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

/* char *Ringbuffer_gets(Ringbuffer *buffer, int amount); */

#define Ringbuffer_available_data(B) (\
  ((B)->end + 1) % (B)->length - (B)->start - 1)

#define Ringbuffer_available_space(B) (\
  (B)->length - (B)->end - 1)

#define Ringbuffer_full(B) (Ringbuffer_available_data((B))\
  - (B)->length == 0)

#define Ringbuffer_empty(B) (\
  Ringbuffer_available_data((B)) == 0)

#define Ringbuffer_puts(B, D) Ringbuffer_write(\
  (B), bdata((D)), blength((D)))

/* #define Ringbuffer_get_all(B) Ringbuffer_gets(\ */
/*   (B), Ringbuffer_available_data((B))) */

#define Ringbuffer_starts_at(B) (\
  (B)->buffer + (B)->start)

#define Ringbuffer_ends_at(B) (\
  (B)->buffer + (B)->end)

#define Ringbuffer_commit_read(B, A) (\
  (B)->start = ((B)->start + (A)) % (B)->length)

#define Ringbuffer_commit_write(B, A) (\
  (B)->end = ((B)->end + (A)) % (B)->length)

#endif
