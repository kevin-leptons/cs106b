/*
NAME
====

    bitvec - bit vector

SYNOPSIS
========

    void bitvec_init(struct bitvec *vector);
    void bitvec_free(struct bitvec *vector);
    int bitvec_resize(struct bitvec *vector, size_t max_byte_size);
    int bitvec_add(struct bitvec *vector, char bit);
    int bitvec_get(struct bitvec *vector, size_t index);
    int bitvec_del(struct bitvec *vector, size_t index);
    int bitvec_cat(struct bitvec *dest, struct bitvec *src);
    int bitvec_clear(struct bitvec *vector);
    int bitvec_copy(struct bitvec *dest, struct bitvec *src);
    int bitvec_clone(struct bitvec **dest, struct bitvec *src);
    int bitvec_dump(struct bitvec *vector);

COMPLEXITY
==========

    fn                              best            worst
    -----------------------------------------------------------
    bitvec_init()                   1               1
    bitvec_free()                   1               1
    -----------------------------------------------------------

    n is number of bits in vector.

DESCRIPTION
===========

    bitvec_init() and bitvec_free() construct/destruct a bitvec.

    bitvec_resize() change max size of bitvec in bytes.

    bitvec_add() append a bit in back of bitvec.

    bitvec_get() retrieve a bit in bitvec by index.

    bitvec_del() remove a bit in bitvec by index, and shift all of bit from
    i to i - 1, with i > index.

    bitvec_cat() join bits from source to dest bitvec.

    bitvec_clear() empty bits in bitvec and hold max size of bitvec.

    bitvec_copy() copy source to dest bitvec.

    bitvec_clone() creates new bitvec called dest, then copy bits from source
    to dest bitvec.

    bitvec_dump() shows bits in bitvect to stdout.

RETURNS
=======

    On success, return 0. On failure, return -1.

    bitvec_get() on success return 0 or 1 correspond with bit's value. On
    failure return -1.

ERRORS
======

    CS106B_EINDEX               Access to invalid bit index
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_BITVECT_H_
#define _CS106B_BITVECT_H_

#include <stdlib.h>

struct bitvec
{
    char *items;
    size_t size;
    size_t max_size;
};

void bitvec_init(struct bitvec *vector);
void bitvec_free(struct bitvec *vector);
int bitvec_resize(struct bitvec *vector, size_t max_byte_size);
int bitvec_add(struct bitvec *vector, char bit);
int bitvec_get(struct bitvec *vector, size_t index);
int bitvec_del(struct bitvec *vector, size_t index);
int bitvec_cat(struct bitvec *dest, struct bitvec *src);
int bitvec_clear(struct bitvec *vector);
int bitvec_copy(struct bitvec *dest, struct bitvec *src);
int bitvec_clone(struct bitvec **dest, struct bitvec *src);
int bitvec_dump(struct bitvec *vector);

#endif
