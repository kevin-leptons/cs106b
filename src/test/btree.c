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
    struct btree tree;
    struct btree_node *node;
    struct callback_arg cb_arg;
    int ret;

    ret = EXIT_FAILURE;
    btree_init(&tree);
    cb_arg.inum = 64;

    // root
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto finish;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "root");
    tree.root = node;

    // left
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "left");
    tree.root->left = node;

    // left left
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "left left");
    tree.root->left->left = node;

    // right
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "right");
    tree.root->right = node;

    // right right
    if (btree_mknode(&node, NULL, NULL, NULL))
        goto free_node;
    if (cs106b_malloc((void *) &node->data, 32))
        goto free_node;
    strcpy(node->data, "right right");
    tree.root->right->right = node;

    printf("walk in pre-order\n");
    if (btree_prew(&tree, _walk_callback, &cb_arg))
        goto free_tree;

    printf("walk in in-order\n");
    if (btree_inw(&tree, _walk_callback, &cb_arg))
        goto free_tree;

    printf("walk in post-order\n");
    if (btree_postw(&tree, _walk_callback, &cb_arg))
        goto free_tree;

    printf("walk in level-order\n");
    if (btree_levelw(&tree, _walk_callback, &cb_arg))
        goto free_tree;

    ret = EXIT_SUCCESS;

free_node:
    free(node);
free_tree:
    btree_free(&tree);
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
