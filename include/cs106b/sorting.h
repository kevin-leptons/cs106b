/*
NAME

    sorting - Commom sorting algorithms

SYNOPSIS

    int ins_sort(struct vector *v, enum sortdir direction,
                 int (*cmp_fn)(void *left, void *right));
    int sel_sort(struct vector *v, enum sortdir direction,
                 int (*cmp_fn)(void *left, void *right));
    int mer_sort(struct vector *v, enum sortdir direction,
                 int (*cmp_fn)(void *left, void *right));
    int qui_sort(struct vector *v, enum sortdir direction,
                 int (*cmp_fn)(void *left, void *right));

DESCRIPTION

    All of functions have the same interfaces and use to sort vector. 
    Comparision function MUST be provide to help sorting function know that
    relationship of element in vector: greater, less or equal.

    cmp_fn MUST return:
    
        0 on left is equal right
        1 on left is less than right
        -1 on left is greater than right

RETURN

    On success, return 0. On failure return -1.

ERRORS

    SYS_ENOMEM

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef _CSO16_SORTING_
#define _CSO16_SORTING_

#include <cs106b/vector.h>

enum sortdir {
    SORTDIR_DEC,
    SORTDIR_INC
};

int ins_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right));
int sel_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right));
int mer_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right));
int qui_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right));

#endif
