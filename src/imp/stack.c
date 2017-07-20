#include <stdlib.h>
#include <stdio.h>

#include <cs106b/stack.h>
#include <cs106b/error.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct stack s;
    int *d;
    size_t i;

    printf("init: stack\n");
    if (stack_init(&s) != 0)
        exit_err("stack_init");

    for (i = 0; i < ITEM_NUM; i++) {
        d = malloc(sizeof(*d));
        if (d == NULL)
            exit_err("malloc");
        *d = i;
        if (stack_push(&s, d) != 0)
            exit_err("stack_push");
        printf("push: stack <= %lu, size=%lu\n", i, s.size);
    }

    for (d = stack_pop(&s); d != NULL; d = stack_pop(&s)) {
        printf("pop: stack => %i, size=%lu\n", *(int*)d, s.size);
        free(d);
    }

    stack_free(&s, true);
    return EXIT_SUCCESS;
}
