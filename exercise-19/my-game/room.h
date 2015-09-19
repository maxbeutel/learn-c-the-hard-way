#ifndef _room_h
#define _room_h

#include <stdlib.h>

typedef enum {
    DIRECTION_NORTH = 0,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_EAST,
    
    DIRECTION_COUNT
} Direction;

typedef struct Room {
    char *description;
    struct Room *siblings[DIRECTION_COUNT];
} Room_t;

Room_t *Room_create(const char *description, size_t description_len);
void Room_assignSiblingToRandomDirection(Room_t *room, Room_t *siblings);
void Room_destroy(Room_t *room);

#endif
