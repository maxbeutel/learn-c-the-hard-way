#include <stdlib.h>
#include <assert.h>

// debug
#include <stdio.h>

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


static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *data)
{
    BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
    assert(node != NULL);

    node->parent = parent;
    node->key = key;
    node->data = data;

    return node;
}

static inline void BSTree_appendChild(BSTree *map, BSTreeNode *parent, void *key, void *data)
{
    assert(map != NULL);
    assert(parent != NULL);

    int compareResult = map->compare(parent->key, key);

    // create new child node left
    if (compareResult <= 0) {
        if (parent->left == NULL) {
            parent->left = BSTreeNode_create(parent, key, data);
        } else {
            BSTree_appendChild(map, parent->left, key, data);
        }

        return;
    }

    // create new child on the right
    if (parent->right == NULL) {
        parent->right = BSTreeNode_create(parent, key, data);
    } else {
        BSTree_appendChild(map, parent->right, key, data);
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
    map->size++;
}

void BSTree_findChild(BSTree *map, BSTreeNode **child, BSTreeNode *parent, void *key)
{
    assert(map != NULL);

    if (parent == NULL) {
        return;
    }

    int compareResult = map->compare(parent->key, key);

    if (compareResult == 0) {
        *child = parent;

        return;
    }

    // go left
    if (compareResult <= 0) {
        BSTree_findChild(map, child, parent->left, key);
        return;
    }

    // go right
    BSTree_findChild(map, child, parent->right, key);
}

void *BSTree_get(BSTree *map, void *key)
{
    assert(map != NULL);

    if (map->size == 0) {
        return NULL;
    }

    BSTreeNode *child = NULL;
    BSTree_findChild(map, &child, map->root, key);

    // not found
    // @TODO how to distinguish between not found and NULL data?!
    if (child == NULL) {
        return NULL;
    }

    return child->data;
}
