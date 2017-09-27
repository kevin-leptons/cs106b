#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/htab.h>

int main(int argc, char *argv[])
{
    struct htab t1;
    struct htab t2;
    struct htab *t3;
    char *value;
    int ret;

    ret = EXIT_FAILURE;

    htab_init(&t1);
    printf("htab_init(t1)\n");

    if (htab_set(&t1, "a", "hello friend"))
        goto free_t1;
    printf("htab_set('a', 'hello friend')\n");

    if (htab_set(&t1, "b", "hi, how are you?"))
        goto free_t1;
    printf("htab_set('b', 'hi, how are you?')\n");

    if (htab_set(&t1, "z", "I am good"))
        goto free_t1;
    printf("htab_set('z', 'I am good')\n");

    if (htab_get(&t1, "a", (void *) &value))
        goto free_t1;
    printf("htab_get('a') = '%s'\n", value);

    if (htab_get(&t1, "b", (void *) &value))
        goto free_t1;
    printf("htab_get('b') = '%s'\n", value);

    if (htab_get(&t1, "z", (void *) &value))
        goto free_t1;
    printf("htab_get('z') = '%s'\n", value);

    if (htab_del(&t1, "b"))
        goto free_t1;
    printf("htab_del('b')");
    if (htab_get(&t1, "b", (void *) &value)) {
        espace_clear();
        printf(", key 'b' was removed\n");
    } else {
        goto free_t1;
    }

    if (htab_get(&t1, "z", (void *) &value))
        goto free_t1;
    printf("htab_get('z') = '%s'\n", (char *) value);

    if (htab_set(&t1, "z", "new value"))
        goto free_t1;
    printf("htab_set('z', 'new value')\n");

    if (htab_get(&t1, "z", (void *) &value))
        goto free_t1;
    printf("htab_get('z') = '%s', size=%zu\n", value, t1.size);

    htab_init(&t2);
    printf("htab_init(t2)\n");
    if (htab_copy(&t2, &t1))
        goto free_t2;
    if (htab_get(&t2, "a", (void *) &value))
        goto free_t2;
    printf("htab_get(t2, 'a') = '%s'\n", value);

    if (htab_clone(&t3, &t1))
        goto free_t2;
    if (htab_get(t3, "z", (void *) &value))
        goto free_t3;
    printf("htab_get(t3, 'z') = '%s'\n", value);

    ret = EXIT_SUCCESS;

free_t3:
    htab_free(t3);
free_t2:
    htab_free(&t2);
free_t1:
    htab_free(&t1);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
