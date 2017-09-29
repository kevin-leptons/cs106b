/*
NAME
====

    pset - set of pointers

SYNOPSIS
========

    void pset_init(struct pset *set);
    void pset_free(struct pset *set);
    int pset_add(struct pset *set, void *p);
    int pset_del(struct pset *set, void *p);
    int pset_exist(struct pset *set, void *p);
    int pset_next(struct pset *set, void **p);
    void pset_ireset(struct pset *set);

DESCRIPTION
===========

    pset_init() and pset_free() construct/destroy an pset.

    pset_add() add a pointer to pset.

    pset_del() remove a pointer from pset.
    
    pset_exist() check a pointer in pset or not.

    pset_next() iterate through pointers in set.

    pset_ireset() reset set's iterator to begin.

RETURNS
=======

    On success, return 0. On failure, return -1.

    pset_exist() on exist return 1. On non exist return 0.

ERRORS
======

    CS106B_EINDEX               End of iterator
    CS106B_EKEY                 Access to non exist pointer key
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_PSET_H_
#define _CS106B_PSET_H_

#include <stdlib.h>
#include <stdbool.h>

#include <cs106b/htab.h>

struct pset
{
    struct htab tab;
    size_t iter;
    size_t iter_count;
};

void pset_init(struct pset *set);
void pset_free(struct pset *set);
int pset_add(struct pset *set, void *p);
int pset_del(struct pset *set, void *p);
int pset_exist(struct pset *set, void *p);
int pset_next(struct pset *set, void **p);
void pset_ireset(struct pset *set);

#endif
