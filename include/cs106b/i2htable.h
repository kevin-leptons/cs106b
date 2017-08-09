#ifndef __CS106B_I2HTABLE_H__
#define __CS106B_I2HTABLE_H__

#include <stdlib.h>

#include <cs106b/htable.h>

struct i2htable
{
    struct htable htable;
};

int i2htable_init(struct i2htable *table);
int i2htable_set(struct i2htable *table, size_t k1, size_t k2, void *value);
void * i2htable_get(struct i2htable *table, size_t k1, size_t k2);
int i2htable_del(struct i2htable *table, size_t k1, size_t k2);
size_t i2htable_size(struct i2htable *table);
void i2htable_free(struct i2htable *table);

#endif
