/*
SYNOPSIS
========

    void str_init(struct str *str);
    void str_free(struct str *str);
    int str_cmp(struct str *left, struct str *right);
    int str_cat(struct str *dest, struct str *part);
    int str_ins(struct str *dest, size_t index, struct str *part);
    int str_rpl(struct str *dest, size_t index, size_t len, struct str *part);
    int str_era(struct str *str, size_t index, size_t len);
    int str_sub(struct str *dest, struct str *src, size_t index, size_t len);
    int str_cpy(struct str *dest, struct str *src);
    int str_clone(struct str **dest, struct str *src);

    int str_cpyc(struct str *dest, const char *src);
    int str_catc(struct str *dest, const char *part);

COMPLEXITY
==========

    fn                  best        worst
    -----------------------------------------------
    str_init()          O(1)        O(1)
    str_free()          O(1)        O(1)
    str_cmp()           O(n)        O(n)
    str_cat()           O(n)        O(n)
    str_ins()           O(n)        O(n)
    str_rpl()           O(n)        O(n)
    str_era()           O(n)        O(n)
    str_sub()           O(n)        O(n)
    str_cpy()           O(n)        O(n)
    str_clone()         O(n)        O(n)

    str_cpyc()          O(n)        O(n)
    str_catc()          O(n)        O(n)
    -----------------------------------------------

    n is number of characters in string.

DESCRIPTION
===========

    str_init() and str_free() construct/destroy a string.

    str_cmp() compre two strings.

    str_cat() append part to dest string.

    str_ins() insert part string into index of dest string.

    str_rpl() replace part to dest at index by length.

    str_era() remove number of characters from index.

    str_sub() return new string with content start from index and length.

    str_copy() copy source to dest string.

    str_clone() create new string and copy content from source string.

    str_cpyc() cpy C string into string.

    str_catc() concats C string to dest.

RETURNS
=======

    On success, return 0. On failure, return -1.

    str_cmp() on two string is equal, return 0. Other case, return 1.

ERRORS
======

    CS106B_EINDEX               Invalid index of character in string
    SYS_ENOMEM                  System memory is full
*/

#ifndef _CS106B_STR_H_
#define _CS106B_STR_H_

#include <stdlib.h>

struct str
{
    char *data;
    size_t size;
    size_t max_size;
};

int str_init(struct str *str);
void str_free(struct str *str);
int str_cmp(struct str *left, struct str *right);
int str_cat(struct str *dest, struct str *part);
int str_ins(struct str *dest, size_t index, struct str *part);
int str_rpl(struct str *dest, size_t index, size_t len, struct str *part);
int str_era(struct str *str, size_t index, size_t len);
int str_sub(struct str *dest, struct str *src, size_t index, size_t len);
int str_cpy(struct str *dest, struct str *src);
int str_clone(struct str **dest, struct str *src);

int str_cpyc(struct str *dest, const char *src);
int str_catc(struct str *dest, const char *part);

#endif
