#include <stdlib.h>
#include <stdio.h>
#include <cs106b/error.h>
#include <cs106b/set.h>
#include <cs106b/fset.h>

int main(int argc, char *argv[])
{
    struct set set;
    int ret;

    ret = EXIT_FAILURE;
    if (argc != 2) {
        printf("USAGE\n");
        printf("    fset file\n\n");
        printf("ARGUMENT\n");
        printf("    file    File contains world\n");
        printf("            An sample file locate in asset/dict.txt\n");
        return EXIT_FAILURE;
    }

    // load words in file into set
    set_init(&set);
    if (fset_load(&set, argv[1]))
        goto free_set;
    printf("fset_load(set, '%s') size=%zu, max_size=%zu\n",
           argv[1], set.tab.size, set.tab.max_size);

    // check word is exist or not
    if (set_exist(&set, "hello"))
        printf("set_exist(set, 'hello') yes\n");
    else
        goto free_set;
    if (set_exist(&set, "joke"))
        printf("set_exist(set, 'joke') yes\n");
    else
        goto free_set;
    if (set_exist(&set, "xyzk"))
        goto free_set;
    else
        printf("set_exist('xyzk') no\n");

    ret = EXIT_SUCCESS;

free_set:
    set_free(&set);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}
