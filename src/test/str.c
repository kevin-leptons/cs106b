#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/str.h>

void dump_pairs(struct str *s1, struct str *s2);

int main(int argc, char *argv[])
{
    struct str s1;
    struct str s2;
    struct str s3;
    int ret;

    ret = EXIT_FAILURE;

    if (str_init(&s1))
        goto finish;
    printf("str_init(s1)\n");
    if (str_init(&s2))
        goto free_s1;
    printf("str_init(s2)\n");

    printf("str_cpyc(s1, 'hello world')\n");
    if (str_cpyc(&s1, "hello world"))
        goto free_s2;
    printf("s1: %s\n", s1.data);

    printf("str_cpy(s2, s1)\n");
    if (str_cpy(&s2, &s1))
        goto free_s2;
    printf("s2: %s\n", s2.data);

    dump_pairs(&s1, &s2);
    if (str_cmp(&s1, &s2) == 0)
        printf("str_cmp(s1, s2) equal\n");
    else
        goto free_s2;

    if (str_cpyc(&s2, "hi world"))
        goto free_s2;
    dump_pairs(&s1, &s2);
    if (str_cmp(&s1, &s2))
        printf("str_cmp(s1, s2)not equal\n");
    else
        goto free_s2;

    str_free(&s1);
    printf("str_free(s1)\n");

    dump_pairs(&s1, &s2);
    if (str_cat(&s1, &s2))
        goto free_s2;
    printf("str_cat(s1, s2)\n");
    printf("s1: %s\n", s1.data);

    if (str_cpyc(&s2, ","))
        goto free_s2;
    dump_pairs(&s1, &s2);
    if (str_ins(&s1, 2, &s2))
        goto free_s2;
    printf("str_ins(s1, 2, s2)\n");
    printf("s1: %s\n", s1.data);

    if (str_cpyc(&s2, "earth"))
        goto free_s2;
    dump_pairs(&s1, &s2);
    if (str_rpl(&s1, 4, 5, &s2))
        goto free_s2;
    printf("str_rpl(s1, 4, 5, s2)\n");
    printf("s1: %s\n", s1.data);

    printf("\ns1: %s\n", s1.data);
    if (str_era(&s1, 2, 1))
        goto free_s2;
    printf("str_erase(s1, 2, 1)\n");
    printf("s1: %s\n", s1.data);

    if (str_init(&s3))
        goto free_s2;
    printf("\nstr_init(s3)\n");
    printf("s1: '%s'\n", s1.data);
    printf("s3: '%s'\n", s3.data);
    if (str_sub(&s3, &s1, 3, 5))
        goto free_s3;
    printf("str_sub(s3, s1, 3, 5)\n");
    printf("s3: %s\n", s3.data);

    ret = EXIT_SUCCESS;

free_s3:
    str_free(&s3);
free_s2:
    str_free(&s2);
free_s1:
    str_free(&s1);
finish:
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}

void dump_pairs(struct str *s1, struct str *s2)
{
    printf("\ns1: '%s', size=%lu, max_size=%lu\n", 
            s1->data, s1->size, s1->max_size);
    printf("s2: '%s', size=%lu, max_size=%lu\n", 
            s2->data, s2->size, s2->max_size);
}
