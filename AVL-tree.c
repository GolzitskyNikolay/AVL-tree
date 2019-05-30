#include "AVL-tree.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct avl_node *new_node(struct avl_tree *tree, char *data) {
    struct avl_node *node = malloc(sizeof *node); // выделяем память под узел дерева
    node->data = data;
    node->link[0] = NULL; // создаём пустые корни
    node->link[1] = NULL;
    node->bal = 0;       // изначально коэффициент сбалансированности = 0
    tree->count++;       // увеличиваем количество узлов в дереве на 1
    return node;
};

int insert(struct avl_tree *tree, char *data, int isString) {
    struct avl_node **v, *w, *x, *y, *z;

/* если в дереве еще нет узлов */
    v = &tree->root;
    x = z = tree->root;
    if (x == NULL) {
        tree->root = new_node(tree, data);
        return tree->root != NULL;
    }

    /* фрагмент №1 поиск подходящей позиции и последующая вставка элемента */
    for (;;) {
        int dir;
        /* такой элемент уже есть в дереве – функцию можно завершить */
        if ((isString == 1 && strcmp(data, z->data) == 0) || (isString == 0 && atof(data) == atof(z->data))) return 2;

        if (isString == 1) {
            if (strcmp(data, z->data) == -1) {
                dir = 0;
            } else {
                dir = (strcmp(data, z->data)); // data > z->data
            }
        } else {
            dir = (atof(data) > atof(z->data));
        }

        y = z->link[dir];
        if (y == NULL) {
            y = z->link[dir] = new_node(tree, data);
            if (y == NULL) return 0;
            break;
        }
        if (y->bal != 0) {
            v = &z->link[dir];
            x = y;
        }
        z = y;
    }
    /* фрагмент №2. Пересчет коэффициентов сбалансированности для узлов, затронутых вставкой */

    if (isString == 1) {
        if (strcmp(data, x->data) == -1) {
            w = z = x->link[0];
        } else {
            w = z = x->link[strcmp(data, x->data)];  // data > x->data
        }
    } else {
        w = z = x->link[atof(data) > atof(x->data)];
    }

    while (z != y)
        if ((isString == 1 && strcmp(data, z->data) == -1) ||
            (isString == 0 && (atof(data) < atof(z->data)))) {  // если data < z->data
            z->bal = -1;
            z = z->link[0];
        } else {
            z->bal = +1;
            z = z->link[1];
        }

/* фрагмент № 3. балансировка при добавлении нового узла в левое поддерево */
    if ((isString == 1 && strcmp(data, x->data) == -1) ||
        (isString == 0 && (atof(data) < atof(x->data)))) {  // если data < x->data
        if (x->bal != -1)
            x->bal--;
        else if (w->bal == -1) {
            *v = w;
            x->link[0] = w->link[1];
            w->link[1] = x;
            x->bal = w->bal = 0;
        } else {
            *v = z = w->link[1];
            w->link[1] = z->link[0];
            z->link[0] = w;
            x->link[0] = z->link[1];
            z->link[1] = x;
            if (z->bal == -1) {
                x->bal = 1;
                w->bal = 0;
            } else if (z->bal == 0)
                x->bal = w->bal = 0;
            else {
                x->bal = 0;
                w->bal = -1;
            }
            z->bal = 0;
        }
    }
/* фрагмент № 4. Балансировка при добавлении нового узла в правое поддерево */
    else {
        if (x->bal != +1)
            x->bal++;
        else if (w->bal == +1) {
            *v = w;
            x->link[1] = w->link[0];
            w->link[0] = x;
            x->bal = w->bal = 0;
        } else {
            *v = z = w->link[0];
            w->link[0] = z->link[1];
            z->link[1] = w;
            x->link[1] = z->link[0];
            z->link[0] = x;
            if (z->bal == +1) {
                x->bal = -1;
                w->bal = 0;
            } else if (z->bal == 0)
                x->bal = w->bal = 0;
            else {
                x->bal = 0;
                w->bal = 1;
            }
            z->bal = 0;
        }
    }
    return 1;
}