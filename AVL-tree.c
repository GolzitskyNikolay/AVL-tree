#include "AVL-tree.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

// Используется, чтобы отслеживать последний пройденный узел с ненулевым коэффициентом сбалансированности,
// чтобы после вставки элемента сбалансировать дерево.
struct avl_node **v;

// Последний пройденный узел с ненулевым коэффициентом сбалансированности
struct avl_node *x;

// Потомок x, который содержит вставленный узел
struct avl_node *w;

// Узел, который добавили
struct avl_node *y;

// Узел, хранящий значение y
struct avl_node *z;

struct avl_node *createNode(struct avl_tree *tree, char *data) {
    struct avl_node *node = malloc(sizeof *node); // выделяем память под узел дерева
    node->data = data;
    node->link[0] = NULL; // создаём пустых потомков
    node->link[1] = NULL;
    node->balance = 0;    // изначально коэффициент сбалансированности = 0
    tree->count++;       // увеличиваем количество узлов в дереве на 1
    return node;
};

/*
 * Поиск подходящей позиции и последующая вставка элемента.
 */
int searchPlaceForInsert(char *data, struct avl_tree *tree, int isString) {
    struct avl_node *nextNode = y;
    struct avl_node *node = z;

    while (1) {
        int rightOrLeft;

        // Если такой элемент уже есть в дереве, то функцию можно завершить.
        if ((isString == 1 && strcmp(data, node->data) == 0) || (isString == 0 && atof(data) == atof(node->data)))
            return 2;

        // Сравниваем "rightOrLeft" со значением текущего node, если node больше, то rightOrLeft = 0
        //(идем влево от node), а иначе rightOrLeft = 1 (идем вправо от node).
        if (isString == 1) {
            if (strcmp(data, node->data) == -1) {
                rightOrLeft = 0;
            } else {
                rightOrLeft = (strcmp(data, node->data));
            }
        } else {
            rightOrLeft = (atof(data) > atof(node->data));
        }

        nextNode = node->link[rightOrLeft];

        // В случае если y == NULL, то происходит вставка элемента в дерево
        if (nextNode == NULL) {
            nextNode = node->link[rightOrLeft] = createNode(tree, data);
            if (nextNode == NULL) return 0;
            break;
        }

        // Отслеживаем последний пройденный узел с ненулевым коэффициентом сбалансированности,
        // чтобы после вставки элемента сбалансировать дерево (если коэффициент сбалансированности равен 0,
        // то при вставке нового узла он изменится на +1 или -1, и в этом случае балансировка не потребуется.
        // Если же коэффициент изначально не равен 0, то после вставки узла дерево придется балансировать).
        if (nextNode->balance != 0) {
            v = &node->link[rightOrLeft];
            x = nextNode;
        }
        node = nextNode;
    }
    y = nextNode;
    z = node;
    return 0;
}

/*
 * Пересчет коэффициентов сбалансированности для узлов, затронутых вставкой
 */
void recalculateCoefficientBalance(char *data, int isString) {
    if (isString == 1) {
        if (strcmp(data, x->data) == -1) {
            z = x->link[0];
            z->balance--;
            w = z;
        } else {
            z = x->link[strcmp(data, x->data)];
            z->balance++;
            w = z;
        }
    } else {
        int rightOrLeft = atof(data) > atof(x->data);
        z = x->link[rightOrLeft];
        if (rightOrLeft == 1) {
            z->balance++;
        } else {
            z->balance--;
        }
        w = z;
    }

    // z принимает значение y
    while (z != y)
        if ((isString == 1 && strcmp(data, z->data) == -1) || (isString == 0 && (atof(data) < atof(z->data)))) {
            z->balance = -1;     // если вставленный элемент левый потомок z
            z = z->link[0];
        } else {
            z->balance = +1;    // если вставленный элемент правый потомок z
            z = z->link[1];
        }
}

int insert(struct avl_tree *tree, char *data, int isString) {

    // Если в дереве еще нет узлов
    v = &tree->root;
    x = z = tree->root;
    if (x == NULL) {
        tree->root = createNode(tree, data);
        return tree->root != NULL;
    }

    // Поиск подходящей позиции и последующая вставка элемента
    if (searchPlaceForInsert(data, tree, isString) == 2) return 0;

    // Пересчет коэффициентов сбалансированности для узлов, затронутых вставкой
    recalculateCoefficientBalance(data, isString);

// Балансировка при добавлении нового узла в левое поддерево, то есть дерево имеет следующий вид:
//             |                                         |
//          -2(x)                                      0(w)
//           /   \                                     /   \
//       -1(w)     C   __h-2        ==>               A     (x)0
//        /   \                                      /     /  \
//       A     B     ___h-1                       (y)    B     C  _________h
//       |
//      (y)   _________h

    if ((isString == 1 && strcmp(data, x->data) == -1) || (isString == 0 && (atof(data) < atof(x->data)))) {
        struct avl_node **v1 = v;
        struct avl_node *x1 = x;
        struct avl_node *w1 = w;
        if (x->balance != -1) {
            x->balance--;                // x->balance = 0,
        } else
            if (w->balance == -1) {
            *v = w;                      // x->balance = -2 и w->balance = -1 => делаем балансировку для 1 случая
            x->link[0] = w->link[1];
            w->link[1] = x;
            x->balance = w->balance = 0;
        } else {                         // x->balance = -2 и w->balance = +1 => делаем балансировку для 2 случая
            *v = z = w->link[1];
            w->link[1] = z->link[0];
            z->link[0] = w;
            x->link[0] = z->link[1];
            z->link[1] = x;
            if (z->balance == -1) {
                x->balance = 1;
                w->balance = 0;
            } else if (z->balance == 0)
                x->balance = w->balance = 0;
            else {
                x->balance = 0;
                w->balance = -1;
            }
            z->balance = 0;
        }

    }

/* фрагмент № 4. Балансировка при добавлении нового узла в правое поддерево */
    else {
        if (x->balance != +1) {
            x->balance++;
        }else
            if (w->balance == +1) {
            *v = w;
            x->link[1] = w->link[0];
            w->link[0] = x;
            x->balance = w->balance = 0;
        } else {
            *v = z = w->link[0];
            w->link[0] = z->link[1];
            z->link[1] = w;
            x->link[1] = z->link[0];
            z->link[0] = x;
            if (z->balance == +1) {
                x->balance = -1;
                w->balance = 0;
            } else if (z->balance == 0)
                x->balance = w->balance = 0;
            else {
                x->balance = 0;
                w->balance = 1;
            }
            z->balance = 0;
        }
    }
    return 1;
}