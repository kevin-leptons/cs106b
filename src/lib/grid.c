#include <cs106b/grid.h>

static int _grid_resize(struct grid *grid, size_t row_size, size_t col_size);

int grid_init(struct grid *grid, size_t row_size, size_t col_size)
{
    grid->items = NULL;
    grid->row_size = 0;
    grid->col_size = 0;
    if (_grid_resize(grid, row_size, col_size) != 0)
        return -1;

    return 0; 
}

int grid_resize(struct grid *grid, size_t row_size, size_t col_size)
{
    if (_grid_resize(grid, row_size, col_size) != 0)
        return -1;

    return 0;
}

union grid_item *grid_at(struct grid *grid, size_t row, size_t col)
{
    if (row >= grid->row_size || col >= grid->col_size)
        return NULL;

    return grid->items + row * grid->col_size + col;
}

void grid_free(struct grid *grid)
{
    grid->row_size = 0;
    grid->col_size = 0;

    if (grid->items != NULL)
        free((void*)grid->items);
}

static int _grid_resize(struct grid *grid, size_t row_size, size_t col_size)
{
    union grid_item *new_items;
    size_t min_row;
    size_t min_col;
    size_t row;
    size_t col;
    size_t old_index;
    size_t new_index;
    size_t new_item_size;

    min_row = grid->row_size < row_size ? grid->row_size : row_size;
    min_col = grid->col_size < col_size ? grid->col_size : col_size;

    new_item_size = (row_size * col_size) * sizeof(union grid_item);
    new_items = malloc(new_item_size);
    if (new_items == NULL)
        return -1;
    memset(new_items, 0, new_item_size);
    for (row = 0; row < min_row; row++) {
        for (col = 0; col < min_col; col++) {
            old_index = row * grid->col_size + col;
            new_index = row * row_size + col;
            *(new_items + new_index) = *(grid->items + old_index);
        }
    }

    free(grid->items);
    grid->items = new_items;
    grid->row_size = row_size;
    grid->col_size = col_size;

    return 0;
}
