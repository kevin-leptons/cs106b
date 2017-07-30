/*
NAME
    
    005 - Model a hospital emergency room

SYNOPSIS

    005 file
    005 -h

DESCRIPTION

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
        first line is first partients arrive to hospital.  Priority value is
        non-negative numbers and sorted by increasement.  It is mean that who
        have smaller priority value and early arrive then will be go to
        emergency soon.
    
    "005 -h" display help information.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/pqueue.h>

static int _pqueue_load(struct pqueue *q, const char *file);
static int _pqueue_parse(const char *line, struct pqueue_item *item);
static void _cli_help(void);

int main(int argc, char *argv[])
{
    int opt;
    struct pqueue q;
    char *data;

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

    if (pqueue_init(&q, PQUEUE_SMALLEST))
        exit_err("pqueue_init");
    if (_pqueue_load(&q, argv[1]))
        exit_err("_pqueue_load");

    data = pqueue_top(&q);
    if (data == NULL)
        exit_err("pqueue_top");
    printf("top: %s\n", (char *) data);

    for (;;) {
        data = pqueue_pop(&q);
        if (data == NULL)
            break;
        printf("pop: %s\n", data);
    }

    pqueue_free(&q);
    return EXIT_SUCCESS;
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

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -1;
    if (fstat(fd, &stat))
        return -1;

    dsize = stat.st_size + 1;
    data = malloc(sizeof(char) * dsize);
    if (data == NULL)
        return -1;

    for (p = data; ;) {
        rsize = read(fd, p, 1024);
        if (rsize == 0)
            break;
        if (rsize < 0)
            goto ERROR;
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
            goto ERROR;
        item_data = malloc(sizeof(char) * strlen(item.data) + 1);
        if (item_data == NULL)
            goto ERROR;
        strcpy(item_data, item.data);
        if (pqueue_push(q, item_data, item.priority))
            goto ERROR;
        s = p + 1;
    }

    free(data);
    return 0;

ERROR:
    if (data != NULL)
        free(data);

    return -1;
}

static int _pqueue_parse(const char *line, struct pqueue_item *item)
{
    char *p;
    char *s;
    char *e;

    e = (char *) line + strlen(line);

    // get priority
    for (s = (char *) line; *s != 0; p++) {
        if (*s != ' ')
            break;
    }
    if (s == e)
        return -1;
    for (p = s; *p != 0; p++) {
        if (*p == ' ')
            break;
    }
    if (s == e)
        return -1;
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
