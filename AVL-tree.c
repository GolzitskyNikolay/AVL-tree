#include "AVL-tree.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/*
 * Функция создания узла дерева.
 */
struct avl_node *createNode(struct avl_tree *tree, char *data) {
    struct avl_node *node = malloc(sizeof *node);  // выделяем память под узел дерева
    node->data = data;
    node->link[0] = NULL;  // создаём пустых потомков
    node->link[1] = NULL;
    node->balance = 0;     // изначально коэффициент сбалансированности = 0
    tree->count++;         // увеличиваем количество узлов в дереве на 1
    return node;
};

/*
 * Поиск подходящей позиции и последующая вставка элемента.
 * Здесь tree->z - это текущий узел дерева, а tree->y - следующий.
 */
int searchPlaceForInsert(char *data, struct avl_tree *tree, int isString) {

    while (1) {
        int rightOrLeft;

        // Если такой элемент уже есть в дереве, то функцию можно завершить.
        if ((isString == 1 && strcmp(data, tree->z->data) == 0) ||
        (isString == 0 && atof(data) == atof(tree->z->data))) return 2;

        // Сравниваем "rightOrLeft" со значением текущего узла, если узел больше, то rightOrLeft = 0
        //(идем влево от node), а иначе rightOrLeft = 1 (идем вправо от узла).
        if (isString == 1) {
            if (strcmp(data, tree->z->data) == -1) {
                rightOrLeft = 0;
            } else {
                rightOrLeft = (strcmp(data, tree->z->data));
            }
        } else {
            rightOrLeft = (atof(data) > atof(tree->z->data));
        }

        tree->y = tree->z->link[rightOrLeft];

        // В случае если y == NULL, то происходит вставка элемента в дерево
        if (tree->y == NULL) {
            tree->y = tree->z->link[rightOrLeft] = createNode(tree, data);
            if (tree->y == NULL) return 0;
            break;
        }

        // Отслеживаем последний пройденный узел с ненулевым коэффициентом сбалансированности,
        // чтобы после вставки элемента сбалансировать дерево.
        // Если коэффициент сбалансированности равен 0, то при вставке нового узла он изменится на +1 или -1,
        // и в этом случае балансировка не потребуется. Если же коэффициент изначально не равен 0, то после
        // вставки узла дерево придется балансировать.
        if (tree->y->balance != 0) {
            tree->v = &tree->z->link[rightOrLeft];
            tree->x = tree->y;
        }
        tree->z = tree->y;
    }
    return 0;
}

/*
 * Пересчет коэффициентов сбалансированности для узлов, затронутых вставкой
 */
void recalculateCoefficientBalance(char *data, struct avl_tree *tree, int isString) {
    if (isString == 1) {
        if (strcmp(data, tree->x->data) == -1) {
            tree->z = tree->x->link[0];
            tree->w = tree->z;
        } else {
            tree->z = tree->x->link[strcmp(data, tree->x->data)];
            tree->w = tree->z;
        }
    } else {
        int rightOrLeft = atof(data) > atof(tree->x->data);
        tree->z = tree->x->link[rightOrLeft];
        tree->w = tree->z;
    }

    while (tree->z != tree->y)
        if ((isString == 1 && strcmp(data, tree->z->data) == -1) ||
        (isString == 0 && (atof(data) < atof(tree->z->data)))) {
            tree->z->balance = -1;     // если вставленный элемент левый потомок z
            tree->z = tree->z->link[0];
        } else {
            tree->z->balance = +1;    // если вставленный элемент правый потомок z
            tree->z = tree->z->link[1];
        }
}

