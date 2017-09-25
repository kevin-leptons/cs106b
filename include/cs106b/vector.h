/*
SYNOPSIS

    int vector_init(struct vector *vector);
    int vector_resize(struct vector *vector, size_t size);
    int vector_get(struct vector *vector, size_t index, void **value);
    int vector_set(struct vector *vector, size_t index, void *data);
    int vector_put(struct vector *vector, size_t index, void *data);
    int vector_add(struct vector *vector, void *data);
    int vector_del(struct vector *vector, size_t index);
    int vector_end(struct vector *vector, void **value);
    int vector_copy(struct vector *dest, struct vector *src);
    int vector_clone(struct vector **dest, struct vector *src);
    void vector_free(struct vector *vector);

COMPLEXITY

    fn                  best        worst
    ----------------------------------------
    vector_init()       O(1)        O(1)     
    vector_resize()     O(n)        O(n)
    vector_get()        O(1)        O(1)     
    vector_set()        O(1)        O(1)     
    vector_put()        O(1)        O(n)     
    vector_add()        O(1)        O(n)     
    vector_del()        O(1)        O(n)     
    vector_end()        O(1)        O(1)
    vector_copy()       O(n)        O(n)
    vector_clone()      O(n)        O(n)
    vector_free()       O(1)        O(1)     
    ----------------------------------------

    n is number of items in vector.

DESCRIPTION

    vector_init() construct new of vector

    vector_resize() change max size of vector. If new max size is less than
    current size, few items will be disapears.

    vector_get() and vector_set() retrieve/store items in vector by index.
    If index is out of range, calling will fails and espace is set to 
    CS106B_EINDEX.

    vector_put() store item into index and move right items at i to i + 1.
    If index is out of range, calling will fails and espace is set to
    CS106B_EINDEX. If vector's size is zero, you can't put item into
    0 index, that cause CS106B_EINDEX.

    vector_add() put item into back of vector.

    vector_del() remove item at index and move right items at i to i - 1.

    vector_end() retrieve end item in vector. If vector's size 0, calling
    will fails and espace is set to CS106B_EINDEX.

    vector_copy() copy items from dest to source. If source vector is not
    empty and max size is less than source's size, calling frees dest vector
    then alloc new memory block.

    vector_clone() create new vector, then copy items of source vector to
    new vector.

    vector_free() free memory usase by vector.

ARGUMENTS

    Index argument is unsigned integer, start from zero to max of size_t.

    Item argument is presents by void pointer.

RETURNS

    On success, return 0. On failure, return -1 and espace is set to
    corresponding error.

ERRORS

    CS106B_EINDEX           Out of index value
    SYS_ENOMEM              System memory is full

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/
    
#ifndef _CS106B_VECTOR_H_
#define _CS106B_VECTOR_H_

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

void vector_init(struct vector *vector);
int vector_resize(struct vector *vector, size_t size);
int vector_get(struct vector *vector, size_t index, void **value);
int vector_set(struct vector *vector, size_t index, void *data);
int vector_put(struct vector *vector, size_t index, void *data);
int vector_add(struct vector *vector, void *data);
int vector_del(struct vector *vector, size_t index);
int vector_end(struct vector *vector, void **value);
int vector_copy(struct vector *dest, struct vector *src);
int vector_clone(struct vector **dest, struct vector *src);
void vector_free(struct vector *vector);

#endif
