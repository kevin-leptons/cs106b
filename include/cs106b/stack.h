/*
SYNOPSIS

    int stack_init(struct stack *stack);
    int stack_push(struct stack *stack, void *data);
    void *stack_pop(struct stack *stack);
    void *stack_top(struct stack *stack);
    int stack_copy(struct stack *dest, struct stack *src);
    struct stack * stack_clone(struct stack *src);
    void stack_free(struct stack *stack, bool free_data);

COMPLEXITY

    fn                  best        worst
    -------------------------------------------
    stack_init()        O(1)        O(1)
    stack_push()        O(1)        O(1)
    stack_pop()         O(1)        O(1)
    stack_top()         O(1)        O(1)
    stack_copy()        O(n)        O(n)
    stack_clone()       O(n)        O(n)
    stack_free()        O(n)        O(n)
    -------------------------------------------

    n is number of items in stack.

DESCRIPTIION

    stack_init() construct metadata of stack.

    stack_push() insert an item into top of stack.

    stack_pop() retrieve item which located in top into stack and remove that
    item from stack.

    stack_top() retrieve item which located in top of stack and 
    doesn't remove that item from stack.

    stack_copy() copy an stack into dest stack, data reference is the same as
    src stack except stack memory is new.

    stack_clone() create new stack then copy source stack to that stack.

    stack_free() free memory usage by stack.

ARGUMENTS

    If free_data is true, call free() with (struct stack_item).data for each
    items.

RETURNS

    stack_init(), stack_push(), stack_copy(), on success return 0, on
    failure return -1.

    stack_clone() on success return pointer to new stack, on error return NULL.

    stack_pop(), stack_top() on success return pointer to item, on failure
    return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_STACK_H__
#define __CS106B_STACK_H__

#include <stdlib.h>
#include <stdbool.h>

struct stack_item
{
    void *data;
    struct stack_item *next;
};

struct stack
{
    struct stack_item *top;
    size_t size;
};

int stack_init(struct stack *stack);
int stack_push(struct stack *stack, void *data);
void *stack_pop(struct stack *stack);
void *stack_top(struct stack *stack);
int stack_copy(struct stack *dest, struct stack *src);
struct stack * stack_clone(struct stack *src);
void stack_free(struct stack *stack, bool free_data);

#endif
