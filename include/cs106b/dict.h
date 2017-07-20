/*
SYNOPSIS

    int dict_init(struct dict *dict, const char *file);
    int dict_add(struct dict *dict, const char *word);
    int dict_del(struct dict *dict, const char *word);
    int dict_exist(struct dict *dict, const char *word);
    void dict_free(struct dict *dict);

COMPLEXITY

    fn                  best                worst
    ----------------------------------------------------------
    dict_init()         htable_init()       htable_init() 
    dict_add()          htable_set()        htable_set()
    dict_del()          htable_del()        htable_del()
    dict_exist()        htable_get()        htable_get()
    dict_free()         htable_free()       htable_free()
    ----------------------------------------------------------

    n is number words in dictionary.

RETURNS

    dict_init(), dict_add(), dict_del() on success return 0, on failure
    return -1.

    dict_exist on word is exist return 1, on word is not exist return 0.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_DICT_H__
#define __CS106B_DICT_H__

#include <cs106b/htable.h>

struct dict
{
    struct htable map;
    char *data;
};

int dict_init(struct dict *dict, const char *file);
int dict_add(struct dict *dict, const char *word);
int dict_del(struct dict *dict, const char *word);
int dict_exist(struct dict *dict, const char *word);
void dict_free(struct dict *dict);

#endif
