/*
NAME

    byfile - byte file.

SYNOPSIS

    int byfile_init(struct byfile *f, int fd);
    int byfile_write(struct byfile *f, uint8_t byte);
    int byfile_flush(struct byfile *f);
    int byfile_read(struct byfile *f, uint8_t *byte);
    void byfile_free(struct byfile *f);

DESCRIPTION

    byfile_init() construct an byte file from file descriptor.

    byfile_write() write one byte to file. It is not ensure that byte will be
    write to file directly, it may be stores in memory buffer. To ensure that,
    call byfile_flush().

    byfile_flush() write all of data in buffer to file.

    byfile_read() read one byte from file.

    byfile_free() release all of resources which uses by byfile.

ARGUMENTS

    fd is file descriptor.

RETURNS

    byfile_init(), byfile_write(), byfile_flush(), on success return 0, on
    failure return -1.

    byfile_read() on success return 1, on end of file return 0, on failure
    return -1.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_BYFILE_H__
#define __CS106B_BYFILE_H__

#include <stdlib.h>
#include <inttypes.h>

struct byfile
{
    int fd;
    uint8_t rbuf[1024];
    size_t rbuf_off;
    size_t rbuf_size;

    uint8_t wbuf[1024];
    size_t wbuf_size;
};

int byfile_init(struct byfile *f, int fd);
int byfile_read(struct byfile *f, uint8_t *byte);
int byfile_write(struct byfile *f, uint8_t byte);
int byfile_flush(struct byfile *f);
void byfile_free(struct byfile *f);

#endif
