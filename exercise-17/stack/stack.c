#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//////// -- Persons --
typedef struct Person
{
    char *name;
    int age; 
} Person_t;

Person_t *Person_create(char *name, int age)
{
    Person_t *person = malloc(sizeof(Person_t));
    assert(person != NULL);
    
    person->name = malloc(strlen(name) + 1);
    assert(person->name != NULL);
    strcpy(person->name, name);

    person->age = age;
    
    return person;
}

void Person_destroy(Person_t *person)
{
    if (person->name != NULL) {
        free(person->name);
    }

    free(person);
}


//////// -- Singly Linked List --
typedef struct SinglyLinkedList_Node
{
    Person_t *person;
    struct SinglyLinkedList_Node *next;
} SinglyLinkedList_Node_t;

typedef struct SinglyLinkedList
{
    SinglyLinkedList_Node_t *head;
    int size;
} SinglyLinkedList_t;


SinglyLinkedList_t *SinglyLinkedList_create()
{
    SinglyLinkedList_t *list = malloc(sizeof(SinglyLinkedList_t));
    assert(list != NULL);
    
    list->head = NULL;
    list->size = 0;
    
    return list;
}

void SinglyLinkedList_destroy(SinglyLinkedList_t *list)
{
    if (list) {
        SinglyLinkedList_Node_t *currentNode = list->head;

        while (currentNode != NULL) {
            SinglyLinkedList_Node_t *tmp = currentNode;
            currentNode = tmp->next;
    
            free(tmp);
        }   
    }

    free(list);
}


// list access
void SinglyLinkedList_push(SinglyLinkedList_t *list, Person_t *person)
{
    assert(list != NULL);
    assert(person != NULL);

    SinglyLinkedList_Node_t *node = malloc(sizeof(SinglyLinkedList_Node_t));
    assert(node != NULL);

    node->person = person;
    node->next = NULL;
    
    if (list->head != NULL) {
        node->next = list->head;
    }

    list->head = node;
    list->size += 1;
}

bool SinglyLinkedList_remove(SinglyLinkedList_t *list, Person_t *person)
{
    assert(list != NULL);
    assert(person != NULL);
    
    SinglyLinkedList_Node_t *currentNode = list->head;
    SinglyLinkedList_Node_t *previousNode = NULL;
        
    if (currentNode == NULL) {
        return false;
    }
    
    bool matching = false;
    
    while (currentNode != NULL) {
        if (matching) {
            previousNode->next = currentNode->next;
            list->size -= 1;

            free(currentNode);
            
            return true;
        }

        if (strcmp(currentNode->person->name, person->name) == 0) {
            matching = true;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    
    return false;
}

void SinglyLinkedList_dump(SinglyLinkedList_t *list)
{
    assert(list != NULL);
    
    SinglyLinkedList_Node_t *currentNode = list->head;

    if (currentNode == NULL) {
        printf("ERROR: can't dump empty list\n");
        
        return;
    }
    
    printf("DUMP: List size is %d\n", list->size);
    
    int i = 0;
    
    while (currentNode != NULL) {
        i++;

        printf("DUMP: entry %d - current person: %s\n", i, currentNode->person->name);

        currentNode = currentNode->next;
    }
}

Person_t *SinglyLinkedList_pop(SinglyLinkedList_t *list)
{
    assert(list != NULL);
    
    if (list->head == NULL) {
        return NULL;
    }
    
    SinglyLinkedList_Node_t *currentNode = list->head;
    
    list->head = currentNode->next;
    list->size -= 1;
    
    Person_t *person = currentNode->person;
    
    free(currentNode);
    
    return person;
}

int main(void)
{    
    // check: http://codereview.stackexchange.com/questions/67617/singly-linked-list-implementation-in-c
    
    Person_t *person_1 = Person_create("Max", 28);
    Person_t *person_2 = Person_create("Manfred", 21);
    Person_t *person_3 = Person_create("Hugo", 46);
    Person_t *person_4 = Person_create("Stefan", 33);
    Person_t *person_5 = Person_create("Karl", 41);
    Person_t *person_6 = Person_create("Otto", 15);

    SinglyLinkedList_t *list = SinglyLinkedList_create();
    
    SinglyLinkedList_push(list, person_1);
    SinglyLinkedList_push(list, person_2);
    SinglyLinkedList_push(list, person_3);
    SinglyLinkedList_push(list, person_4);
    SinglyLinkedList_push(list, person_5);
    SinglyLinkedList_push(list, person_6);
    
    SinglyLinkedList_dump(list);
    
    printf("===== removing 1 person ======\n");
    
    if (SinglyLinkedList_remove(list, person_6)) {
        printf("REMOVAL: success\n");
    }
    
    SinglyLinkedList_dump(list);
    
    printf("===== popping 2 items ======\n");
    
    SinglyLinkedList_pop(list);
    SinglyLinkedList_pop(list);
    
    SinglyLinkedList_dump(list);
    
    Person_destroy(person_1);
    Person_destroy(person_2);
    Person_destroy(person_3);
    Person_destroy(person_4);
    Person_destroy(person_5);
    Person_destroy(person_6);
    
    SinglyLinkedList_destroy(list);

    return 0;
}








