#include <cs106b/htable.h>

#include <string.h>
#include <inttypes.h>

#define HTABLE_BSIZE 8
#define HTABLE_ESIZE 2
#define LOAD_FACTOR_BORDER 1 / 2.0f

static uint32_t _murmur3_32(const uint8_t* key, size_t len, uint32_t seed);
static size_t _hash_str(const char *str, size_t mask);
static size_t _index_key(const char *str, size_t mask);
static int _htable_resize(struct htable *table, size_t max_size);
static struct htable_item * _htable_lookup(struct htable *table,
                                          const char *key, size_t index);

int htable_init(struct htable *table)
{
    table->size = 0;
    table->mask = 0;
    table->max_size = 0;
    table->items = NULL;

    return 0;
}

int htable_set(struct htable *table, const char *key, const void *value)
{
    size_t index;
    struct htable_item *item;
    size_t key_len;

    if (table->items == NULL) {
        if (_htable_resize(table, HTABLE_BSIZE))
            return -1;
    } else {
        if (table->size / (double) table->max_size >= LOAD_FACTOR_BORDER)
            if (_htable_resize(table, table->max_size * HTABLE_ESIZE))
                return -1;
    }

    index = _index_key(key, table->mask);
    item = _htable_lookup(table, key, index);
    if (item == NULL)
        return -1;

    if (item->key == NULL) {
        key_len = strlen(key);
        item->key = malloc(key_len + 1);
        if (item->key == NULL)
            return -1;
        item->key[key_len - 1] = 0;
        memcpy(item->key, key, key_len);
        item->value = (void *) value;
        item->index = index;
        table->size += 1;
    }
    else {
        item->value = (void *) value;
    }

    return 0;
}

void *htable_get(struct htable *table, const char *key)
{
    size_t index;
    struct htable_item *item;

    if (table->mask == 0)
        return NULL;

    index = _index_key(key, table->mask);
    item = _htable_lookup(table, key, index);
    if (item == NULL || item->key == NULL)
        return NULL;

    return item->value;
}

int htable_del(struct htable *table, const char *key)
{
    size_t index;
    struct htable_item *item;
    struct htable_item *next_item;

    if (table->mask == 0)
        return -1;

    index = _index_key(key, table->mask);
    item = _htable_lookup(table, key, index);
    if (item == NULL || item->key == NULL)
        return -1;
    item->key = NULL;

    for (index++; index < table->max_size; index++) {
        next_item = table->items + index;
        if (next_item->key == NULL)
            break;
        if (next_item->index == item->index) {
            item->key = next_item->key;
            item->value = next_item->value;
            item = next_item;
            next_item->key = NULL;
        }
    }

    table->size -= 1;
    return 0;
}

int htable_clear(struct htable *table)
{
    if (table->items != NULL)
        memset(table->items, 0, table->max_size * sizeof(*table->items));
    table->size = 0;

    return 0;
}

int htable_resize(struct htable *table, size_t max_size)
{
    return _htable_resize(table, max_size);
}

void htable_free(struct htable *table)
{
    if (table->items != NULL) {
        free(table->items);
        table->items = NULL;
    }
    table->size = 0;
    table->mask = 0;
    table->max_size = 0;
}

static int _htable_resize(struct htable *table, size_t max_size)
{
    struct htable_item *new_items;
    struct htable_item *old_item;
    size_t index;
    size_t mem_size;
    size_t mask;
    size_t i;

    mask = max_size - 1;
    mem_size = max_size * sizeof(*new_items);
    new_items = malloc(mem_size);
    if (new_items == NULL)
        return -1;
    memset(new_items, 0, mem_size);

    for (i = 0; i < table->max_size; i++) {
        old_item = table->items + i;
        if (old_item->key == NULL)
            continue;
        index = _index_key(old_item->key, mask);
        (new_items + index)->index = index;
        for (; index < max_size; index++) {
            if ((new_items + index)->key == NULL)
                break;
        }
        if (index == max_size)
            goto ERROR;
        (new_items + index)->key = old_item->key;
        (new_items + index)->value = old_item->value;
    }

    free(table->items);
    table->items = new_items;
    table->max_size = max_size;
    table->mask = mask;

    return 0;
ERROR:
    if (new_items != NULL)
        free(new_items);
    return -1;
}

static struct htable_item *
_htable_lookup(struct htable *table, const char *key, size_t index)
{
    struct htable_item *item;

    for (; index < table->max_size; index++) {
        item = table->items + index;
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
    size_t hash;

    hash = _hash_str(str, mask);
    return hash % mask;
}
