/*
SYNOPSIS
========

    Assignment 002a
    Word Ladder
    ADT: stack, queue, set, hash-table

USAGE
=====

    $ ./dest/bin/002a code data
    code cade cate date data
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/stack.h>
#include <cs106b/queue.h>
#include <cs106b/set.h>
#include <cs106b/htab.h>
#include <cs106b/fset.h>

#define FIRST_WORD_DEF "code"
#define SECOND_WORD_DEF "data"

static void _cli_help(void);
static int _nb_words(const char *word, struct queue *nb, struct set *dict);

int main(int argc, char *argv[])
{
    int opt;
    const char *w1;
    const char *w2;
    struct queue q;         // queue of stacks
    struct stack s_w1;      // stack of words
    struct stack *s;        // current stack
    struct set dict;        // english dictionary
    char *wtop;             // top word of stack of words
    struct queue nb_words;  // neighbour words
    char *nb;               // neighbour word
    struct set used_words;  // words which used in any word ladder
    struct stack *new_s;    // new stack on copy
    int ret;

    ret = EXIT_FAILURE;
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

    set_init(&dict);
    if (fset_load(&dict, argv[1]))
        goto free_dict;
    if (!set_exist(&dict, w1) || !set_exist(&dict, w2)) {
        _cli_help();
        return EXIT_FAILURE;
    }

    set_init(&used_words);
    stack_init(&s_w1);
    queue_init(&q);

    if (set_add(&used_words, w1))
        goto free_used_words;
    if (stack_push(&s_w1, (void *) w1))
        goto free_s_w1;
    if (queue_push(&q, &s_w1))
        goto free_q;

    for (;;) {
        // break condition
        if (queue_pop(&q, (void *) &s)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                goto free_q;
            }
        }

        if (stack_top(s, (void *) &wtop))
            goto free_q;
        queue_init(&nb_words);
        if (_nb_words(wtop, &nb_words, &dict))
            goto free_nb_words;

        for (;;) {
            // break condition
            if (queue_pop(&nb_words, (void *) &nb)) {
                if (espace_catch(CS106B_EINDEX)) {
                    espace_clear();
                    break;
                } else {
                    goto free_q;
                }
            }

            if (set_exist(&used_words, nb))
                continue;
            if (set_add(&used_words, nb))
                goto free_q;
            if (strcmp(nb, w2) == 0) {
                if (stack_push(s, nb))
                    goto free_q;
                for (;;) {
                    // break condition
                    if (stack_pop(s, (void *) &wtop)) {
                        if (espace_catch(CS106B_EINDEX)) {
                            espace_clear();
                            break;
                        } else {
                            goto free_q;
                        }
                    }
                    printf("%s <= ", wtop);
                }
                printf("\n");
                break;
            } else {
                if (stack_clone(&new_s, s))
                    goto free_q;
                if (stack_push(new_s, nb))
                    goto free_q;
                if (queue_push(&q, new_s))
                    goto free_q;
            }
        }
        queue_free(&nb_words);
    }
    ret = EXIT_SUCCESS;

free_q:
    queue_free(&q);
free_s_w1:
    stack_free(&s_w1);
free_used_words:
    set_free(&used_words);
free_nb_words:
    queue_free(&nb_words);
free_dict:
    set_free(&dict);
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}

static int _nb_words(const char *word, struct queue *nb, struct set *dict)
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
            if (!set_exist(dict, nb_word)) {
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
