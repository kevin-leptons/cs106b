#include <cs106b/svmap.h>
#include <string.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

static char * _vec_to_key(struct vector *vector);

void svmap_init(struct svmap *map)
{
    htab_init(&map->vtab);
    htab_init(&map->ktab);
    vector_init(&map->keys);
}

void svmap_free(struct svmap *map)
{
    vector_free(&map->keys);
    htab_free(&map->ktab);
    htab_free(&map->vtab);
}

int svmap_set(struct svmap *map, struct vector *key, void *value)
{
    char *skey;
    struct vector *inkey;
    size_t *key_index;
    int ret;

    ret = -1;
    skey = _vec_to_key(key);
    if (skey == NULL)
        goto finish;

    // store value
    if (htab_set(&map->vtab, skey, value))
        goto free_skey;

    // store key in vector
    if (vector_clone(&inkey, key))
        goto free_skey;
    if (vector_add(&map->keys, inkey))
        goto free_inkey;

    // map key -> vector index
    if (cs106b_malloc((void *) &key_index, sizeof(*key_index)))
        goto free_inkey;
    *key_index = map->keys.size - 1;
    if (htab_set(&map->ktab, skey, key_index))
        goto free_key_index;

    ret = 0;
    goto finish;

free_key_index:
    free(key_index);
free_inkey:
    free(inkey);
free_skey:
    free(skey);
finish:
    return ret;
}

int svmap_get(struct svmap *map, struct vector *key, void **value)
{
    char *skey;

    skey = _vec_to_key(key);
    if (skey == NULL) {
        espace_raise(CS106B_EKEY);
        return -1;
    }
    if (htab_get(&map->vtab, skey, value))
        return -1;

    free(skey);
    return 0;
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
    if (htab_del(&map->vtab, skey))
        return -1;

    // remove from vector keys
    if (htab_get(&map->ktab, skey, (void *) &key_index))
        return -1;
    if (vector_del(&map->keys, *key_index))
        return -1;

    // down each index to one
    i = *key_index + 1;
    free(key_index);
    free(skey);
    for (; i < map->keys.size; i++) {
        if (vector_get(&map->keys, i, (void *) &vkey))
            return -1;
        skey = _vec_to_key(vkey);
        if (skey == NULL) 
            return -1;
        if (htab_get(&map->ktab, skey, (void *) &key_index)) {
            free(skey);
            return -1;
        }
        *key_index -= 1;
    }

    return 0;
}

struct vector * svmap_keys(struct svmap *map)
{
    return &map->keys;
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
        if (vector_get(vector, i, (void *) &item))
            return NULL;
        key_size += strlen(item) + 1;
    }
    
    key = malloc(key_size);
    if (key == NULL)
        return NULL;

    p = key;
    for (i = 0; i < vector->size; i++) {
        if (vector_get(vector, i, (void *) &item))
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
