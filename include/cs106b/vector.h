/*
SYNOPSIS

    int vector_init(struct vector *vector, size_t bsize, size_t esize);
    int vector_insert(struct vector *vector, size_t index, void *data);
    int vector_add(struct vector *vector, void *data);
    int vector_remove(struct vector *vector, size_t index);
    void *vector_at(struct vector *vector, size_t index);
    void vector_free(struct vector *vector, bool free_data);

COMPLEXITY

    fn                  best        worst
    ----------------------------------------
    vector_init()       O(1)        O(1)     
    vector_insert()     O(1)        O(n)     
    vector_add()        O(1)        O(n)     
    vector_remove()     O(1)        O(n)     
    vector_at()         O(1)        O(1)     
    vector_copy()       O(n)        O(n)
    vector_clone()      O(n)        O(n)
    vector_free()       O(1)        O(1)     
    ----------------------------------------

    n is number of item in vector.

DESCRIPTION

    vector_init() construct new metadata of vector.

    vector_insert() put item into index and move any item from index + i
    to n to right.

    vector_add() puth item into back of vector.

    vector_remove() remove item at index and move any item from index + i
    to n to left.

    vector_copy() free destination vector then copy items of source vector
    to destination vector.

    vector_clone() create new vector, then copy items of source vector to
    new vector.

    vector_free() free memory usase by vector.

ARGUMENTS

    If free_data is true, call free() with pointer (struct vector_item).data
    of each items.

RETURNS

    vector_init(), vector_insert(), vector_add(), vector_remove(), 
    vector_copy(), on success, return 0. On failure, return -1.

    vector_clone on success return pointer to new vector, on failure return
    NULL.

    vector_at() on success return pointer to data of item. On failure, return
    NULL.


AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/
    
#ifndef __CS106B_VECTOR_H__
#define __CS106B_VECTOR_H__

#include <stdlib.h>
#include <stdbool.h>

struct vector_item
{
    void *data;
};

struct vector
{
    struct vector_item *front;
    size_t size;
    size_t max_size;
    size_t base_size;
    size_t ext_size;
};

int vector_init(struct vector *vector);
int vector_insert(struct vector *vector, size_t index, void *data);
int vector_add(struct vector *vector, void *data);
int vector_remove(struct vector *vector, size_t index);
void *vector_at(struct vector *vector, size_t index);
void *vector_end(struct vector *vector);
int vector_copy(struct vector *dest, struct vector *src);
struct vector * vector_clone(struct vector *src);
void vector_free(struct vector *vector, bool free_data);

#endif
