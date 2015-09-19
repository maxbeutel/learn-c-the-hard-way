#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "room.h"

Room_t *Room_create(const char *description, size_t descriptionLen)
{
    assert(description != NULL);

    Room_t *room = malloc(sizeof(Room_t));
    assert(room != NULL);

    room->siblings[DIRECTION_NORTH] = NULL;
    room->siblings[DIRECTION_SOUTH] = NULL;
    room->siblings[DIRECTION_WEST] = NULL;
    room->siblings[DIRECTION_EAST] = NULL;

    room->description = malloc(sizeof(char) * descriptionLen + 1);
    assert(room->description != NULL);
    
    strncpy(room->description, description, descriptionLen);
    room->description[descriptionLen] = '\0';
    
    return room;
}

void Room_assignSiblingToRandomDirection(Room_t *room, Room_t *siblingRoom)
{
    assert(room != NULL);
    assert(siblingRoom != NULL);
    
    int randomDirection = rand() % 4;
    
    printf("[DEBUG] room '%s' gets sibling at %d\n", room->description, randomDirection);

    room->siblings[randomDirection] = siblingRoom;  
}

void Room_destroy(Room_t *room)
{
    if (room == NULL) {
        return;
    }

    if (room->description) {
        free(room->description);
    }

    free(room);
}
