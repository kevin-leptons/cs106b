#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/set.h>

int main(int argc, char *argv[])
{
    struct set set;
    char *data;

    if (set_init(&set))
        exit_err("set_init");
    printf("init: set\n");

    if (set_add(&set, "item 1"))
        exit_err("set_add");
    printf("add: set, item=item 1\n");
    if (set_add(&set, "item 2"))
        exit_err("set_add");
    printf("add: set, item=item 2\n");

    printf("start iter\n");
    for (;;) {
        data = set_next(&set);
        if (data == NULL)
            break;
        printf("%s\n", data);
    }

    if (set_exist(&set, "item 1"))
        printf("exist: set, item=item 1, yes\n");
    else
        exit_err("set_exist");

    if (set_del(&set, "item 2"))
        exit_err("set_del");
    printf("del: set, item=item 2\n");

    if (!set_exist(&set, "item 2"))
        printf("exist: set, item=item 2, no\n");
    else
        exit_err("set_exist");


    return EXIT_SUCCESS;

}
