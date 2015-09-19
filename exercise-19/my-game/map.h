#ifndef _map_h
#define _map_h

#include <stdbool.h>
#include "room.h"

typedef struct Map {
    Room_t *currentRoom;
} Map_t;

Map_t *Map_create(int roomCount);
bool Map_directionIsAvailable(Map_t *map, Direction direction);
bool Map_goToDirection(Map_t *map, Direction direction);
void Map_destroy(Map_t *map);

#endif
