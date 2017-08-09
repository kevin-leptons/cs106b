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
    struct pset set;
    struct info items[ITEM_SIZE];
    struct info other_item;
    struct info *item;
    size_t i;

    if (pset_init(&set))
        exit_err("pset_init");

    for (i = 0; i < ITEM_SIZE; i++) {
        items[i].value = i;
        if (pset_add(&set, items + i))
            exit_err("pset_add");
        printf("add: set, p=%p\n", items + i);
    }

    if (pset_exist(&set, items + 1))
        printf("pset: exist, p=%p, yes\n", items + 1);
    else
        exit_err("pset_exist");

    if (!pset_exist(&set, &other_item))
        printf("pset: exist, p=%p, no\n", &other_item);
    else
        exit_err("pset_exist");

    if (pset_del(&set, items + 2))
        exit_err("pset_del");
    printf("del: pset, p=%p\n", items + 2);

    printf("start iter\n");
    for (;;) {
        item = pset_next(&set);
        if (item == NULL)
            break;
        printf("next: pset, p=%p, value=%lu\n", item, item->value);
    }

    return EXIT_SUCCESS;
}
