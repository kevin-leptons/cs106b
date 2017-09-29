#include <stdlib.h>
#include <stdio.h>
#include <cs106b/error.h>
#include <cs106b/ihtab.h>

#define NUM_ITEM 4

struct info
{
    size_t value;
};

int main(int argc, char *argv[])
{
    struct ihtab tab1;
    struct info items[NUM_ITEM];
    size_t i;
    struct info *item;
    int ret;

    ret = EXIT_FAILURE;
    printf("ihtab_init(tab1)\n");
    ihtab_init(&tab1);

    // push items into table
    for (i = 0; i < NUM_ITEM; i++) {
        items[i].value = i;
        if (ihtab_set(&tab1, i, items + i))
            goto free_tab1;
        printf("ihtab_set(tab1, %zu, %p)\n", i, items + i);
    }

    // retrieve items
    if (ihtab_get(&tab1, 1, (void *) &item))
        goto free_tab1;
    printf("ihtab_get(tab1, 1) = %p\n", item);

    ret = EXIT_SUCCESS;

free_tab1:
    ihtab_free(&tab1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
