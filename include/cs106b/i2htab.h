/*
NAME
====

    i2htab - hash table with two integer keys <k1, k2>

SYNOPSIS
========

    void i2htab_init(struct i2htab_ *table);
    void i2htab_free(struct i2htab_ *table);
    int i2htab_set(struct i2htab_ *table, size_t k1, size_t k2, void *value);
    int i2htab_get(struct i2htab_ *table, size_t k1, size_t k2, void **value);
    int i2htab_del(struct i2htab_ *table, size_t k1, size_t k2);
    size_t i2htab_size(struct i2htab_ *table);

DESCRIPTION
===========

    i2htab_init() and i2htab_free() constructs/destructs an i2htab.

    i2htab_get() retrieves value from i2htab by key.

    i2htab_set() stores an key-value pair into i2htab.

    i2htab_del() removes key-value pair from i2htab.

    i2htab_size() returns size of i2htab.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EKEY             Access to non exist key
    SYS_ENOMEM              System memory is full
*/

#ifndef _CS106B_I2HTAB_H_
#define _CS106B_I2HTAB_H_

#include <stdlib.h>

#include <cs106b/htab.h>

struct i2htab
{
    struct htab tab;
};

void i2htab_init(struct i2htab *table);
void i2htab_free(struct i2htab *table);
int i2htab_set(struct i2htab *table, size_t k1, size_t k2, void *value);
int i2htab_get(struct i2htab *table, size_t k1, size_t k2, void **value);
int i2htab_del(struct i2htab *table, size_t k1, size_t k2);
size_t i2htab_size(struct i2htab *table);

#endif
