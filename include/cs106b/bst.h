/*
NAME
====

    bst - binary search tree

SYSNOPSIS
=========

    void bst_free(struct bst **tree);
    int bst_add(struct bst **tree, size_t key, void *value);
    int bst_del(struct bst **tree, size_t key);
    size_t bst_min(struct bst *tree);
    size_t bst_max(struct bst *tree);

DESCRIPTION
===========

    bst_free() recursive releases tree and sub-trees.

    bst_add() and bst_del() stores/removes key-value node of tree. 

    bst_min() and bst_max() returns minimum/maximum value in tree.
    If tree is empty, it returns 0.

RETURN
======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EKEY                     Add duplicate key
    SYS_ENOMEM                      System memory is full
*/

#ifndef _CS106B_BST_H_
#define _CS106B_BST_H_

#include <stdlib.h>
#include <cs106b/error.h>

struct bst
{
    void *value;
    size_t key;
    struct bst *left;
    struct bst *right;
};

void bst_free(struct bst **tree);
int bst_add(struct bst **tree, size_t key, void *value);
int bst_del(struct bst **tree, size_t key);
size_t bst_min(struct bst *tree);
size_t bst_max(struct bst *tree);

#endif
