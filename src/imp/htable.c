#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/htable.h>

int main(int argc, char *argv[])
{
    struct htable table;
    char *value;

    if (htable_init(&table))
        exit_err("htable_init");
    printf("init: htable\n");


    if (htable_set(&table, "a", &"hello friend"))
        exit_err("htable_set");
    printf("set: htable, key=a, value=hello friend\n");

    if (htable_set(&table, "b", &"hi, how are you?"))
        exit_err("htable_set");
    printf("set: htable, key=b, value=hi, how are you\n");

    if (htable_set(&table, "z", &"I am good"))
        exit_err("htable_set");
    printf("set: htable, key=z, value=I am good\n");

    value = htable_get(&table, "a");
    if (value == NULL)
        exit_err("htable_get");
    printf("get: htable, key=a, value=%s\n", (char*) value);

    value = htable_get(&table, "b");
    if (value == NULL)
        exit_err("htable_get");
    printf("get: htable, key=b, value=%s\n", (char*) value);

    value = htable_get(&table, "z");
    if (value == NULL)
        exit_err("htable_get");
    printf("get: htable, key=z, value=%s\n", (char*) value);

    if (htable_del(&table, "b"))
        exit_err("htable_del");
    printf("del: htable, key=b\n");
    if (htable_get(&table, "b") == NULL)
        printf("key=b was removed\n");
    else
        exit_err("htable_del\n");

    value = htable_get(&table, "z");
    if (value == NULL)
        exit_err("htable_get");
    printf("get: htable, key=z, value=%s\n", value);

    if (htable_set(&table, "z", "new value"))
        exit_err("htable_set");
    printf("set: htable, key=z, value=new value\n");

    value = htable_get(&table, "z");
    if (value == NULL)
        exit_err("htable_get");
    printf("get: htable, key=z, value=%s, size=%lu\n", value, table.size);


    htable_free(&table);
    return EXIT_SUCCESS;
}
