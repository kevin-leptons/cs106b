#include <cs106b/huffman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>

#include <cs106b/error.h>
#include <cs106b/pqueue.h>
#include <cs106b/btree.h>
#include <cs106b/bitvec.h>
#include <cs106b/queue.h>
#include <cs106b/byfile.h>
#include <cs106b/bifile.h>
#include <cs106b/mem.h>

#define HUFF_NCODE 256
#define HUFF_NFILE_MODE S_IRWXU | S_IRWXO | S_IRWXG

enum huff_node_type
{
    HUFF_NODE_CODE,
    HUFF_NODE_CLONE
};

struct huff_node
{
    uint8_t code;
    size_t count; 
    enum huff_node_type type;
};

struct wmeta_arg
{
    int fd;
    struct huff_info *info;
};

struct btree_pnode
{
    struct btree_node *node;
    struct bitvec path;
};

typedef
int (*btree_pathcb)(struct btree_node *node, struct bitvec *path, void *arg);

static int _huff_count(int fd, struct huff_node nodes[HUFF_NCODE]);
static int _huff_pqueue(struct huff_node nodes[HUFF_NCODE], struct pqueue *q);
static int _huff_tree(struct pqueue *q, struct btree *tree);
static int _huff_map(struct btree *tree, struct bitvec map[HUFF_NCODE]);
static int _huff_wmeta(struct btree *tree, int sfd, int dfd);
static int _huff_wencode(struct bitvec *map, int sfd, int dfd);
static int _huff_retree(int sfd, struct huff_info *info, struct btree *tree);
static int
_huff_wdecode(int sfd, int dfd, struct huff_info *info, struct btree *tree);

static int
_btree_pathw(struct btree *tree, btree_pathcb callback, struct bitvec *map);
static int _huff_wmeta_cb(struct btree_node *node, void *arg);
static int
_huff_map_cb(struct btree_node *node, struct bitvec *path, void *arg);

int huff_encode(const char *src, const char *dest)
{
    int sfd;
    int dfd;
    struct huff_node nodes[HUFF_NCODE];
    struct pqueue q;
    struct btree tree;
    struct bitvec map[HUFF_NCODE];

    sfd = open(src, O_RDONLY);
    if (sfd < 0)
        return -1;
    dfd = open(dest, O_RDWR | O_CREAT | O_TRUNC, HUFF_NFILE_MODE);
    if (dfd < 0)
        goto ERROR;

    // count number of code which occurs in file
    if (_huff_count(sfd, nodes))
        goto ERROR;

    // priority queue with priority is frequent of code
    if (_huff_pqueue(nodes, &q))
        goto ERROR;

    // convert queue to queue with leaf is code and other is clone
    if (_huff_tree(&q, &tree))
        goto ERROR;

    // convert tree to encoding map
    memset(map, 0, sizeof(struct huff_node) * HUFF_NODE_CODE);
    if (_huff_map(&tree, map))
        goto ERROR;

    // write metadata of encoding to file
    if (_huff_wmeta(&tree, sfd, dfd))
        goto ERROR;

    // convert data and write to dest file
    if (_huff_wencode(map, sfd, dfd))
        goto ERROR;

    close(sfd);
    close(dfd);
    return 0;

ERROR:
    if (sfd >= 0)
        close(sfd);
    if (dfd >= 0)
        close(dfd);

    return -1;
}

int huff_decode(const char *src, const char *dest)
{
    struct huff_info info;      // meta data of encoded file
    struct btree tree;          // huff tree
    int sfd;
    int dfd;

    sfd = -1;
    dfd = -1;

    sfd = open(src, O_RDONLY);
    if (sfd < 0)
        goto ERROR;
    dfd = open(dest, O_CREAT | O_TRUNC | O_WRONLY);
    if (dfd < 0)
        goto ERROR;
    btree_init(&tree);

    if (read(sfd, &info, sizeof(info)) != sizeof(info))
        goto ERROR;
    if (_huff_retree(sfd, &info, &tree))
        goto ERROR;
    if (_huff_wdecode(sfd, dfd, &info, &tree))
        goto ERROR;

    close(sfd);
    close(dfd);
    btree_free(&tree);
    return 0;

ERROR:
    if (sfd >= 0)
        close(sfd);
    if (dfd >= 0)
        close(dfd);
    btree_free(&tree);

    return -1;
}

