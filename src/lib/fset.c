#include <cs106b/fset.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define READ_BLOCK_SIZE 1024

int fset_load(struct set *set, const char *file)
{
    int fd;
    struct stat stat;
    char *fdata;
    char *s;
    char *p;
    char *e;
    ssize_t rsize;
    size_t dict_size;
    int ret;

    ret = -1;
    fd = open(file, O_RDONLY);
    if (fd < 0) {
        sys_raise(errno);
        errno = 0;
        return -1;
    }
    if (fstat(fd, &stat)) {
        sys_raise(errno);
        errno = 0;
        goto free_fd;
    }
    if (cs106b_malloc((void *) &fdata, stat.st_size + 1))
        goto free_fd;

    for (p = fdata; ;) {
        rsize = read(fd, p, READ_BLOCK_SIZE);
        if (rsize == 0)
            break;
        if (rsize < 0) {
            sys_raise(errno);
            errno = 0;
            goto free_fdata;
        }
        p += rsize;
    }

    e = fdata + stat.st_size;
    dict_size = 0;
    for (s = fdata, p = s; p <= e; p++) {
        if (*p == '\n' || p == e) {
            *p = 0;
            if (p - s != 0)
                dict_size += 1;
            s = p + 1;
        }
    }

    if (set_resize(set, 2 * dict_size))
        goto free_set;

    for (s = fdata, p = s; p <= e; p++) {
        if (*p == 0) {
            if (p - s != 0) {
                if(set_add(set, s))
                    goto free_set;
            }
            s = p + 1;
        }
    }

    ret = 0;
    goto free_fdata;

free_set:
    set_free(set);
free_fdata:
    free(fdata);
free_fd:
    close(fd);
    return ret;
}
