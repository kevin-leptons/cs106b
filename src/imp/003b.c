/*
SYNOPSIS

    Assignment 003b
    Grammar solver
    ADT: htable, vector, queue
    Algorithm: Recursion

USAGE

    $ ./dest/bin/003b asset/003b-input.txt

AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cs106b/error.h>
#include <cs106b/htable.h>
#include <cs106b/vector.h>
#include <cs106b/queue.h>
#include <cs106b/random.h>

#define GRM_ASGN_START ':'
#define GRM_NTERM_START '<'
#define GRM_NTERM_END '>'
#define GRM_CASE_START '|'
#define GRM_IGNORE_CHAR ' '

enum grm_flag
{
    GRM_FLAG_NONE,
    GRM_FLAG_ASGN,
    GRM_FLAG_NTERM,
    GRM_FLAG_TERM,
    GRM_FLAG_CASE
};

enum grm_token_type
{
    GRM_TOKEN_ASGN,
    GRM_TOKEN_TERM,
    GRM_TOKEN_NTERM,
    GRM_TOKEN_CASE
};

struct grm_token
{
    enum grm_token_type type;
    char *value;
};

struct grm
{
    struct htable map;      // map <symbol> => vector<vector<token>>
};

static int _grm_init(struct grm *grm, const char *file);
static char * _grm_make(struct grm *grm, const char *nterm);
static void _grm_free(struct grm *grm);
static struct vector * _grm_expand(struct grm *grm, struct grm_token *token);
static int _grm_add_expr(struct grm *grm, const char *expr);
static struct vector * _grm_expr_parse(const char *expr);
static int _grm_is_term(const char c);
static void _cli_help(void);

int main(int argc, char *argv[])
{
    int opt;
    struct grm grm;
    char *s;

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

    if (argc != 2) {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (_grm_init(&grm, argv[1]))
        exit_err("_grm_init");

    s = _grm_make(&grm, "s");
    if (s == NULL)
        exit_err("_grm_make");
    printf("%s\n", s);

    _grm_free(&grm);
    return EXIT_SUCCESS;
}

static int _grm_init(struct grm *grm, const char *file)
{
    int fd;
    ssize_t rsize;
    char *fdata;
    struct stat stat;
    ssize_t dsize;
    char *s;
    char *p;
    char *e;

    fdata = NULL;

    if (htable_init(&grm->map))
        goto ERROR;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        goto ERROR;
    if (fstat(fd, &stat))
        goto ERROR;

    // load file to memory
    dsize = stat.st_size + 1;
    fdata = malloc(dsize);
    if (fdata == NULL)
        goto ERROR;
    for (p = fdata; ;) {
        rsize = read(fd, fdata, 1024);
        if (rsize == 0)
            break;
        if (rsize < 0)
            goto ERROR;
        p += rsize;
    }
    fdata[dsize - 1] = 0;
    close(fd);
    fd = -1;

    // parse expression from file data
    e = fdata + dsize - 1;
    for (s = fdata, p = s; p <= e; ++p) {
        if (*p != '\n' && *p != 0)
            continue;
        *p = 0;
        if (strlen(s) != 0) {
            if (_grm_add_expr(grm, s))
                goto ERROR;
        }
        s = p + 1;
    }

    free(fdata);
    return 0;

ERROR:
    if (fd >= 0)
        close(fd);
    if (fdata != NULL)
        free(fdata);

    return -1;
}

static char * _grm_make(struct grm *grm, const char *nterm)
{
    struct vector *rv;              // result as vector<token>
    char *rc;                       // result as string
    struct grm_token nterm_token;
    struct grm_token *token;
    size_t i;
    size_t rc_size;
    size_t value_size;
    char *p;

    rc = NULL;

    // token will be expand
    nterm_token.type = GRM_TOKEN_NTERM;
    nterm_token.value = malloc(sizeof(nterm) + 1);
    strcpy(nterm_token.value, nterm);

    // expand 
    rv = _grm_expand(grm, &nterm_token);
    if (rv == NULL)
        return NULL;

    // detect size of all of characters
    rc_size = 0;
    for (i = 0; i < rv->size; i++) {
        token = vector_at(rv, i);
        if (token == NULL)
            return NULL;
        rc_size += strlen((char *) token->value);
    }
    rc_size += 1;

    // copy data from vector to one-block memory
    rc = malloc(rc_size);
    if (rc == NULL)
        return NULL;
    rc[rc_size - 1] = 0;
    p = rc;
    for (i = 0; i < rv->size; i++) {
        token = vector_at(rv, i);
        if (token == NULL)
            goto ERROR;
        value_size = strlen(token->value);
        memcpy(p, token->value, value_size);
        p += value_size;
    }

    return rc;

ERROR:
    if (rc != NULL)
        free(rc);
    return NULL;
}

static struct vector * _grm_expand(struct grm *grm, struct grm_token *token)
{
    size_t rcase;
    struct vector *rside;
    struct vector *tcase;
    struct vector *new_tokens;      // vector<token>
    struct vector *exp_tokens;      // vector<token>, expand non-term token
    struct vector *exp_token;
    size_t i;
    size_t k;

    new_tokens = NULL;
    printf("expand %s\n", token->value);

    // query right side
    if (token->type != GRM_TOKEN_NTERM)
        goto ERROR;
    rside = htable_get(&grm->map, token->value);
    if (rside == NULL)
        goto ERROR;
    if (rside->size == 0)
        goto ERROR;

    // chose random token case
    if (rrand(0, rside->size - 1, &rcase))
        goto ERROR;
    printf("rand %lu %lu\n", rside->size - 1, rcase);
    tcase = vector_at(rside, rcase);
    if (tcase == NULL)
        goto ERROR;
    if (tcase->size == 0)
        goto ERROR;

    new_tokens = malloc(sizeof(*new_tokens));
    if (new_tokens == NULL)
        goto ERROR;
    if (vector_init(new_tokens))
        goto ERROR;

    // expand non-term token and copy term token
    for (i = 0; i < tcase->size; i++) {
        token = vector_at(tcase, i);
        if (token == NULL)
            goto ERROR;
        if (token->type == GRM_TOKEN_NTERM) {
            exp_tokens = _grm_expand(grm, token);
            if (exp_tokens == NULL)
                goto ERROR;
            for (k = 0; k < exp_tokens->size; k++) {
                exp_token = vector_at(exp_tokens, k);
                if (exp_token == NULL)
                    goto ERROR;
                if (vector_add(new_tokens, exp_token))
                    goto ERROR;
            }
        } else if (token->type == GRM_TOKEN_TERM) {
            if (vector_add(new_tokens, token))
                goto ERROR;
        }
    }

    return new_tokens;

ERROR:
    if (new_tokens != NULL)
        free(new_tokens);

    return NULL;
}

static int _grm_add_expr(struct grm *grm, const char *expr)
{
    struct vector *tokens;
    struct grm_token *key_token;
    struct grm_token *token;
    struct vector *rside;               // vector<vector<token>>
    struct vector *rside_part;          // vector<token>
    ssize_t i;

    tokens = _grm_expr_parse(expr);
    if (tokens == NULL)
        exit_err("_grm_expr_parse");
    
    // non-terminal and assign token: <non-term> ::= ...
    key_token = vector_at(tokens, 0);
    if (key_token == NULL || key_token->type != GRM_TOKEN_NTERM)
        return -1;
    token = vector_at(tokens, 1);
    if (token == NULL)
        return -1;
    if (token->type != GRM_TOKEN_ASGN)
        return -1;

    // right side: <non-term>: ::= right-side
    rside = malloc(sizeof(*rside));
    if (rside == NULL)
        goto ERROR;
    if (vector_init(rside))
        goto ERROR;
    rside_part = NULL;
    for (i = 2; i < tokens->size; i++) {
        if (rside_part == NULL) {
            rside_part = malloc(sizeof(*rside_part));
            if (rside_part == NULL)
                goto ERROR;
            if (vector_init(rside_part))
                goto ERROR;
        }

        token = (struct grm_token *) vector_at(tokens, i);
        if (token == NULL) {
            return -1;
        } else if (token->type == GRM_TOKEN_ASGN) {
            goto ERROR;
        } else if (token->type == GRM_TOKEN_CASE) {
            if (rside_part->size == 0)
                goto ERROR;
            if (vector_add(rside, rside_part))
                goto ERROR;
            rside_part = NULL;
        } else if (token->type == GRM_TOKEN_TERM) {
            if (vector_add(rside_part, token))
                goto ERROR;
        } else if (token->type == GRM_TOKEN_NTERM) {
            if (vector_add(rside_part, token))
                goto ERROR;
        }

    }
    if (vector_add(rside, rside_part))
        goto ERROR;

    // put to grammar map
    if (htable_set(&grm->map, key_token->value, rside)) 
        goto ERROR;

    return 0;

ERROR:
    if (rside != NULL)
        free(rside);
    if (rside_part != NULL)
        free(rside_part);
    return -1;
}

static struct vector * _grm_expr_parse(const char *expr)
{
    struct vector *tokens;
    struct grm_token *token;
    enum grm_flag flag;
    char *s;
    char *p;
    char *e;
    ssize_t value_size;

    token = NULL;

    tokens = malloc(sizeof(*tokens));
    if (tokens == NULL)
        return NULL;
    if (vector_init(tokens))
        goto ERROR;

    flag = GRM_FLAG_NONE;
    p = (char *) expr;
    e = p + strlen(expr);
    for (; p <= e; p++) {
        if (flag == GRM_FLAG_NONE) {
            if (*p == 0)
                break;
            if (*p == GRM_IGNORE_CHAR)
                continue;

            // create new token
            token = malloc(sizeof(*token));
            if (token == NULL)
                goto ERROR;

            // mark start of token
            s = p;

            // detect token type
            if (_grm_is_term(*p)) {
                flag = GRM_FLAG_TERM;
                token->type = GRM_TOKEN_TERM;
            } else if (*p == GRM_ASGN_START) {
                flag = GRM_FLAG_ASGN;
                token->type = GRM_TOKEN_ASGN;
            } else if (*p == GRM_NTERM_START) {
                flag = GRM_FLAG_NTERM;
                token->type = GRM_TOKEN_NTERM;
            } else if (*p == GRM_CASE_START) {
                token->type = GRM_TOKEN_CASE;
                token->value = malloc(2);
                if (token->value == NULL)
                    goto ERROR;
                memcpy(token->value, "|", 1);
                token->value[1] = 0;
            } else {
                goto ERROR;
            }

            // add token to part
            if (vector_add(tokens, token))
                goto ERROR;
        } else if (flag == GRM_FLAG_ASGN) {
            if (p - s == 1) {
                if (*p != ':')
                    goto ERROR;
            } else if (p - s == 2) {
                if (*p == '=') {
                    value_size = 4;
                    token->value = malloc(value_size);
                    memcpy(token->value, "::=", 3);
                    token->value[3] = 0;
                    flag = GRM_FLAG_NONE;
                } else {
                    goto ERROR;
                }
            }
        } else if (flag == GRM_FLAG_NTERM) {
            if (*p == GRM_NTERM_END) {
                value_size = p - s - 1;
                if (value_size < 0)
                    goto ERROR;
                token->value = malloc(value_size + 1);
                memcpy(token->value, s + 1, value_size);
                token->value[value_size] = 0;
                flag = GRM_FLAG_NONE;
            } else if (_grm_is_term(*p)) {
                continue;
            } else {
                goto ERROR;
            }
        } else if (flag == GRM_FLAG_TERM) {
            if (*p == GRM_NTERM_START || *p == GRM_CASE_START || 
                *p == GRM_ASGN_START || *p == GRM_IGNORE_CHAR || *p == 0) 
            {
                value_size = p - s;
                token->value = malloc(value_size + 1);
                if (token->value == NULL)
                    goto ERROR;
                memcpy(token->value, s, value_size);
                token->value[value_size] = 0;
                flag = GRM_FLAG_NONE;
                if (*p != 0)
                    p -= 1;
            } else if (_grm_is_term(*p)) {
                continue;
            } else {
                goto ERROR;
            }
        } else {
            goto ERROR;
        }
    }

    if (flag != GRM_FLAG_NONE)
        goto ERROR;

    return tokens;

ERROR:
    if (tokens != NULL)
        free(tokens);
    if (token != NULL)
        free(token);

    return NULL;
}

static int _grm_is_term(const char c)
{
    if (c >= 48 && c <= 57)
        return 1;
    else if (c >= 65 && c <= 90)
        return 1;
    else if (c >= 97 && c <= 122)
        return 1;
    else if (c == 45)
        return 1;
    else
        return 0;
}

static void _grm_free(struct grm *grm)
{
    htable_free(&grm->map);
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    003b file\n");
    printf("    003b -h\n\n");

    printf("ARGUMENTS\n");
    printf("    file    Path to grammar file\n");
    printf("            An example file located at asset/003b-input.txt\n");
}
