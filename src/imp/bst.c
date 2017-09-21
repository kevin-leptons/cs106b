#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/bst.h>

int main(int argc, char *argv[])
{
    struct bst *bst;
    size_t key;

    bst = NULL;
    if (bst_add(&bst, 1, NULL))
        goto error;
    if (bst_add(&bst, 2, NULL))
        goto error;
    if (bst_add(&bst, 3, NULL))
        goto error;

    key = bst_min(bst);
    printf("min: %zu\n", key);

    key = bst_max(bst);
    printf("max: %zu\n", key);

    if (bst_del(&bst, 1))
        goto error;
    printf("remove key: 1\n");

    key = bst_min(bst);
    printf("min: %zu\n", key);
    key = bst_max(bst);
    printf("max: %zu\n", key);

    bst_free(&bst);
    return EXIT_SUCCESS;

error:
    if (espace_check())
        espace_perror(espace);
    return EXIT_FAILURE;
}
