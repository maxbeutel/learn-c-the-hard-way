#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "doubly_linked_list.h"

LCTHW_DoublyLinkedList_List_t *List_create()
{
    LCTHW_DoublyLinkedList_List_t *list = calloc(1, sizeof(LCTHW_DoublyLinkedList_List_t));
    assert(list != NULL);

    return list;
}

void List_destroy(LCTHW_DoublyLinkedList_List_t *list)
{
    assert(list != NULL);
    
    LIST_FOREACH(list, first, next, cur) {
        if (cur->prev) {
            //printf("at %s - freeing %s\n", (char *) cur->value, (char *) cur->prev->value);
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_push(LCTHW_DoublyLinkedList_List_t *list, void *value)
{
    assert(list != NULL);
    
    LCTHW_DoublyLinkedList_Node_t *node = calloc(1, sizeof(LCTHW_DoublyLinkedList_Node_t));
    assert(node != NULL);
    
    node->value = value;
    
    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }
    
    list->count++;
}

void *List_pop(LCTHW_DoublyLinkedList_List_t *list) 
{
    assert(list != NULL);
    
    LCTHW_DoublyLinkedList_Node_t *node = list->last;
    
    if (node == NULL) {
        return NULL;
    }
    
    // @TODO besser value zwischenspeichern? warum reunted List_remove() den value??!?
    return List_remove(list, node);
}

void List_unshift(LCTHW_DoublyLinkedList_List_t *list, void *value)
{
    assert(list != NULL);
    
    LCTHW_DoublyLinkedList_Node_t *node = calloc(1, sizeof(LCTHW_DoublyLinkedList_Node_t));
    assert(node != NULL);
    
    node->value = value;
    
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }
    
    list->count++;
}

void *List_shift(LCTHW_DoublyLinkedList_List_t *list)
{
    assert(list != NULL);
    
    LCTHW_DoublyLinkedList_Node_t *node = list->first;
    
    if (node == NULL) {
        return NULL;
    }
    
    return List_remove(list, node);
}

void *List_remove(LCTHW_DoublyLinkedList_List_t *list, LCTHW_DoublyLinkedList_Node_t *node) 
{
    assert(list != NULL);
    assert(node != NULL);
    
    // list must be not empty
    assert(list->first != NULL);
    assert(list->last != NULL);

    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) {
        list->first = node->next;
        // Invalid list, somehow got a first that is NULL.
        assert(list->first != NULL);
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        // Invalid list, somehow got a next that is NULL.
        assert(list->last != NULL);
        list->last->next = NULL;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    
    list->count--;
    void *result = node->value;
    free(node);

    return result;
}

void List_reverse(LCTHW_DoublyLinkedList_List_t *list)
{
    assert(list != NULL);

    // list must be not empty
    assert(list->first != NULL);
    assert(list->last != NULL);

    if (list->first == list->last) {
        // only contains one element, nothing to do
        assert(list->count == 1);
        return;
    }

    LCTHW_DoublyLinkedList_Node_t *loopContinue = NULL;

    for (LCTHW_DoublyLinkedList_Node_t *cur = list->last; cur != NULL; cur = loopContinue) {
        LCTHW_DoublyLinkedList_Node_t *curPrev = cur->prev;
        LCTHW_DoublyLinkedList_Node_t *curNext = cur->next;
        
        cur->prev = curNext;
        cur->next = curPrev;
        
        loopContinue = curPrev;
        
        /*printf("worked on '%s':\n", (char *) cur->value);
        printf("\t prev: %s\n", (cur->prev == NULL ? "-" : (char *) cur->prev->value));
        printf("\t next: %s\n", (cur->next == NULL ? "-" : (char *) cur->next->value));*/
    }
    
    LCTHW_DoublyLinkedList_Node_t *currentFirstNode = list->first;
    LCTHW_DoublyLinkedList_Node_t *currentLastNode = list->last;

    list->first = currentLastNode;
    list->last = currentFirstNode;
    
    /*printf("---------------\n");
    
    LIST_FOREACH(list, first, next, cur) {
        printf("current: %s\n", (char *) cur->value);
    }*/
}

/*static void List_swapNodes(LCTHW_DoublyLinkedList_List_t *node1, LCTHW_DoublyLinkedList_List_t *node2)
{
    assert(node1 != NULL && node2 != NULL, "Both nodes for swapping are NULL.");

    if (node1 == node2) {
        return;
    }
    

}*/

int List_bubble_sort(LCTHW_DoublyLinkedList_List_t *list, sort_callback callback) 
{
    assert(list != NULL);

    
    //printf("========================\n");
    
    bool swapped;
    int i = 0;
   
    do {    
        i++;
        swapped = false;
        
        int j = 0;
        LCTHW_DoublyLinkedList_Node_t *loopContinue = NULL;
    
        //printf("Starting pass %d\n", i);
    
        for (LCTHW_DoublyLinkedList_Node_t *cur = list->first; cur != NULL; cur = loopContinue) {
            j++;

            /*printf("next iter: \n");
            printf("Working on %s\n", (cur != NULL) ? (char *) cur->value : "-");
            printf("\tcur->prev: %s\n", (cur->prev != NULL ? (char *) cur->prev->value : "-"));
            printf("\tcur->next: %s\n", (cur->next != NULL ? (char *) cur->next->value : "-"));
            printf("Comparing %s and %s\n", (char *) cur->value, (cur->next != NULL) ? (char *) cur->next->value : "-");*/

            if (cur->next != NULL && callback(cur->value, cur->next->value) > 0) {
                LCTHW_DoublyLinkedList_Node_t *nodeToSwapLeft = cur;        // C
                LCTHW_DoublyLinkedList_Node_t *nodeToSwapLeft_prev = nodeToSwapLeft->prev; // B
                
                LCTHW_DoublyLinkedList_Node_t *nodeToSwapRight = cur->next; // D
                LCTHW_DoublyLinkedList_Node_t *nodeToSwapRight_next = nodeToSwapRight->next; // E

                nodeToSwapLeft->prev = nodeToSwapRight;// D
                nodeToSwapLeft->next = nodeToSwapRight_next;// E
                
                nodeToSwapRight->prev = nodeToSwapLeft_prev; // B
                nodeToSwapRight->next = nodeToSwapLeft; // C
                
                if (nodeToSwapLeft_prev != NULL) {
                    nodeToSwapLeft_prev->next = nodeToSwapRight; 
                }
                
                if (nodeToSwapRight_next != NULL) {
                    nodeToSwapRight_next->prev = nodeToSwapLeft;
                }
                
                if (list->first == nodeToSwapLeft) {
                    list->first = nodeToSwapRight;
                    list->first->prev = NULL;
                }
            
                if (list->last == nodeToSwapRight) {
                    list->last = nodeToSwapLeft;
                    list->last->next = NULL;
                }
                
                
                /*
                //printf("swapping %s and %s\n", (char *) cur->value, (char *) cur->next->value);
                
                cur->prev = currentNext; // C
                cur->next = currentNext->next; // D
                // C <- B -> D
            
                //currentNext->prev = currentPrev; // A
                //printf("\t setting prev to %s on %s\n   ", currentPrev ? (char *) currentPrev->value : "-", (char *) currentNext->value);
                //currentNext->next = cur; // B
                // A <- C -> B
            
                if (currentPrev != NULL) {
                    currentPrev->next = currentNext;
                }
            
                if (currentNext != NULL) {
                    currentNext->prev = currentPrev;
                    //printf("setting prev to %s at %s\n",  currentPrev ? (char *) currentPrev->value : "-",  (char *) currentNext->value);
                }*/

                //printf("\tcur->prev: %s\n", (cur->prev != NULL ? (char *) cur->prev->value : "-"));
                //printf("\tcur->next: %s\n", (cur->next != NULL ? (char *) cur->next->value : "-"));
            
                loopContinue = cur;
                swapped = true;
            } else {
                loopContinue = cur->next;
            }

            /*printf("Continuing with: %s\n", (loopContinue != NULL) ? (char *) loopContinue->value : "-");
            printf("\t prev: %s\n", (loopContinue == NULL || loopContinue->prev == NULL ? "-" : (char *) loopContinue->prev->value));
            printf("\t next: %s\n", (loopContinue == NULL || loopContinue->next == NULL ? "-" : (char *) loopContinue->next->value));
            printf("first: %s\n", (char *) list->first->value);
            printf("first->next: %s\n", (char *) list->first->next->value);
            printf("--------------\n");*/
        }
    
        /*printf("============ CURRENT STATE ============\n");
   
        //printf("first: %s\n", (char *) list->first->value);
        //printf("first->next: %s\n", (char *) list->first->next->value);
    
        LIST_FOREACH(list, first, next, cur) {
            printf("cur %s\n", (char *) cur->value);
            printf("\tcur->prev: %s\n", (cur->prev != NULL ? (char *) cur->prev->value : "-"));
            printf("\tcur->next: %s\n", (cur->next != NULL ? (char *) cur->next->value : "-"));
        }
    
        printf("\n");
        printf("========================\n");*/
    
        /*if (swapped) {
            printf("Swap happened, next pass\n\n");
        } else {
            printf("NO Swap happened, end\n\n");
        }*/
        
        
        //break;

    } while (swapped);
    
    return 0;
}
