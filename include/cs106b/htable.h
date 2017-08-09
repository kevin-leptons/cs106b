/*
SYNOPSIS

    int htable_init(struct htable *table);
    int htable_set(struct htable *table, const char *key, const void *value);
    void *htable_get(struct htable *table, const char *key);
    int htable_del(struct htable *table, const char *key);
    int htable_clear(struct htable *table);
    int htable_resize(struct htable *table, size_t max_size);
    void htable_free(struct htable *table);

COMPLEXITY

    fn                  best        worst
    -------------------------------------------------
    htable_init()       O(1)        O(1)
    htable_set()        O(1)        O(n)
    htable_get()        O(1)        O(n)
    htable_del()        O(1)        O(n)
    htable_clear()      O(1)        O(1)
    htable_resize()     O(n)        O(n)
    htable_free()       O(1)        O(1)
    -------------------------------------------------

    n is number of bins in table.

DESCRIPTION

    htable_init() construct hash table.

    htable_set() set an pair key-value in table.

    htable_get() retrieve value correspond with key in table.

    htable_del() remove an pair key-value in table.

    htable_clear() clear all of keys, keep memory usage.

    htable_resize() extend or narrow max size of table.

    htable_free() free memory usage by table.

RETURNS

    htable_init(), htable_set(), htable_del(), htable_resize() on success
    return 0, on failure return -1.

    htable_get() on success return pointer to value, on failure return NULL.

AUTHORS

    Kevin Leptons <kevin.letons@gmail.com>
*/

#ifndef __CS106B_HTABLE_H__
#define __CS106B_HTABLE_H__

#include <stdlib.h>

struct htable_item
{
    char *key;
    void *value;
};

struct htable
{
    size_t size;
    size_t mask;
    size_t max_size;
    struct htable_item *items;
};

int htable_init(struct htable *table);
int htable_set(struct htable *table, const char *key, const void *value);
void *htable_get(struct htable *table, const char *key);
int htable_del(struct htable *table, const char *key);
int htable_clear(struct htable *table);
int htable_resize(struct htable *table, size_t max_size);
void htable_free(struct htable *table);

#endif
