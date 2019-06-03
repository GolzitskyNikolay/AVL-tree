#include <malloc.h>
#include "demo.h"
#include "AVL-tree.h"
#include "string.h"

void printTree(struct avl_node *node, int n, struct avl_tree *tree, FILE *output) {
    if (tree->root == NULL) {
        printf("Tree is empty!\n");
        return;
    }
    if (node->link[1] != NULL && node != node->link[1]) {
        printTree(node->link[1], n + 3, tree, output); // ищем самый большой элемент и затем выводим его
    }
    for (int i = 0; i < n; i++) {
        if (output == NULL) {
            printf(" ");          // вывод на экран
        } else {
            fputs(" ", output);   // вывод в файл
        }
    }

    if (output == NULL) {
        printf(" %s\n", node->data);  // вывод на экран
    } else {
        fputs(" ", output);           // вывод в файл
        fputs(node->data, output);
        fputs("\n", output);

    }

    if (node->link[0] != NULL && node != node->link[0]) {
        printTree(node->link[0], n + 3, tree, output);   // выводим элементы в порядке убывания
    }
}
/*
 * Проверка на то, что поступает, то есть проверка на строки, целые числа, числа с плавающей запятой,
 * и последущая вставка элемента, если нужно.
 */
int checkForStringAndInputNodeIfWeWant(FILE *input, char *string, struct avl_tree *tree) {
    int checkForDouble = 0;  // проверка на double
    int checkForMinus = 0;  // проверка на минус
    int isString = 0;

    char symbol;   // символ, считываемый из файла

    int i = 0;

    while (((input == NULL) && ((int) strlen(string) != i + 1)) || (input != NULL && !feof(input))) {
        if (input != NULL) {
            symbol = (char) getc(input);      // считываем символы из входного файла
        } else {
            symbol = string[i];
        }

        if ((input == NULL || !feof(input)) && symbol != ' ' && symbol != '\n') {
            string[i] = symbol;

            if (input != NULL) {
                // если считываемое слово оказалось больше выделенной памяти, то увеличиваем память
                if (_msize(string) == strlen(string) + 1) {
                    char *copy = calloc(_msize(string) + _msize(string), sizeof(char));
                    strcpy(copy, string);
                    string = copy;
                }
            }

            if (i == 0 && symbol == '-') {
                checkForMinus = 1; // проверяем что поступает, отрицательное число или строка
            }
            if (checkForDouble == 1 && string[i - 1] != '.' && string[i + 1] != '.') {
                isString = 0;  // поступило число, с плавающей точкой
            }
            if (isString == 0 && symbol == '.' && i != 0) {
                checkForDouble = 1;  // на следующем шаге проверим что
                // поступает, число с плавающей точкой или строка
            }
            if (((checkForMinus == 0) &&
                 (isString == 0 && symbol != '0' && symbol != '1' && symbol != '2' && symbol != '3' &&
                  symbol != '4' && symbol != '5' && symbol != '6' && symbol != '7' && symbol != '8' &&
                  symbol != '9')) && checkForDouble == 0) {
                isString = 1;       // поступают строки
                checkForDouble = 0;
                checkForMinus = 0;
            }
            if (checkForMinus == 1) {
                checkForMinus = 0;
            }
            i++;
        } else if (i != 0) {
            if (string[i - 1] == '.' || string[i - 1] == '-') {
                isString = 1; // в случае, если поcледний символ был "." или "-", то поступают строки
                checkForDouble = 0;
            }
            if (input != NULL) {
                insert(tree, string, isString);       // вставка элемента в AVL-дерево
                string = calloc(20, sizeof(char));    // количество выделенной памяти снова становится = 20
            }
            i = 0;
        }
    }
    return isString;
}

/*
 * Стандартное считывание из файла, запись в дерево, вывод в файл.
 */
int doStandard(char **str) {
    FILE *input;
    FILE *output;

    struct avl_tree *tree = malloc(sizeof *tree);  // создаём пустое AVL-дерево
    tree->root = NULL;
    tree->count = 0;

    input = fopen(str[1], "rt");

    if (input == NULL) {                        // проверяем есть ли файл
        printf("Cannot open source file!!!\n");
        return 1;
    } else if (feof(input)) {                   // проверяем не пустой ли файл
        printf("Input file is empty!!!\n");
        return 1;
    }

    char *string = calloc(20, sizeof(char));  // массив считанных символов (выделили память под 20 символов)
    checkForStringAndInputNodeIfWeWant(input, string, tree);
    fclose(input);

    output = fopen(str[2], "wt");
    printTree(tree->root, 0, tree, output);
    fclose(output);

    free(tree);   // освобождение памяти, выделенной под AVL-дерево

    return 0;
}

/*
 * Свободный доступ к командам, они выводятся сразу после вызова программы. Программа не завершится, пока
 * не будет прервана пользователем.
 */
int doCommands() {
    FILE *input;
    FILE *output;

    struct avl_tree *tree = malloc(sizeof *tree);  // создаём пустое AVL-дерево
    tree->root = NULL;
    tree->count = 0;

    int end = 0;  // флаг окончания программы
    int isString = 0;   // проверка на то что поступает, строка или число

    if (end == 0) {
        printf("You can use next options:"
               "\nput [Number]\ndel [Number]\ntake [InputFile]\nputIn [OutputFile]\nshow tree\nend prog\n\n");
    }

    while (end == 0) {

        char *command = malloc(sizeof(command));
        char *forCommand = malloc(sizeof(forCommand));

        scanf("%s %s", command, forCommand);

        if (strcmp(command, "end") == 0 && ((strcmp(forCommand, "prog")) == 0)) {  // функция завершения
            end = 1;
            free(command);
        }

        if (strcmp(command, "show") == 0 && (strcmp(forCommand, "tree") == 0)) {  // напечатать дерево
            printTree(tree->root, 0, tree, NULL);
            free(command);
        }

        if (strcmp(command, "del") == 0) {        // удаление
            delete(tree, forCommand, isString
            );
        }

        if (strcmp(command, "take") == 0) {  // взять всё из файла

            input = fopen(forCommand, "rt");

            if (input == NULL) {                        // проверяем есть ли файл
                printf("Cannot open source file!!!\n");
                return 1;
            } else if (feof(input)) {                   // проверяем не пустой ли файл
                printf("Input file is empty!!!\n");
                return 1;
            }

            char *string = calloc(20, sizeof(char));  // массив считанных символов (выделили память под 20 символов)

            checkForStringAndInputNodeIfWeWant(input, string, tree
            );

            fclose(input);
        }

        if (strcmp(command, "putIn") == 0) {        // печать в файл
            output = fopen(forCommand, "wt");
            printTree(tree->root, 0, tree, output);
            fclose(output);
            end = 1;
        }

        if (strcmp(command, "put") == 0) {           // добавление узла в дерево
            insert(tree, forCommand, checkForStringAndInputNodeIfWeWant(NULL, forCommand, tree));
        }
    }

    free(tree);   // освобождение памяти, выделенной под AVL-дерево

    return 0;
}