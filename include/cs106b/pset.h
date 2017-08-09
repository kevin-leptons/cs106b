#ifndef __CS106B_PSET_H__
#define __CS106B_PSET_H__

#include <stdlib.h>
#include <stdbool.h>

#include <cs106b/htable.h>

struct pset
{
    struct htable htable;
    size_t iter;
    size_t iter_count;
};

int pset_init(struct pset *set);
int pset_add(struct pset *set, void *p);
int pset_del(struct pset *set, void *p);
bool pset_exist(struct pset *set, void *p);
void *pset_next(struct pset *set);
void pset_rsiter(struct pset *set);
void pset_free(struct pset *set);

#endif
