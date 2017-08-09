#include <cs106b/string.h>

#include <string.h>

int str_init(struct string *str)
{
    str->data = malloc(1);
    if (str->data == NULL)
        return -1;
    str->data[0] = 0;

    str->size = 0;
    str->max_size = 1;

    return 0;
}

int str_cpy(struct string *dest, struct string *src)
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

int str_cmp(struct string *dest, struct string *src)
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

int str_cat(struct string *dest, struct string *part)
{
    return str_catc(dest, part->data);
}

int str_ins(struct string *dest, size_t index, struct string *part)
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

int str_rpl(struct string *dest, size_t index, size_t len,
            struct string *part)
{
    size_t i;

    if (index >= dest->size || part->size < len)
        return -1;

    for (i = 0; i < len; i++)
        dest->data[index + i] = part->data[i];

    return 0;
}

int str_erase(struct string *dest, size_t index, size_t len)
{
    size_t data_size;
    char *new_data;
    size_t i;

    if (index >= dest->size || index + len > dest->size)
        return -1;
    data_size = dest->size - len + 1;

    if (data_size < dest->max_size / 2) {
        new_data = malloc(data_size);
        if (new_data == NULL)
            return -1;
        memcpy(new_data, dest->data, index);
        for (i = 0; i < len; i++)
            new_data[index + i] = dest->data[index + len + i];
        free(dest->data);
        dest->data = new_data;
        dest->max_size = data_size;
    } else {
        for (i = 0; i < len; i++)
            dest->data[index + i] = dest->data[index + len + i];
    }

    dest->size = data_size - 1;
    dest->data[dest->size] = 0;

    return 0;
}

struct string *str_sub(struct string *str, size_t index, size_t len)
{
    struct string *new_str;

    if (index >= str->size || index + len > str->size)
        return NULL;

    new_str = malloc(sizeof(*new_str));
    if (new_str == NULL)
        return NULL;

    new_str->size = len;
    new_str->max_size = len + 1;
    new_str->data = malloc(new_str->max_size);
    if (new_str->data == NULL)
        return NULL;
    
    memcpy(new_str->data, str->data + index, len);
    new_str->data[new_str->size] = 0;

    return new_str;
}

struct string * str_clone(const struct string *src)
{
    struct string *new_str;

    new_str = malloc(sizeof(*new_str));
    if (new_str == NULL)
        return NULL;
    if (str_init(new_str))
        goto ERROR;
    if (str_cpy(new_str, (struct string *) src))
        goto ERROR;

    return new_str;

ERROR:
    if (new_str != NULL) {
        str_free(new_str);
        free(new_str);
    }

    return NULL;
}

void str_free(struct string *str)
{
    if (str->data != NULL) {
        free(str->data);
        str->data = NULL;
    }
    str->size = 0;
    str->max_size = 0;
}

int str_cpyc(struct string *dest, const char *src)
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

int str_catc(struct string *dest, const char *part)
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
