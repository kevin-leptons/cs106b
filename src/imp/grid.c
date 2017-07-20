#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/grid.h>

#define ROW_SIZE 4
#define COL_SIZE 4
#define NEW_ROW_SIZE 6
#define NEW_COL_SIZE 6
#define NEW_ROW_SIZE2 3
#define NEW_COL_SIZE2 3

void dump_grid(struct grid *grid);

int main(int argc, char *argv[])
{
    struct grid grid;
    union grid_item *item;
    size_t row;
    size_t col;

    printf("init: grid, row_size=%u, col_size=%u\n", ROW_SIZE, COL_SIZE);
    if (grid_init(&grid, ROW_SIZE, COL_SIZE) != 0)
        exit_err("grid_init");

    printf("put items of grid\n");
    for (row = 0; row < grid.row_size; row++) {
        for (col = 0; col < grid.col_size; col++) {
            item = grid_at(&grid, row, col);
            if (item == NULL)
                exit_err("grid_at");
            item->uint32 = row * grid.col_size + col;
        }
    }

    printf("retrieve items of grid\n");
    dump_grid(&grid);

    printf("resize grid, row_size=%u, col_size=%u\n",
           NEW_ROW_SIZE, NEW_COL_SIZE);
    if (grid_resize(&grid, NEW_ROW_SIZE, NEW_COL_SIZE) != 0)
        exit_err("grid_resize");
    printf("retrieve items of grid\n");
    dump_grid(&grid);
    
    printf("resize grid, row_size=%u, col_size=%u\n",
           NEW_ROW_SIZE2, NEW_COL_SIZE2);
    if (grid_resize(&grid, NEW_ROW_SIZE2, NEW_COL_SIZE2) != 0)
        exit_err("grid_resize");
    printf("retrieve items of grid\n");
    dump_grid(&grid);

    grid_free(&grid);
    printf("free: grid\n");

    return EXIT_SUCCESS;
}

void dump_grid(struct grid *grid)
{
    size_t row;
    size_t col;
    union grid_item *item;

    for (row = 0; row < grid->row_size; row++) {
        for (col = 0; col < grid->col_size; col++) {
            item = grid_at(grid, row, col);
            if (item == NULL)
                return;
            printf("%02u ", item->uint32);
        }
        printf("\n");
    }
}
