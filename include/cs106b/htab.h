/*
SYNOPSIS
========

    void htab_init(struct htab *t);
    void htab_free(struct htab *t);
    int htab_resize(struct htab *t, size_t max_size);
    int htab_get(struct htab *t, const char *key, void **value);
    int htab_set(struct htab *t, const char *key, void *value);
    int htab_del(struct htab *t, const char *key);
    int htab_clear(struct htab *t);
    int htab_copy(struct htab *dest, struct htab  *src);
    int htab_clone(struct htab **dest, struct htab  *src);

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

    htab_init() and htab_free() construct/destruct hash table.

    htab_free() free memory usage by table.

    htab_resize() extend or narrow max size of table.

    htab_get() and htab_set() retrieve/store value in hash table.

    htab_del() remove an key-value pair in table.

    htab_clear() clear all of keys, keep memory usage.

    htab_copy() copy key-value pairs from source to dest table.

    htab_clone() create new hash table and copy key-value pairs from source
    to dest table.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EKEY             No key in hash table
    SYS_ENOMEM              System memory is full
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
