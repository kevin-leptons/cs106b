/*
NAME
====

    huffman - data compression api.

SYNOPSIS
========

    int huff_encode(const char *src, const char *dest);
    int huff_decode(const char *src, const char *dest);
    int huff_rinfo(const char *file, struct huff_info *info);

DESCRIPTION
===========

    huff_encode() compress data from source file and write compressed data
    into destination file.

    huff_decode() decompress compressed data from source file and write
    uncompressed data into destination file.

    huff_rinfo() read information about compression in compressed file.

RETURNS
=======

    On success, return 0. On failure, return -1.

ERRORS
======

    SYS_ENOMEM                  System memory is full
    SYS_EACCESS                 File is invalid
    SYS_EPERM                   Read or write prermission to file
*/
    
#ifndef __CS106B_HUFFMAN_H__
#define __CS106B_HUFFMAN_H__

#include <stdlib.h>

struct huff_info
{
    size_t tree_size;           // number of bits
    size_t enc_byte_size;       // number of bytes of encoded data
    size_t enc_bit_size;        // number of bits in last byte encoded data
    size_t raw_size;            // real size of file before encoded
};

int huff_encode(const char *src, const char *dest);
int huff_decode(const char *src, const char *dest);
int huff_rinfo(const char *file, struct huff_info *info);

#endif
