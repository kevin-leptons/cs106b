/*
SYNOPSIS

    Assignment 002b
    Ngram
    ADT: svmap, vector, queue
    Additional: _vector_uadd()

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>

#include <cs106b/error.h>
#include <cs106b/svmap.h>
#include <cs106b/vector.h>
#include <cs106b/queue.h>
#include <cs106b/random.h>

#define NDEF 3

struct sword
{
    char *data;
    size_t size;
    size_t offset;
};

struct ngram
{
    size_t size;            // number of words in gram
    struct svmap map;       // map vector of words => vector of words
    struct sword sword;     // serial words
    char *data;             // content of ngram file
};

static void _cli_help(void);
static int _ngram_init(struct ngram *ngram, size_t size, const char *file);
static char * _ngram_make(struct ngram *ngram, size_t size);
void _ngram_free(struct ngram *ngram);
static int _sword_init(struct sword *sword, const char *data, size_t size);
static char * _sword_next(struct sword *sword);
static void _sword_free(struct sword *sword);
static ssize_t _read_to_buf(const char *file, char **buf);

// add item to vector if it is not early exists
static int _vector_uadd(struct vector *vector, const char *value);

int main(int argc, char *argv[])
{
    int opt;
    size_t n;
    size_t w; 
    struct ngram ngram;
    char *words;

    n = 0;
    w = 0;

    for (;;) {
        opt = getopt(argc, argv, "hn:w:");
        if (opt == -1)
            break;
        if (opt == 'h') {
            _cli_help();
            return EXIT_SUCCESS;
        } else if (opt == 'n') {
            n = atoi(optarg);
            if (n < 1) {
                _cli_help();
                return EXIT_FAILURE;
            }
        } else if (opt == 'w') {
            w = atoi(optarg);
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    }
    if (optind >= argc) {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (n == 0)
        n = NDEF;
    if (w == 0)
        w = 10 * n;
    if (w < n) {
        _cli_help();
        return EXIT_FAILURE;
    }

    
    if (_ngram_init(&ngram, n, argv[optind]))
        exit_err("_ngram_init");

    words = _ngram_make(&ngram, w);
    if (words == NULL)
        exit_err("_ngram_make");
    printf("%s\n", words);

    return EXIT_SUCCESS;
}

static int _ngram_init(struct ngram *ngram, size_t size, const char *file)
{
    ssize_t dsize;
    char *word;
    struct vector window;   // vector of string
    struct vector *value;   // vector of string
    bool reloop;
    size_t n;

    dsize = _read_to_buf(file, &ngram->data);
    if (dsize < 0)
        goto ERROR;
    if (_sword_init(&ngram->sword, ngram->data, dsize))
        goto ERROR;

    if (svmap_init(&ngram->map))
        goto ERROR;
    if (vector_init(&window))
        goto ERROR;

    reloop = false;
    for (;;) {
        word = _sword_next(&ngram->sword);

        // last word of file, back to begin of file and run n - 1 words
        if (word == NULL && !reloop) {
            ngram->sword.offset = 0;
            reloop = true;
            n = size;
            continue;
        }
        if (reloop) {
            n -= 1;
            if (n == 0)
                break;
        }

        // push to window to size of ngram
        if (window.size < size - 1) {
            vector_add(&window, word);
            continue;
        }

        // use window as key
        // if key is not exist, create new one value and add to ngram
        value = (struct vector *) svmap_get(&ngram->map, &window);
        if (value == NULL) {
            value = malloc(sizeof(*value));
            if (value == NULL)
                goto ERROR;
            if (vector_init(value))
                goto ERROR;
            if (svmap_set(&ngram->map, &window, value))
                goto ERROR;
        }

        // add word to next words, if word is early exists, ignore
        if (_vector_uadd(value, word))
            goto ERROR;

        // remove first word from window
        // put new word to end of vector
        if (vector_remove(&window, 0))
            goto ERROR;
        if (vector_add(&window, word))
            goto ERROR;
    }

    // reset serial word
    ngram->sword.offset = 0;

    ngram->size = size;
    return 0;

ERROR:
    if (ngram->data != NULL) {
        free(ngram->data);
        ngram->data = NULL;
    }

    return -1;
}

static char * _ngram_make(struct ngram *ngram, size_t size)
{
    struct queue words;         // store words was made
    struct vector window;       // vector of strings
    size_t nword;               // number of words was made
    char *word;                 // word to put to queue
    size_t msize;               // memory size of words was made
    size_t findex;              // first randomize index
    size_t windex;              // index of next randomize word
    char *next_word;
    struct vector *next_words;  // next words of ngram
    struct vector *keys;        // keys in ngram
    struct vector *key;         // key in ngram
    char *data;                 // store result string
    char *p;
    size_t len;

    if (queue_init(&words))
        return NULL;

    // first window
    if (vector_init(&window))
        return NULL;
    keys = svmap_keys(&ngram->map);
    if (keys == NULL)
        return NULL;
    if (rrand(0, keys->size - 1, &findex))
        return NULL;
    key = vector_at(keys, findex);
    if (key == NULL)
        return NULL;
    if (vector_copy(&window, key))
        return NULL;

    // loop until make enough words
    msize = 0;
    for (nword = 0; nword < size; nword++) {
        // random next word
        next_words = svmap_get(&ngram->map, &window);
        if (next_words == NULL)
            return NULL;
        if (rrand(0, next_words->size - 1, &windex))
            return NULL;
        next_word = (char *) vector_at(next_words, windex);
        if (next_word == NULL)
            return NULL;

        // pick first word in window, push to queue of result words
        // remove that word in window
        word = vector_at(&window, 0);
        if (word == NULL)
            return NULL;
        if (queue_push(&words, word))
            return NULL;
        if (vector_remove(&window, 0))
            return NULL;

        // add to back of window next word
        if (vector_add(&window, next_word))
            return NULL;

        // add to total size
        msize += strlen(word);
    }

    // memory size include size of words and space
    // and terminal character 0
    msize += nword;

    // generate result
    data = malloc(msize);
    if (data == NULL)
        return NULL;
    for (p = data; ;) {
        word = queue_pop(&words);
        if (word == NULL)
            break;
        len = strlen(word);
        memcpy(p, word, len);
        p[len] = ' ';
        p = p + len + 1;
    }
    data[msize - 1] = 0;

    return data;
}

void _ngram_free(struct ngram *ngram)
{
    _sword_free(&ngram->sword);
    svmap_free(&ngram->map);
    if (ngram->data != NULL) {
        free(ngram->data);
        ngram->data = NULL;
    }
    ngram->size = 0;
}

static int _sword_init(struct sword *sword, const char *data, size_t size)
{
    sword->data = (char *) data;
    sword->size = size;
    sword->offset = 0;

    return 0;
}

static char * _sword_next(struct sword *sword)
{
    char *s;
    char *p;
    char *e;

    e = sword->data + sword->size - 1;

    // point to first character of word
    for (s = sword->data + sword->offset; s <= e; s++)
        if (*s != 0 && *s != ' ')
            break;

    // find end of word 
    for (p = s; p <= e; p++) {
        if (*p != 0)
            continue;
        sword->offset = p - sword->data + 1;
        return s;
    }

    return NULL;
}

static void _sword_free(struct sword *sword)
{
    sword->data = NULL;
    sword->size = 0;
    sword->offset = 0;
}

static ssize_t _read_to_buf(const char *file, char **buf)
{
    int fd;
    ssize_t rsize;
    size_t buf_size;
    struct stat stat;
    char *p;
    char *e;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -1;

    if (fstat(fd, &stat))
        goto ERROR;
    buf_size = stat.st_size + 1;
    *buf = malloc(buf_size);
    if (*buf == NULL)
        return -1;

    // read from file to memory
    for (p = *buf; ;) {
        rsize = read(fd, p, 1024);
        if (rsize == 0)
            break;
        else if (rsize < 0)
            goto ERROR;
        p += rsize;
    }

    // replace newline and space by terminala character
    // replace uppercase by lowercase
    e = *buf + buf_size - 1;
    *e = 0;
    for (p = *buf; p < e; p++) {
        if (*p == '\n' || *p == ' ')
            *p = 0;
        else
            *p = (char) tolower(*p);
    }

    return stat.st_size + 1;

ERROR:
    if (*buf != NULL) {
        free(*buf);
        *buf = NULL;
    }

    return -1;
}

static int _vector_uadd(struct vector *vector, const char *value)
{
    size_t i;
    char *v;

    for (i = 0; i < vector->size; i++) {
        v = vector_at(vector, i);
        if (v == NULL)
            return -1;
        if (strcmp(v, value) == 0)
            return 0;
    }

    return vector_add(vector, (char*) value);
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    002b [-n n] [-w w] file\n");
    printf("    002 -h\n\n");

    printf("ARGUMENTS\n");
    printf("    file        Path to file contains sample sentenses\n\n");

    printf("OPTIONS\n");
    printf("    n       Value of ngram, >= 1, default is %u\n", NDEF);
    printf("    w       Number of words to make, >= n, default is 10n\n");
}
