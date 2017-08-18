#include <cs106b/error.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CS106BE_NUM 2
#define CS106BES_UNKNOW "Unknow Error"

static const char *CS106BES[CS106BE_NUM] = {
    "No Error",
    "Out of Memory"
};

void exit_err(const char *caller)
{
    fprintf(stderr, "Error: %s:%s\n", caller, strerror(errno));
    exit(EXIT_FAILURE);
}

void msg_err(const char *caller)
{
    fprintf(stderr, "Error: %s:%s\n", caller, strerror(errno));
}

const char * cs106b_errstr(size_t code)
{
    if (code >= CS106BE_NUM)
        return CS106BES_UNKNOW;
    return CS106BES[code];
}

EQUEUE_DEFINE(cs106berr, cs106b_errstr)
