#include <cs106b/grid.h>
#include <string.h>
#include <cs106b/mem.h>
#include <cs106b/error.h>

void grid_init(struct grid *grid, size_t item_size)
{
    grid->items = NULL;
    grid->row_size = 0;
    grid->col_size = 0;
    grid->item_size = item_size;
}

void grid_free(struct grid *grid)
{
    grid->row_size = 0;
    grid->col_size = 0;
    if (grid->items != NULL)
        free(grid->items);
}

int grid_resize(struct grid *grid, size_t row_size, size_t col_size)
{
    void *new_items;
    size_t min_row;
    size_t min_col;
    size_t old_index;
    size_t new_index;
    size_t new_item_size;
    size_t i;

    min_row = grid->row_size < row_size ? grid->row_size : row_size;
    min_col = grid->col_size < col_size ? grid->col_size : col_size;

    new_item_size = (row_size * col_size) * grid->item_size;
    if (cs106b_malloc((void *) &new_items, new_item_size))
        return -1;
    memset(new_items, 0, new_item_size);
    for (i = 0; i < min_row; i++) {
        old_index = grid->item_size * (i * grid->col_size);
        new_index = grid->item_size * (i * row_size);
        memcpy(new_items + new_index, grid->items + old_index,
               grid->item_size * min_col);
    }

    free(grid->items);
    grid->items = new_items;
    grid->row_size = row_size;
    grid->col_size = col_size;

    return 0;
}

int grid_get(struct grid *grid, size_t row, size_t col, void **value)
{
    if (row >= grid->row_size || col >= grid->col_size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    *value = grid->items + grid->item_size * (row * grid->col_size + col);
    return 0;
}

int grid_set(struct grid *grid, size_t row, size_t col, void *value)
{
    if (row >= grid->row_size || col >= grid->col_size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    memcpy(grid->items + grid->item_size * (row * grid->col_size + col),
           value, grid->item_size);
    return 0;
}

int grid_copy(struct grid *dest, struct grid *src)
{
    grid_free(dest);
    grid_init(dest, src->item_size);
    if (grid_resize(dest, src->row_size, src->col_size))
        return -1;

    memcpy(dest->items, src->items, 
           src->item_size * (src->row_size * src->col_size));
    return 0;
}

int grid_clone(struct grid **dest, struct grid *src)
{
    if (cs106b_malloc((void *) *dest, sizeof(struct grid *)))
        return -1;
    grid_init(*dest, 0);
    return grid_copy(*dest, src);
}
