#include <cs106b/pset.h>
#include <string.h>
#include <stdio.h>
#include <cs106b/error.h>

// store address in hex based, with "0x" leading and NULL terminal
#define PSET_PTRS_SIZE 2 * sizeof(void *) + 2 + 1

void _pset_p_to_str(void *p, char str[PSET_PTRS_SIZE]);

void pset_init(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
    htab_init(&set->tab);
}

void pset_free(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
    htab_free(&set->tab);
}

int pset_add(struct pset *set, void *p)
{
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);
    return htab_set(&set->tab, p_str, p);
}

int pset_del(struct pset *set, void *p)
{
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);
    return htab_del(&set->tab, p_str);
}

int pset_exist(struct pset *set, void *p)
{
    int is_exist;
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);
    is_exist = htab_get(&set->tab, p_str, NULL) == 0;
    espace_clear();
    return is_exist;
}

int pset_next(struct pset *set, void **p)
{
    struct htab_item *item;

    if (set->iter_count == set->tab.size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    for (; set->iter < set->tab.max_size; set->iter++) {
        item = set->tab.items + set->iter;
        if (item->key != NULL) {
            set->iter += 1;
            set->iter_count += 1;
            *p = item->value;
            return 0;
        }
    }

    espace_raise(CS106B_EINDEX);
    return -1;
}

void pset_ireset(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
}

void _pset_p_to_str(void *p, char str[PSET_PTRS_SIZE])
{
    snprintf(str, PSET_PTRS_SIZE - 1, "%p", p);
    str[PSET_PTRS_SIZE - 1] = 0;
}
