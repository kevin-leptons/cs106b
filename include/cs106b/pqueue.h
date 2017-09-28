/*
NAME
====

    pqueue - priority queue

SYNOPSIS
========

    void pqueue_init(struct pqueue *q, enum pqueue_dir direction);
    void pqueue_free(struct pqueue *q);
    int pqueue_push(struct pqueue *q, void *data, size_t priority);
    int pqueue_pop(struct pqueue *q, void **data);
    int pqueue_top(struct pqueue *q, void **data);
    int pqueue_tpri(struct pqueue *q, size_t *priority);
    int pqueue_copy(struct pqueue *dest, struct pqueue *src);
    int pqueue_clone(struct pqueue **dest, struct pqueue *src);

COMPLEXITY
==========

    fn                  best            worst
    ------------------------------------------------------
    pqueue_init()       O(1)            O(1)
    pqueue_free()       O(1)            O(n.log2(n))
    pqueue_push()       O(1)            O(log2(n))
    pqueue_pop()        O(1)            O(log2(n))
    pqueue_top()        O(1)            O(1)
    pqueue_tpri()       O(1)            O(1)
    pqueue_copy()       O(1)            O(1)
    pqueue_clone()      O(1)            O(1)
    ------------------------------------------------------

    n is number of items in queue.

DESCRIPTION
===========

    pqueue_init() and pqueue_free() construct/destruct a pqueue.

    pqueue_push() insert an item into priority pqueue. Pushing ignores
    duplicate of value.

    pqueue_pop() retrieve top item in queue and remove that item from queue.

    pqueue_top() retrieve top item in queue and doesn't remove that item
    from queue.

    pqueue_copy() copy source to dest queue.

    pqueue_clone() create new queue called dest, then copy source to dest
    queue.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EINDEX               Pop or top on empty queue
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_PQUEUE_H_
#define _CS106B_PQUEUE_H_

#include <stdlib.h>

enum pqueue_dir
{
    PQUEUE_INC,                 // top => bot ~ smallest => greatest
    PQUEUE_DEC                  // top => bot ~ greatest => smallest
};

struct pqueue_item
{
    size_t priority;
    void *data;
};

struct pqueue
{
    struct pqueue_item *items;  // use from 1 index instead of 0
    size_t size;
    size_t max_size;
    enum pqueue_dir dir;
};

void pqueue_init(struct pqueue *q, enum pqueue_dir direction);
void pqueue_free(struct pqueue *q);
int pqueue_push(struct pqueue *q, void *data, size_t priority);
int pqueue_pop(struct pqueue *q, void **data);
int pqueue_top(struct pqueue *q, void **data);
int pqueue_tpri(struct pqueue *q, size_t *priority);
int pqueue_copy(struct pqueue *dest, struct pqueue *src);
int pqueue_clone(struct pqueue **dest, struct pqueue *src);

#endif
