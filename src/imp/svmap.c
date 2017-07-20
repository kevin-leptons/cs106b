#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/svmap.h>
#include <cs106b/vector.h>

int main(int argc, char *argv[])
{
    struct svmap map;
    struct vector k1;
    struct vector k2;
    struct vector k3;
    char *item;

    // init vector k1
    if (vector_init(&k1))
        exit_err("vector_init");
    if (vector_add(&k1, &"k1-item-1"))
        exit_err("vector_add");
    if (vector_add(&k1, &"k1-item-2"))
        exit_err("vector_add");
    if (vector_add(&k1, &"k1-item-3"))
        exit_err("vector_add");

    // init vector k2
    if (vector_init(&k2))
        exit_err("vector_init");
    if (vector_add(&k2, &"k2-item-1"))
        exit_err("vector_add");
    if (vector_add(&k2, &"k2-item-2"))
        exit_err("vector_add");

    // init vector k3
    if (vector_init(&k3))
        exit_err("vector_init");
    if (vector_add(&k3, &"k3-item-1"))
        exit_err("vector_add");

    printf("init: vector k1 k2 k3\n");

    // init string vector map
    if (svmap_init(&map))
        exit_err("svmap_init");
    printf("init: svmap\n");

    // add item into map
    if (svmap_set(&map, &k1, "value of k1"))
        exit_err("svmap_set");
    printf("set: svmap, key=k1, value=value of k1\n");

    if (svmap_set(&map, &k2, "value of k2"))
        exit_err("svmap_set");
    printf("set: svmap, key=k2, value=value of k2\n");

    if (svmap_set(&map, &k3, "value of k3"))
        exit_err("svmap_set");
    printf("set: svmap, key=k3, value=value of k3\n");

    // retrieve item from map
    item = svmap_get(&map, &k1);
    if (item == NULL)
        exit_err("svmap_get");
    printf("get: svmap, key=k1, value=%s\n", item);

    item = svmap_get(&map, &k2);
    if (item == NULL)
        exit_err("svmap_get");
    printf("get: svmap, key=k2, value=%s\n", item);

    item = svmap_get(&map, &k3);
    if (item == NULL)
        exit_err("svmap_get");
    printf("get: svmap, key=k3, value=%s\n", item);

    return EXIT_SUCCESS;
}
