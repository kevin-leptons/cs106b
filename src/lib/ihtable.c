#include <cs106b/ihtable.h>

#include <stdio.h>

// store in hex string with 0x leading and null terminal
#define IHTABLE_KEY_SIZE 2 * sizeof(size_t) + 2 + 1 

static void _ihtable_key_to_str(size_t key, char str[IHTABLE_KEY_SIZE]);

int ihtable_init(struct ihtable *table)
{
    return htable_init(&table->htable);
}

int ihtable_set(struct ihtable *table, size_t key, void *value)
{
    char str_key[IHTABLE_KEY_SIZE];
    
    _ihtable_key_to_str(key, str_key);
    return htable_set(&table->htable, str_key, value);
}

void * ihtable_get(struct ihtable *table, size_t key)
{
    char str_key[IHTABLE_KEY_SIZE];

    _ihtable_key_to_str(key, str_key);
    return htable_get(&table->htable, str_key);
}

int ihtable_del(struct ihtable *table, size_t key)
{
    char str_key[IHTABLE_KEY_SIZE];

    _ihtable_key_to_str(key, str_key);
    return htable_del(&table->htable, str_key);
}

size_t ihtable_size(struct ihtable *table)
{
    return table->htable.size;
}

void ihtable_free(struct ihtable *table)
{
    htable_free(&table->htable);
}

static void _ihtable_key_to_str(size_t key, char str[IHTABLE_KEY_SIZE])
{
    snprintf(str, IHTABLE_KEY_SIZE - 1, "%#zx", key);
    str[IHTABLE_KEY_SIZE - 1] = 0;
}
