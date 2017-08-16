#include <stdlib.h>
#include <stdio.h>

#include <equeue/equeue.h>

#define ERR_STR_SIZE 1024

const char * cs106berrstr(size_t code)
{
    return "This is example error string";
}

EQUEUE_DEFINE(cs106berr, cs106berrstr)

int main(int argc, char *argv[])
{
    struct equeue_eitem *error;
    char errstr[ERR_STR_SIZE];

    cs106berr_push(111);
    error = equeue_pop();
    equeue_errstr(error, errstr, ERR_STR_SIZE);
    printf("%s\n", errstr);

    return EXIT_SUCCESS;
}
