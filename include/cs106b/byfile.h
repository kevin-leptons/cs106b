/*
NAME
====

    byfile - byte file.

SYNOPSIS
========

    void byfile_init(struct byfile *f, int fd);
    void byfile_free(struct byfile *f);
    int byfile_read(struct byfile *f, uint8_t *byte);
    int byfile_write(struct byfile *f, uint8_t byte);
    int byfile_flush(struct byfile *f);

DESCRIPTION
===========

    byfile_init() and byfile_free() construct/destruct an byte file.

    byfile_write() write one byte to file. It is not ensure that byte will be
    write to file directly, it may be stores in memory buffer. To ensure that,
    call byfile_flush().

    byfile_flush() write all of data in buffer to file.

    byfile_read() read one byte from file.

ARGUMENTS
=========

    <fd> is file descriptor.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======
    
    CS106B_EINDEX               End of file
    SYS_NOMEM                   System memory is full
*/

#ifndef _CS106B_BYFILE_H_
#define _CS106B_BYFILE_H_

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

void byfile_init(struct byfile *f, int fd);
void byfile_free(struct byfile *f);
int byfile_read(struct byfile *f, uint8_t *byte);
int byfile_write(struct byfile *f, uint8_t byte);
int byfile_flush(struct byfile *f);

#endif
