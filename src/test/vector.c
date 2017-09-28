#include <stdlib.h>
#include <stdio.h>
#include <espace/error.h>
#include <espace/sys.h>
#include <cs106b/error.h>
#include <cs106b/vector.h>
#include <cs106b/mem.h>

#define NUM_ITEM 8

int main(int argc, char *argv[])
{
    struct vector vector;
    size_t *data;
    size_t i;

    printf("vector_init()\n");
    vector_init(&vector);

    // add items to vector
    for (i = 0; i < NUM_ITEM; i++) {
        if (cs106b_malloc((void *) &data, sizeof(*data)))
            goto error;
        *data = i;
        if (vector_add(&vector, data) != 0)
            goto error;
        printf("vector_add(%lu), size=%lu\n", *data, vector.size);
    }

    // resize vector
    if (vector_resize(&vector, 2 * NUM_ITEM))
        goto error;
    printf("vector_resize(%u), size=%zu\n", 2 * NUM_ITEM, vector.max_size);

    // remove item from vector
    if (vector_del(&vector, 4) != 0)
        goto error;
    printf("vector_del(4)\n");

    // insert item to vector
    if (cs106b_malloc((void *) &data, sizeof(*data)))
        goto error;
    *data  = 123;
    if (vector_put(&vector, 0, data) != 0)
        goto error;
    printf("vector_put(0, %zu)\n", *data);

    // get items from vector
    for (i = 0; i < vector.size; i++) {
        if (vector_get(&vector, i, (void *) &data))
            goto error;
        printf("vector_get(%zu) %lu\n", i, *data);
    }

    // free vector
    vector_free(&vector);
    printf("vector_free()\n");
    return EXIT_SUCCESS;

error:
    fprintf(stderr, "Error: %s\n", espace->id);
    return EXIT_FAILURE;
}
