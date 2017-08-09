#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/pqueue.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct pqueue q;
    size_t items[ITEM_NUM];
    size_t *item;
    size_t i;
    size_t prio;

    if (pqueue_init(&q, PQUEUE_SMALLEST))
        exit_err("pqueue_init");

    for (i = 0; i < ITEM_NUM; i++) {
        items[i] = i;
        if (pqueue_push(&q, items + i, items[i]))
            exit_err("pqueue_push");
    }

    for (; q.size > 0;) {
        prio = pqueue_tpri(&q);
        item = pqueue_pop(&q);
        if (item == NULL)
            exit_err("pqueue_pop");
        printf("priority=%zu, value=%zu, size=%zu\n", prio, *item, q.size);
    }

    pqueue_free(&q);
    return EXIT_SUCCESS;
}
