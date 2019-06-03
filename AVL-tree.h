#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <stdio.h>

struct avl_node{
    struct avl_node *link[2];  // потомки
    char *data;  // значение
    // коэффициент сбалансированности (-1, 0 или 1), разница между высотой правого и левого поддеревьев
    int balance;
};

struct avl_tree{
    int count;  // количество элементов в дереве
    struct avl_node *root;

    // Используются в поиске позиции для вставки элемента, для пересчёта коэффициентов
    // сбалансированности и для хранения значения добавляемого узла.
    struct avl_node *y, *z;

    // Используются, чтобы отслеживать последний пройденный узел с ненулевым коэффициентом сбалансированности,
    // чтобы после вставки элемента сбалансировать дерево.
    struct avl_node **v, *x;

    // Запоминаем w, чтобы правильно сделать балансировку.
    // Когда нужно делать балансировку, то становится потомком x, который содержит последний вставленный узел
    struct avl_node *w;
};

struct avl_node *createNode(struct avl_tree *tree, char *data);

int insert(struct avl_tree *tree, char *data, int stringOrNot);

int delete(struct avl_tree *tree, char *data, int isString);

#endif
