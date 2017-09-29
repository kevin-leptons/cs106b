#include <stdlib.h>
#include <stdio.h>
#include <cs106b/error.h>
#include <cs106b/pset.h>

#define ITEM_SIZE 4

struct info
{
    size_t value;
};

int main(int argc, char *argv[])
{
    struct pset s1;
    struct info items[ITEM_SIZE];
    struct info other_item;
    struct info *item;
    size_t i;
    int ret;

    ret = EXIT_FAILURE;
    printf("pset_init(s1)\n");
    pset_init(&s1);

    for (i = 0; i < ITEM_SIZE; i++) {
        items[i].value = i;
        if (pset_add(&s1, items + i))
            goto free_s1;
        printf("pset_add(s1, '%p')\n", items + i);
    }

    if (pset_exist(&s1, items + 1))
        printf("pset_exist(s1, '%p') yes\n", items + 1);
    else
        goto free_s1;

    if (!pset_exist(&s1, &other_item))
        printf("pset_exist(s1, '%p') no\n", &other_item);
    else
        goto free_s1;

    printf("pset_del(s1, '%p')\n", items + 2);
    if (pset_del(&s1, items + 2))
        goto free_s1;

    for (;;) {
        if (pset_next(&s1, (void *) &item)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                goto free_s1;
            }
        }
        printf("pset_next(s1) = %p, value=%lu\n", item, item->value);
    }

    ret = EXIT_SUCCESS;

free_s1:
    pset_free(&s1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
