/*
SYNOPSIS
========

    void htab_init(struct htab *t);
    void htab_free(struct htab *t);
    int htab_get(struct htab *t, const char *key, void **value);
    int htab_set(struct htab *t, const char *key, void *value);
    int htab_del(struct htab *t, const char *key);
    int htab_clear(struct htab *t);
    int htab_resize(struct htab *t, size_t max_size);

COMPLEXITY
==========

    fn                  best        worst
    -------------------------------------------------
    htab_init()         O(1)        O(1)
    htab_free()         O(1)        O(1)
    htab_resize()       O(n)        O(n)
    htab_get()          O(1)        O(n)
    htab_set()          O(1)        O(n)
    htab_del()          O(1)        O(n)
    htab_clear()        O(1)        O(1)
    htab_copy()         O(n)        O(n)
    htab_clone()        O(n)        O(n)
    -------------------------------------------------

    n is number of bins in table.

DESCRIPTION
===========

    htab_init() construct hash table.

    htab_set() set an pair key-value in table.

    htab_get() retrieve value correspond with key in table.

    htab_del() remove an pair key-value in table.

    htab_clear() clear all of keys, keep memory usage.

    htab_resize() extend or narrow max size of table.

    htab_free() free memory usage by table.

RETURNS
=======

    htable_init(), htable_set(), htable_del(), htable_resize() on success
    return 0, on failure return -1.

    htable_get() on success return pointer to value, on failure return NULL.

ERRORS
======

    CS106B_EKEY
    SYS_ENOMEM
*/

#ifndef _CS106B_HTAB_H_
#define _CS106B_HTAB_H_

#include <stdlib.h>

struct htab_item
{
    char *key;
    void *value;
};

struct htab
{
    size_t size;
    size_t mask;
    size_t max_size;
    struct htab_item *items;
};

void htab_init(struct htab *t);
void htab_free(struct htab *t);
int htab_resize(struct htab *t, size_t max_size);
int htab_get(struct htab *t, const char *key, void **value);
int htab_set(struct htab *t, const char *key, void *value);
int htab_del(struct htab *t, const char *key);
int htab_clear(struct htab *t);
int htab_copy(struct htab *dest, struct htab  *src);
int htab_clone(struct htab **dest, struct htab  *src);

#endif
