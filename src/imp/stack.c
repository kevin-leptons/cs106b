#include <stdlib.h>
#include <stdio.h>

#include <cs106b/stack.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct stack s1;
    struct stack s2;
    struct stack *s3;
    size_t *value;
    size_t i;
    int ret;

    ret = EXIT_FAILURE;
    printf("stack_init()\n");
    stack_init(&s1);

    // push items to stack
    for (i = 0; i < ITEM_NUM; i++) {
        if (cs106b_malloc((void *) &value, sizeof(*value)))
            goto free_s1;
        *value = i;
        if (stack_push(&s1, value))
            goto free_s1;
        printf("stack_push(%zu)\n", *value);
    }

    // pop items from stack
    for (; s1.size > ITEM_NUM - 2; ) {
        if (stack_pop(&s1, (void *) &value))
            goto free_s1;
        printf("stack_pop() = %zu\n", *value);
        free(value);
    }

    // copy stack
    stack_init(&s2);
    if (stack_copy(&s2, &s1))
        goto free_s1;
    printf("stack_copy(s2, s1)\n");
    if (stack_pop(&s2, (void *) &value))
        goto free_s2;
    printf("stack_pop(s2) = %zu\n", *value);

    // clone stack
    if (stack_clone(&s3, &s1))
        goto free_s2;
    printf("stack_clone(s3, s1)\n");
    if (stack_pop(s3, (void *) &value))
        goto free_s3;
    printf("stack_pop(s3) = %zu\n", *value);

    ret = EXIT_SUCCESS;

free_s3:
    stack_free(s3);
free_s2:
    stack_free(&s2);
free_s1:
    stack_free(&s1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
