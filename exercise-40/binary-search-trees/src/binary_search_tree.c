#include <stdlib.h>
#include <assert.h>

// debug
#include <stdio.h>

#include "binary_search_tree.h"
#include "bstrlib.h"

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

BSTree *BSTree_create(BSTree_compare compare)
{
    BSTree *map = calloc(1, sizeof(BSTree));
    assert(map != NULL);

    map->compare = compare == NULL ? default_compare : compare;

    return map;
}

/* static int BSTree_destroy_cb(BSTreeNode *node) */
/* { */
/*     assert(node != NULL); */

/*     free(node); */

/*     return 0; */
/* } */

void BSTree_destroy(BSTree *map)
{
    /* if (map) { */
    /*     BSTree_traverse(map, BSTree_destroy_cb); */
    /* } */

    if (map->root) {
        free(map->root);
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
        }

        return;
    }

    // create new child on the right
    if (parent->right == NULL) {
        parent->right = BSTreeNode_create(parent, key, data);
    }
}

void BSTree_set(BSTree *map, void *key, void *data)
{
    assert(map != NULL);

    if (map->root == NULL) {
        map->root = BSTreeNode_create(NULL, key, data);
        map->count++;

        return;
    }

    BSTree_appendChild(map, map->root, key, data);
    map->count++;
}

void *BSTree_get(BSTree *map, void *key)
{
    assert(map != NULL);

    if (map->count == 0) {
        return NULL;
    }

    // sanity check
    assert(map->root != NULL);

    int rootCompareResult = map->compare(root->key, key);

    if (rootCompareResult == 0) {
        return map->root->data;
    }

    // go left
    if (compareResult <= 0) {

    }

    // go right
}
