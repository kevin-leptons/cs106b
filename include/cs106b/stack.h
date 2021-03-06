/*
SYNOPSIS
========

    void stack_init(struct stack *stack);
    void stack_free(struct stack *stack);
    int stack_push(struct stack *stack, void *value);
    int stack_pop(struct stack *stack, void **value);
    int stack_top(struct stack *stack, void **top);
    int stack_copy(struct stack *dest, struct stack *src);
    int stack_clone(struct stack **dest, struct stack *src);

COMPLEXITY
==========

    fn                  best        worst
    -------------------------------------------
    stack_init()        O(1)        O(1)
    stack_free()        O(n)        O(n)
    stack_push()        O(1)        O(1)
    stack_pop()         O(1)        O(1)
    stack_top()         O(1)        O(1)
    stack_copy()        O(n)        O(n)
    stack_clone()       O(n)        O(n)
    -------------------------------------------

    n is number of items in stack.

DESCRIPTIION
============

    stack_init() and stack_free() construct/destruct an stack.

    stack_push() insert an item into top of stack.

    stack_pop() retrieve top item in stack and remove that item from stack.

    stack_top() retrieve top item in stack but doesn't remove that item from
    stack.

    stack_copy() copy items from source to dest stack. 

    stack_clone() create new stack called dest then copy source to dest stack.

RETURNS
=======

    On success, return 0. On failure, return -1;

ERRORS
======

    CS106B_EINDEX           Push or top while stack is empty
    SYS_ENOMEM              System memory is full

AUTHORS
=======

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef _CS106B_STACK_H_
#define _CS106B_STACK_H_

#include <stdlib.h>
#include <stdbool.h>

struct stack_item
{
    void *value;
    struct stack_item *next;
};

struct stack
{
    struct stack_item *top;
    size_t size;
};

void stack_init(struct stack *stack);
void stack_free(struct stack *stack);
int stack_push(struct stack *stack, void *value);
int stack_pop(struct stack *stack, void **value);
int stack_top(struct stack *stack, void **top);
int stack_copy(struct stack *dest, struct stack *src);
int stack_clone(struct stack **dest, struct stack *src);

#endif
