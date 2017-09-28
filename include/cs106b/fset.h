/*
SYNOPSIS
========

    int fset_load(struct set *set, const char *file);

COMPLEXITY
==========

    fn                  best                worst
    ----------------------------------------------------------
    fset_load()         n                   n
    ----------------------------------------------------------

    n is number character in file.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    SYS_ENOENT                  No such file
    SYS_EACCESS                 Have not permission to search file 
    SYS_EPERM                   Have not permission to read
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_FSET_H_
#define _CS106B_FSET_H_

#include <cs106b/set.h>

struct dict
{
    struct htab tab;
    char *data;
};

int fset_load(struct set *set, const char *file);

#endif
