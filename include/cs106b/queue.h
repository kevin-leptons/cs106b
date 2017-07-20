/*
SYNOPSIS

    int queue_init(struct queue *queue);
    int queue_push(struct queue *queue, void *data);
    void *queue_pop(struct queue *queue);
    void *queue_front(struct queue *queue);
    void queue_free(struct queue *queue, bool free_data);

COMPLEXITY

    fn                  best        worst
    -------------------------------------------
    queue_init()        O(1)        O(1)
    queue_push()        O(1)        O(1)
    queue_pop()         O(1)        O(1)
    queue_front()       O(1)        O(1)
    queue_free()        O(n)        O(n)
    -------------------------------------------

    n is number of items in queue.

DESCRIPTION

    queue_init() construct metadata of queue.

    queue_push() insert an item into back of queue.

    queue_pop() retrieve item in front of queue and remove that item from 
    queue.

    queue_front() retrieve item in front of queue and doesn't remove that item
    from queue.

    queue_free() free memory usage  by queue.

ARGUMENTS

    If free_data is true, call free() with (struct queue_item).data for each
    items.

RETURNS

    queue_init(), queue_push() on success return 0, on failure return -1.

    queue_pop(), queue_front() on success return pointer to item, on failure
    return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_QUEUE_H__
#define __CS106B_QUEUE_H__

#include <stdlib.h>
#include <stdbool.h>

struct queue_item
{
    void *data;
    struct queue_item *next;
};

struct queue
{
    struct queue_item *front;
    struct queue_item *back;
    size_t size;
};

int queue_init(struct queue *queue);
int queue_push(struct queue *queue, void *data);
void *queue_pop(struct queue *queue);
void *queue_front(struct queue *queue);
void queue_free(struct queue *queue, bool free_data);

#endif
