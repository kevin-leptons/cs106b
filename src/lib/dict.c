#include <cs106b/dict.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#define READ_BLOCK_SIZE 1024

static size_t _round_htable_size(size_t size);

int dict_init(struct dict *dict, const char *file)
{
    int fd;
    struct stat stat;
    char *s;
    char *p;
    char *e;
    size_t rsize;
    size_t dict_size;

    dict->data = NULL;
    if (htable_init(&dict->map))
        return -1;

    if (file == NULL)
        return 0;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -1;

    if (fstat(fd, &stat))
        return -1;
    dict->data = malloc(stat.st_size + 1);
    if (dict->data == NULL)
        return -1;

    for (p = dict->data; ;) {
        rsize = read(fd, p, READ_BLOCK_SIZE);
        if (rsize == 0)
            break;
        if (rsize < 0)
            goto ERROR;
        p += rsize;
    }

    e = dict->data + stat.st_size;
    dict_size = 0;
    for (s = dict->data, p = s; p <= e; p++) {
        if (*p == '\n' || p == e) {
            *p = 0;
            if (p - s != 0)
                dict_size += 1;
            s = p + 1;
        }
    }

    dict_size = _round_htable_size(dict_size);
    if (htable_resize(&dict->map, dict_size))
        return -1;

    for (s = dict->data, p = s; p <= e; p++) {
        if (*p == 0) {
            if (p - s != 0)
                htable_set(&dict->map, s, dict);
            s = p + 1;
        }
    }

    return 0;

ERROR:
    if (dict->data != NULL) {
        free(dict->data);
        dict->data = NULL;
    }
    
    return -1;
}

int dict_add(struct dict *dict, const char *word)
{
    return htable_set(&dict->map, word, dict);
}

int dict_del(struct dict *dict, const char *word)
{
    return htable_del(&dict->map, word);
}

int dict_exist(struct dict *dict, const char *word)
{
    struct htable_item *item;

    item = htable_get(&dict->map, word);
    return item != NULL;
}

void dict_free(struct dict *dict)
{
    htable_free(&dict->map);
    if (dict->data != NULL) {
        free(dict->data);
        dict->data = NULL;
    }
}

static size_t _round_htable_size(size_t size)
{
    double log_size;

    size = size * 3 / 2;
    log_size = log(size) / log(2);
    log_size = ceil(log_size);

    return pow(2, log_size);
}
