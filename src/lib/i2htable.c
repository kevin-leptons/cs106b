#include <cs106b/i2htable.h>

#include <stdio.h>
#include <string.h>

// store in hex format and null terminal
#define I2HTABLE_KEY_SIZE 2 * 2 * sizeof(size_t) + 1

static void _i2htable_key(size_t k1, size_t k2, char key[I2HTABLE_KEY_SIZE]);

int i2htable_init(struct i2htable *table)
{
    return htable_init(&table->htable);
}

int i2htable_set(struct i2htable *table, size_t k1, size_t k2, void *value)
{
    char key[I2HTABLE_KEY_SIZE];

    _i2htable_key(k1, k2, key);
    return htable_set(&table->htable, key, value);
}

void * i2htable_get(struct i2htable *table, size_t k1, size_t k2)
{
    char key[I2HTABLE_KEY_SIZE];

    _i2htable_key(k1, k2, key);
    return htable_get(&table->htable, key);
}

int i2htable_del(struct i2htable *table, size_t k1, size_t k2)
{
    char key[I2HTABLE_KEY_SIZE];

    _i2htable_key(k1, k2, key);
    return htable_del(&table->htable, key);
}

size_t i2htable_size(struct i2htable *table)
{
    return table->htable.size;
}

void i2htable_free(struct i2htable *table)
{
    htable_free(&table->htable);
}

static void _i2htable_key(size_t k1, size_t k2, char key[I2HTABLE_KEY_SIZE])
{
    snprintf(key, I2HTABLE_KEY_SIZE - 1, "%zx%zx", k1, k2);
    key[I2HTABLE_KEY_SIZE - 1] = 0;
}
