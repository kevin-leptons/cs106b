#include <cs106b/vector.h>
#include <cs106b/error.h>

#define VECTOR_BSIZE 16
#define VECTOR_ESIZE 2


static int _vector_extend(struct vector *vector);
static int _vector_narrow(struct vector *vector);

int vector_init(struct vector *vector)
{
    vector->size = 0;
    vector->max_size = 0;
    vector->front = NULL;
    vector->base_size = VECTOR_BSIZE;
    vector->ext_size = VECTOR_ESIZE;

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

void * vector_at(struct vector *vector, size_t index)
{
    if (index >= vector->size) {
        espace_raise(CS106B_EINDEX);
        return NULL;
    }
    return (vector->front + index)->data;
}

void * vector_get(struct vector *vector, size_t index)
{
    if (index >= vector->size) {
        espace_raise(CS106B_EINDEX);
        return NULL;
    }
    return vector->front[index].data;
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

void *vector_end(struct vector *vector)
{
    if (vector->size == 0) {
        espace_raise(CS106B_EINDEX);
        return NULL;
    }
    return vector_at(vector, vector->size - 1);
}

int vector_copy(struct vector *dest, struct vector *src)
{
    size_t i;

    vector_free(dest, false);
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

struct vector * vector_clone(struct vector *src)
{
    struct vector *new_vector;

    new_vector = malloc(sizeof(*new_vector));
    if (new_vector == NULL) {
        espace_raise(SYS_ENOMEM);
        return NULL;
    }
    vector_init(new_vector);

    if (vector_copy(new_vector, src))
        goto ERROR;

    return new_vector;

ERROR:
    if (new_vector != NULL)
        free(new_vector);
    return NULL;
}

void vector_free(struct vector *vector, bool free_data)
{
    size_t i;

    if (free_data) {
        for (i = 0; i < vector->size; i++)
            free((vector->front + i)->data);
    }
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