int huff_rinfo(const char *file, struct huff_info *info)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -1;
    if (read(fd, info, sizeof(*info)) != sizeof(*info))
        return -1;

    return 0;
}

static int _huff_count(int fd, struct huff_node nodes[HUFF_NCODE])
{
    uint8_t buf[1024];
    ssize_t rsize;
    ssize_t i;

    memset(nodes, 0, sizeof(*nodes) * HUFF_NCODE);
    for (i = 0; i < HUFF_NCODE; i++)
        nodes[i].code = i;

    for (;;) {
        rsize = read(fd, buf, sizeof(buf));
        if (rsize == 0)
            break;
        if (rsize < 0)
            return -1;

        for (i = 0; i < rsize; i++)
            nodes[buf[i]].count += 1;
    }

    return 0;
}

static int _huff_pqueue(struct huff_node nodes[HUFF_NCODE], struct pqueue *q)
{
    size_t i;
    struct btree_node *btree_node;

    pqueue_init(q, PQUEUE_INC);

    for (i = 0; i < HUFF_NCODE; i++) {
        if (nodes[i].count == 0)
            continue;

        nodes[i].type = HUFF_NODE_CODE;
        if (btree_mknode(&btree_node, NULL, NULL, nodes + i))
            return -1;
        if (pqueue_push(q, btree_node, nodes[i].count))
            return -1;
    }

    return 0;
}

static int _huff_tree(struct pqueue *q, struct btree *tree)
{
    struct btree_node *node1;
    struct btree_node *node2;
    struct btree_node *clone;
    struct huff_node *huff_node1;
    struct huff_node *huff_node2;
    struct huff_node *huff_clone;

    for (; q->size > 1;) {
        if (pqueue_pop(q, (void *) &node1))
            return -1;
        if (pqueue_pop(q, (void *) &node2))
            return -1;

        if (cs106b_malloc((void *) &clone, sizeof(*clone)))
            return -1;
        if (cs106b_malloc((void *) &clone->data, sizeof(struct huff_node)))
            return -1;
        huff_node1 = (struct huff_node *) node1->data;
        huff_node2 = (struct huff_node *) node2->data;
        huff_clone = (struct huff_node *) clone->data;

        huff_clone->type = HUFF_NODE_CLONE;
        huff_clone->code = 0;
        huff_clone->count = huff_node1->count + huff_node2->count;
        clone->left = node1;
        clone->right = node2;

        if (pqueue_push(q, clone, huff_clone->count))
            return -1;
    }

    btree_init(tree);
    if (pqueue_top(q, (void *) &tree->root))
        return -1;

    return 0;
}

static int _huff_map(struct btree *tree, struct bitvec map[HUFF_NCODE])
{
    if (_btree_pathw(tree, _huff_map_cb, map))
        return -1;

    return 0;
}

static int _huff_wmeta(struct btree *tree, int sfd, int dfd)
{
    struct huff_info info;
    struct wmeta_arg wmeta_arg;
    ssize_t wsize;
    struct stat sfd_stat;

    // point file cursor to after huff_info
    if (lseek(dfd, 0, SEEK_SET) < 0)
        return -1;
    wsize = write(dfd, &info, sizeof(info));
    if (wsize != sizeof(info))
        return -1;

    // write tree specification
    info.tree_size = 0;
    wmeta_arg.fd = dfd;
    wmeta_arg.info = &info;
    if (btree_levelw(tree, _huff_wmeta_cb, &wmeta_arg))
        return -1;

    // rewrite huff_info with correct info
    if (lseek(dfd, 0, SEEK_SET) < 0)
        return -1;
    if (fstat(sfd, &sfd_stat))
        return -1;
    info.raw_size = sfd_stat.st_size;
    wsize = write(dfd, &info, sizeof(info));
    if (wsize != sizeof(info))
        return -1;

    return 0;
}

