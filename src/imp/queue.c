#include <stdlib.h>
#include <stdio.h>
#include <cs106b/queue.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct queue q1;
    struct queue q2;
    struct queue *q3;
    size_t *value;
    size_t i;
    int ret;

    ret = EXIT_FAILURE;
    printf("queue_init(q1)\n");
    queue_init(&q1);

    // push items to queue
    for (i = 0; i < ITEM_NUM; i++) {
        if (cs106b_malloc((void *) &value, sizeof(*value)))
            goto free_q1;
        *value = i;
        if (queue_push(&q1, value))
            goto free_q1;
        printf("queue_push(q1, %zu)\n", *value);
    }

    // pop items from queue
    for (; q1.size > ITEM_NUM - 2;) {
        if (queue_pop(&q1, (void *) &value))
            goto free_q1;
        printf("queue_pop(q1) = %zu\n", *value);
        free(value);
    }

    // copy queue
    queue_init(&q2);
    if (queue_copy(&q2, &q1))
        goto free_q2;
    printf("queue_copy(q2, q1)\n");
    if (queue_pop(&q2, (void *) &value))
        goto free_q2;
    printf("queue_pop(q2) = %zu\n", *value);

    // clone queue
    if (queue_clone(&q3, &q1))
        goto free_q2;
    printf("queue_clone(q3, q1)\n");
    if (queue_pop(q3, (void *) &value))
        goto free_q3;
    printf("queue_pop(q3) = %zu\n", *value);

    ret = EXIT_SUCCESS;

free_q3:
    queue_free(q3);
free_q2:
    queue_free(&q2);
free_q1:
    queue_free(&q1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
