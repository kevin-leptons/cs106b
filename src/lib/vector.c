#include <cs106b/vector.h>
#include <string.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define VECTOR_BSIZE 16
#define VECTOR_ESIZE 2


static int _vector_extend(struct vector *vector);
static int _vector_narrow(struct vector *vector);

int vector_init(struct vector *vector)
{
    if (vector == NULL) {
        espace_raise(SYS_EINVAL);
        return -1;
    }

    vector->size = 0;
    vector->max_size = 0;
    vector->front = NULL;
    vector->base_size = VECTOR_BSIZE;
    vector->ext_size = VECTOR_ESIZE;

    return 0;
}

int vector_resize(struct vector *vector, size_t max_size)
{
    struct vector_item *new_front;
    size_t tmp_msize;

    if (max_size == vector->max_size)
        return 0;

    tmp_msize = max_size * sizeof(struct vector_item);
    if (cs106b_malloc((void *) &new_front, tmp_msize))
        return -1;
    memset(new_front, 0, tmp_msize);

    if (vector->size > 0) {
        vector->size = max_size < vector->size ? max_size : vector->size;
        tmp_msize = vector->size * sizeof(struct vector_item);
        memcpy(new_front, vector->front, tmp_msize);
        free(vector->front);
    }
    vector->front = new_front;
    vector->max_size = max_size;
    return -1;
}

int vector_get(struct vector *vector, size_t index, void **value)
{
    if (index >= vector->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    *value = vector->front[index].data;
    return 0;
}

int vector_set(struct vector *vector, size_t index, void *data)
{
    if (index >= vector->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    vector->front[index].data = data;
    return 0;
}

int vector_insert(struct vector *vector, size_t index, void *data)
{
    size_t i;

    if (index > vector->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    if (vector->size == vector->max_size)
        if (_vector_extend(vector) != 0)
            return -1;

    for (i = vector->size; i > index; i--)
        vector->front[i] = vector->front[i - 1];

    (vector->front + index)->data = data;
    vector->size += 1;

    return 0;
}

int vector_add(struct vector *vector, void *data)
{
    if (vector->size == vector->max_size)
        if (_vector_extend(vector) != 0)
            return -1;

    (vector->front + vector->size)->data = data;
    vector->size += 1;

    return 0;
}

int vector_remove(struct vector *vector, size_t index)
{
    size_t i;

    if (index >= vector->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    for (i = index + 1; i < vector->size; i++)
        *(vector->front + i - 1) = *(vector->front + i);
    vector->size -= 1;

    if (vector->size <  vector->max_size / 2)
        if (_vector_narrow(vector))
            return -1;

    return 0;
}

int vector_end(struct vector *vector, void **value)
{
    if (vector->size == 0) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    return vector_get(vector, vector->size - 1, value);
}

int vector_copy(struct vector *dest, struct vector *src)
{
    size_t i;

    vector_free(dest);
    dest->front = malloc(src->size * sizeof(struct vector_item));
    if (dest->front == NULL) {
        espace_raise(SYS_ENOMEM);
        return -1;
    }
    dest->size = src->size;
    dest->max_size = src->size;

    for (i = 0; i < src->size; i++)
        dest->front[i] = src->front[i];

    return 0;
}

int vector_clone(struct vector **dest, struct vector *src)
{
    *dest = malloc(sizeof(struct vector));
    if (*dest == NULL) {
        espace_raise(SYS_ENOMEM);
        return -1;
    }
    vector_init(*dest);

    if (vector_copy(*dest, src)) {
        free(*dest);
        return -1;
    }

    return 0;
}

void vector_free(struct vector *vector)
{
    if (vector->front != NULL) {
        free(vector->front);
        vector->front = NULL;
    }
    vector->size = 0;
    vector->max_size = 0;
}

static int _vector_extend(struct vector *vector)
{
    struct vector_item *new_front;
    size_t new_msize;

    if (vector->max_size == 0)
        new_msize = vector->base_size;
    else
        new_msize = vector->max_size * vector->ext_size;
    new_front = realloc(vector->front, new_msize * sizeof(*new_front));
    if (new_front == NULL) {
        espace_raise(SYS_ENOMEM);
        return -1;
    }

    vector->front = new_front;
    vector->max_size = new_msize;

    return 0;
}

static int _vector_narrow(struct vector *vector)
{
    struct vector_item *new_front;
    size_t new_msize;

    new_msize = vector->max_size / 2;
    new_front = realloc(vector->front,
                        new_msize * sizeof(struct vector_item));
    if (new_front == NULL) {
        espace_raise(SYS_ENOMEM);
        return -1;
    }

    vector->front = new_front;
    vector->max_size = new_msize;

    return 0;
}
