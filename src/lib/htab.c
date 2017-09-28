/*
DESCRIPTION
===========

    Hash string algorithm: Murmur version 3, string input, 32bits output.

    Collision algorithm: Open addressing - Quadratic probing with alternating
    sign.
*/

#include <cs106b/htab.h>

#include <string.h>
#include <inttypes.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define HTABLE_BSIZE 8
#define HTAB_ESIZE 2
#define LOAD_FACTOR_TOP 1 / 2.0f
#define LOAD_FACTOR_BOT LOAD_FACTOR_TOP / 2
#define HTAB_MOD_N 4
#define HTAB_MOD_B 3
#define HTAB_MOD_KE 2
#define HTAB_MOD_KB 2 * HTAB_MOD_KE

static uint32_t _murmur3_32(const uint8_t* key, size_t len, uint32_t seed);
static size_t _hash_str(const char *str, size_t mask);
static size_t _index_key(const char *str, size_t mask);
static int _htab_resize_modk(struct htab *table, size_t mod_k);
static int _htab_extend(struct htab *table);
static int _htab_narrow(struct htab *table);
static struct htab_item *
_htab_lookup(struct htab_item *items, size_t max_size, const char *key,
             size_t index);

void htab_init(struct htab *table)
{
    table->size = 0;
    table->mask = 0;
    table->max_size = 0;
    table->items = NULL;
}

void htab_free(struct htab *table)
{
    if (table->items != NULL) {
        free(table->items);
        table->items = NULL;
    }
    table->size = 0;
    table->mask = 0;
    table->max_size = 0;
}

int htab_resize(struct htab *table, size_t max_size)
{
    size_t mod_k;

    mod_k = (max_size - HTAB_MOD_B) / HTAB_MOD_N;
    if ((max_size - HTAB_MOD_B) % HTAB_MOD_N != 0)
        mod_k += 1;

    return _htab_resize_modk(table, mod_k);
}

int htab_set(struct htab *table, const char *key, void *value)
{
    size_t index;
    struct htab_item *item;
    size_t key_len;

    if (_htab_extend(table))
        return -1;

    index = _index_key(key, table->mask);
    item = _htab_lookup(table->items, table->max_size, key, index);
    if (item == NULL)
        return -1;

    if (item->key == NULL) {
        // create new key-value pair
        key_len = strlen(key);
        item->key = malloc(key_len + 1);
        if (item->key == NULL)
            return -1;
        memcpy(item->key, key, key_len);
        item->key[key_len] = 0;
        item->value = (void *) value;
        table->size += 1;
    }
    else {
        // override value of key
        item->value = (void *) value;
    }

    return 0;
}

int htab_get(struct htab *table, const char *key, void **value)
{
    size_t index;
    struct htab_item *item;

    if (table->mask == 0) {
        espace_raise(CS106B_EKEY);
        return -1;
    }

    index = _index_key(key, table->mask);
    item = _htab_lookup(table->items, table->max_size, key, index);
    if (item == NULL || item->key == NULL) {
        espace_raise(CS106B_EKEY);
        return -1;
    }

    if (value != NULL)
        *value =  item->value;
    return 0;
}

int htab_del(struct htab *table, const char *key)
{
    size_t index;
    struct htab_item *item;

    if (table->mask == 0)
        return -1;

    index = _index_key(key, table->mask);
    item = _htab_lookup(table->items, table->max_size, key, index);
    if (item == NULL || item->key == NULL)
        return -1;
    free(item->key);
    item->key = NULL;

    if (_htab_narrow(table))
        return -1;

    table->size -= 1;
    return 0;
}

int htab_clear(struct htab *table)
{
    if (table->items != NULL)
        memset(table->items, 0, table->max_size * sizeof(*table->items));
    table->size = 0;

    return 0;
}

int htab_copy(struct htab *dest, struct htab  *src)
{
    size_t mem_size;

    htab_free(dest);
    mem_size = src->max_size * sizeof(struct htab_item);
    if (cs106b_malloc((void *) &dest->items, mem_size))
        return -1;
    memcpy(dest->items, src->items, mem_size);

    dest->max_size = src->max_size;
    dest->mask = src->mask;
    dest->size = src->size;
    return 0;
}

