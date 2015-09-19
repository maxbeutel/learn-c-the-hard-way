#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "map.h"

#define MAP_MAX_ROOM_COUNT 100

Room_t *Map_rooms[MAP_MAX_ROOM_COUNT];

 int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

Map_t *Map_create(int roomCount)
{
    Map_t *map = malloc(sizeof(Map_t));
    assert(map != NULL);
    assert(roomCount <= MAP_MAX_ROOM_COUNT);
    
    char *room_root_description = "Root Room";
    Room_t *room_root = Room_create(room_root_description, strlen(room_root_description));
    Map_rooms[0] = room_root;
    
    map->currentRoom = room_root;
    Room_t *previous_room = room_root;
    
    for (int i = 0; i < (roomCount - 1); i++) {
        char *room_random_description_format = "Room %d random";
        int room_random_description_buffer_len = 50;

        char *room_random_description = malloc(sizeof(char) * room_random_description_buffer_len);
        assert(room_random_description != NULL);

        assert(snprintf(room_random_description, room_random_description_buffer_len, room_random_description_format, i) > 0);

        Room_t *room_random = Room_create(room_random_description, strlen(room_random_description));
        Room_assignSiblingToRandomDirection(previous_room, room_random);

        Map_rooms[i + 1] = room_random;
        previous_room = room_random;

        free(room_random_description);
    }

    return map;
}

bool Map_directionIsAvailable(Map_t *map, Direction direction)
{
    assert(map != NULL);
    assert(map->currentRoom != NULL);
    
    if (map->currentRoom->siblings[direction] != NULL) {
        return true;
    }
    
    return false;
}

bool Map_goToDirection(Map_t *map, Direction direction)
{
    assert(map != NULL);
    assert(map->currentRoom != NULL);
    
    if (!Map_directionIsAvailable(map, direction)) {
        return false;
    }
    
    Room_t *previousCurrentRoom = map->currentRoom;
    map->currentRoom = previousCurrentRoom->siblings[direction];
    
    return true;
}

void Map_destroy(Map_t *map)
{
    if (map == NULL) {
        return;
    }

    for (int i = 0; i < MAP_MAX_ROOM_COUNT; i++) {
        Room_destroy(Map_rooms[i]);
    }
    
    free(map);
}
