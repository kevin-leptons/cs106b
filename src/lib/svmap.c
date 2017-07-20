#include <cs106b/svmap.h>

#include <string.h>

static char * _vec_to_key(struct vector *vector);

int svmap_init(struct svmap *map)
{
    if (htable_init(&map->hmap))
        return -1;
    if (vector_init(&map->keys))
        return -1;
    if (htable_init(&map->kmap))
        return -1;

    return 0;
}

int svmap_set(struct svmap *map, struct vector *key, void *value)
{
    char *skey;
    struct vector *inkey;
    size_t *key_index;

    inkey = NULL;
    key_index = NULL;

    skey = _vec_to_key(key);
    if (skey == NULL)
        return -1;

    // store value
    if (htable_set(&map->hmap, skey, value))
        goto ERROR;

    // store key in vector
    inkey = vector_clone(key);
    if (inkey == NULL)
        goto ERROR;
    if (vector_add(&map->keys, inkey))
        goto ERROR;

    // map key -> vector index
    key_index = malloc(sizeof(*key_index));
    if (key_index == NULL)
        goto ERROR;
    *key_index = map->keys.size - 1;
    if (htable_set(&map->kmap, skey, key_index))
        goto ERROR;

    return 0;

ERROR:
    if (skey != NULL)
        free(skey);
    if (inkey != NULL)
        free(inkey);
    if (key_index != NULL)
        free(key_index);

    return -1;
}

void * svmap_get(struct svmap *map, struct vector *key)
{
    char *skey;
    void *value;

    skey = _vec_to_key(key);
    if (skey == NULL)
        return NULL;

    value = htable_get(&map->hmap, skey);

    free(skey);
    return value;
}

int svmap_del(struct svmap *map, struct vector *key)
{
    char *skey;
    size_t *key_index;
    struct vector *vkey;
    size_t i;

    skey = _vec_to_key(key);
    if (skey == NULL)
        return -1;

    // remove from value table
    if (htable_del(&map->hmap, skey))
        goto ERROR;

    // remove from vector keys
    key_index = htable_get(&map->kmap, skey);
    if (key_index == NULL)
        goto ERROR;
    if (vector_remove(&map->keys, *key_index))
        goto ERROR;

    // down each index to one
    for (i = *key_index; i < map->keys.size; i++) {
        vkey = (struct vector *) vector_at(&map->keys, i);
        if (vkey == NULL)
            goto ERROR;
        key_index = htable_get(&map->kmap, (void *) vkey);
        *key_index -= 1;
    }

    free(skey);
    return 0;

ERROR:
    if (skey != NULL)
        free(skey);

    return -1;
}

struct vector * svmap_keys(struct svmap *map)
{
    return &map->keys;
}

void svmap_free(struct svmap *map)
{
    htable_free(&map->hmap);
}

static char * _vec_to_key(struct vector *vector)
{
    char *key;
    size_t key_size;
    size_t i;
    char *item;
    size_t item_size;
    char *p;

    if (vector->size == 0)
        return NULL;

    // key size is sum of len of string in vector
    // and space for each string, and terminal character 0 at end
    key_size = 0;
    for (i = 0; i < vector->size; i++) {
        item = (char *) vector_at(vector, i);
        if (item == NULL)
            return NULL;
        key_size += strlen(item) + 1;
    }
    
    key = malloc(key_size);
    if (key == NULL)
        return NULL;

    p = key;
    for (i = 0; i < vector->size; i++) {
        item = (char *) vector_at(vector, i);
        if (item == NULL)
            goto ERROR;
        item_size = strlen(item);
        memcpy(p, item, item_size);
        p[item_size] = ' ';
        p += item_size + 1;
    }
    key[key_size - 1] = 0;

    return key;

ERROR:
    if (key != NULL)
        free(key);
    return NULL;
}