/*
 *  Балансировка при добавлении нового узла в левое поддерево, то есть дерево имеет следующий вид:
 *  ------------------------------------------------------------------------------------------
 *              |v                                        |v
 *           -2(x)                                      0(w)
 *            /   \                                     /   \
 *        -1(w)     C   __h-2        ==>               A     (x)0
 *         /   \                                      /     /  \
 *        A     B     ___h-1                       (y)    B     C  _________h
 *        |
 *       (y)   _________h
 *
 * --------------------------------------------------------------------------------------------
 *              |v                                         |v
 *           -2(x)                                        (y)
 *            /   \                                     /     \
 *        +1(w)     D   __h-2        ==>              (w)     (x)
 *         /   \                                      / \     /  \
 *       A   -1(y)     ___h-1                       A    B   C    D
 *            /   \
 *           B     C  _____h
 * --------------------------------------------------------------------------------------------
 */
void leftBalancing(struct avl_tree *tree) {
    if (tree->x->balance != -1) {
        tree->x->balance--;
    } else if (tree->w->balance == -1) {
        *tree->v = tree->w;                      // x->balance = -2 и w->balance = -1 => делаем балансировку для 1 случая
        tree->x->link[0] = tree->w->link[1];
        tree->w->link[1] = tree->x;
        tree->x->balance = tree->w->balance = 0;
    } else {                         // x->balance = -2 и w->balance = +1 => делаем балансировку для 2 случая
        *tree->v = tree->z = tree->w->link[1];   // v и z = y
        tree->w->link[1] = tree->z->link[0];     // w->link[1] = B
        tree->z->link[0] = tree->w;              // y->link[0] = w
        tree->x->link[0] = tree->z->link[1];     // x->link[0] = C
        tree->z->link[1] = tree->x;              // y->link[1] = x
        if (tree->z->balance == -1) {
            tree->x->balance = 1;
            tree->w->balance = 0;
        } else if (tree->z->balance == 0) {
            tree->x->balance = 0;
            tree->w->balance = 0;
        } else {
            tree->x->balance = 0;
            tree->w->balance = -1;
        }
        tree->z->balance = 0;
    }
}

/*
 * Балансировка при добавлении нового узла в правое поддерево (противоположно leftBalancing)
 */
void rightBalance(struct avl_tree *tree) {
    if (tree->x->balance != +1) {
        tree->x->balance++;
    } else if (tree->w->balance == +1) {
        *tree->v = tree->w;                       // x->balance = +2
        tree->x->link[1] = tree->w->link[0];
        tree->w->link[0] = tree->x;
        tree->x->balance = tree->w->balance = 0;
    } else {                          // x->balance = +2
        *tree->v = tree->z = tree->w->link[0];
        tree->w->link[0] = tree->z->link[1];
        tree->z->link[1] = tree->w;
        tree->x->link[1] = tree->z->link[0];
        tree->z->link[0] = tree->x;
        if (tree->z->balance == +1) {
            tree->x->balance = -1;
            tree->w->balance = 0;
        } else if (tree->z->balance == 0)
            tree->x->balance = tree->w->balance = 0;
        else {
            tree->x->balance = 0;
            tree->w->balance = 1;
        }
        tree->z->balance = 0;
    }
}

/*
 * Вставка элемента в дерево.
 */
int insert(struct avl_tree *tree, char *data, int isString) {

    tree->v = &tree->root;
    tree->x = tree->root;
    tree->z = tree->root;

    if (tree->x == NULL) {    // Если в дереве еще нет узлов
        tree->root = createNode(tree, data);
        return tree->root != NULL;
    }

    // Поиск подходящей позиции и последующая вставка элемента
    if (searchPlaceForInsert(data, tree, isString) == 2) return 0;

    // Пересчет коэффициентов сбалансированности для узлов, затронутых вставкой
    recalculateCoefficientBalance(data, tree, isString);

    if ((isString == 1 && strcmp(data, tree->x->data) == -1) ||
    (isString == 0 && (atof(data) < atof(tree->x->data)))) {
        leftBalancing(tree);  // Балансировка при добавлении нового узла в левое поддерево
    } else {
        rightBalance(tree);   // Балансировка при добавлении нового узла в правое поддерево
    }

    return 1;
}