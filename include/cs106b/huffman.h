/*
NAME

    huffman - data compression api.

SYNOPSIS

    int huff_encode(const char *src, const char *dest);
    int huff_decode(const char *src, const char *dest);
    int huff_rinfo(const char *file, struct huff_info *info);

DESCRIPTION

    huff_encode() compress data from source file and write compressed data
    into destination file.

    huff_decode() decompress compressed data from source file and write
    uncompressed data into destination file.

    huff_rinfo() read information about compression in compressed file.

RETURNS

    huff_encode(), huff_decode(), huff_rinfo() on success return 0, on failure
    return -1.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
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
