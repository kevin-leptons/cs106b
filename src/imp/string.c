#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/string.h>

void dump_pairs(struct string *s1, struct string *s2);

int main(int argc, char *argv[])
{
    struct string s1;
    struct string s2;
    struct string *s3;

    printf("str_init: s1, s2\n");
    if (str_init(&s1))
        exit_err("str_init:s1");
    if (str_init(&s2))
        exit_err("str_init:s2");

    printf("str_cpyc: s1 <= hello world\n");
    if (str_cpyc(&s1, "hello world"))
        exit_err("str_cpyc:s1");
    printf("s1: %s\n", s1.data);

    printf("str_cpy: s2 <= s1\n");
    if (str_cpy(&s2, &s1))
        exit_err("str_cpy:s2");
    printf("s2: %s\n", s2.data);

    dump_pairs(&s1, &s2);
    if (str_cmp(&s1, &s2) == 0)
        printf("str_cmp: s1, s2 equal\n");

    if (str_cpyc(&s2, "hi world"))
        exit_err("str_cpyc");
    dump_pairs(&s1, &s2);
    if (str_cmp(&s1, &s2))
        printf("str_cmp: s1, s2 not equal\n");

    str_free(&s1);
    printf("str_free: s1\n");

    dump_pairs(&s1, &s2);
    if (str_cat(&s1, &s2))
        exit_err("str_cat");
    printf("str_cat: s1 <= s1 + s2\n");
    printf("s1: %s\n", s1.data);

    if (str_cpyc(&s2, "d worl"))
        exit_err("str_cpyc");
    dump_pairs(&s1, &s2);
    if (str_ins(&s1, s1.size - 1, &s2))
        exit_err("str_ins");
    printf("str_ins: s1 at end with s2\n");
    printf("s1: %s\n", s1.data);

    if (str_cpyc(&s2, "earth"))
        exit_err("str_cpyc");
    dump_pairs(&s1, &s2);
    if (str_rpl(&s1, 3, 5, &s2))
        exit_err("str_rpl");
    printf("str_rpl: s1 at 3 with 5 characters by s2\n");
    printf("s1: %s\n", s1.data);

    dump_pairs(&s1, &s2);
    if (str_erase(&s1, 3, 6))
        exit_err("str_erase");
    printf("str_erase: s1 at 3 with 6 characres\n");
    printf("s1: %s\n", s1.data);

    dump_pairs(&s1, &s2);
    s3 = str_sub(&s1, 3, 5);
    if (s3 == NULL)
        exit_err("str_sub");
    printf("str_sub: s3 <= s1 at 3 with 5 characters\n");
    printf("s3: %s\n", s3->data);

    return EXIT_SUCCESS;
}

void dump_pairs(struct string *s1, struct string *s2)
{
    printf("\ns1: %s, size=%lu, max_size=%lu\n", 
            s1->data, s1->size, s1->max_size);
    printf("s2: %s, size=%lu, max_size=%lu\n", 
            s2->data, s2->size, s2->max_size);
}
