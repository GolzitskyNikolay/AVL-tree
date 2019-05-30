#include "AVL-tree.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void printTree(struct avl_node *node, int n, struct avl_tree *tree, FILE *output) {
    if (node->link[1]) {
        printTree(node->link[1], n + 3, tree, output);
    }
    for (int i = 0; i < n; i++) {
        if (output == NULL) {
            printf(" ");
        } else {
            fputs(" ", output);
        }
    }

    if (output == NULL) {
        printf(" %s\n", node->data);
    } else {
        fputs(" ", output);
        fputs(node->data, output);
        fputs("\n", output);

    }

    if (node->link[0]) {
        printTree(node->link[0], n + 3, tree, output);
    }
}

int main(int arg, char *str[]) {

    if (arg != 3) {
        printf("Enter input file name and output file name\nAVL_tree.exe input.txt output.txt\n");
        return 1;
    }

    FILE *input = fopen(str[1], "rt");
    if (input == NULL) {                        // проверяем есть ли файл
        printf("Cannot open source file!!!\n");
        return 1;
    } else if (feof(input)) {                   // проверяем не пустой ли файл
        printf("Input file is empty!!!\n");
        return 1;
    }

    FILE *output = fopen(str[2], "wt");

    struct avl_tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->count = 0;

    char symbol;
    char *string = calloc(20, sizeof(char));

    int isString = 0;   // проверка на то что поступает, строка или число
    int checkForDouble = 0;  // проверка на double
    int checkForMinus = 0;  // проверка на минус

    int i = 0;

    while (!feof(input)) {
        symbol = (char) getc(input);                   // считываем символы из входного файла
        if (!feof(input) && symbol != ' ' && symbol != '\n') {
            string[i] = symbol;
            if (_msize(string) == strlen(string) + 1) {
                char *copy = calloc(_msize(string) + _msize(string), sizeof(char));
                strcpy(copy, string);
                string = copy;
            }
            if (i == 0 && symbol == '-') {
                checkForMinus = 1;
            }
            if (checkForDouble == 1 && string[i - 1] != '.' && string[i + 1] != '.') {
                isString = 0;
            }
            if (isString == 0 && symbol == '.' && i != 0) {
                checkForDouble = 1;
            }
            if (((checkForMinus == 0) &&
                 (isString == 0 && symbol != '0' && symbol != '1' && symbol != '2' && symbol != '3' &&
                  symbol != '4' && symbol != '5' && symbol != '6' && symbol != '7' && symbol != '8' &&
                  symbol != '9')) && checkForDouble == 0) {
                printf("these are strings!\n\n");
                isString = 1;
                checkForDouble = 0;
                checkForMinus = 0;
            }
            if (checkForMinus == 1) {
                checkForMinus = 0;
            }
            i++;
        } else if (i != 0) {
            if (string[i - 1] == '.' || string[i - 1] == '-') {
                printf("these are strings!\n\n");
                isString = 1;
                checkForDouble = 0;
            }
            char *copy = calloc(_msize(string), sizeof(char));
            insert(tree, strcpy(copy, string), isString);
            i = 0;
            free(string);
            string = calloc(20, sizeof(char));
        }
    }

    printTree(tree->root, 0, tree, NULL);
    printTree(tree->root, 0, tree, output);

    free(tree);

    fclose(input);
    fclose(output);
    return 0;
}