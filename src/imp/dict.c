#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/dict.h>

int main(int argc, char *argv[])
{
    struct dict d;

    if (argc != 2) {
        printf("Usage: dict file\n");
        printf("An dictionary file locate in asset/dict.txt\n");
        return EXIT_FAILURE;
    }

    if (dict_init(&d, argv[1]))
        exit_err("dict_init");
    printf("init: dict, size=%lu, max_size=%lu\n", d.map.size, d.map.max_size);

    if (dict_exist(&d, "hello"))
        printf("exist: dict, word=hello, yes\n");
    else
        exit_err("dict_exist");

    if (dict_exist(&d, "xyzk"))
        exit_err("dict_exist");
    else
        printf("exist: dict, word=xyzk, no\n");

    return EXIT_SUCCESS;
}
