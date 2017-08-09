/*
NAME

    ihtable - integer hash table.

SYNOPSIS
DESCRIPTION

    Hash table with key is integer number.

RETURNS
AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS016B_IHTABLE_H__
#define __CS016B_IHTABLE_H__

#include <stdlib.h>

#include <cs106b/htable.h>

struct ihtable
{
    struct htable htable;
};

int ihtable_init(struct ihtable *table);
int ihtable_set(struct ihtable *table, size_t key, void *value);
void * ihtable_get(struct ihtable *table, size_t key);
int ihtable_del(struct ihtable *table, size_t key);
size_t ihtable_size(struct ihtable *table);
void ihtable_free(struct ihtable *table);

#endif