static int _huff_wencode(struct bitvec *map, int sfd, int dfd)
{
    struct byfile ifile;              // input as serial of bytes
    struct bifile ofile;               // output as serial of vector<bit>
    uint8_t code;
    struct bitvec *encoded;
    size_t byte_count;
    size_t bit_count;
    struct huff_info info;

    if (lseek(sfd, 0, SEEK_SET) < 0)
        return -1;
    if (lseek(dfd, 0, SEEK_END) < 0)
        return -1;

    byfile_init(&ifile, sfd);
    bifile_init(&ofile, dfd, 0);

    byte_count = 0;
    bit_count = 0;
    for (;;) {
        if (byfile_read(&ifile, &code)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                return -1;
            }
        }

        encoded = map + code;
        if (encoded->size == 0)
            return -1;

        bit_count += encoded->size;
        byte_count += bit_count / 8;
        bit_count = bit_count % 8;
        if (bifile_writev(&ofile, encoded))
            return -1;
    }
    if (bifile_flush(&ofile))
        return -1;

    // rewrite huff info
    if (lseek(dfd, 0, SEEK_SET) < 0)
        return -1;
    if (read(dfd, &info, sizeof(info)) != sizeof(info))
        return -1;

    info.enc_byte_size = byte_count;
    info.enc_bit_size = bit_count;
    if (lseek(dfd, 0, SEEK_SET) < 0)
        return -1;
    if (write(dfd, &info, sizeof(info)) != sizeof(info))
        return -1;

    return 0;
}

static int _huff_wmeta_cb(struct btree_node *node, void *arg)
{
    struct wmeta_arg *wmeta_arg;
    struct huff_node *huff_node;
    uint8_t buf[3];
    ssize_t wsize;
    size_t to_wsize;

    wmeta_arg = (struct wmeta_arg *) arg;
    memset(buf, 0, sizeof(buf));
    to_wsize = 2;

    if (node->left != NULL)
        buf[0] = 1;
    if (node->right != NULL)
        buf[1] = 1;

    if (node->left == NULL && node->right == NULL) {
        huff_node = (struct huff_node *) node->data;
        buf[2] = huff_node->code;
        to_wsize += 1;
    }

    wsize = write(wmeta_arg->fd, buf, to_wsize);
    if (wsize != to_wsize)
        return -1;
    wmeta_arg->info->tree_size += wsize;

    return 0;
}

static int
_btree_pathw(struct btree *tree, btree_pathcb callback, struct bitvec *map)
{
    struct queue queue;             // queue<btree_pnode>
    struct btree_pnode *fnode;      // first node in queue
    struct btree_pnode *node;       // temporary node
    struct btree_pnode *cnode;      // child of temporary node
    int r;

    fnode = NULL;

    // queue
    queue_init(&queue);

    // first queue's node
    if (cs106b_malloc((void *) &fnode, sizeof (*fnode)))
        goto ERROR;
    bitvec_init(&fnode->path);
    fnode->node = tree->root;
    if (queue_push(&queue, fnode))
        goto ERROR;

    for (; queue.size > 0;) {
        if (queue_pop(&queue, (void *) &node))
            goto ERROR;
        r = callback(node->node, &node->path, map);
        if (r < 0)
            goto ERROR;
        else if (r == 1)
            break;

        if (node->node->left != NULL) {
            if (cs106b_malloc((void *) &cnode, sizeof(*cnode)))
                goto ERROR;
            bitvec_init(&cnode->path);
            if (bitvec_copy(&cnode->path, &node->path))
                goto ERROR;
            if (bitvec_add(&cnode->path, 0))
                goto ERROR;
            cnode->node = node->node->left;
            if (queue_push(&queue, cnode))
                goto ERROR;
        }
        if (node->node->right != NULL) {
            if (cs106b_malloc((void *) &cnode, sizeof(*cnode)))
                goto ERROR;
            bitvec_init(&cnode->path);
            if (bitvec_copy(&cnode->path, &node->path))
                goto ERROR;
            if (bitvec_add(&cnode->path, 1))
                goto ERROR;
            cnode->node = node->node->right;
            if (queue_push(&queue, cnode))
                goto ERROR;
        }

        bitvec_free(&node->path);
        free(node);
    }

    return 0;

ERROR:
    queue_free(&queue);
    return -1;
}

static int
_huff_map_cb(struct btree_node *node, struct bitvec *path, void *arg)
{
    struct huff_node *hnode;
    struct bitvec *map;

    if (node->left == NULL && node->right == NULL) {
        hnode = (struct huff_node *) node->data;
        map = (struct bitvec *) arg;
        bitvec_init(map + hnode->code);
        if (bitvec_copy(map + hnode->code, path))
            return -1;
    }

    return 0;
}

