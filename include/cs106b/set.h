/*
SYNOPSIS

    int set_init(struct set *set);
    int set_add(struct set *set, const char *item);
    int set_del(struct set *set, const char *item);
    int set_exist(struct set *set, const char *item);
    void set_free(struct set *set);

COMPLEXITY

    fn                  best                worst
    -----------------------------------------------------------
    set_init()          htable_init()       htable_init() 
    set_add()           htable_set()        htable_set()
    set_del()           htable_del()        htable_del()
    set_exist()         htable_get()        htable_get()
    set_free()          htable_free()       htable_free()
    -----------------------------------------------------------

    n is number of items in set.

DESCRIPTION

    set_init() construct an set.

    set_add() add an item into set. If item is early exist, do nothing.

    set_del() remove an item from set.

    set_exist() check item in set or not.

    set_free() free memory usage by set.

RETURNS

    set_init(), set_add(), set_del() on success return 0, on failure return -1.
    
    set_exist() on item is exist return 1, on item is not exist return 0.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_SET_H__
#define __CS106B_SET_H__

#include <stdlib.h>

#include <cs106b/htable.h>

struct set
{
    struct htable map;
};

int set_init(struct set *set);
int set_add(struct set *set, const char *item);
int set_del(struct set *set, const char *item);
int set_exist(struct set *set, const char *item);
void set_free(struct set *set);

#endif
