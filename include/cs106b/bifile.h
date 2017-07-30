/*
NAME

    bifile - bit file.

SYNOPSIS

    int bifile_init(struct bifile *f, int fd, size_t bit_off);
    int bifile_write(struct bifile *f, struct bitvec *v);
    int bifile_flush(struct bifile *f);
    int bifile_read(struct bifile *f, char *bit);
    void bifile_free(struct bifile *f);

DESCRIPTION

    bifile_init() construct an bit file from file descriptor.

    bifile_write() write an bit to file.

    bifile_writev() write an bit vector to file.
    
    With bifile_write() and bifile_writev(), data will not ensure to write to
    file directly, it can be store in memory buffer. To ensure data is write
    to file, call bifile_flush().

    bifile_flush() push all of data in buffer to file.

    bifile_read() read an bit.

    bifile_free() release all of resource which use by bifile.

ARGUMENTS

    fd is file descriptor.

    bit_off is offset of bit in current file offset. For now, it ignore
    specific and always set it to 0.

    v is vector of bits.

RETURNS

    bifile_init(), bifile_write(), bifile_writev(), bifile_flush() on success
    return 0, on failure return -1.

    bifile_read() on success return 1, on end of file return 0, on failure
    return -1.

EXAMPLES

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

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_BITFILE_H__
#define __CS106B_BITFILE_H__

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

int bifile_init(struct bifile *f, int fd, size_t bit_off);
int bifile_write(struct bifile *f, uint8_t bit);
int bifile_writev(struct bifile *f, struct bitvec *v);
int bifile_flush(struct bifile *f);
int bifile_read(struct bifile *f, uint8_t *bit);
void bifile_free(struct bifile *f);

#endif
