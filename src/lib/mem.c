#include <cs106b/mem.h>
#include <stdlib.h>
#include <cs106b/error.h>

int cs106b_malloc(void **mem, size_t size)
{
    *mem = malloc(size);
    if (*mem == NULL) {
        espace_raise(SYS_ENOMEM);
        return -1;
    }
    return 0;
}
