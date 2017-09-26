#include <cs106b/error.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <espace/error.h>

ESPACE_ERRDEF(CS106B_EINDEX);
ESPACE_ERRDEF(CS106B_EKEY);
ESPACE_ERRDEF(CS106B_EFORMAT);

void exit_err(const char *caller)
{
    fprintf(stderr, "Error: %s:%s\n", espace->id, caller);
    exit(EXIT_FAILURE);
}

void msg_err(const char *caller)
{
    fprintf(stderr, "Error: %s:%s\n", espace->id, caller);
}
