/*
SYNOPSIS
========

    void grid_init(struct grid *grid);
    void grid_free(struct grid *grid);
    int grid_resize(struct grid *grid, size_t row_size, size_t col_size);
    int grid_get(struct grid *grid, size_t row, size_t col, void **value);
    int grid_set(struct grid *grid, size_t row, size_t col, void *value);
    int grid_copy(struct grid *dest, struct grid *src);
    int grid_clone(struct grid **dest, struct grid *src);

COMPLEXITY
==========

    fn                  best        worst
    --------------------------------------------
    grid_init()         O(1)        O(1)
    grid_free()         O(1)        O(1)
    grid_resize()       O(n)        O(n)
    grid_get()          O(1)        O(1)
    grid_set()          O(1)        O(1)
    grid_copy()         O(n)        O(n)
    grid_clone()        O(n)        O(n)
    --------------------------------------------

    n is number of items in grid, n = row_size * col_size.

DESCRIPTION
===========

    grid_init() and grid_free() construct/destroy an grid.

    grid_resize() changes number of rows and cols of grid.

    grid_get() and grid_set() retrieve/store item into cell of grid.

    grid_copy() copy items of source to dest grid.

    grid_clone() create new grid called dest grid, then copy items of source
    grid to dest grid.

ARGUMENTS
=========

    <item_size> is size of items in grid by bytes.

    <row_size>, <col_size> is maximum number of rows and columns in grid.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    CS106B_EINDEX           Row or column index is out of range
    SYS_ENOMEM              System memory is full

AUTHORS
=======

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef _cs106b_grid_h
#define _cs106b_grid_h

#include <stdlib.h>

struct grid_item
{
    void *value;
};

struct grid
{
    struct grid_item *items;
    size_t row_size;
    size_t col_size;
};

void grid_init(struct grid *grid);
void grid_free(struct grid *grid);
int grid_resize(struct grid *grid, size_t row_size, size_t col_size);
int grid_get(struct grid *grid, size_t row, size_t col, void **value);
int grid_set(struct grid *grid, size_t row, size_t col, void *value);
int grid_copy(struct grid *dest, struct grid *src);
int grid_clone(struct grid **dest, struct grid *src);

#endif