int htab_clone(struct htab **dest, struct htab  *src)
{
    if (cs106b_malloc((void*) dest, sizeof(struct htab)))
        return -1;
    htab_init(*dest);

    if (htab_copy(*dest, src)) {
        free(*dest);
        return -1;
    }
    return 0;
}

static int _htab_resize_modk(struct htab *table, size_t mod_k)
{
    struct htab_item *new_items;
    struct htab_item *old_item;
    struct htab_item *new_item;
    size_t new_msize;
    size_t new_dsize;
    size_t new_mask;
    size_t index;
    size_t to_copy;
    size_t copy_count;
    size_t i;

    new_msize = mod_k * HTAB_MOD_N + HTAB_MOD_B;
    new_mask = new_msize - 1;
    new_dsize = new_msize * sizeof(*new_items);
    new_items = malloc(new_dsize);
    if (new_items == NULL)
        return -1;
    memset(new_items, 0, new_dsize);
    to_copy = new_msize < table->size ? new_msize : table->size;

    // hash old items into new memory block
    copy_count = 0;
    for (i = 0; i < table->max_size; i++) {
        // break condition
        if (copy_count == to_copy)
            break;

        // skip for empty slot
        old_item = table->items + i;
        if (old_item->key == NULL)
            continue;

        // find slot
        index = _index_key(old_item->key, new_mask);
        new_item = _htab_lookup(new_items, new_msize, old_item->key, index);
        if (new_item == NULL)
            goto ERROR;

        // copy data
        *new_item = *old_item;

        copy_count += 1;
    }

    if (table->items != NULL)
        free(table->items);
    table->items = new_items;
    table->max_size = new_msize;
    table->mask = new_mask;

    return 0;

ERROR:
    if (new_items != NULL)
        free(new_items);
    return -1;
}

static int _htab_extend(struct htab *table)
{
    double load_factor;
    size_t mod_k;

    load_factor = (table->size + 1) / (double) table->max_size;
    if (load_factor < LOAD_FACTOR_TOP)
        return 0;

    mod_k = (table->max_size - HTAB_MOD_B) / HTAB_MOD_N;
    mod_k += 1;

    return _htab_resize_modk(table, mod_k);
}

static int _htab_narrow(struct htab *table)
{
    double load_factor;
    size_t mod_k;

    load_factor = table->size / table->max_size;
    if (load_factor < LOAD_FACTOR_BOT)
        return 0;

    mod_k = (table->max_size - HTAB_MOD_B) / HTAB_MOD_N;
    mod_k -= 1;

    return _htab_resize_modk(table, mod_k);
}

static struct htab_item *
_htab_lookup(struct htab_item *items, size_t max_size, const char *key,
               size_t index)
{
    struct htab_item *item;
    size_t new_index;
    size_t i;

    for (i = 0; i < max_size; i++) {
        if (i % 2 == 0)
            new_index = (index + i * i) % max_size;
        else
            new_index = (index - i * i) % max_size;
        item = items + new_index;
        if (item->key == NULL)
            return item;
        else if (strcmp(item->key, key) == 0)
            return item;
    }

    return NULL;
}

uint32_t _murmur3_32(const uint8_t* key, size_t len, uint32_t seed)
{
    uint32_t h;

    h = seed;

    if (len > 3) {
        const uint32_t* key_x4 = (const uint32_t*) key;
        size_t i = len >> 2;
        do {
            uint32_t k = *key_x4++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = (h * 5) + 0xe6546b64;
        } while (--i);
        key = (const uint8_t*) key_x4;
    }

    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        key = &key[i - 1];
        do {
            k <<= 8;
            k |= *key--;
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }

    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

static size_t _hash_str(const char *str, size_t mask)
{
    return _murmur3_32((uint8_t *) str, strlen(str), mask);
}

static size_t _index_key(const char *str, size_t mask)
{
    return _hash_str(str, mask) % mask;
}
