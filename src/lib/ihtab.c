#include <cs106b/ihtab.h>
#include <stdio.h>

// store in hex string with 0x leading and null terminal
#define IHTAB_KEY_SIZE 2 * sizeof(size_t) + 2 + 1 

static void _ihtab_key_to_str(size_t key, char str[IHTAB_KEY_SIZE]);

void ihtab_init(struct ihtab *table)
{
    return htab_init(&table->tab);
}

void ihtab_free(struct ihtab *table)
{
    htab_free(&table->tab);
}

int ihtab_set(struct ihtab *table, size_t key, void *value)
{
    char str_key[IHTAB_KEY_SIZE];
    
    _ihtab_key_to_str(key, str_key);
    return htab_set(&table->tab, str_key, value);
}

int ihtab_get(struct ihtab *table, size_t key, void **value)
{
    char str_key[IHTAB_KEY_SIZE];

    _ihtab_key_to_str(key, str_key);
    return htab_get(&table->tab, str_key, value);
}

int ihtab_del(struct ihtab *table, size_t key)
{
    char str_key[IHTAB_KEY_SIZE];

    _ihtab_key_to_str(key, str_key);
    return htab_del(&table->tab, str_key);
}

size_t ihtab_size(struct ihtab *table)
{
    return table->tab.size;
}

static void _ihtab_key_to_str(size_t key, char str[IHTAB_KEY_SIZE])
{
    snprintf(str, IHTAB_KEY_SIZE - 1, "%#zx", key);
    str[IHTAB_KEY_SIZE - 1] = 0;
}
