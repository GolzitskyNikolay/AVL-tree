#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <stdio.h>

struct avl_node{
    struct avl_node *link[2];
    char *data;
    // коэффициент сбалансированности (-1, 0 или 1), разница между высотой правого и левого поддеревьев
    short balance;
};

struct avl_tree{
    int count;
    struct avl_node *root;
};

struct avl_node *createNode(struct avl_tree *tree, char *data);

int insert(struct avl_tree *tree, char *data, int stringOrNot);

#endif
