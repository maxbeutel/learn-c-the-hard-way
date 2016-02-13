#ifndef _binary_search_trees_h
#define _binary_search_trees_h

#include <stdlib.h>

typedef int (*BSTree_compare)(void *a, void *b);

typedef struct BSTreeEdge {
    void *key;
    void *childKey;
    int childPosition;
} BSTreeEdge;

typedef enum {
    BS_TREE_NODE_CHILD_POSITION_UNDEFINED = -1,
    BS_TREE_NODE_CHILD_POSITION_LEFT = 0,
    BS_TREE_NODE_CHILD_POSITION_RIGHT,
} BSTreeNode_childPosition;

typedef struct BSTreeNode {
    void *key;
    void *data;

    struct BSTreeNode *left;
    struct BSTreeNode *right;
    struct BSTreeNode *parent;
} BSTreeNode;

typedef struct BSTree {
    size_t size;
    BSTree_compare compare;
    BSTreeNode *root;
} BSTree;

BSTree *BSTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree *map);

void BSTree_set(BSTree *map, void *key, void *data);
void *BSTree_get(BSTree *map, void *key);

int BSTree_contains(BSTree *map, void *key);

void BSTree_delete(BSTree *map, void *key);

#endif
