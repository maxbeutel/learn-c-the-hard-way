#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "ex19.h"

int Map_init(void *self)
{
    Map *map = self;

    // make some rooms for a small map
    Room *hall = NEW(Room, "The great Hall");
    Room *throne = NEW(Room, "The throne room");
    Room *arena = NEW(Room, "The arena, with the minotaur");
    Room *kitchen = NEW(Room, "Kitchen, you have the knife now");

    // put the bad guy in the arena
    arena->bad_guy = NEW(Monster, "The evil minotaur");

    // setup the map rooms
    hall->north = throne;

    throne->west = arena;
    throne->east = kitchen;
    throne->south = hall;

    arena->east = throne;
    kitchen->west = throne;

    // start the map and the character off in the hall
    map->start = hall;
    map->location = hall;

    return 1;
}

Object MapProto = {
    .init = Map_init,
    .move = Map_move,
    .attack = Map_attack
};

int process_input(Map *game)
{
    assert(game != NULL);
    
    printf("\n> ");
    
    char ch = getchar();
    getchar(); // eat ENTER
    
    int damage = rand() % 4;
    
    switch (ch) {
        case -1:
        case 'q':
            printf("Giving up? You suck.\n");
            return 0;
            break;

        case 'n':
            game->_(move)(game, NORTH);
            break;

        case 's':
            game->_(move)(game, SOUTH);
            break;

        case 'e':
            game->_(move)(game, EAST);
            break;

        case 'w':
            game->_(move)(game, WEST);
            break;

        case 'a':

            game->_(attack)(game, damage);
            break;
        case 'l':
            printf("You can go:\n");

            if(game->location->north) {
                printf("NORTH\n");
            }

            if(game->location->south) {
                printf("SOUTH\n");
            }

            if(game->location->east) {
                printf("EAST\n");
            }

            if(game->location->west) {
                printf("WEST\n");
            }
            break;

        default:
            printf("What?: %d\n", ch);
            break;
    }
    
    return 1;
}

int main(int argc, char *argv[]) 
{
    // simple way to setup the randomness    
    srand(time(NULL));
    
    // make our map to work with
    Map *game = NEW(Map, "The Hall of the Minotaur.");
    printf("You enter the ");
    game->location->_(describe)(game->location);
    
    while (process_input(game)) {
    }

    return 0;
}
