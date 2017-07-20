/*
SYNOPSIS

    Assignment 004
    Boggle, find all word of an random boogle
    ADT: grid, linking, vector, set, string

USAGE

    $ ./dest/bin/004b

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <cs106b/error.h>
#include <cs106b/grid.h>
#include <cs106b/vector.h>
#include <cs106b/set.h>
#include <cs106b/random.h>
#include <cs106b/string.h>
#include <cs106b/dict.h>

#define BCUBE_NUM 16
#define BCUBE_FACE 6
#define BCUBE_NNB 8
#define BGRID_ROW 4
#define BGRID_COL 4

struct bcell
{
    struct bcell *ncells[BCUBE_NNB];
    char value;
    bool visted;
};

struct bgrid
{
    struct bcell cells[BGRID_ROW][BGRID_COL];
    struct grid grid;
};

char BCUBES[BCUBE_NUM][BCUBE_FACE] = {
    "aaeegn", "aoottw", "distty", "eiosst",
    "abbjoo", "cimotu", "eeghnw", "elrtty",
    "achops", "deilrx", "eeinsu", "himnqu",
    "affkps", "delrvy", "ehrtvw", "hlnnrz"
};

static int _bgrid_init(struct bgrid *grid);
static int _bgrid_dump(struct bgrid *grid);
static struct vector * _bgrid_solve(struct bgrid *grid, const char *dfile);
static void _bgrid_free(struct bgrid *grid);
static int _bgrid_rand(struct bgrid *grid);
static int _bgrid_init_cell(struct grid *grid, size_t row, size_t col);
static struct vector * _bgrid_cell_solve(struct bcell *cell,
                                         const struct string *word,
                                         struct dict *dict);
static void _cli_help(void);

int main(int argc, char *argv[])
{
    struct bgrid grid;
    struct vector *words;
    char *word;
    size_t i;
    int opt;

    for (;;) {
        opt = getopt(argc, argv, "h");
        if (opt < 0)
            break;
        if (opt == 'h') {
            _cli_help();
            return EXIT_SUCCESS;
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    }

    if (argc != 2) {
        _cli_help();
        return EXIT_FAILURE;
    }

    // init boggle grid
    if (_bgrid_init(&grid))
        exit_err("_bgrid_init");
    if (_bgrid_rand(&grid))
        exit_err("_bgrid_rand");
    if (_bgrid_dump(&grid))
        exit_err("_bgrid_dump");
    printf("\n");

    // find all of results
    words = _bgrid_solve(&grid, argv[1]);
    if (words == NULL)
        exit_err("_bgrid_solve");

    // display results
    if (words->size == 0) {
        printf("No result\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < words->size; i++) {
        word = vector_at(words, i);
        printf("%s\n", (char *) word);
    }
    
    _bgrid_free(&grid);
    return EXIT_SUCCESS;
}

static int _bgrid_init(struct bgrid *grid)
{
    size_t row;
    size_t col;
    union grid_item *item;

    // init grid with cells is not linked
    memset(grid->cells, 0, sizeof(*grid));
    if (grid_init(&grid->grid, BGRID_ROW, BGRID_COL))
        return -1;
    for (row = 0; row < BGRID_ROW; row ++) {
        for (col = 0; col < BGRID_COL; col++) {
            item = grid_at(&grid->grid, col, row);
            if (item == NULL)
                return -1;
            item->data = &grid->cells[row][col];
        }
    }

    // make cell is linked
    for (row = 0; row < BGRID_ROW; row++) {
        for (col = 0; col < BGRID_COL; col++) {
            if (_bgrid_init_cell(&grid->grid, row, col))
                return -1;
        }
    }

    return 0;
}

static struct vector * _bgrid_solve(struct bgrid *grid, const char *dfile)
{
    size_t i;
    size_t row;
    size_t col;
    union grid_item *item;
    struct bcell *cell;
    struct vector *result_words;
    struct vector *cell_words;
    char *word;
    struct string empty_str;
    struct dict dict;

    result_words = malloc(sizeof(*result_words));
    if (result_words == NULL)
        goto ERROR;
    if (vector_init(result_words))
        goto ERROR;
    if (str_init(&empty_str))
        goto ERROR;
    if (dict_init(&dict, dfile))
        goto ERROR;

    for (row = 0; row < grid->grid.row_size; row++) {
        for (col = 0; col < grid->grid.col_size; col++) {
            item = grid_at(&grid->grid, row, col);
            if (item == NULL)
                return NULL;
            cell = (struct bcell *) item->data;
            cell_words = _bgrid_cell_solve(cell, &empty_str, &dict);
            if (cell_words == NULL)
                continue;
            for (i = 0; i < cell_words->size; i++) {
                word = vector_at(cell_words, i);
                if (vector_add(result_words, word))
                    goto ERROR;
            }
        }
    }

    return result_words;

ERROR:
    if (result_words != NULL) {
        vector_free(result_words, false);
        free(result_words);
    }

    return NULL;
}

static int _bgrid_init_cell(struct grid *grid, size_t row, size_t col)
{
    union grid_item *item;
    union grid_item *nitem;
    struct bcell *cell;

    item = grid_at(grid, row, col);
    if (item == NULL)
        return -1;
    cell = (struct bcell *) item->data;

    // top
    nitem = grid_at(grid, row - 1, col);
    if (nitem != NULL)
        cell->ncells[0] = nitem->data;

    // right top
    nitem = grid_at(grid, row - 1, col + 1);
    if (nitem != NULL)
        cell->ncells[1] = nitem->data;

    // right
    nitem = grid_at(grid, row, col +  1);
    if (nitem != NULL)
        cell->ncells[2] = nitem->data;

    // right bot
    nitem = grid_at(grid, row + 1, col + 1);
    if (nitem != NULL)
        cell->ncells[3] = nitem->data;

    // bot
    nitem = grid_at(grid, row + 1, col);
    if (nitem != NULL)
        cell->ncells[4] = nitem->data;

    // lbot
    nitem = grid_at(grid, row + 1, col - 1);
    if (nitem != NULL)
        cell->ncells[5] = nitem->data;

    // left
    nitem = grid_at(grid, row, col - 1);
    if (nitem != NULL)
        cell->ncells[6] = nitem->data;

    // ltop
    nitem = grid_at(grid, row - 1, col - 1);
    if (nitem != NULL)
        cell->ncells[7] = nitem->data;

    return 0;
}

static int _bgrid_dump(struct bgrid *grid)
{
    size_t row;
    size_t col;
    union grid_item *item;
    struct bcell *cell;

    for (row = 0; row < grid->grid.row_size; row++) {
        for (col = 0; col < grid->grid.col_size; col++) {
            item =  grid_at(&grid->grid, row, col);
            if (item == NULL)
                return -1;
            cell = (struct bcell *) item->data;
            printf("%c ", cell->value);
        }
        printf("\n");
    }

    return 0;
}

static int _bgrid_rand(struct bgrid *grid)
{
    size_t row;
    size_t col;
    union grid_item *item;
    struct bcell *cell;
    struct set used_cubes;
    size_t *rand_cube;
    size_t rand_side;
    char key[sizeof(*rand_cube) +  1];

    if (set_init(&used_cubes))
        return -1;
    memset(key, 0, sizeof(key));
    rand_cube = (size_t *) key;

    for (row = 0; row < grid->grid.row_size; row++) {
        for (col = 0; col < grid->grid.col_size; col++) {
            // rand cube and side of cube
            for (;;) {
                if (rrand(0, BCUBE_NUM - 1, rand_cube))
                    return -1;
                if (set_exist(&used_cubes, key))
                    continue;
                if (set_add(&used_cubes, key))
                    return -1;
                if (rrand(0, BCUBE_FACE - 1, &rand_side))
                    return -1;
                break;
            }

            // set value to cell
            item = grid_at(&grid->grid, row, col);
            if (item == NULL)
                return -1;
            cell = (struct bcell *) item->data;
            cell->value = BCUBES[*rand_cube][rand_side];
        }
    }
    
    set_free(&used_cubes);
    return 0;
}

static void _bgrid_free(struct bgrid *grid)
{
    grid_free(&grid->grid);
}

static struct vector *
_bgrid_cell_solve(struct bcell *cell, const struct string *word,
                  struct dict *dict)
{
    size_t i;
    size_t k;
    struct vector *result_words;        // vector<string>
    struct vector *ncell_words;         // vector<string>
    struct string *new_word;
    struct bcell *ncell;
    char cell_str[2];
    char *ncell_word;

    result_words = malloc(sizeof(*result_words));
    if (result_words == NULL)
        return NULL;
    if (vector_init(result_words))
        goto ERROR;
    cell->visted = true;

    cell_str[1] = 0;
    for (i = 0; i < BCUBE_NNB; i++) {
        ncell = cell->ncells[i];
        if (ncell == NULL)
            continue;
        if (ncell->visted)
            continue;

        new_word = str_clone(word);
        if (new_word == NULL)
            goto ERROR;
        cell_str[0] = ncell->value;
        if (str_catc(new_word, cell_str))
            goto ERROR;
        if (dict_exist(dict, new_word->data)) {
            if (vector_add(result_words, new_word->data))
                goto ERROR;
        }

        ncell_words = _bgrid_cell_solve(ncell, new_word, dict);
        if (ncell_words == NULL)
            goto ERROR;
        for (k = 0; k < ncell_words->size; k++) {
            ncell_word = vector_at(ncell_words, k);
            if (ncell_word == NULL)
                goto ERROR;
            if (vector_add(result_words, ncell_word))
                goto ERROR;
        }
    }

    cell->visted = false;
    return result_words;

ERROR:
    if (result_words != NULL) {
        vector_free(result_words, true);
        free(result_words);
    }
    cell->visted = false;
        
    return NULL;
}

static void _cli_help(void)
{
    printf("USAGE\n\n");
    printf("    004 dfile\n");
    printf("    004 -h\n\n");

    printf("ARGUMENTS\n\n");
    printf("    dfile       Path to English dictionary\n");
    printf("                An dictionary located in asset/dict.txt\n");
}
