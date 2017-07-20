/*
SYNOPSIS

    int str_init(struct string *str);
    int str_cpy(struct string *dest, struct string *src);
    int str_cmp(struct string *dest, struct string *src);
    int str_cat(struct string *dest, struct string *part);
    int str_ins(struct string *dest, size_t index, struct string *part);
    int str_rpl(struct string *dest, size_t index, size_t len,
                struct string *part);
    int str_erase(struct string *dest, size_t index, size_t len);
    struct string * str_sub(struct string *str, size_t index, size_t len);
    struct string * str_clone(const struct string *src);
    void str_free(struct string *str);

    int str_cpyc(struct string *dest, const char *src);
    int str_catc(struct string *dest, const char *part);

COMPLEXITY

    fn                  best        worst
    -----------------------------------------------
    str_init()          O(1)        O(1)
    str_cpy()           O(n)        O(n)
    str_cmp()           O(n)        O(n)
    str_cat()           O(n)        O(n)
    str_ins()           O(n)        O(n)
    str_rpl()           O(n)        O(n)
    str_erase()         O(n)        O(n)
    str_sub()           O(n)        O(n)
    str_clone()         O(n)        O(n)
    str_free()          O(1)        O(1)

    str_cpyc()          O(n)        O(n)
    str_catc()          O(n)        O(n)
    -----------------------------------------------

    n is number of characters in string.

DESCRIPTION

    str_init() construct empty string.

    str_cpy() copy content of source to dest string.

    str_cmp() compre two strings.

    str_cat() append to dest string with part.

    str_ins() insert part string into index of source string.

    str_rpl() replace part to dest at index by length.

    str_erase() remove number of characters from index.

    str_sub() return new string with content start from index and length.

    str_clone() create new string and copy content from source string.

    str_free() free memory which use by string.

    str_cpyc() cpy C string into string.

    str_catc() concats C string to dest.

RETURNS

    All of functions except str_free() and str_cmp() on success return 0,
    on failure return -1;

    str_cmp() on equal return 0, on different return 1.

    str_rpl() return -1 if length of part is less than len.

    str_clone() on success return pointer to new string, on error return NULL.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_STRING_H__
#define __CS106B_STRING_H__

#include <stdlib.h>

struct string
{
    char *data;
    size_t size;
    size_t max_size;
};

int str_init(struct string *str);
int str_cpy(struct string *dest, struct string *src);
int str_cmp(struct string *dest, struct string *src);
int str_cat(struct string *dest, struct string *part);
int str_ins(struct string *dest, size_t index, struct string *part);
int str_rpl(struct string *dest, size_t index, size_t len,
            struct string *part);
int str_erase(struct string *dest, size_t index, size_t len);
struct string * str_sub(struct string *str, size_t index, size_t len);
struct string * str_clone(const struct string *src);
void str_free(struct string *str);

int str_cpyc(struct string *dest, const char *src);
int str_catc(struct string *dest, const char *part);

#endif
