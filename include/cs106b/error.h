#ifndef __CS106B_ERROR_H__
#define __CS106B_ERROR_H__

#include <stdlib.h>
#include <equeue/equeue.h>

#define CS106BE_NONE 0
#define CS106BE_OUTMEM 1

void exit_err(const char *caller);
const char * cs106b_errstr(size_t code);

EQUEUE_DECLARE(cs106berr, cs106b_errstr)

#endif
