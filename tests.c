#include <assert.h>
#include <malloc.h>
#include "AVL-tree.h"
#include <string.h>

void testForCreateNode() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    tree->root = createNode(tree, "5");  // проверка на правильное создание node

    assert(strcmp(tree->root->data, "5") == 0);
    assert(tree->root->balance == 0);
    assert(tree->root->link[0] == NULL);
    assert(tree->root->link[1] == NULL);
    assert(tree->count == 1);

    free(tree);
}

void testForRoot() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "6", 0);
    assert(strcmp(tree->root->data, "6") == 0); // проверка, что для пустого дерева вставка произошла в корень
    free(tree);
}

void testForEquality() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "4", 0);
    assert(insert(tree, "4", 0) == 2); // проверка на то, что такой же элемент не вставится
    free(tree);
}

void testForLeft1() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "5", 0);                                //
    insert(tree, "6", 0);                                //           (5)
    insert(tree, "3", 0);                                //          /  |
    insert(tree, "4", 0);                                //        (3) (6)   __2
    insert(tree, "2", 0);                                //       /  |
    insert(tree, "1", 0);                                //     (2) (4)     ___3
                                                         //      |
                                                         //     (1)        ____4

    assert(strcmp(tree->root->data, "3") == 0);                     //
    assert(strcmp(tree->root->link[1]->data, "5") == 0);            //          (3)
    assert(strcmp(tree->root->link[1]->link[1]->data, "6") == 0);   //         /  |
    assert(strcmp(tree->root->link[1]->link[0]->data, "4") == 0);   //       (2) (5)
    assert(strcmp(tree->root->link[0]->data, "2") == 0);            //      /   / |
    assert(strcmp(tree->root->link[0]->link[0]->data, "1") == 0);   //    (1) (4)(6)
}

void testForLeft2() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "5", 0);                                //
    insert(tree, "6", 0);                                //           (5)
    insert(tree, "3", 0);                                //          /  |
    insert(tree, "4", 0);                                //        (3) (6)   __2
    insert(tree, "2", 0);                                //       /  |
    insert(tree, "3.5", 0);                              //     (2) (4)     ___3
                                                         //         /
                                                         //     (3.5)       ____4

    assert(strcmp(tree->root->data, "4") == 0);                     //
    assert(strcmp(tree->root->link[1]->data, "5") == 0);            //          (4)
    assert(strcmp(tree->root->link[1]->link[1]->data, "6") == 0);   //         /   |
    assert(strcmp(tree->root->link[0]->data, "3") == 0);            //      (3)    (5)
    assert(strcmp(tree->root->link[0]->link[1]->data, "3.5") == 0); //     /  |     |
    assert(strcmp(tree->root->link[0]->link[0]->data, "2") == 0);   //   (2) (3.5) (6)
}

void testForRight1() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "-5", 0);                               //
    insert(tree, "-6", 0);                               //          (-5)
    insert(tree, "3", 0);                                //          /  |
    insert(tree, "4", 0);                                //       (-6) (3)   ___2
    insert(tree, "2", 0);                                //            / |
    insert(tree, "5", 0);                                //          (2) (4) ___3
                                                         //               |
                                                         //              (5) ___4
                                                         //
                                                         //
    assert(strcmp(tree->root->data, "3") == 0);                     //
    assert(strcmp(tree->root->link[1]->data, "4") == 0);            //            (3)
    assert(strcmp(tree->root->link[1]->link[1]->data, "5") == 0);   //           /  |
    assert(strcmp(tree->root->link[0]->data, "-5") == 0);           //        (-5) (4)
    assert(strcmp(tree->root->link[0]->link[1]->data, "2") == 0);   //        / |   |
    assert(strcmp(tree->root->link[0]->link[0]->data, "-6") == 0);  //     (-6)(2) (5)
}

void testForRight2() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "-5", 0);                               //
    insert(tree, "-6", 0);                               //          (-5)
    insert(tree, "3", 0);                                //          /  |
    insert(tree, "4", 0);                                //       (-6) (3)   ___2
    insert(tree, "2", 0);                                //            / |
    insert(tree, "5", 0);                                //          (2) (4) ___3
                                                         //           |
                                                         //         (2.5)    ___4
                                                         //
                                                         //
    assert(strcmp(tree->root->data, "3") == 0);                     //
    assert(strcmp(tree->root->link[1]->data, "4") == 0);            //             (2)
    assert(strcmp(tree->root->link[1]->link[1]->data, "5") == 0);   //            /  |
    assert(strcmp(tree->root->link[0]->data, "-5") == 0);           //        (-5)  (3)
    assert(strcmp(tree->root->link[0]->link[1]->data, "2") == 0);   //        /    /  |
    assert(strcmp(tree->root->link[0]->link[0]->data, "-6") == 0);  //     (-6) (2.5) (4)
}

void testForDelete() {
    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    insert(tree, "-5", 0);                               //
    insert(tree, "-6", 0);                               //          (-5)
    insert(tree, "3", 0);                                //          /  |
    insert(tree, "4", 0);                                //       (-6) (3)   ___2
    insert(tree, "2", 0);                                //            / |
    insert(tree, "2.5", 0);                              //          (2) (4) ___3
                                                         //           |
                                                         //         (2.5)    ___4
                                                         //
                                                         //
    delete(tree, "4", 0);
    assert(strcmp(tree->root->data, "2") == 0);                             //
    assert(strcmp(tree->root->link[1]->data, "3") == 0);                    //      (2)
    assert(strcmp(tree->root->link[1]->link[0]->data, "2.5") == 0);         //      / |
    assert(strcmp(tree->root->link[0]->data, "-5") == 0);                   //   (-5) (3)
    assert(strcmp(tree->root->link[0]->link[0]->data, "-6") == 0);          //    /    /
                                                                            // (-6)   (2.5)
}


void doAllTests() {
    testForCreateNode();
    testForRoot();
    testForEquality();
    testForLeft1();
    testForLeft2();
    testForRight1();
    testForRight2();
    testForDelete();
    printf("All 8 tests are passed!");
}