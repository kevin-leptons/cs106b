/*
SYNOPSIS
========

    Assignment 005
    Model a hospital emergency room

USAGE
=====

    005 file
    005 -h

DESCRIPTION
===========

    "005 file" read input file such as patients arrive at the hospital,
    push each partients into priority queue then pop and display items
    until priority queue is empty. For example:

        005-input.txt
        -------------------------
        10  kevin leptons
        3   ariana grade
        5   katty perry
        6   lady gaga
        2   michael jackson
        4   mika
        8   eminem
        0   ceelo green
        -------------------------
    
        First column is priority value, second column is name of patients and
        first line is first partients arrive to hospital. Priority value is
        non-negative numbers and sorted by increasement. It is mean that who
        have smaller priority value and early arrive then will be go to
        emergency soon.
    
    "005 -h" display help information.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <cs106b/error.h>
#include <cs106b/pqueue.h>
#include <cs106b/mem.h>

static int _pqueue_load(struct pqueue *q, const char *file);
static int _pqueue_parse(const char *line, struct pqueue_item *item);
static void _cli_help(void);

int main(int argc, char *argv[])
{
    int opt;
    struct pqueue q;
    char *data;
    size_t priority;
    int ret;

    // verify options
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

    // verify arguments
    if (argc != 2) {
        _cli_help();
        return EXIT_FAILURE;
    }

    // load input file
    ret = EXIT_FAILURE;
    pqueue_init(&q, PQUEUE_INC);
    if (_pqueue_load(&q, argv[1]))
        goto finish;

    // show order of people
    for (;;) {
        if (pqueue_tpri(&q, &priority)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                goto free_q;
            }
        }
        if (pqueue_pop(&q, (void *) &data))
            goto free_q;
        printf("%02zu - %s\n", priority, data);
    }
    ret = EXIT_SUCCESS;

free_q:
    pqueue_free(&q);
finish:
    if (espace_check())
        fprintf(stderr, "Error :%s\n", espace->id);
    return ret;
}

static int _pqueue_load(struct pqueue *q, const char *file)
{
    int fd;
    char *data;
    char *p;
    char *s;
    char *e;
    ssize_t rsize;
    ssize_t dsize;
    struct stat stat;
    struct pqueue_item item;
    char *item_data;
    int ret;
    size_t item_size;

    ret = -1;
    fd = open(file, O_RDONLY);
    if (fd < 0) {
        sys_raise(errno);
        goto finish;
    }
    if (fstat(fd, &stat)) {
        sys_raise(errno);
        goto free_fd;
    }

    dsize = stat.st_size + 1;
    if (cs106b_malloc((void *) &data, sizeof(char) * dsize))
        goto free_fd;

    for (p = data; ;) {
        rsize = read(fd, p, 1024);
        if (rsize == 0)
            break;
        if (rsize < 0) {
            sys_raise(errno);
            goto free_data;
        }
        p += rsize;
    }

    e = data + dsize - 1;
    *e = '\n';
    for (s = data, p = s; p <= e; p++) {
        if (*p != '\n')
            continue;
        *p = 0;
        if (p - s == 0)
            continue;
        if (_pqueue_parse(s, &item))
            goto free_data;
        item_size = sizeof(char) * strlen(item.data) + 1;
        if (cs106b_malloc((void *) &item_data, item_size))
            goto free_data;
        strcpy(item_data, item.data);
        if (pqueue_push(q, item_data, item.priority))
            goto free_data;
        s = p + 1;
    }

    ret = 0;

free_data:
    free(data);
free_fd:
    close(fd);
finish:
    return ret;
}

static int _pqueue_parse(const char *line, struct pqueue_item *item)
{
    char *p;
    char *s;
    char *e;

    e = (char *) line + strlen(line);

    // skip for space
    for (s = (char *) line; *s != 0; s++) {
        if (*s != ' ')
            break;
    }
    if (s == e) {
        espace_raise(CS106B_EFORMAT);
        return -1;
    }

    // get priority
    for (p = s; *p != 0; p++) {
        if (*p == ' ')
            break;
    }
    if (p == e) {
        espace_raise(CS106B_EFORMAT);
        return -1;
    }
    *p = 0;
    item->priority = atoi(s);
    *p = ' ';

    // get data
    for (; *p != 0; p++) {
        if (*p != ' ')
            break;
    }
    item->data = p;

    return 0;
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    005 file\n");
    printf("    005 -h\n\n");

    printf("ARGUMENTS\n\n");
    printf("    file        Path to input file\n");
    printf("                An sample file located in asset/005-input.txt\n");
}
