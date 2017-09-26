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

void dump_grid(struct grid *grid)
{
    size_t row;
    size_t col;
    size_t *item;

    for (row = 0; row < grid->row_size; row++) {
        for (col = 0; col < grid->col_size; col++) {
            if (grid_get(grid, row, col, (void *) &item))
                exit_err("grid_get");
            printf("%02zu ", *item);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    struct grid grid;
    size_t *item;
    size_t row;
    size_t col;
    int ret = EXIT_FAILURE;

    printf("grid_init()\n");
    grid_init(&grid, sizeof(*item));
    if (grid_resize(&grid, ROW_SIZE, COL_SIZE))
        goto error;
    printf("grid_resize(%u, %u)\n", ROW_SIZE, COL_SIZE);

    printf("grid_set()\n");
    for (row = 0; row < grid.row_size; row++) {
        for (col = 0; col < grid.col_size; col++) {
            if (grid_get(&grid, row, col, (void *) &item))
                goto error;
            *item = row * grid.col_size + col;
        }
    }

    printf("grid_dump()\n");
    dump_grid(&grid);

    printf("grid_resize(%u, %u)\n", NEW_ROW_SIZE, NEW_COL_SIZE);
    if (grid_resize(&grid, NEW_ROW_SIZE, NEW_COL_SIZE))
        goto error;
    printf("grid_dump()\n");
    dump_grid(&grid);
    
    printf("grid_resize(%u, %u)\n", NEW_ROW_SIZE2, NEW_COL_SIZE2);
    if (grid_resize(&grid, NEW_ROW_SIZE2, NEW_COL_SIZE2))
        goto error;
    printf("grid_dump()\n");
    dump_grid(&grid);

    ret = EXIT_SUCCESS;

error:
    grid_free(&grid);
    printf("grid_free()\n");
    return ret;
}
