/*
NAME
====

    sorting - sorting family 

SYNOPSIS
========

    typedef int (*sort_cmp)(void *left, void *right);

    int sort_ins(struct vector *v, enum sort_dir dir, sort_cmp cmp);
    int sort_sel(struct vector *v, enum sort_dir dir, sort_cmp cmp);
    int sort_mer(struct vector *v, enum sort_dir dir, sort_cmp cmp);
    int sort_qui(struct vector *v, enum sort_dir dir, sort_cmp cmp);

COMPLEXITY
==========

    fn              worst O()       best 0()        worst S()
    ======================================================
    sort_ins        n^2             n               n 
    sort_sel        n^2             n^2             n
    sort_mer        n.log2(n)       n.log2(n)       n.log2(n)
    sort_qui        n^2             n.log(n)        n
     
DESCRIPTION
===========

    All of functions have the same interfaces and use to sort vector. 
    However, thier OPERATION_COMPLEXITY and SPACE_COMPLEXITY is different.

ARGUMENTS
=========

    <v> is vector, it can contains any data type. 

    <dir> is direction to sort. Two valid direction are:

        SORT_INC: Sorting follows increasing order
        SORT_DEC: Sorting follows decreasing order
   
    <cmp> is function pointer which point to comparision function which
    use to compare two value called left and right. It MUST returns:

        0 on left is equal right
        1 on left is less than right
        -1 on left is greater than right

RETURN
======

    On success, return 0. On failure return -1.

ERRORS
======

    SYS_ENOMEM      System memory is full 

AUTHORS
=======

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef _CSO16_SORTING_
#define _CSO16_SORTING_

#include <cs106b/vector.h>

enum sort_dir {
    SORT_DEC,
    SORT_INC
};

typedef int (*sort_cmp)(void *left, void *right);

int sort_ins(struct vector *v, enum sort_dir dir, sort_cmp cmp);
int sort_sel(struct vector *v, enum sort_dir dir, sort_cmp cmp);
int sort_mer(struct vector *v, enum sort_dir dir, sort_cmp cmp);
int sort_qui(struct vector *v, enum sort_dir dir, sort_cmp cmp);

#endif
