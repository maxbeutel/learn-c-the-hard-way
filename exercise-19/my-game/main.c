#include <stdio.h>
#include <assert.h>
#include "main.h"
#include <stdlib.h>
#include <time.h>

 void describeCurrentRoom(Map_t *map)
{
    assert(map != NULL);
    assert(map->currentRoom != NULL);
    
    printf("You are now in '%s'\n", map->currentRoom->description);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    printf("Starting game\n");
    printf("*** Available commands:\n");
    printf("1 - show available directions\n");
    printf("5 - go NORTH\n");
    printf("6 - go SOUTH\n");
    printf("7 - go WEST\n");
    printf("8 - go EAST\n");
    printf("q - quit\n\n");


    Map_t *map = Map_create(5);

    if (Map_directionIsAvailable(map, DIRECTION_NORTH)) {
        Map_goToDirection(map, DIRECTION_NORTH);
    }
    
    if (Map_directionIsAvailable(map, DIRECTION_SOUTH)) {
        Map_goToDirection(map, DIRECTION_SOUTH);
    }
    
    if (Map_directionIsAvailable(map, DIRECTION_WEST)) {
        Map_goToDirection(map, DIRECTION_WEST);
    }
    
    if (Map_directionIsAvailable(map, DIRECTION_EAST)) {
        Map_goToDirection(map, DIRECTION_EAST);
    }
    
    Map_destroy(map);
    
    /*char c;

    do {
        c = getchar();
        getchar(); // ignore enter
        
        // show available directions
        if (c == '1') {
            printf("Available directions:\n");
            
            int directionsAvailable = 0;
            
            if (Map_directionIsAvailable(map, DIRECTION_NORTH)) {
                printf("NORTH ");
                directionsAvailable++;
            }
            
            if (Map_directionIsAvailable(map, DIRECTION_SOUTH)) {
                printf("SOUTH ");
                directionsAvailable++;
            }
            
            if (Map_directionIsAvailable(map, DIRECTION_WEST)) {
                printf("WEST ");
                directionsAvailable++;
            }
            
            if (Map_directionIsAvailable(map, DIRECTION_EAST)) {
                printf("EAST ");
                directionsAvailable++;
            }

    
            if (directionsAvailable == 0) {
                printf("[NOWHERE]");
            }
            
            printf("\n");
        }
        
        
        // go NORTH
        if (c == '5') {
            if (Map_goToDirection(map, DIRECTION_NORTH)) {
                describeCurrentRoom(map);
            } else {
                printf("There is nowhere to go\n");
            }
        }
        
        // go SOUTH
        if (c == '6') {
            if (Map_goToDirection(map, DIRECTION_SOUTH)) {
                describeCurrentRoom(map);
            } else {
                printf("There is nowhere to go\n");
            }
        }
        
        // go WEST
        if (c == '7') {
            if (Map_goToDirection(map, DIRECTION_WEST)) {
                describeCurrentRoom(map);
            } else {
                printf("There is nowhere to go\n");
            }
        }
        
        // go EAST
        if (c == '8') {
            if (Map_goToDirection(map, DIRECTION_EAST)) {
                describeCurrentRoom(map);
            } else {
                printf("There is nowhere to go\n");
            }
        }
        
        
        printf("------------------\n");
    } while (c != 'q');

    printf("Game end\n");

    Map_destroy(map);*/
    
    return 0;
}






/*char foo[] = {'f', 'o', 'o', '\0'};
char *foo_2 = malloc(sizeof(char) * strlen(foo) + 1);
char *foo_3 = malloc(sizeof(char) * strlen(foo) + 1);
char *foo_4 = malloc(sizeof(char) * strlen(foo) + 1);

char foo_bad[] = {'f', 'o', 'o'};
char *foo_5 = malloc(sizeof(char) * strlen(foo) + 1);
char *foo_6 = malloc(sizeof(char) * strlen(foo) + 1);
char *foo_7 = malloc(sizeof(char) * strlen(foo) + 1);


strcpy(foo_2, foo);
memcpy(foo_3, foo, strlen(foo));
memcpy(foo_4, foo, strlen(foo) + 1);

strcpy(foo_5, foo_bad);
memcpy(foo_6, foo_bad, strlen(foo));
memcpy(foo_7, foo_bad, strlen(foo) + 1);

printf("%s\n", foo);

free(foo_2);
free(foo_3);
free(foo_4);
free(foo_5);
free(foo_6);
free(foo_7);*/
