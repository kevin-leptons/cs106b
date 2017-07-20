/*
SYNOPSIS

    Assignment 002a
    Word Ladder
    ADT: stack, queue, set, hash-table

USAGE

    $ ./dest/bin/002a code data
    code cade cate date data

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/stack.h>
#include <cs106b/queue.h>
#include <cs106b/set.h>
#include <cs106b/htable.h>
#include <cs106b/dict.h>

#define FIRST_WORD_DEF "code"
#define SECOND_WORD_DEF "data"

static void _cli_help(void);
static int _nb_words(const char *word, struct queue *nb, struct dict *dict);

int main(int argc, char *argv[])
{
    int opt;
    const char *w1;
    const char *w2;
    struct queue q;         // queue of stacks
    struct stack s_w1;      // stack of words
    struct stack *s;        // current stack
    struct dict dict;       // english dictionary
    char *wtop;             // top word of stack of words
    struct queue nb_words;  // neighbour words
    char *nb;               // neighbour word
    struct set used_words;  // words which used in any word ladder
    struct stack *new_s;    // new stack on copy

    for (;;) {
        opt = getopt(argc, argv, "h");
        if (opt < 0)
            break;

        if (opt == 'h') {
            _cli_help();
            return EXIT_SUCCESS;
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    }

    if (argc == 2) {
        w1 = (const char*)&FIRST_WORD_DEF;
        w2 = (const char*)&SECOND_WORD_DEF;
    } else if (argc == 4) {
        w1 = argv[2];
        w2 = argv[3];
    } else {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (strlen(w1) != strlen(w2)) {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (dict_init(&dict, argv[1]))
        exit_err("dict_init");
    if (!dict_exist(&dict, w1) || !dict_exist(&dict, w2)) {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (set_init(&used_words))
        exit_err("set_init");
    if (set_add(&used_words, w1))
        exit_err("set_add");
    if (queue_init(&q))
        exit_err("queue_init");
    if (stack_init(&s_w1))
        exit_err("stack_init");
    if (stack_push(&s_w1, (void*) w1))
        exit_err("stack_push");
    if (queue_push(&q, &s_w1))
        exit_err("queue_push");

    for (s = queue_pop(&q); s != NULL; s = queue_pop(&q)) {
        wtop = stack_top((struct stack *) s);
        if (queue_init(&nb_words))
            exit_err("stack_init");
        if (_nb_words(wtop, &nb_words, &dict))
            exit_err("_nb_words");

        nb = queue_pop(&nb_words);
        for (; nb != NULL; nb = queue_pop(&nb_words)) {
            if (set_exist(&used_words, nb))
                continue;
            if (set_add(&used_words, nb))
                exit_err("set_add");
            if (strcmp(nb, w2) == 0) {
                if (stack_push(s, nb))
                    exit_err("stack_push");
                for (;;) {
                    wtop = stack_pop(s);
                    if (wtop == NULL)
                        break;
                    printf("%s <= ", wtop);
                }
                printf("\n");
                return EXIT_SUCCESS;
            } else {
                new_s = stack_clone(s);
                if (new_s == NULL)
                    exit_err("stack_clone");
                if (stack_push(new_s, nb))
                    exit_err("stack_push");
                if (queue_push(&q, new_s))
                    exit_err("queue_push");
            }
        }
        queue_free(&nb_words, true);
    }

    queue_free(&nb_words, true);
    dict_free(&dict);
    stack_free(&s_w1, false);
    queue_free(&q, false);

    return EXIT_FAILURE;
}

static int _nb_words(const char *word, struct queue *nb, struct dict *dict)
{
    size_t wlen;
    char *nb_word;
    size_t i;
    size_t k;

    wlen = strlen(word);

    for (i = 0; i < wlen; i++) {
        for (k = 97; k <= 122; k++) {
            if (k == word[i])
                continue;
            nb_word = malloc(wlen + 1);
            if (nb_word == NULL)
                return -1;
            strcpy(nb_word, word);
            nb_word[i]  = k;
            if (!dict_exist(dict, nb_word)) {
                free(nb_word);
                continue;
            }
            if (queue_push(nb, nb_word))
                goto ERROR;
        }
    }

    return 0;

ERROR:
    if (nb_word != NULL)
        free(nb_word);
    return -1;
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    002a dict [first second]\n");
    printf("    002a -h\n\n");

    printf("ARGUMENTS\n");
    printf("    dict                Dictionary in asset/dict.txt\n");
    printf("    first, second       English word with same length\n");
}
