#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <stdio.h>

struct avl_node{
    struct avl_node *link[2];
    char *data;
    short bal;  // коэффициент сбалансированности (-1, 0 или 1)
};

struct avl_tree{
    int count;
    struct avl_node *root;
};

struct avl_node *new_node(struct avl_tree *tree, char *data);

int insert(struct avl_tree *tree, char *data, int stringOrNot);

#endif
