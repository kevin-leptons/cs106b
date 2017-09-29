#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <cs106b/error.h>
#include <cs106b/bifile.h>
#include <cs106b/bitvec.h>

#define FILE_MODE S_IRWXU | S_IRWXG | S_IRWXO
#define VEC_NUM 3
#define VEC_SIZE 11
#define RBUF_MAX_BIT 8 * 1024

char bits[VEC_NUM][VEC_SIZE] = {
    {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0},
    {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1}
};

static void _read(const char *file);
static void _write(const char *file);
static void _read_write(const char *src, const char *dest);

static int _bitvec_push(struct bitvec *v, char *bits, size_t size);
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
    struct bitvec vectors[VEC_NUM];
    int fd;
    struct bifile ofile;
    size_t i;

    for (i = 0; i < VEC_NUM; i++) {
        bitvec_init(vectors + i);
        if (_bitvec_push(vectors + i, bits[i], VEC_SIZE))
            exit_err("_bitvec_push");
    }

    for (i = 0; i < VEC_NUM; i++) {
        if (bitvec_dump(vectors + i))
            exit_err("bitvec_dump");
        printf("\n");
    }

    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, FILE_MODE);
    if (fd < 0) {
        sys_raise(errno);
        errno = 0;
        exit_err("open");
    }
    bifile_init(&ofile, fd, 0);

    for (i = 0; i < VEC_NUM; i++) {
        if (bifile_writev(&ofile, vectors + i))
            exit_err("bifile_writev");
    }
    if (bifile_flush(&ofile))
        exit_err("bifile_flush");

    for (i = 0; i < VEC_NUM; i++)
        bitvec_free(vectors + i);

    close(fd);
    bifile_free(&ofile) ;
    exit(0);
}

static void _read(const char *file)
{
    int fd;
    struct bifile ifile;
    uint8_t bit;
    size_t i;
    size_t k;
    int ret;
    
    ret = EXIT_FAILURE;
    fd = open(file, O_RDONLY);
    if (fd < 0) {
        sys_raise(errno);
        errno = 0;
        goto finish;
    }
    bifile_init(&ifile, fd, 0);

    i = 0;
    k = 0;
    for (;;) {
        if (bifile_read(&ifile, &bit)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                goto free_ifile;
            }
        }
        if (bit)
            printf("1");
        else
            printf("0");
        k += 1;
        if (k == 8) {
            k = 0;
            i += 1;
            printf(" ");
        }

        if (i == 6) {
            i = 0;
            printf("\n");
        }
    }
    printf("\n");
    ret = EXIT_SUCCESS;

free_ifile:
    bifile_free(&ifile);
    close(fd);
finish:
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    exit(ret);
}

static void _read_write(const char *src, const char *dest)
{
    struct bitvec rbuf;
    uint8_t bit;
    struct bifile ifile;
    struct bifile ofile;
    int sfd;
    int dfd;
    size_t rbit_count;
    size_t wbit_count;

    sfd = open(src, O_RDONLY);
    if (sfd < 0) {
        sys_raise(errno);
        errno = 0;
        exit_err("open");
    }
    dfd = open(dest, O_CREAT | O_TRUNC | O_RDWR, FILE_MODE);
    if (dfd < 0) {
        sys_raise(errno);
        errno = 0;
        exit_err("open");
    }

    bitvec_init(&rbuf);
    if (bitvec_resize(&rbuf, RBUF_MAX_BIT))
        exit_err("bitvec_resize");

    bifile_init(&ifile, sfd, 0);
    bifile_init(&ofile, dfd, 0);

    rbit_count = 0;
    wbit_count = 0;
    for (;;) {
        if (bifile_read(&ifile, &bit)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                exit_err("bifile_read");
            }
        }
        rbit_count += 1;

        if (bitvec_add(&rbuf, bit))
            exit_err("bitvec_add");
        if (rbuf.size == RBUF_MAX_BIT) {
            if (bifile_writev(&ofile, &rbuf))
                exit_err("bifile_writev");
            wbit_count += rbuf.size;
            if (bitvec_clear(&rbuf))
                exit_err("bitvec_clear");
        }
    }
    if (bifile_writev(&ofile, &rbuf))
        exit_err("bifile_writev");
    wbit_count += rbuf.size;
    if (bifile_flush(&ofile))
        exit_err("bifile_flush");

    printf("read: %lu bits = %lu bytes + %lu bits\n", 
            rbit_count, rbit_count / 8, rbit_count % 8);
    printf("write: %lu bits = %lu bytes + %lu bits\n",
            wbit_count, wbit_count / 8, wbit_count % 8);
    if (rbit_count == wbit_count)
        printf("ok\n");
    else
        printf("failure\n");

    bitvec_free(&rbuf);
    bifile_free(&ifile);
    bifile_free(&ofile);
    close(sfd);
    close(dfd);
    exit(0);
}

static int _bitvec_push(struct bitvec *v, char bits[], size_t size)
{
    size_t i;

    for (i = 0; i < size; i++) {
        if (bitvec_add(v, bits[i]))
            return -1;
    }

    return 0;
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    bifile write file\n");
    printf("    bifile read file\n");
    printf("    bifile rw src dest\n\n");

    printf("ARGUMENTS\n");
    printf("    file        Path to file to read or write\n");
    printf("    src, dest   Read from src and write to dest\n");
}
