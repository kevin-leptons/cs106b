/*
NAME
====

    ihtable - hash table by integer key.

SYNOPSIS
========

    void ihtab_init(struct ihtab *table);
    void htab_free(struct ihtab *table);
    int ihtab_set(struct ihtab *table, size_t key, void *value);
    int ihtab_get(struct ihtab *table, size_t key, void **value);
    int ihtab_del(struct ihtab *table, size_t key);
    size_t ihtab_size(struct ihtab *table);

DESCRIPTION
===========

    ihtab_init() and ihtab_free() constructs/destructs an ihtab.

    htab_set() stores an key-value pair into ihtab.

    htab_get() retrieves value by key in ihtab.

    htab_del() removes key-value pair in ihtab by key.

    ihtab_size() returns size of ihtab.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EKEY                     Access to non exist key
    SYS_ENOMEM                      System memory is full
*/

#ifndef _CS016B_IHTAB_H_
#define _CS016B_IHTAB_H_

#include <stdlib.h>

#include <cs106b/htab.h>

struct ihtab
{
    struct htab tab;
};

void ihtab_init(struct ihtab *table);
void ihtab_free(struct ihtab *table);
int ihtab_set(struct ihtab *table, size_t key, void *value);
int ihtab_get(struct ihtab *table, size_t key, void **value);
int ihtab_del(struct ihtab *table, size_t key);
size_t ihtab_size(struct ihtab *table);

#endif
