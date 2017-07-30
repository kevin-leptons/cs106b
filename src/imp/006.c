/*
SYNOPSIS

    006 encode <src> <dest>
    006 decode <src> <dest>
    006 info   <file>
    006 -h

DESCRIPTION

    "encode" encode source file to dest file.

    "decode" decode source file to dest file.

    "info" show information about encoded file.

EXAMPLES

    try encode then decode a file 

    $ mkdir -p tmp
    $ 006 encode asset/006-input.txt tmp/006-encoded.huf
    $ 006 decode tmp/006-encoded.huf tmp/006-decoded.txt

    verify decoded file

    $ diff tmp/006-decoded.txt asset/006-input.txt

RETURNS

    On success return 0, on failure return 1.

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/huffman.h>

static void _cli_help(void);

int main(int argc, char *argv[])
{
    int opt;
    struct huff_info info;

    for (;;) {
        opt = getopt(argc, argv, "h");
        if (opt < 0)
            break;
        if (opt == 'h') {
            _cli_help();
            return EXIT_SUCCESS;
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    }

    if (argc == 4) {
        if (strcmp(argv[1], "encode") == 0) {
            if (huff_encode(argv[2], argv[3]))
                exit_err("huff_encode");
        } else if (strcmp(argv[1], "decode") == 0) {
            if (huff_decode(argv[2], argv[3]))
                exit_err("huff_decode");
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "info") == 0) {
            if (huff_rinfo(argv[2], &info))
                exit_err("huff_info");
            printf("header size         %lu bytes\n", sizeof(info));
            printf("tree size           %lu bytes\n", info.tree_size);
            printf("encoded size        %lu bytes, %lu bits\n",
                    info.enc_byte_size, info.enc_bit_size);
            printf("origin size         %lu bytes\n", info.raw_size);
            printf("compress ratio      %.4f\n",
                    (float) info.raw_size / (info.enc_byte_size + 1));
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    } else {
        _cli_help();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void _cli_help(void)
{
    printf("USAGE\n\n");
    printf("    006     encode <src> <dest>\n");
    printf("    006     decode <src> <dest>\n");
    printf("    006     info   <file>\n");
    printf("    006     -h\n\n");

    printf("COMMANDS\n\n");
    printf("    encode  Encode source file to dest file\n");
    printf("    decode  Decode encoded file in source to dest file\n");
    printf("    info    Show information about encoded file\n");
}
