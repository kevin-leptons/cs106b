/*
SYNOPSIS
========

    void queue_init(struct queue *q);
    void queue_free(struct queue *q);
    int queue_push(struct queue *q, void *value);
    int queue_pop(struct queue *q, void **value);
    int queue_front(struct queue *q, void **value);
    int queue_copy(struct queue *dest, struct queue *src);
    int queue_clone(struct queue **dest, struct queue *src);

COMPLEXITY
==========

    fn                  best        worst
    -------------------------------------------
    queue_init()        O(1)        O(1)
    queue_free()        O(n)        O(n)
    queue_push()        O(1)        O(1)
    queue_pop()         O(1)        O(1)
    queue_front()       O(1)        O(1)
    queue_copy()        O(n)        O(n)
    queue_clone()       O(n)        O(n)
    -------------------------------------------

    n is number of items in queue.

DESCRIPTION
===========

    queue_init() and queue_free() construct/destruct an queue.

    queue_push() insert an item into back of queue.

    queue_pop() retrieve front item of queue and remove that from queue.

    queue_front() retrieve front item of queue and doesn't remove that from
    queue.

    queue_copy() copy items from source to dest queue.

    queue_clone() create new queue called dest, then copy items from source to
    dest queue.

RETURNS
=======

    On success, return 0. On failure, return -1;

ERRORS
======

    CS106B_EINDEX               Pop or font while queue is empty
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_QUEUE_H_
#define _CS106B_QUEUE_H_

#include <stdlib.h>
#include <stdbool.h>

struct queue_item
{
    void *value;
    struct queue_item *next;
};

struct queue
{
    struct queue_item *front;
    struct queue_item *back;
    size_t size;
};

void queue_init(struct queue *q);
void queue_free(struct queue *q);
int queue_push(struct queue *q, void *value);
int queue_pop(struct queue *q, void **value);
int queue_front(struct queue *q, void **value);
int queue_copy(struct queue *dest, struct queue *src);
int queue_clone(struct queue **dest, struct queue *src);

#endif
