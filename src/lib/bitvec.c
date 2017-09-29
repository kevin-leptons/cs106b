#include <cs106b/bitvec.h>
#include <string.h>
#include <stdio.h>
#include <cs106b/mem.h>
#include <cs106b/error.h>

#define BITVEC_BSIZE 8
#define BITVEC_ESIZE 2

static int _bitvec_resize(struct bitvec *vector, size_t max_size);

void bitvec_init(struct bitvec *vector)
{
    vector->items = NULL;
    vector->size = 0;
    vector->max_size = 0;
}

void bitvec_free(struct bitvec *vector)
{
    if (vector->items != NULL) {
        free(vector->items);
        vector->items = NULL;
    }
    vector->max_size = 0;
    vector->size = 0;
}

int bitvec_resize(struct bitvec *vector, size_t max_byte_size)
{
    size_t byte_size;

    byte_size = vector->size / 8;
    if (byte_size % 8)
        byte_size += 1;

    if (max_byte_size > byte_size) {
        return _bitvec_resize(vector, max_byte_size);       // extend
    } else if (max_byte_size < byte_size) {
        vector->size = byte_size;
        return _bitvec_resize(vector, max_byte_size);       // narrow
    } else {
        return 0;                                           // unchanged
    }
}

int bitvec_add(struct bitvec *vector, char bit)
{
    char *ibyte;
    size_t byte_index;
    size_t bit_index;

    if (vector->max_size == 0) {
        if (_bitvec_resize(vector, BITVEC_BSIZE))
            return -1;
    }
    if (vector->size == 8 * vector->max_size) {
        if (_bitvec_resize(vector, vector->max_size * BITVEC_ESIZE))
            return -1;
    }
    
    byte_index = vector->size / 8;
    bit_index = vector->size % 8;
    vector->size += 1;

    ibyte = vector->items + byte_index;
    if (bit)
        *ibyte = *ibyte | (1 << (7 - bit_index));
    else
        *ibyte = *ibyte & ~(1 << (7 - bit_index));

    return 0;
}

int bitvec_get(struct bitvec *vector, size_t index)
{
    size_t byte_index;
    size_t bit_index;
    char ibyte;

    if (index > (vector->size - 1)) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    byte_index = index / 8;
    bit_index = index % 8;

    ibyte = vector->items[byte_index];
    ibyte = ibyte & (1 << (7 - bit_index));

    return ibyte != 0;
}

int bitvec_dump(struct bitvec *vector)
{
    size_t i;
    int bit;

    for (i = 0; i < vector->size; i++) {
        bit = bitvec_get(vector, i);
        if (bit < 0)
            return -1;
        if (bit)
            printf("1 ");
        else
            printf("0 ");
    }

    return 0;
}

int bitvec_copy(struct bitvec *dest, struct bitvec *src)
{
    if (src->max_size == 0)
        return 0;

    bitvec_free(dest);
    dest->size = src->size;
    dest->max_size = src->max_size;
    if (cs106b_malloc((void *) &dest->items, src->max_size))
        return -1;
    memcpy(dest->items, src->items, src->max_size);

    return 0;
}

int bitvec_clone(struct bitvec **dest, struct bitvec *src)
{
    if (cs106b_malloc((void *) dest, sizeof(struct bitvec)))
        return -1;
    bitvec_init(*dest);
    if (bitvec_copy(*dest, src)) {
        free(*dest);
        return -1;
    }
    return 0;
}

int bitvec_cat(struct bitvec *dest, struct bitvec *src)
{
    size_t i;
    int bit;

    for (i = 0; i < src->size; i++) {
        bit = bitvec_get(src, i);
        if (bit < 0 || bit > 1)
            return -1;
        if (bitvec_add(dest, (char) bit))
            return -1;
    }

    return 0;
}

int bitvec_clear(struct bitvec *vector)
{
    vector->size = 0;
    memset(vector->items, 0, vector->max_size);
    return 0;
}

static int _bitvec_resize(struct bitvec *vector, size_t max_size)
{
    char *new_items;
    size_t dsize;
    size_t byte_size;

    byte_size = sizeof(char) * max_size;
    if (cs106b_malloc((void *) &new_items, byte_size))
        return -1;
    memset(new_items, 0, byte_size);

    if (vector->items != NULL) {
        dsize = vector->size / 8;
        if (vector->size % 8)
            dsize += 1;
        memcpy(new_items, vector->items, dsize);
        free(vector->items);
    }

    vector->items = new_items;
    vector->max_size = max_size;
    return 0;
}

