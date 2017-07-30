#include <cs106b/bifile.h>

#include <sys/types.h>
#include <unistd.h>

#define BIFILE_BUF_MAX 8 * 1024     // max number of bits of buffer

int static _bit_at(char byte, size_t index); // index from left to right

int bifile_init(struct bifile *f, int fd, size_t bit_off)
{
    f->fd = fd;
    f->bit_off = bit_off;

    if (bitvec_init(&f->wbuf))
        return -1;

    // read buffer
    f->rbuf_off = 0;
    f->rbuf_max_size = 0;

    return 0;
}

int bifile_write(struct bifile *f, uint8_t bit)
{
    if (bitvec_add(&f->wbuf, bit))
        return -1;

    if (f->wbuf.size >= BIFILE_BUF_MAX) {
        if (bifile_flush(f))
            return -1;
    }

    return 0;
}

int bifile_writev(struct bifile *f, struct bitvec *v)
{
    if (bitvec_cat(&f->wbuf, v))
        return -1;

    if (f->wbuf.size >= BIFILE_BUF_MAX) {
        if (bifile_flush(f))
            return -1;
    }

    return 0;
}

int bifile_flush(struct bifile *f)
{
    struct bitvec new_v;
    char fbyte;
    size_t bit_size;
    size_t byte_size;
    size_t i;

    if (f->wbuf.size == 0)
        return 0;

    bit_size = f->wbuf.size + f->bit_off;
    byte_size = bit_size / 8;
    if (bit_size % 8)
        byte_size += 1;

    if (bitvec_init(&new_v))
        return -1;
    if (bitvec_resize(&new_v, bit_size))
        return -1;

    if (f->bit_off != 0) {
        if (read(f->fd, &fbyte, 1) != 1)
            goto ERROR;
        if (lseek(f->fd, -1, SEEK_CUR) < 0)
            goto ERROR;
        for (i = 0; i < f->bit_off; i++)
           if (bitvec_add(&new_v, _bit_at(fbyte, i)))
                goto ERROR;
    }

    if (bitvec_cat(&new_v, &f->wbuf))
        goto ERROR;
    if (bitvec_clear(&f->wbuf))
        goto ERROR;
    if (f->wbuf.size > BIFILE_BUF_MAX) {
        if (bitvec_resize(&f->wbuf, BIFILE_BUF_MAX))
            goto ERROR;
    }

    if (write(f->fd, new_v.items, byte_size) != byte_size)
        goto ERROR;

    f->bit_off = new_v.size % 8;
    if (f->bit_off != 0) {
        if (lseek(f->fd, -1, SEEK_CUR) < 0)
            goto ERROR;
    }

    return 0;

ERROR:
    bitvec_free(&new_v);
    return -1;
}


int bifile_read(struct bifile *f, uint8_t *bit)
{
    ssize_t rsize;
    size_t byte_off;
    size_t bit_off;
    char ibyte;

    if (f->rbuf_off == f->rbuf_max_size) {
        rsize = read(f->fd, f->rbuf, sizeof(f->rbuf));
        if (rsize == 0)
            return 0;
        if (rsize < 0)
            return -1;
        f->rbuf_max_size = 8 * rsize;
        f->rbuf_off = 0;
    }

    byte_off = f->rbuf_off / 8;
    bit_off = f->rbuf_off % 8;
    ibyte = f->rbuf[byte_off];

    *bit = ibyte & (1 << (7 - bit_off));
    *bit = *bit != 0;

    f->rbuf_off += 1;
    return 1;
}

void bifile_free(struct bifile *f)
{
    bitvec_free(&f->wbuf);
}

int static _bit_at(char byte, size_t index)
{
    char bit;

    bit = byte & (1 << (7 - index));

    return bit != 0;
}
