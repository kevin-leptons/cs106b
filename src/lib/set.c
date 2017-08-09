#include <cs106b/set.h>

#include <string.h>

int set_init(struct set *set)
{
    if (htable_init(&set->map))
        return -1;
    if (vector_init(&set->keys))
        return -1;
    set->iter = 0;

    return 0;
}

int set_add(struct set *set, const char *item)
{
    if (htable_set(&set->map, item, set))
        return -1;
    if (vector_add(&set->keys, (void*) item))
        return -1;

    return 0;
}

int set_del(struct set *set, const char *item)
{
    size_t i;
    char *key;

    if (htable_del(&set->map, item))
        return -1;

    for (i = 0; i < set->keys.size; i++) {
        key = vector_at(&set->keys, i);
        if (key == NULL)
            return -1;
        if (strcmp(key, item) != 0)
            continue;
        if (vector_remove(&set->keys, i))
            return -1;
        return 0;
    }

    return -1;
}

int set_exist(struct set *set, const char *item)
{
    return htable_get(&set->map, item) == set;
}

void *set_next(struct set *set)
{
    void *key;

    key = vector_at(&set->keys, set->iter);
    if (key == NULL)
        return NULL;

    set->iter += 1;

    return key;
}

int set_iter_reset(struct set *set)
{
    set->iter = 0;
    return 0;
}

void set_free(struct set *set)
{
    htable_free(&set->map);
}
