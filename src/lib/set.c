#include <cs106b/set.h>

int set_init(struct set *set)
{
    return htable_init(&set->map);
}

int set_add(struct set *set, const char *item)
{
    return htable_set(&set->map, item, set);
}

int set_del(struct set *set, const char *item)
{
    return htable_del(&set->map, item);
}

int set_exist(struct set *set, const char *item)
{
    return htable_get(&set->map, item) == set;
}

void set_free(struct set *set)
{
    htable_free(&set->map);
}
