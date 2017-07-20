/*
SYNOPSIS

    int grid_init(struct grid *grid, size_t row_size, size_t col_size);
    int grid_resize(struct grid *grid, size_t row_size, size_t col_size);
    union grid_item *grid_at(struct grid *grid, size_t row, size_t col);
    void grid_free(struct grid *grid);

COMPLEXITY

    fn                  best        worst
    --------------------------------------------
    grid_init()         O(1)        O(1)
    grid_resize()       O(n)        O(n)
    grid_at()           O(1)        O(1)
    grid_free()         O(1)        O(1)
    --------------------------------------------

    n is number of items in grid, n = row_size * col_size.

DESCRIPTION

    grid_init() construct an grid.

    grid_resize() resize of grid, it change number of rows and cols.

    grid_at() retrieve item at row and col.

    grid_free() release memory which use by grid.

ARGUMENTS

    row_size is maximum number of rows in grid. col_size is maximum number of
    columns in grid.

RETURNS

    grid_init(), grid_resize(), on success return 0, on failure return -1.

    grid_at() on success return pointer to grid item, on failure return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <inttypes.h>

union grid_item
{
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    uint64_t uint64;
    void *data;
};

struct grid
{
    union grid_item *items;
    size_t row_size;
    size_t col_size;
};

int grid_init(struct grid *grid, size_t row_size, size_t col_size);
int grid_resize(struct grid *grid, size_t row_size, size_t col_size);
union grid_item *grid_at(struct grid *grid, size_t row, size_t col);
void grid_free(struct grid *grid);
