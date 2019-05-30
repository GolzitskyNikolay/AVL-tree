#include <stdio.h>
#include <malloc.h>
#include "AVL-tree.h"

void testForString() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "a", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "ab", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "b", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "cd", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "e", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "ea", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");

    insert(tree, "eb", 1);
    printTree(tree->root, 0, tree, NULL);
    printf("----------------\n");
}