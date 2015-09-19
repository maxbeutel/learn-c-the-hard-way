#ifndef _lcthw_doubly_linked_list_h
#define _lcthw_doubly_linked_list_h

struct LCTHW_DoublyLinkedList_Node;

typedef struct LCTHW_DoublyLinkedList_Node {
    struct LCTHW_DoublyLinkedList_Node *prev;
    struct LCTHW_DoublyLinkedList_Node *next;
    void *value;
} LCTHW_DoublyLinkedList_Node_t;

typedef struct LCTHW_DoublyLinkedList_List {
    int count;
    LCTHW_DoublyLinkedList_Node_t *first;
    LCTHW_DoublyLinkedList_Node_t *last;
} LCTHW_DoublyLinkedList_List_t;

LCTHW_DoublyLinkedList_List_t *List_create();
void List_destroy(LCTHW_DoublyLinkedList_List_t *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first == NULL ? NULL : (A)->first->value)
#define List_last(A) ((A)->last == NULL ? NULL : (A)->last->value)

void List_push(LCTHW_DoublyLinkedList_List_t *list, void *value);
void *List_pop(LCTHW_DoublyLinkedList_List_t *list);

void List_unshift(LCTHW_DoublyLinkedList_List_t *list, void *value);
void *List_shift(LCTHW_DoublyLinkedList_List_t *list);

void *List_remove(LCTHW_DoublyLinkedList_List_t *list, LCTHW_DoublyLinkedList_Node_t *node);

void List_reverse(LCTHW_DoublyLinkedList_List_t *list);

typedef int (*sort_callback)(const void *str1, const void *str2);
int List_bubble_sort(LCTHW_DoublyLinkedList_List_t *list, sort_callback callback);

#define LIST_FOREACH(L, S, M, V) LCTHW_DoublyLinkedList_Node_t *_node = NULL;\
    LCTHW_DoublyLinkedList_Node_t *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
