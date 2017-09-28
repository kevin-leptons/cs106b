/*
SYNOPSIS
========

    svmap - string vector map.

    int svmap_init(struct svmap *map);
    int svmap_set(struct svmap *map, struct vector *key, void *value);
    void * svmap_get(struct svmap *map, struct vector *key);
    int svmap_del(struct svmap *map, struct vector *key);
    struct vector * svmap_keys(struct svmap *map);
    void svmap_free(struct svmap *map);

COMPLEXITY
==========

    fn                  best                worst
    ------------------------------------------------------------
    svmap_init()        htable_init()       htable_init()
    svmap_set()         htable_set()        htable_init()
    svmap_get()         htable_get()        htable_init()
    svmap_del()         htable_del()        htable_init()
    svmap_keys()        O(1)                O(1)
    svmap_free()        htable_free()       htable_init()
    ------------------------------------------------------------

    n is number of items in map.

DESCRIPTION
===========

    String vector map store key-value pair. Key is string vector and value
    is pointer to any types.

    svmap_init() construct an map.

    svmap_set() put key-value pair into map. If key is early exists, value
    will be replace by new value.

    svmap_get() retrieve value of key.

    svmap_del() remove key-value pair from map.

    svmap_keys() retrieve vector of vector<string> as keys.

    svmap_free() free memory which use by map.

ARGUMENTS
=========

    <key> is vector of string.

RETURNS
=======

    On success, return 0. On failure, return -1;

ERRORS
======

    CS106B_EKEY                 Access to non exist key
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_SVMAP_H_
#define _CS106B_SVMAP_H_

#include <cs106b/vector.h>
#include <cs106b/htab.h>

struct svmap
{
    struct htab vtab;           // string_key => value
    struct htab ktab;           // string_key => index_of_key in keys
    struct vector keys;         // vector<key>
};

void svmap_init(struct svmap *map);
void svmap_free(struct svmap *map);
int svmap_set(struct svmap *map, struct vector *key, void *value);
int svmap_get(struct svmap *map, struct vector *key, void **value);
int svmap_del(struct svmap *map, struct vector *key);
struct vector * svmap_keys(struct svmap *map);

#endif
