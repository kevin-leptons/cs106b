/*
SYNOPSIS

    Assignment 001
    Game of Life
    https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
    ADT: grid

USAGE

    $ ./dest/bin/001 -f asset/001-kevin.txt

AUTHORS

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
size_t count_n(struct grid *grid, size_t row, size_t col);
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
    union grid_item *item;

    pfile = NULL;
    rtime = DEF_RTIME;

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
            exit_err("open_pfile");
    } else {
        if (grid_init(&g1, 6, 6) != 0)
            exit_err("grid_init:g1");
        for (row = 0; row < g1.row_size; row++) {
            for (col = 0; col < g1.col_size; col++) {
                item = grid_at(&g1, row, col);
                item->uint8 = PATTERN_TOAD[row][col] != 0;
            }
        }
    }

    if (grid_init(&g2, g1.row_size, g1.col_size) != 0)
        exit_err("grid_init:d2");

    rtime = rtime * 1000;
    gtmp1 = &g1;
    gtmp2 = &g2;
    for (;;) {
        if (dump_grid(gtmp1) != 0)
            exit_err("dump_grid");
        if (live(gtmp1, gtmp2) != 0)
            exit_err("live");
        gtmp = gtmp1;
        gtmp1 = gtmp2;
        gtmp2 = gtmp;
        usleep(rtime);
    }

    grid_free(&g1);
    grid_free(&g2);
    return EXIT_SUCCESS;
}

int dump_grid(struct grid *grid)
{
    size_t row;
    size_t col;
    union grid_item *item;

    CLRSCR;
    for (row = 0; row < grid->row_size; row++) {
        for (col = 0; col < grid->col_size; col++) {
            item = grid_at(grid, row, col);
            if (item == NULL)
                return -1;
            if (item->uint8 == 0)
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
    union grid_item *src_item;
    union grid_item *dest_item;
    size_t n;

    for (row = 0; row < src->row_size; row++) {
        for (col = 0; col < src->col_size; col++) {
           n = count_n(src, row, col);
           src_item = grid_at(src, row, col);
           if (src_item == NULL)
               return -1;
           dest_item = grid_at(dest, row, col);
           if (dest_item == NULL)
               return -1;

           if (src_item->uint8 != 0) {
               if (n == 2 || n == 3)
                   dest_item->uint8 = 1;
               else
                   dest_item->uint8 = 0;
           } else if (n == 3) {
               dest_item->uint8 = 1;
           } else {
               dest_item->uint8 = 0;
           }
        }
    }

    return 0;
}

size_t count_n(struct grid *grid, size_t row, size_t col)
{
    bool has_top;
    bool has_right;
    bool has_bot;
    bool has_left;
    union grid_item *item;
    size_t n;

    n = 0;
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
        item = grid_at(grid, row - 1, col);
        if (item->uint8)
            n += 1;
        if (has_left) {
            item = grid_at(grid, row - 1, col - 1);
            if (item->uint8)
                n += 1;
        }
        if (has_right) {
            item = grid_at(grid, row - 1, col + 1);
            if (item->uint8)
                n += 1;
        }
    }

    if (has_bot) {
        item = grid_at(grid, row + 1, col);
        if (item->uint8)
            n += 1;
        if (has_left) {
            item = grid_at(grid, row + 1, col - 1);
            if (item->uint8)
                n += 1;
        }
        if (has_right) {
            item = grid_at(grid, row + 1, col + 1);
            if (item->uint8)
                n += 1;
        }
    }

    if (has_left) {
        item = grid_at(grid, row, col - 1);
        if (item->uint8)
            n += 1;
    }
    if (has_right) {
        item = grid_at(grid, row, col + 1);
        if (item->uint8)
            n += 1;
    }

    return n;
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
    union grid_item *item;

    fd = -1;
    data = NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        goto ERROR;
    if (fstat(fd, &stat) != 0)
        goto ERROR;
    data = malloc(stat.st_size);
    if (data == NULL)
        goto ERROR;

    rsize = read(fd, data, stat.st_size);
    if (rsize != stat.st_size)
        goto ERROR;
    close(fd);
    fd = -1;

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
            errno = EINVAL;
            goto ERROR;
        }
        row_size += 1;
    }
    if (p != s) {
        errno = EINVAL;
        goto ERROR;
    }

    if (grid_init(grid, row_size, col_size) != 0)
        goto ERROR;

    row = 0;
    col = 0;
    for (p = data; p <= e; p++) {
        if (*p == '\n') {
            col = 0;
            row += 1;
            continue;
        }
        printf("%lu %lu\n", row, col);
        item = grid_at(grid, row, col);
        if (*p == '1')
            item->uint8 = 1;
        else
            item->uint8 = 0;
        col += 1;
    }

    free(data);
    return 0;

ERROR:
    if (fd >= 0)
        close(fd);
    if (data != NULL)
        free(data);
    return -1;
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
