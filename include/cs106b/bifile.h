/*
NAME
====

    bifile - bit file.

SYNOPSIS
=========

    void bifile_init(struct bifile *f, int fd, size_t bit_off);
    void bifile_free(struct bifile *f);
    int bifile_write(struct bifile *f, uint8_t bit);
    int bifile_writev(struct bifile *f, struct bitvec *v);
    int bifile_flush(struct bifile *f);
    int bifile_read(struct bifile *f, uint8_t *bit);

DESCRIPTION
===========

    bifile_init() and bifile_free() construct/destruct an bit file.

    bifile_write() write an bit to file.

    bifile_writev() write an bit vector to file.
    
    bifile_write() and bifile_writev(), data doesn't ensures to write all of
    data to file directly, it can be store in memory buffer. To ensure data is
    write to file, call bifile_flush().

    bifile_flush() push all of data in buffer to file.

    bifile_read() read an bit.

ARGUMENTS
=========

    <fd> is file descriptor.

    <bit_off> is offset of bit in current file offset. For now, it ignore
    specific and always set it to 0.

    <v> is vector of bits.

RETURNS
=======

    On success, return 0. On failure return -1.

ERRORS
======

    CS106B_EINDEX               End of file
    SYS_ENOMEM                  System memory is full

EXAMPLES
========

    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <fcntl.h>

    #include <cs106b/bifile.h>

    int fd;
    struct bifile f;

    fd = open("/tmp/data");
    if (fd < 0)
        perror("open");

    if (bifile_init(&f, fd, 0)) 
        perror("bifile_init");

    if (bifile_write(&f, 0))
        perror("bifile_write");
    if (bifile_write(&f, 1))
        perror("bifile_write");
*/

#ifndef _CS106B_BITFILE_H_
#define _CS106B_BITFILE_H_

#include <inttypes.h>

#include <cs106b/bitvec.h>

struct bifile
{
    int fd;
    size_t bit_off;             // bit offset in file

    struct bitvec wbuf;         // write buffer

    uint8_t rbuf[1024];         // read buffer
    size_t rbuf_off;            // offset of reading bit
    size_t rbuf_max_size;       // size in bits
};

void bifile_init(struct bifile *f, int fd, size_t bit_off);
void bifile_free(struct bifile *f);
int bifile_write(struct bifile *f, uint8_t bit);
int bifile_writev(struct bifile *f, struct bitvec *v);
int bifile_flush(struct bifile *f);
int bifile_read(struct bifile *f, uint8_t *bit);

#endif
