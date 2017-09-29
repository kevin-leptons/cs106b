/*
NAME
====

    btree - binary tree

SYNOPSIS
========
    void btree_init(struct btree *tree);
    void btree_free(struct btree *tree);
    int btree_mknode(struct btree_node **new_node, struct btree_node *left,
                     struct btree_node *right, void *data);
    int btree_prew(struct btree *tree, btree_walkcb callback, void *arg);
    int btree_inw(struct btree *tree, btree_walkcb callback, void *arg);
    int btree_postw(struct btree *tree, btree_walkcb callback, void *arg);
    int btree_levelw(struct btree *tree, btree_walkcb callback, void *arg);
    int btree_dump(struct btree * tree);

COMPLEXITY
==========

    fn                          best            worst
    -------------------------------------------------------
    btree_init()                1               1
    btree_free()                log2(n)         log2(n)
    btree_mknode()              1               1
    btree_prew()                log2(n)         log2(n)
    btree_inw()                 log2(n)         log2(n)
    btree_postw()               log2(n)         log2(n)
    btree_levelw()              log2(n)         log2(n)
    btree_dump()                log2(n)         log2(n)
    -------------------------------------------------------

    n is number of nodes in tree.

DESCRIPTION
===========

    btree_init() and btree_free() construct/destruct an btree.

    btree_mknode() create new node.

    btree_prew() walks though tree follow order:

        - call callback function at parent
        - recursive walk at left child
        - recursive walk at right child

    btree_inw() walks through tree follow order:

        - recursive walk at left child
        - call callback function at parent
        - recursive walk at right child

    btree_postw() walks though tree follow order:

        - recursive walk at left child
        - recursive walk at right child
        - call callback function at parent

    btree_levelw() walks though tree follow order low level to high level
    of nodes.

    btree_dump() shows nodes of tree to stdout.

    If callback function return 0, walking is continue. If callback function
    return -1, walking is terminate.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_BTREE_H_
#define _CS106B_BTREE_H_

struct btree_node
{
    void *data;
    struct btree_node *left;
    struct btree_node *right;
};

struct btree
{
    struct btree_node *root;
};

typedef int (*btree_walkcb)(struct btree_node *node, void *arg);

void btree_init(struct btree *tree);
void btree_free(struct btree *tree);
int btree_mknode(struct btree_node **new_node, struct btree_node *left,
                 struct btree_node *right, void *data);
int btree_prew(struct btree *tree, btree_walkcb callback, void *arg);
int btree_inw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_postw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_levelw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_dump(struct btree * tree);

#endif
