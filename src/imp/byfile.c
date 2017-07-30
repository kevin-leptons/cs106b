#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/byfile.h>

#define FILE_MODE S_IRWXU | S_IRWXG | S_IRWXO

char bytes[] = "hello, i am kevin. who are you?\n";

static void _write(const char *file);
static void _read(const char *file);
static void _read_write(const char *src, const char *dest);

static void _cli_help(void);

int main(int argc, char *argv[])
{
    if (argc == 3) {
        if (strcmp(argv[1], "write") == 0) {
            _write(argv[2]);
        } else if (strcmp(argv[1], "read") == 0) {
            _read(argv[2]);
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    } else if (argc == 4) {
        if (strcmp(argv[1], "rw") == 0) {
            _read_write(argv[2], argv[3]);
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    } else {
        _cli_help();
        return EXIT_FAILURE;
    }
}

static void _write(const char *file)
{
    int fd;
    struct byfile ofile;
    size_t i;

    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, FILE_MODE);
    if (fd < 0)
        exit_err("open");
    if (byfile_init(&ofile, fd))
        exit_err("byfile_init");

    for (i = 0; i < strlen(bytes); i++) {
        if (byfile_write(&ofile, bytes[i])) 
            exit_err("byfile_write");
    }
    if (byfile_flush(&ofile))
        exit_err("byfile_flush");

    exit(0);
}

static void _read(const char *file)
{
    int fd;
    struct byfile ifile;
    uint8_t byte;
    int r;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        exit_err("open");
    if (byfile_init(&ifile, fd))
        exit_err("byfile_init");

    for (;;) {
        r = byfile_read(&ifile, &byte);
        if (r == 0)
            break;
        if (r < 0)
            exit_err("byfile_read");
        printf("%c", byte);
    }

    exit(0);
}

static void _read_write(const char *src, const char *dest)
{
    int sfd;
    int dfd;
    struct byfile ifile;
    struct byfile ofile;
    int r;
    uint8_t byte;

    sfd = open(src, O_RDONLY);
    if (sfd < 0)
        exit_err("open");
    dfd = open(dest, O_CREAT | O_TRUNC | O_RDWR, FILE_MODE);
    if (dfd < 0)
        exit_err("open");

    if (byfile_init(&ifile, sfd))
        exit_err("byfile_init");
    if (byfile_init(&ofile, dfd))
        exit_err("byfile_init");

    for (;;) {
        r = byfile_read(&ifile, &byte);
        if (r == 0)
            break;
        if (r < 0)
            exit_err("byfile_read");

        if (byfile_write(&ofile, byte))
            exit_err("byfile_write");
    }
    if (byfile_flush(&ofile))
        exit_err("byfile_flush");

    byfile_free(&ifile);
    byfile_free(&ofile);
    close(sfd);
    close(dfd);

    exit(0);
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    write file\n");
    printf("    read file\n");
    printf("    rw src dest\n\n");

    printf("ARGUMENTS\n");
    printf("    file            Path to file to read or write\n");
    printf("    src, dest       Read from src and write to dest\n");
}
