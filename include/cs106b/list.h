/*
NAME
====

    list

SYNOPSIS
========

    void list_init(struct list *list);
    void list_free(struct list *list);
    int list_get(struct list *list, size_t index, size_t **value);
    int list_add(struct list *list, void *value);
    int list_ins(struct list *list, void *value, size_t index);
    int list_del(struct list *list, size_t index);
    int list_next(struct list *list, void **value);
    void list_ireset(struct list *list);

DESCRIPTION
===========

    list_init() and list_free() constructs/destructs an list.

    list_get() retrieves item in list.

    list_add() appends an item into back of list.

    list_ins() puts an item into index of list, and moves left items from
    i to i + 1.

    list_del() removes item from list and moves left items from i to i - 1.

    list_next() iterate though list.

    list_ireset() resets iterator of list to begin.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106_EINDEX                Access to invalid index
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_LIST_H_
#define _CS106B_LIST_H_

#include <stdlib.h>

struct list_item
{
    struct list_item *next;
    struct list_item *prev;
    void *value;
};

struct list
{
    struct list_item *front;
    struct list_item *back;
    size_t size;
    struct list_item *iter;
};

void list_init(struct list *list);
void list_free(struct list *list);
int list_add(struct list *list, void *value);
int list_ins(struct list *list, void *value, size_t index);
int list_del(struct list *list, size_t index);
int list_get(struct list *list, size_t index, size_t **value);
int list_next(struct list *list, void **value);
void list_ireset(struct list *list);

#endif
