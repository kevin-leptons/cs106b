#ifndef __CS106B_ERROR_H__
#define __CS106B_ERROR_H__

#include <stdlib.h>
#include <espace/error.h>
#include <espace/sys.h>

ESPACE_ERRDEC(CS106B_EINDEX);
ESPACE_ERRDEC(CS106B_EKEY);
ESPACE_ERRDEC(CS106B_EFORMAT);

void exit_err(const char *caller);

#endif
