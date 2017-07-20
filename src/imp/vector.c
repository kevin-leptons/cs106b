#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/vector.h>

#define NUM_ITEM 8

int main(int argc, char *argv[])
{
    struct vector vector;
    size_t *data;
    size_t i;

    printf("init: vector\n");
    if (vector_init(&vector) != 0)
        exit_err("vector_init");

    for (i = 0; i < NUM_ITEM; i++) {
        data = malloc(sizeof(*data));
        *data = i;
        if (vector_add(&vector, data) != 0)
            exit_err("vector_add");
        printf("add: vector <= %lu, size=%lu\n", *data, vector.size);
    }

    if (vector_remove(&vector, 4) != 0)
        exit_err("vector_remove");
    printf("remove: vector[4]\n");

    data = malloc(sizeof(*data));
    *data  = 123;
    if (vector_insert(&vector, 0, data) != 0)
        exit_err("vector_insert");
    printf("insert: vector[0] = %lu\n", *data);

    for (i = 0; i < NUM_ITEM; i++) {
        data = (size_t*) vector_at(&vector, i);
        if (data == NULL)
            exit_err("vector_at");
        printf("at: vector[%lu] = %lu\n", i, *data);
    }

    vector_free(&vector, true);
    printf("free: vector\n");

    return EXIT_SUCCESS;
}
