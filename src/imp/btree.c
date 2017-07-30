#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cs106b/error.h>
#include <cs106b/btree.h>

struct callback_arg
{
    int inum;
};

static int _walk_callback(struct btree_node *node, void *arg);

int main(int argc, char *argv[])
{
    struct btree tree;
    struct btree_node *node;
    struct callback_arg cb_arg;

    if (btree_init(&tree))
        exit_err("btree_init");
    cb_arg.inum = 64;

    // root
    node = btree_mknode(NULL, NULL, NULL);
    if (node == NULL)
        exit_err("btree_mknode");
    node->data = malloc(32);
    if (node->data == NULL)
        exit_err("malloc");
    strcpy(node->data, "root");
    tree.root = node;

    // left
    node = btree_mknode(NULL, NULL, NULL);
    if (node == NULL)
        exit_err("btree_mknode");
    node->data = malloc(32);
    if (node->data == NULL)
        exit_err("malloc");
    strcpy(node->data, "left");
    tree.root->left = node;

    // left left
    node = btree_mknode(NULL, NULL, NULL);
    if (node == NULL)
        exit_err("btree_mknode");
    node->data = malloc(32);
    if (node->data == NULL)
        exit_err("malloc");
    strcpy(node->data, "left left");
    tree.root->left->left = node;

    // right
    node = btree_mknode(NULL, NULL, NULL);
    if (node == NULL)
        exit_err("btree_mknode");
    node->data = malloc(32);
    if (node->data == NULL)
        exit_err("malloc");
    strcpy(node->data, "right");
    tree.root->right = node;

    // right right
    node = btree_mknode(NULL, NULL, NULL);
    if (node == NULL)
        exit_err("btree_mknode");
    node->data = malloc(32);
    if (node->data == NULL)
        exit_err("malloc");
    strcpy(node->data, "right right");
    tree.root->right->right = node;

    printf("walk in pre-order\n");
    if (btree_prew(&tree, _walk_callback, &cb_arg))
        exit_err("btree_prew");

    printf("walk in in-order\n");
    if (btree_inw(&tree, _walk_callback, &cb_arg))
        exit_err("btree_inw");

    printf("walk in post-order\n");
    if (btree_postw(&tree, _walk_callback, &cb_arg))
        exit_err("btree_postw");

    printf("walk in level-order\n");
    if (btree_levelw(&tree, _walk_callback, &cb_arg))
        exit_err("btree_levelw");

    btree_free(&tree);
    return EXIT_SUCCESS;
}

static int _walk_callback(struct btree_node *node, void *arg)
{
    struct callback_arg *cb_arg;

    cb_arg = (struct callback_arg *) arg;
    printf("    data=%s, arg=%i\n", (char *) node->data, cb_arg->inum);
    return 0;
}
