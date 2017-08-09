#include <cs106b/pset.h>

#include <string.h>
#include <stdio.h>

// store address in hex based, with "0x" leading and NULL terminal
#define PSET_PTRS_SIZE 2 * sizeof(void *) + 2 + 1

void _pset_p_to_str(void *p, char str[PSET_PTRS_SIZE]);

int pset_init(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
    return htable_init(&set->htable);
}

int pset_add(struct pset *set, void *p)
{
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);
    return htable_set(&set->htable, p_str, p);
}

int pset_del(struct pset *set, void *p)
{
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);
    return htable_del(&set->htable, p_str);
}

bool pset_exist(struct pset *set, void *p)
{
    char p_str[PSET_PTRS_SIZE];

    _pset_p_to_str(p, p_str);

    return htable_get(&set->htable, p_str) != NULL;
}

void *pset_next(struct pset *set)
{
    struct htable_item *item;

    if (set->iter_count == set->htable.size)
        return NULL;

    for (; set->iter < set->htable.max_size; set->iter++) {
        item = set->htable.items + set->iter;
        if (item->key != NULL) {
            set->iter += 1;
            set->iter_count += 1;
            return item->value;
        }
    }

    return NULL;
}

void pset_rsiter(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
}

void pset_free(struct pset *set)
{
    set->iter = 0;
    set->iter_count = 0;
    htable_free(&set->htable);
}

void _pset_p_to_str(void *p, char str[PSET_PTRS_SIZE])
{
    snprintf(str, PSET_PTRS_SIZE - 1, "%p", p);
    str[PSET_PTRS_SIZE - 1] = 0;
}
