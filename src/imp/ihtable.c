#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/ihtable.h>

#define NUM_ITEM 4

struct info
{
    size_t value;
};

int main(int argc, char *argv[])
{
    struct ihtable table;
    struct info items[NUM_ITEM];
    size_t i;
    struct info *item;

    if (ihtable_init(&table))
        exit_err("ihtable_init");
    for (i = 0; i < NUM_ITEM; i++) {
        items[i].value = i;
        if (ihtable_set(&table, i, items + i))
            exit_err("ihtable_set");
        printf("set: ihtable, key=%zu, value=%p\n", i, items + i);
    }

    item = ihtable_get(&table, 1);
    if (item == NULL)
        exit_err("ihtable_get");
    printf("get: ihtable, key=%i, value=%p\n", 1, item);

    return EXIT_SUCCESS;
}
