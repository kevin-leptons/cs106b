#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cs106b/error.h>
#include <cs106b/vector.h>
#include <cs106b/sorting.h>

#define ITEMS_NUM 16
size_t ITEMS[ITEMS_NUM] = {
    7, 4, 5, 3, 2, 6, 1, 0, 
    128, 17, 2, 16, 8, 10, 9, 17
};

int sizet_cmpfn(void *left, void *right)
{
    size_t leftv;
    size_t rightv;

    leftv = *(size_t *) left;
    rightv = *(size_t *) right;

    if (leftv < rightv)
        return 1;
    else if (leftv > rightv)
        return -1;
    else
        return 0;
}

void dump_vec(struct vector *v)
{
    size_t i;

    for (i = 0; i < v->size; i++) {
        printf("%zu ", *(size_t *) vector_get(v, i));
    }
    printf("\n");
}

void cli_help(void)
{
    printf("USAGE\n");
    printf("    sort TYPE\n");
    printf("TYPES\n");
    printf("    ins:    insertion sort\n");
    printf("    sel:    selection sort\n");
    printf("    mer:    merge sort\n");
    printf("    qui:    quick sort\n");
}

int (*sort_fn)(struct vector *v, enum sortdir direction,
               int (*cmp_fn)(void *left, void *right));

int main(int argc, char *argv[])
{
    struct vector v;
    size_t i;

    // detect what sorting algorithm to use
    if (argc != 2) {
        cli_help();
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "ins") == 0) {
        sort_fn = ins_sort;
        printf("algorithm: insertion\n");
    } else if (strcmp(argv[1], "sel") == 0) {
        sort_fn = sel_sort;
        printf("algorithm: selection\n");
    } else if (strcmp(argv[1], "mer") == 0) {
        sort_fn = mer_sort;
        printf("algorithm: merge\n");
    } else if (strcmp(argv[1], "qui") == 0) {
        sort_fn = qui_sort;
        printf("algorithm: quick\n");
    } else {
        cli_help();
        return EXIT_FAILURE;
    }

    vector_init(&v);
    for (i = 0; i < ITEMS_NUM; i++) {
        if (vector_add(&v, ITEMS + i))
            goto error;
    }
    printf("input: ");
    dump_vec(&v);

    if (sort_fn(&v, SORTDIR_INC, sizet_cmpfn))
        goto error;
    printf("output SORTDIR_INC: ");
    dump_vec(&v);

    if (sort_fn(&v, SORTDIR_DEC, sizet_cmpfn))
        goto error;
    printf("output SORTDIR_DEC: ");
    dump_vec(&v);

    vector_free(&v, false);
    return EXIT_SUCCESS;

error:
    espace_perror(espace);
    return EXIT_FAILURE;
}
