#include <stdlib.h>
#include <assert.h>

// debug
//#include <stdio.h>

#include "binary_search_tree.h"
#include "bstrlib.h"

/* *** Internal Queue implementation *** */
typedef struct Queue {
    size_t size; // logical size, number of elements
    size_t capacity; // physical size

    void **items;
} Queue;

static Queue *Queue_create(size_t capacity)
{
    Queue *queue = calloc(1, sizeof(Queue));
    assert(queue != NULL);

    queue->size = 0;
    queue->capacity = capacity;

    queue->items = malloc(sizeof(void *) * capacity);

    if (queue->items == NULL) {
        goto error;
    }

    return queue;

error:
    if (queue != NULL) {
        free(queue->items);
    }

    free(queue);

    return NULL;
}

static void Queue_destroy(Queue *queue)
{
    if (queue != NULL) {
        free(queue->items);
    }

    free(queue);
}

static int Queue_enqueue(Queue *queue, void *item)
{
    assert(queue != NULL);
    assert(queue->size < queue->capacity);

    queue->items[queue->size] = item;
    queue->size++;

    return -1;
}

static int Queue_dequeue(Queue *queue, void **item)
{
    assert(queue != NULL);
    assert(queue->size <= queue->capacity);

    if (queue->size == 0) {
        return 0;
    }

    *item = queue->items[queue->size - 1];
    queue->size--;

    return 1;
}

/* *** Defaults *** */
static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

/* *** Binary Search Tree Implementation *** */
typedef enum {
    BS_TREE_CHILD_POSITION_LEFT = 0,
    BS_TREE_CHILD_POSITION_RIGHT,
} BSTree_childPosition;

BSTree *BSTree_create(BSTree_compare compare)
{
    BSTree *map = calloc(1, sizeof(BSTree));
    assert(map != NULL);

    map->compare = compare == NULL ? default_compare : compare;

    return map;
}

static void BSTree_enqueueChildren(Queue *queue, BSTreeNode *node)
{
    assert(queue != NULL);
    assert(node != NULL);

    if (node->left != NULL) {
        BSTree_enqueueChildren(queue, node->left);
    }

    if (node->right != NULL) {
        BSTree_enqueueChildren(queue, node->right);
    }

    Queue_enqueue(queue, node);
}

void BSTree_destroy(BSTree *map)
{
    if (map == NULL) {
        return;
    }

    Queue *queue = NULL;

    if (map->size != 0) {
        queue = Queue_create(map->size);

        if (queue == NULL) {
            goto error;
        }

        BSTree_enqueueChildren(queue, map->root);
        assert(queue->size == map->size && "Queue size does not match size tree structure, can't free unknown amount of nodes.");

        BSTreeNode *node = NULL;

        while (Queue_dequeue(queue, (void **) &node)) {
            free(node);
        }
    }

error:
    if (queue != NULL) {
        Queue_destroy(queue);
    }

    free(map);
}


static BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *data)
{
    BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
    assert(node != NULL);

    node->parent = parent;
    node->key = key;
    node->data = data;

    return node;
}

static void BSTree_doAppendChild(BSTree *map, BSTreeNode *parent, void *key, void *data, BSTree_childPosition childPosition)
{
    assert(map != NULL);
    assert(parent != NULL);

    if (childPosition == BS_TREE_CHILD_POSITION_LEFT) {
        parent->left = BSTreeNode_create(parent, key, data);
    } else if (childPosition == BS_TREE_CHILD_POSITION_RIGHT) {
        parent->right = BSTreeNode_create(parent, key, data);
    }

    map->size++;
}

static void BSTree_appendChild(BSTree *map, BSTreeNode *parent, void *key, void *data)
{
    assert(map != NULL);
    assert(parent != NULL);

    int compareResult = map->compare(parent->key, key);

    // create new child node left
    if (compareResult < 0) {
        if (parent->left == NULL) {
            /* parent->left = BSTreeNode_create(parent, key, data); */
            /* map->size++; */
            BSTree_doAppendChild(map, parent, key, data, BS_TREE_CHILD_POSITION_LEFT);
        } else {
            BSTree_appendChild(map, parent->left, key, data);
        }
    // create new child on the right
    } else if (compareResult > 0) {
        if (parent->right == NULL) {
            /* parent->right = BSTreeNode_create(parent, key, data); */
            /* map->size++; */
            BSTree_doAppendChild(map, parent, key, data, BS_TREE_CHILD_POSITION_RIGHT);
        } else {
            BSTree_appendChild(map, parent->right, key, data);
        }
    } else {
        //printf("replacing data\n");
        parent->data = data;
    }
}

void BSTree_set(BSTree *map, void *key, void *data)
{
    assert(map != NULL);

    if (map->root == NULL) {
        map->root = BSTreeNode_create(NULL, key, data);
        map->size++;

        return;
    }

    BSTree_appendChild(map, map->root, key, data);
}

static void BSTree_findChild(BSTree *map, BSTreeNode **childFound, BSTreeNode *child, void *key)
{
    assert(map != NULL);
    assert(child != NULL);

    int compareResult = map->compare(child->key, key);


    // go left
    if (compareResult < 0) {
        if (child->left) {
            BSTree_findChild(map, childFound, child->left, key);
        }
    // go right
    } else if (compareResult > 0) {
        if (child->right) {
            BSTree_findChild(map, childFound, child->right, key);
        }
    } else {
        *childFound = child;
    }
}

void *BSTree_get(BSTree *map, void *key)
{
    assert(map != NULL);

    if (map->size == 0) {
        return NULL;
    }

    BSTreeNode *childFound = NULL;
    BSTree_findChild(map, &childFound, map->root, key);

    if (childFound == NULL) {
        return NULL;
    }

    return childFound->data;
}

static void BSTree_deleteChild(BSTree *map, BSTreeNode *child, BSTreeNode *parent, void *key)
{
    assert(map != NULL);
    assert(child != NULL);

    int compareResult = map->compare(child->key, key);

    // go left
    if (compareResult < 0) {
        if (child->left) {
            BSTree_deleteChild(map, child->left, child, key);
        }
    // go right
    } else if (compareResult > 0) {
        if (child->right) {
            BSTree_deleteChild(map, child->right, child, key);
        }
    // found
    } else {
        //printf("found something: ");

        // current node doesn't have any children itself
        if (child->left == NULL && child->right == NULL) {
            //printf("easy case: child doesn't have any children - ");

            // current node is on left side of parent
            if (parent->left == child) {
                parent->left = NULL;
                free(child);
                map->size--;

                //printf("freeing child on left side of parent\n");
                // current node is on right side of parent
            } else if (parent->right == child) {
                parent->right = NULL;
                free(child);
                map->size--;

                //printf("freeing child on right side of parent\n");
            }
        } else if (child->left == NULL && child->right != NULL) {
            //printf("not so easy case: child has sibling on the right.\n");

            parent->right = child->right;
            free(child);
            map->size--;
        } else if (child->left != NULL && child->right == NULL) {
            //printf("not so easy case: child has sibling on the left.\n");

            parent->left = child->left;
            free(child);
            map->size--;
        }
    }
}

void BSTree_delete(BSTree *map, void *key)
{
    assert(map != NULL);

    if (map->size == 0) {
        return;
    }

    /* // special case, */
    /* int compareResult = map->compare(root->key, key); */

    /* if (compareResult == 0) { */

    /* } */

    BSTree_deleteChild(map, map->root, NULL, key);
}