static int _huff_retree(int sfd, struct huff_info *info, struct btree *tree)
{
    struct queue queue;
    struct btree_node *root;
    uint8_t next_byte;
    struct byfile ifile;
    struct btree_node *cnode;
    struct btree_node *node;
    struct huff_node *huff_node;
    size_t rbyte_count;

    root = NULL;

    queue_init(&queue);

    // root node
    if (btree_mknode(&root, NULL, NULL, NULL))
        goto ERROR;
    tree->root = root; 
    if (queue_push(&queue, root))
        goto ERROR;

    // read file in serial of bytes
    byfile_init(&ifile, sfd);
    if (lseek(sfd, sizeof(*info), SEEK_SET) < 0) {
        sys_raise(errno);
        errno = 0;
        goto ERROR;
    }

    // rebuild tree
    rbyte_count = 0;
    for (; queue.size > 0;) {
        if (queue_pop(&queue, (void *) &node))
            goto ERROR;

        // make left node
        if (byfile_read(&ifile, &next_byte))
            goto ERROR;
        rbyte_count += 1;
        if (next_byte) {
            if (btree_mknode(&cnode, NULL, NULL, NULL))
                goto ERROR;

            node->left = cnode;
            if (queue_push(&queue, cnode))
                goto ERROR;
        } 

        // make right node
        if (byfile_read(&ifile, &next_byte))
            goto ERROR;
        rbyte_count += 1;
        if (next_byte) {
            if (btree_mknode(&cnode, NULL, NULL, NULL))
                goto ERROR;

            node->right = cnode;
            if (queue_push(&queue, cnode))
                goto ERROR;
        } 

        // node is leaf, copy code
        if (node->left == NULL && node->right == NULL) {
            if (byfile_read(&ifile, &next_byte))
                goto ERROR;
            rbyte_count += 1;
            if (cs106b_malloc((void *) &huff_node, sizeof(*huff_node)))
                goto ERROR;
            huff_node->code = next_byte;
            node->data = huff_node;
        }

        // other break condition
        if (rbyte_count == info->tree_size) 
            break;
    }

    return 0;

ERROR:
    queue_free(&queue);
    byfile_free(&ifile);
    if (root != NULL)
        free(root);

    return -1;
}

static int
_huff_wdecode(int sfd, int dfd, struct huff_info *info, struct btree *tree)
{
    struct btree_node *node;
    size_t rbyte_count;
    size_t rbit_count;
    size_t dec_size;
    uint8_t next_bit;
    struct bifile ifile;
    struct byfile ofile;
    struct huff_node *huff_node;

    bifile_init(&ifile, sfd, 0);
    byfile_init(&ofile, dfd);

    if (lseek(sfd, sizeof(*info) + info->tree_size, SEEK_SET) < 0) {
        sys_raise(errno);
        errno = 0;
        return -1;
    }
    if (lseek(dfd, 0, SEEK_SET) < 0) {
        sys_raise(errno);
        errno = 0;
        return -1;

    }

    node = tree->root;
    rbyte_count = 0;
    rbit_count = 0;
    dec_size = 0;
    for (;;) {
        // get next bit of file
        if (bifile_read(&ifile, &next_bit)) {
            if (espace_catch(CS106B_EINDEX)) {
                espace_clear();
                break;
            } else {
                return -1;
            }
        }

        rbit_count += 1;
        rbyte_count += rbit_count / 8;
        rbit_count = rbit_count % 8;

        // goto left or right
        if (next_bit)
            node = node->right;
        else
            node = node->left;
        if (node == NULL)
            return -1;

        // leaf node, decode
        if (node->left == NULL && node->right == NULL) {
            huff_node = (struct huff_node *) node->data;
            if (byfile_write(&ofile, huff_node->code))
                return -1;
            node = tree->root;
            dec_size += 1;
        }
        
        // break condition
        if (rbyte_count == info->enc_byte_size) {
            if (rbit_count == info->enc_bit_size)
                break;
        }
    }
    if (byfile_flush(&ofile))
        return -1;

    if (dec_size != info->raw_size)
        return -1;

    return 0;
}
