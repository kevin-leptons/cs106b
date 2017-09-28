#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/set.h>

#define KEYS_NUM 4
const char * const KEYS[KEYS_NUM] = {"key 1", "key 2", "key 3", "key 4"};

int main(int argc, char *argv[])
{
    struct set s1;
    struct set s2;
    struct set *s3;
    char *key;
    size_t i;
    int ret;

    ret = EXIT_FAILURE;
    set_init(&s1);
    printf("set_init(s1)\n");

    // add keys to set
    for (i = 0; i < KEYS_NUM; i++) {
        if (set_add(&s1, KEYS[i]))
            goto free_s1;
        printf("set_add(s1, '%s')\n", KEYS[i]);
    }

    // interate keys in set
    for (;;) {
        if (set_next(&s1, (void *) &key)) {
            if (espace_catch(CS106B_EINDEX))
                break;
            else
                goto free_s1;
        }
        printf("set_next(s1) = '%s'\n", key);
    }

    // check key in set
    if (set_exist(&s1, KEYS[0]))
        printf("set_exist(s1, '%s') = yes\n", KEYS[0]);
    else
        goto free_s1;
    if (set_exist(&s1, "abcdef"))
        goto free_s1;
    else
        printf("set_exist(s1, 'abcdef') = no\n");

    // remove keys in set
    if (set_del(&s1, KEYS[1]))
        goto free_s1;
    printf("set_del(s1, '%s')\n", KEYS[1]);
    if (set_exist(&s1, KEYS[1]))
        goto free_s1;
    else
        printf("set_exist(s1, '%s') = no\n", KEYS[1]);

    // copy set
    set_init(&s2);
    if (set_copy(&s2, &s1))
        goto free_s1;
    printf("set_copy(s2, s1)\n");
    if (set_exist(&s2, KEYS[2]))
        printf("set_exist(s2, '%s') = yes\n", KEYS[2]);
    else
        goto free_s1;

    // clone set
    if (set_clone(&s3, &s1))
        goto free_s2;
    printf("set_clone(s3, s1)\n");
    if (set_exist(s3, KEYS[2]))
        printf("set_exist(s3, '%s') = yes\n", KEYS[2]);
    else
        goto free_s3;

    ret = EXIT_SUCCESS;

free_s3:
    set_free(s3);
free_s2:
    set_free(&s2);
free_s1:
    set_free(&s1);
    return ret;
}
