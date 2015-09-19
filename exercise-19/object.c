#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>
// TODO:
// #include <stdbool.h>

void Object_destroy(void *self)
{
    assert(self != NULL);

    Object *obj = self;
    
    if (obj) {
        free(obj->description);
        free(obj);
    }
}

void Object_describe(void *self)
{
    assert(self != NULL);
    
    Object *obj = self;
    printf("%s.\n", obj->description);
}

int Object_init(void *self)
{
    // do nothing really
    return 1;
}

void *Object_move(void *self, Direction direction)
{
    printf("You can't go that direction.\n");
    return NULL;    
}

int Object_attack(void *self, int damage)
{
    printf("You can't attack that.\n");
    return 0;    
}

void *Object_new(size_t size, Object proto, char *description)
{
    assert(description != NULL);
    
    if (proto.init == NULL) {
        proto.init = Object_init;
    }
    
    if (proto.describe == NULL) {
        proto.describe = Object_describe;
    }
    
    if (proto.destroy == NULL) {
        proto.destroy = Object_destroy;
    }
    
    if (proto.move == NULL) {
        proto.move = Object_move;
    }
    
    if (proto.attack == NULL) {
        proto.attack = Object_attack;
    }
    
    Object *el = calloc(1, size);
    assert(el != NULL);

    *el = proto;
    
    size_t description_len = strlen(description);

    el->description = malloc(sizeof(char) * description_len + 1);
    assert(el->description != NULL);

    memcpy(el->description, description, description_len);

    if (!el->init(el)) {
        el->destroy(el);
        return NULL;
    } 
    
    return el;
}
