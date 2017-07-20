#include <stdlib.h>
#include <stdio.h>

#include <cs106b/queue.h>
#include <cs106b/error.h>

#define ITEM_NUM 8

int main(int argc, char *argv[])
{
    struct queue queue;
    int *data;
    int i;

    printf("init: queue\n");
    if (queue_init(&queue) != 0)
        exit_err("queue_init");

    for (i = 0; i < ITEM_NUM; i++) {
        data = malloc(sizeof(*data));
        *data = i;
        if (queue_push(&queue, data) != 0)
            exit_err("queue_push");
        printf("push: %i => queue, size=%lu\n", i, queue.size);
    }

    for (data = queue_pop(&queue); data != NULL; data = queue_pop(&queue)) {
        printf("pop: queue => %i, size=%lu\n", *data, queue.size);
        free(data);
    }

    queue_free(&queue, true);
    return EXIT_SUCCESS;
}
