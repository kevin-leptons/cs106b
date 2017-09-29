#include <cs106b/i2htab.h>
#include <stdio.h>
#include <string.h>

// store in hex format and null terminal
#define I2HTAB_KEY_SIZE 2 * 2 * sizeof(size_t) + 1

static void _i2htab_key(size_t k1, size_t k2, char key[I2HTAB_KEY_SIZE]);

void i2htab_init(struct i2htab *table)
{
    htab_init(&table->tab);
}

void i2htab_free(struct i2htab *table)
{
    htab_free(&table->tab);
}

int i2htab_set(struct i2htab *table, size_t k1, size_t k2, void *value)
{
    char key[I2HTAB_KEY_SIZE];

    _i2htab_key(k1, k2, key);
    return htab_set(&table->tab, key, value);
}

int i2htab_get(struct i2htab *table, size_t k1, size_t k2, void **value)
{
    char key[I2HTAB_KEY_SIZE];

    _i2htab_key(k1, k2, key);
    return htab_get(&table->tab, key, value);
}

int i2htab_del(struct i2htab *table, size_t k1, size_t k2)
{
    char key[I2HTAB_KEY_SIZE];

    _i2htab_key(k1, k2, key);
    return htab_del(&table->tab, key);
}

size_t i2htab_size(struct i2htab *table)
{
    return table->tab.size;
}

static void _i2htab_key(size_t k1, size_t k2, char key[I2HTAB_KEY_SIZE])
{
    snprintf(key, I2HTAB_KEY_SIZE - 1, "%zx%zx", k1, k2);
    key[I2HTAB_KEY_SIZE - 1] = 0;
}
