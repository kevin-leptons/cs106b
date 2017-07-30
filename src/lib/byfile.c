#include <cs106b/byfile.h>

#include <unistd.h>

int byfile_init(struct byfile *f, int fd)
{
    f->fd = fd;
    f->rbuf_off = 0;
    f->rbuf_size = 0;
    f->wbuf_size = 0;

    return 0;
}

int byfile_read(struct byfile *f, uint8_t *byte)
{
    size_t rsize;

    if (f->rbuf_size == 0 || f->rbuf_off == f->rbuf_size) {
        rsize = read(f->fd, f->rbuf, sizeof(f->rbuf));
        if (rsize == 0)
            return 0;
        if (rsize < 0)
            return -1;
        f->rbuf_off = 0;
        f->rbuf_size = rsize;
    }

    *byte = f->rbuf[f->rbuf_off];
    f->rbuf_off += 1;

    return 1;
}

int byfile_write(struct byfile *f, uint8_t byte)
{
    if (f->wbuf_size == sizeof(f->wbuf)) {
        if (write(f->fd, f->wbuf, sizeof(f->wbuf)) < 0)
            return -1;
        f->wbuf_size = 0;
    } 

    f->wbuf_size += 1;
    f->wbuf[f->wbuf_size - 1] = byte;

    return 0;
}

int byfile_flush(struct byfile *f)
{
    if (f->wbuf_size == 0)
        return 0;

    if (write(f->fd, f->wbuf, f->wbuf_size) < 0)
        return -1;
    f->wbuf_size = 0;

    return 0;
}

void byfile_free(struct byfile *f)
{
}
