cmake_minimum_required(VERSION 3.13)
project(AVL_tree C)

set(CMAKE_C_STANDARD 99)

add_executable(AVL_tree main.c AVL-tree.c AVL-tree.h tests.c tests.h)