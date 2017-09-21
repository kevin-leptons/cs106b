/*
NAME

    sorting

SYNOPSIS

DESCRIPTION

    cmp_fn MUST return:
    
        0 on left is equal right
        1 on left is less than right
        -1 on left is greater than right

RETURN

ERRORS

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

#endif
