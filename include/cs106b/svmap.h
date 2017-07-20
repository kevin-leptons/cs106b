/*
SYNOPSIS

    svmap - string vector map.

    int svmap_init(struct svmap *map);
    int svmap_set(struct svmap *map, struct vector *key, void *value);
    void * svmap_get(struct svmap *map, struct vector *key);
    int svmap_del(struct svmap *map, struct vector *key);
    struct vector * svmap_keys(struct svmap *map);
    void svmap_free(struct svmap *map);

COMPLEXITY

    fn                  best                worst
    ------------------------------------------------------------
    svmap_init()        htable_init()       htable_init()
    svmap_set()         htable_set()        htable_init()
    svmap_get()         htable_get()        htable_init()
    svmap_del()         htable_del()        htable_init()
    svmap_keys()        O(1)                O(1)
    svmap_free()        htable_free()       htable_init()
    ------------------------------------------------------------

    n is number of items in map.

DESCRIPTION

    String vector map store key-value pair. Key is string vector and value
    is pointer to any types.

    svmap_init() construct an map.

    svmap_set() put key-value pair into map. If key is early exists, value
    will be replace by new value.

    svmap_get() retrieve value of key.

    svmap_del() remove key-value pair from map.

    svmap_keys() retrieve vector of vector<string> as keys.

    svmap_free() free memory which use by map.

RETURNS

    svmap_init(), svmap_set(), svmap_del() on success return 0, on failure
    return -1.

    svmap_keys() on success return pointer to vector of vector<string>,
    on failure return NULL.

    svmap_get() on success return pointer to value, on failure return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_SVMAP_H__
#define __CS106B_SVMAP_H__

#include <cs106b/vector.h>
#include <cs106b/htable.h>

struct svmap
{
    struct htable hmap;
    struct vector keys;
    struct htable kmap;
};

int svmap_init(struct svmap *map);
int svmap_set(struct svmap *map, struct vector *key, void *value);
void * svmap_get(struct svmap *map, struct vector *key);
int svmap_del(struct svmap *map, struct vector *key);
struct vector * svmap_keys(struct svmap *map);
void svmap_free(struct svmap *map);

#endif
