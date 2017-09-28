#include <cs106b/set.h>
#include <string.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

void set_init(struct set *set)
{
    htab_init(&set->tab);
    vector_init(&set->keys);
    set->iter = 0;
}

void set_free(struct set *set)
{
    vector_free(&set->keys);
    htab_free(&set->tab);
    set->iter = 0;
}

int set_add(struct set *set, const char *key)
{
    if (htab_set(&set->tab, key, NULL))
        return -1;
    if (vector_add(&set->keys, (void*) key))
        return -1;
    return 0;
}

int set_del(struct set *set, const char *key)
{
    size_t i;
    char *vkey;

    if (htab_del(&set->tab, key))
        return -1;

    for (i = 0; i < set->keys.size; i++) {
        if (vector_get(&set->keys, i, (void *) &vkey))
            return -1;
        if (strcmp(vkey, key))
            continue;
        if (vector_del(&set->keys, i))
            return -1;
        return 0;
    }

    espace_raise(CS106B_EKEY);
    return -1;
}

int set_exist(struct set *set, const char *key)
{
    int is_exist;
    
    is_exist = htab_get(&set->tab, key, NULL) == 0;
    if (espace_check())
        espace_clear();
    return is_exist;
}

int set_next(struct set *set, const char **key)
{
    if (vector_get(&set->keys, set->iter, (void *) key))
        return -1;
    set->iter += 1;
    return 0;
}

void set_iter_reset(struct set *set)
{
    set->iter = 0;
}

int set_copy(struct set *dest, struct set *src)
{
    set_free(dest);
    if (htab_copy(&dest->tab, &src->tab))
        return -1;
    if (vector_copy(&dest->keys, &src->keys))
        return -1;
    dest->iter = src->iter;
    return 0;
}

int set_clone(struct set **dest, struct set *src)
{
    if (cs106b_malloc((void *) dest, sizeof(struct set)))
        return -1;
    set_init(*dest);

    if (set_copy(*dest, src)) {
        free(*dest);
        return -1;
    }
    return 0;
}
