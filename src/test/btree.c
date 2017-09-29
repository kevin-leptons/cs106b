#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cs106b/error.h>
#include <cs106b/btree.h>
#include <cs106b/mem.h>

struct callback_arg
{
    int inum;
};

static int _walk_callback(struct btree_node *node, void *arg);

int main(int argc, char *argv[])
{
    struct btree t1;
    struct btree_node *node;
    struct callback_arg cb_arg;
    int ret;

    ret = EXIT_FAILURE;
    printf("btree_init(t1)\n");
    btree_init(&t1);
    cb_arg.inum = 64;

    // root
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto finish;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "root");
    t1.root = node;

    // left
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "left");
    t1.root->left = node;

    // left left
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "left left");
    t1.root->left->left = node;

    // right
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "right");
    t1.root->right = node;

    // right right
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "right right");
    t1.root->right->right = node;

    // walking
    printf("walk in pre-order\n");
    if (btree_prew(&t1, _walk_callback, &cb_arg))
        goto free_t1;
    printf("walk in in-order\n");
    if (btree_inw(&t1, _walk_callback, &cb_arg))
        goto free_t1;
    printf("walk in post-order\n");
    if (btree_postw(&t1, _walk_callback, &cb_arg))
        goto free_t1;
    printf("walk in level-order\n");
    if (btree_levelw(&t1, _walk_callback, &cb_arg))
        goto free_t1;

    ret = EXIT_SUCCESS;

free_node:
    free(node);
free_t1:
    btree_free(&t1);
finish:
    if (espace_check())
        fprintf(stderr, "Error: %s\n", espace->id);
    return ret;
}

static int _walk_callback(struct btree_node *node, void *arg)
{
    struct callback_arg *cb_arg;

    cb_arg = (struct callback_arg *) arg;
    printf("    data=%s, arg=%i\n", (char *) node->data, cb_arg->inum);
    return 0;
}
