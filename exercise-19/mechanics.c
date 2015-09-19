#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "mechanics.h"

int Monster_attack(void *self, int damage)
{
    assert(self != NULL);
    assert(damage >= 0);

    Monster *monster = self;
    
    printf("You attack %s - damage is %d!\n", monster->_(description), damage);
    
    int initial_hit_points = monster->hit_points;
    monster->hit_points -= damage;
    
    printf("Monster status - before: %d, after: %d\n", initial_hit_points, monster->hit_points);
    
    if (monster->hit_points > 0) {
        printf("It is still alive.\n");
        return 0;
    }

    printf("It is dead!\n");
    return 1;
}

int Monster_init(void *self)
{
    assert(self != NULL);

    Monster *monster = self;
    monster->hit_points = 10;
    return 1;
}








void *Room_move(void *self, Direction direction)
{
    assert(self != NULL);
    
    Room *room = self;
    Room *next = NULL;

    if (direction == NORTH && room->north != NULL) {
        printf("You go north, into:\n");
        next = room->north;
    } else if (direction == SOUTH && room->south != NULL) {
        printf("You go south, into:\n");
        next = room->south;
    } else if (direction == EAST && room->east != NULL) {
        printf("You go east, into:\n");
        next = room->east;
    } else if (direction == WEST && room->west != NULL) {
        printf("You go west, into:\n");
        next = room->west;
    } else {
        printf("You can't go that direction.");
        next = NULL;
    }

    if (next) {
        next->_(describe)(next);
    }

    return next;
}

int Room_attack(void *self, int damage)
{
    assert(self != NULL);
    assert(damage >= 0);
    
    Room *room = self;
    Monster *monster = room->bad_guy;
    
    if (monster) {
        monster->_(attack)(monster, damage);
        return 1;
    }
    
    printf("You flail in the air at nothing. Idiot.\n");
    return 0;
}









void *Map_move(void *self, Direction direction)
{
    assert(self != NULL);
    
    Map *map = self;
    
    Room *location = map->location;
    assert(location != NULL);
    
    Room *next = location->_(move)(location, direction);
    
    if (next) {
        map->location = next;
    }
    
    return next;
}

int Map_attack(void *self, int damage)
{
    assert(self != NULL);
    
    Map *map = self;
    Room *location = map->location;
    assert(map->location != NULL);
    
    return location->_(attack)(location, damage);
}




Object MonsterProto = {
    .init = Monster_init,
    .attack = Monster_attack,
};

Object RoomProto = {
    .move = Room_move,
    .attack = Room_attack,
};



