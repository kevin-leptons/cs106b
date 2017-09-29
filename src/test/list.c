#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/list.h>

#define ITEM_NUM 4

struct item
{
    size_t value;
};

int main(int argc, char *argv[])
{
    struct list l1;
    struct item items[ITEM_NUM];
    struct item *item;
    size_t i;
    int ret;

    ret = EXIT_FAILURE;
    printf("list_init(l1)\n");
    list_init(&l1);

    // push items to list
    for (i = 0; i < ITEM_NUM; i++) {
        items[i].value = i;
        if (list_add(&l1, items + i))
            goto free_l1;
        printf("list_add(l1, %p)\n", items + i);
    }

    // retrieve item in list
    if (list_get(&l1, ITEM_NUM - 1, (void *) &item))
        goto free_l1;
    printf("list_get(l1, %u) = %p\n", ITEM_NUM - 1, item);

    // use iterator
    list_ireset(&l1);
    for (;;) {
        if (list_next(&l1, (void *) &item)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                goto free_l1;
            }
        }
        printf("list_next(l1) = %p\n", item);
    }
    ret = EXIT_SUCCESS;

free_l1:
    list_free(&l1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
