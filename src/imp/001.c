/*
SYNOPSIS
========

    Assignment 001
    Game of Life
    https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
    ADT: grid

USAGE
=====

    $ ./dest/bin/001 -f asset/001-kevin.txt

AUTHORS
=======

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <cs106b/error.h>
#include <cs106b/grid.h>
#include <cs106b/mem.h>

#define ASCII_ESC 27
#define LIVE_CELL "\u2588"
#define DIE_CELL " "
#define CLRSCR (printf("%c[0;0f%c[2J", ASCII_ESC, ASCII_ESC))
#define DEF_WIDTH 32
#define DEF_HEIGHT 24
#define DEF_RTIME 100

const char PATTERN_TOAD[6][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0} 
};

int dump_grid(struct grid *grid);
int live(struct grid *src, struct grid *dest);
int count_n(struct grid *grid, size_t row, size_t col, size_t *n);
int open_pfile(const char *file, struct grid *grid);
void cli_help(void);

int main(int argc, char *argv[])
{
    struct grid g1;
    struct grid g2;
    struct grid *gtmp;
    struct grid *gtmp1;
    struct grid *gtmp2;
    char *pfile;
    useconds_t rtime;
    int opt;
    size_t row;
    size_t col;
    uint8_t *item;
    int ret;

    pfile = NULL;
    rtime = DEF_RTIME;
    ret = EXIT_FAILURE;

    for (;;) {
        opt = getopt(argc, argv, "hf:t:");
        if (opt < 0)
            break;
        if (opt == 'h') {
            cli_help();
            return EXIT_SUCCESS;
        } else if (opt == 'f') {
            pfile = optarg;
        } else if (opt == 't') {
            rtime = atoi(optarg);
        } else {
            cli_help();
            return EXIT_FAILURE;
        }
    }
    
    if (pfile != NULL) {
        if (open_pfile(pfile, &g1) != 0)
            goto finish;
    } else {
        grid_init(&g1, sizeof(*item));
        if (grid_resize(&g1, 6, 6))
            goto finish;
        for (row = 0; row < g1.row_size; row++) {
            for (col = 0; col < g1.col_size; col++) {
                if (grid_get(&g1, row, col, (void *) &item))
                    goto finish;
                *item = PATTERN_TOAD[row][col] != 0;
            }
        }
    }

    grid_init(&g2, sizeof(*item));
    if (grid_resize(&g2, g1.row_size, g1.col_size))
        goto clear_g1;

    rtime = rtime * 1000;
    gtmp1 = &g1;
    gtmp2 = &g2;
    for (;;) {
        if (dump_grid(gtmp1) != 0)
            goto clear_g2;
        if (live(gtmp1, gtmp2) != 0)
            goto clear_g2;
        gtmp = gtmp1;
        gtmp1 = gtmp2;
        gtmp2 = gtmp;
        usleep(rtime);
    }

    grid_free(&g1);
    grid_free(&g2);
    ret = EXIT_SUCCESS;

clear_g2:
    grid_free(&g2);
clear_g1:
    grid_free(&g1);
finish:
    if (ret != EXIT_SUCCESS)
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}

int dump_grid(struct grid *grid)
{
    size_t row;
    size_t col;
    uint8_t *item;

    CLRSCR;
    for (row = 0; row < grid->row_size; row++) {
        for (col = 0; col < grid->col_size; col++) {
            if (grid_get(grid, row, col, (void *) &item))
                return -1;
            if (*item == 0)
                printf(DIE_CELL);
            else
                printf(LIVE_CELL);
        }
        printf("\n");
    }

    return 0;
}

int live(struct grid *src, struct grid *dest)
{
    size_t row;
    size_t col;
    uint8_t *src_item;
    uint8_t *dest_item;
    size_t n;

    for (row = 0; row < src->row_size; row++) {
        for (col = 0; col < src->col_size; col++) {
           if (count_n(src, row, col, &n))
               return -1;
           if (grid_get(src, row, col, (void *) &src_item))
               return -1;
           if (grid_get(dest, row, col, (void *) &dest_item))
               return -1;

           if (*src_item != 0) {
               if (n == 2 || n == 3)
                   *dest_item = 1;
               else
                   *dest_item = 0;
           } else if (n == 3) {
               *dest_item = 1;
           } else {
               *dest_item = 0;
           }
        }
    }

    return 0;
}

int count_n(struct grid *grid, size_t row, size_t col, size_t *n)
{
    bool has_top;
    bool has_right;
    bool has_bot;
    bool has_left;
    uint8_t *item;

    *n = 0;
    has_top = false;
    has_right = false;
    has_bot = false;
    has_left = false;

    if (row > 0)
        has_top = true;
    if (col < grid->col_size - 1)
        has_right = true;
    if (row < grid->row_size - 1)
        has_bot = true;
    if (col > 0)
        has_left = true;

    if (has_top) {
        if (grid_get(grid, row - 1, col, (void *) &item))
            return -1;
        if (*item)
            *n += 1;
        if (has_left) {
            if (grid_get(grid, row - 1, col - 1, (void *) &item))
                return -1;
            if (*item)
                *n += 1;
        }
        if (has_right) {
            if (grid_get(grid, row - 1, col + 1, (void *) &item))
                return -1;
            if (*item)
                *n += 1;
        }
    }

    if (has_bot) {
        if (grid_get(grid, row + 1, col, (void *) &item))
            return -1;
        if (*item)
            *n += 1;
        if (has_left) {
            if (grid_get(grid, row + 1, col - 1, (void *) &item))
                return -1;
            if (*item)
                *n += 1;
        }
        if (has_right) {
            if (grid_get(grid, row + 1, col + 1, (void *) &item))
                return -1;
            if (*item)
                *n += 1;
        }
    }

    if (has_left) {
        if (grid_get(grid, row, col - 1, (void *) &item))
            return -1;
        if (*item)
            *n += 1;
    }
    if (has_right) {
        if (grid_get(grid, row, col + 1, (void *) &item))
            return -1;
        if (*item)
            *n += 1;
    }
    return 0;
}

int open_pfile(const char *file, struct grid *grid)
{
    int fd;
    struct stat stat;
    char *data;
    char *s;
    char *p;
    char *e;
    ssize_t rsize;
    size_t row_size;
    size_t col_size;
    size_t col_csize;
    size_t row;
    size_t col;
    uint8_t *item;
    int ret;

    data = NULL;
    ret = -1;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        sys_raise(errno);
        goto finish;
    }
    if (fstat(fd, &stat) != 0) {
        sys_raise(errno);
        goto clear_fd;
    }
    if (cs106b_malloc((void *) &data, stat.st_size))
        goto clear_fd;

    rsize = read(fd, data, stat.st_size);
    if (rsize != stat.st_size) {
        sys_raise(errno);
        goto clear_data;
    }

    row_size = 0;
    col_size = 0;
    e = data + stat.st_size - 1;
    for (s = data, p = data; p <= e; p++) {
        if (*p == '\n')
            col_csize = p - s;
        else if (p == e)
            col_csize = p - s + 1;
        else
            continue;
        s = p + 1;

        if (col_size == 0) {
            col_size = col_csize;
        }
        else if (col_size != col_csize) {
            espace_raise(CS106B_EFORMAT);
            goto clear_data;
        }
        row_size += 1;
    }
    if (p != s) {
        espace_raise(CS106B_EFORMAT);
        goto clear_data;
    }

    grid_init(grid, sizeof(*item));
    if (grid_resize(grid, row_size, col_size))
        goto clear_data;

    row = 0;
    col = 0;
    for (p = data; p <= e; p++) {
        if (*p == '\n') {
            col = 0;
            row += 1;
            continue;
        }
        if (grid_get(grid, row, col, (void *) &item)) {
            grid_free(grid);
            goto clear_data;
        }
        if (*p == '1')
            *item = 1;
        else
            *item = 0;
        col += 1;
    }

    ret = 0;

clear_data:
    free(data);
clear_fd:
    close(fd);
finish:
    return ret;
}

void cli_help(void)
{
    printf("NAME\n");
    printf("    001 - Display grid of Game of Life\n\n");

    printf("USAGE\n");
    printf("    001 [-f file] [-r time]\n\n");

    printf("OPTIONS\n");
    printf("    -f      Pattern file\n");
    printf("            If not specific, blinker pattern will be use\n");
    printf("            See an example in asset/002-toad.txt\n");
    printf("    -t      Refresh time in miliseconds\n");
    printf("            Default is 500 miliseconds\n");
}
