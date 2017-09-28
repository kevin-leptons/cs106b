#include <cs106b/str.h>
#include <string.h>
#include <cs106b/mem.h>
#include <cs106b/error.h>

int str_init(struct str *str)
{
    if (cs106b_malloc((void *) &str->data, 1))
        return -1;
    str->data[0] = 0;
    str->size = 0;
    str->max_size = 1;
    return 0;
}

void str_free(struct str *str)
{
    if (str->data != NULL) {
        free(str->data);
        str->data = NULL;
    }
    str->size = 0;
    str->max_size = 0;
}

int str_cpy(struct str *dest, struct str *src)
{
    size_t data_size;

    data_size = src->size + 1;

    if (dest->data != NULL) {
        if (dest->max_size < src->size || dest->max_size > 2 * src->size) {
            free(dest->data);
            dest->data = NULL;
            dest->max_size = 0;
        }
    }
    if (dest->data == NULL) {
        dest->data = malloc(data_size);
        if (dest->data == NULL)
            return -1;
        dest->max_size = data_size;
    }

    dest->size = data_size - 1;
    memcpy(dest->data, src->data, data_size);
    dest->data[dest->size] = 0;

    return 0;
}

int str_cmp(struct str *dest, struct str *src)
{
    size_t i;

    if (dest->size != src->size)
        return 1;

    for (i = 0; i < dest->size; i++) {
        if (dest->data[i] != src->data[i])
            return 1;
    }

    return 0;
}

int str_cat(struct str *dest, struct str *part)
{
    return str_catc(dest, part->data);
}

int str_ins(struct str *dest, size_t index, struct str *part)
{
    size_t data_size;
    char *new_data;
    size_t i;

    if (index >= dest->size)
        return -1;

    data_size = dest->size + part->size + 1;
    if (dest->max_size < data_size || dest->max_size > 2 * data_size) {
        new_data = malloc(data_size);
        if (new_data == NULL)
            return -1;
        memcpy(new_data, dest->data, index + 1);
        memcpy(new_data + index + part->size, dest->data + index,
               dest->size - index);
        free(dest->data);
        dest->data = new_data;
        dest->max_size = data_size;
    } else {
        for (i = index; i < dest->size; i++)
            dest->data[part->size + i] = dest->data[i];
    }

    memcpy(dest->data + index, part->data, part->size);
    dest->size = data_size - 1;
    dest->data[dest->size] = 0;

    return 0;
}

int str_rpl(struct str *dest, size_t index, size_t len, struct str *part)
{
    size_t i;

    if (index >= dest->size || part->size < len) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    for (i = 0; i < len; i++)
        dest->data[index + i] = part->data[i];

    return 0;
}

int str_era(struct str *dest, size_t index, size_t len)
{
    size_t data_size;
    char *new_data;
    char *dest_data;
    size_t i;

    if (index >= dest->size || index + len > dest->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    data_size = dest->size - len + 1;

    if (data_size < dest->max_size / 2) {
        if (cs106b_malloc((void *) &new_data, data_size))
            return -1;
        memcpy(new_data, dest->data, index);
        dest_data = new_data;
        dest->max_size = data_size;
    } else {
        dest_data = dest->data;
    }

    for (i = index + len; i < dest->size; i++)
        dest_data[i - len] = dest->data[i];
    if (dest_data != dest->data) {
        free(dest->data);
        dest->data = new_data;
    }
    dest->size = data_size - 1;
    dest->data[dest->size] = 0;
    return 0;
}

int str_sub(struct str *dest, struct str *src, size_t index, size_t len)
{
    if (index >= src->size || index + len > src->size) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    str_free(dest);
    dest->size = len;
    dest->max_size = len + 1;
    if (cs106b_malloc((void *) &dest->data, dest->max_size))
        return -1;
    memcpy(dest->data, src->data + index, len);
    dest->data[dest->size] = 0;
    return 0;
}

int str_clone(struct str **dest, struct str *src)
{
    int ret;

    ret = -1;
    if (cs106b_malloc((void *) dest, sizeof(struct str)))
        return -1;
    if (str_init(*dest))
        goto free_dest_mem;
    if (str_cpy(*dest, src))
        goto free_dest;

    ret = 0;
    goto finish;

free_dest:
    str_free(*dest);
free_dest_mem:
    free(*dest);
finish:
    return ret;
}

int str_cpyc(struct str *dest, const char *src)
{
    size_t data_size;

    data_size = strlen(src) + 1;

    if (dest->data != NULL) {
        if (dest->max_size < data_size || dest->max_size > 2 * data_size) {
            free(dest->data);
            dest->data = NULL;
            dest->max_size = 0;
        }
    }
    if (dest->data == NULL) {
        dest->data = malloc(data_size);
        if (dest->data == NULL)
            return -1;
        dest->max_size = data_size;
    }

    memcpy(dest->data, src, data_size);
    dest->size = data_size - 1;
    dest->data[dest->size] = 0;

    return 0;
}

int str_catc(struct str *dest, const char *part)
{
    size_t new_size;
    size_t part_size;
    char *new_data;

    part_size = strlen(part);
    new_size = dest->size + part_size + 1;
    if (dest->max_size < new_size || dest->max_size > 2 * new_size) {
        new_data = malloc(new_size);
        if (new_data == NULL)
            return -1;
        memcpy(new_data, dest->data, dest->size);
        free(dest->data);
        dest->data = new_data;
        dest->max_size = new_size;
    }

    memcpy(dest->data + dest->size, part, part_size);
    dest->size = new_size - 1;
    dest->data[dest->size] = 0;

    return 0;
}
