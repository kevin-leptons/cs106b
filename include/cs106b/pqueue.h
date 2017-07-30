/*
NAME

    pqueue - Priority queue

SYNOPSIS

    int pqueue_init(struct pqueue *q, enum pqueue_type type);
    int pqueue_push(struct pqueue *q, void *data, size_t priority);
    void * pqueue_pop(struct pqueue *q);
    void * pqueue_top(struct pqueue *q);
    void pqueue_free(struct pqueue *q);

COMPLEXITY

    fn                  best            worst
    ------------------------------------------------------
    pqueue_init()       O(1)            O(1)
    pqueue_push()       O(1)            O(log(n))
    pqueue_pop()        O(log(n))       O(log(n))
    pqueue_top()        O(1)            O(1)
    pqueue_free()       O(1)            O(!)
    ------------------------------------------------------

    n is number of items in queue.

DESCRIPTION

    pqueue_init() construct empty priority pqueue.

    pqueue_push() insert an item into priority pqueue. Pushing ignore
    duplicate of data content. It mean that same data or same content in data
    will be ignore.

    pqueue_pop() remove item which lowest priority and more early push from
    queue.

    pqueue_top() return item which lowest priority and more early push in
    queue.

    pqueue_free() release memory which used by priority queue.

RETURNS

    pqueue_init(), pqueue_push() on success return 0, on failure return -1.

    pqueue_pop(), pqueue_top() on success return pointer to data, on failure
    return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_PQUEUE_H__
#define __CS106B_PQUEUE_H__

#include <stdlib.h>

enum pqueue_type
{
    PQUEUE_SMALLEST,
    PQUEUE_BIGGEST
};

struct pqueue_item
{
    size_t priority;
    void *data;
};

struct pqueue
{
    struct pqueue_item *items;
    size_t size;
    size_t max_size;
    enum pqueue_type type;
};

int pqueue_init(struct pqueue *q, enum pqueue_type type);
int pqueue_push(struct pqueue *q, void *data, size_t priority);
void * pqueue_pop(struct pqueue *q);
void * pqueue_top(struct pqueue *q);
void pqueue_free(struct pqueue *q);

#endif
