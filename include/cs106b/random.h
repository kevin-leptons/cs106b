/*
NAME
====

    random - random function family

SYNOPSIS
========

    int rrand(size_t min, size_t max, size_t *result);

DESCRIPTION
===========

    rrand() random unsigned integer number in range [min, max].

RETURN
======

    On success, return 0. On failure, return -1.

ERRORS
======

    SYS_EINVAL                  Invalid min and max value
*/

#ifndef _TLPIEXER_RANDOM_H_
#define _TLPIEXER_RANDOM_H_

#include <stdlib.h>

int rrand(size_t min, size_t max, size_t *result);

#endif
