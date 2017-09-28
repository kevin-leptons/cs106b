/*
SYNOPSIS
========

    void set_init(struct set *set);
    void set_free(struct set *set);
    int set_add(struct set *set, const char *key);
    int set_del(struct set *set, const char *key);
    int set_exist(struct set *set, const char *key);
    int set_next(struct set *set, const char **key);
    void set_ireset(struct set *set);
    int set_copy(struct set *dest, struct set *src);
    int set_clone(struct set **dest, struct set *src);

COMPLEXITY
==========

    fn                  best        worst
    -----------------------------------------------------------
    set_init()          1           1
    set_free()          1           1
    set_add()           1           n
    set_del()           1           n
    set_exist()         1           1
    set_next()          1           n
    set_ireset()        1           1
    set_copy()          n           n
    set_clone()         n           n
    -----------------------------------------------------------

    n is number of keys in set.

DESCRIPTION
===========

    set_init() and set_free() construct/destruct an set.

    set_add() add a key into set. If key is early exist, calling will ignore
    that key.

    set_del() remove an key from set.

    set_exist() check key in set or not.

    set_next() retrieve next key in set.

    set_ireset() reset iterator of set. It cause set_next() start from begin.

    set_copy() copy keys from source to dest set.

    set_clone() create new set called dest, then copy keys from source to dest
    set.

RETURNS
=======

    On success, return 0. On failure, return -1.

    set_exist() on exist, return 1. On not exist return 0.

ERRORS
======

    CS106B_EKEY                 Acess to non-exists key in set
    CS106B_EINDEX               Iterate to end of iterator
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_SET_H_
#define _CS106B_SET_H_

#include <stdlib.h>

#include <cs106b/htab.h>
#include <cs106b/vector.h>

struct set
{
    struct htab tab;
    struct vector keys;
    size_t iter;
};

void set_init(struct set *set);
void set_free(struct set *set);
int set_add(struct set *set, const char *key);
int set_del(struct set *set, const char *key);
int set_exist(struct set *set, const char *key);
int set_next(struct set *set, const char **key);
void set_ireset(struct set *set);
int set_copy(struct set *dest, struct set *src);
int set_clone(struct set **dest, struct set *src);

#endif
