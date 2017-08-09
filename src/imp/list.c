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
    struct list list;
    struct item items[ITEM_NUM];
    struct item *item;
    size_t i;

    if (list_init(&list))
        exit_err("list_init");

    for (i = 0; i < ITEM_NUM; i++) {
        items[i].value = i;
        if (list_add(&list, items + i))
            exit_err("list_add");
    }

    item = list_at(&list, ITEM_NUM - 1);
    if (item != NULL)
        printf("at: list, p=%p, value=%zu\n", item, item->value);
    else
        exit_err("list_at");

    printf("start iter\n");
    list_rsiter(&list);
    for (;;) {
        item = list_next(&list);
        if (item == NULL)
            break;
        printf("next: list, p=%p, value=%zu\n", item, item->value);
    }

    list_free(&list);
    return EXIT_SUCCESS;
}
