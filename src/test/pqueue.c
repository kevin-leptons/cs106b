#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/pqueue.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct pqueue q1;
    struct pqueue q2;
    struct pqueue *q3;
    size_t items[ITEM_NUM];
    size_t *item;
    size_t i;
    size_t prio;
    int ret;

    ret = EXIT_FAILURE;
    pqueue_init(&q1, PQUEUE_INC);
    printf("pqueue_init(q1, PQUEUE_INC)\n");
    pqueue_init(&q2, PQUEUE_INC);
    printf("pqueue_init(q2, PQUEUE_INC)\n");

    // push items to queue
    for (i = 0; i < ITEM_NUM; i++) {
        items[i] = 2 * ITEM_NUM - i;
        if (pqueue_push(&q1, items + i, items[i]))
            goto free_q1;
        printf("pqueue_push(q1, %zu, %zu)\n", items[i], items[i]);
    }

    // pop items from queue
    for (; q1.size > 4;) {
        if (pqueue_tpri(&q1, &prio))
            goto free_q1;
        if (pqueue_pop(&q1, (void *) &item))
            goto free_q1;
        printf("pqueue_pop(q1) priority=%zu, value=%zu\n", prio, *item);
    }

    // copy queue
    if (pqueue_copy(&q2, &q1))
        goto free_q1;
    printf("pqueue_copy(q2, q1)\n");
    if (pqueue_top(&q2, (void *) &item))
        goto free_q2;
    printf("pqueue_top(q2), value=%zu\n", *item);

    // clone queue
    if (pqueue_clone(&q3, &q1))
        goto free_q2;
    printf("pqueue_clone(q3, q1)\n");
    if (pqueue_top(q3, (void *) &item))
        goto free_q2;
    printf("pqueue_top(q3), value=%zu\n", *item);

    ret = EXIT_SUCCESS;
    pqueue_free(q3);
    free(q3);

free_q2:
    pqueue_free(&q2);
free_q1:
    pqueue_free(&q1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
