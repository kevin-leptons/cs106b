#include <cs106b/btree.h>

#include <stdlib.h>

#include <cs106b/queue.h>
#include <cs106b/stack.h>

static int _btree_inw(struct btree_node *node, btree_walkcb callback,
                      void *arg);
static int _btree_postw(struct btree_node *node, btree_walkcb callback,
                        void *arg);

int btree_init(struct btree *tree)
{
    tree->root = NULL;

    return 0;
}

struct btree_node *
btree_mknode(struct btree_node *left, struct btree_node *right, void *data)
{
    struct btree_node *new_node;

    new_node = malloc(sizeof(*new_node));
    if (new_node == NULL)
        return NULL;
    new_node->left = left;
    new_node->right = right;
    new_node->data = data;

    return new_node;
}

int btree_prew(struct btree *tree, btree_walkcb callback, void *arg)
{
    struct stack stack;
    struct btree_node *node;
    int cb_result;

    if (stack_init(&stack))
        return -1;
    if (tree->root != NULL) {
        if (stack_push(&stack, tree->root))
            goto ERROR;
    }

    for (; stack.size > 0;) {
        node = stack_pop(&stack);
        if (node == NULL)
            goto ERROR;

        // do something with this node
        cb_result = callback(node, arg);
        if (cb_result == 1)
            break;
        else if (cb_result < 0)
            goto ERROR;

        // push right and left nodes in order for continue process
        // in order: left first, right last
        if (node->right != NULL) {
            if (stack_push(&stack, node->right))
                goto ERROR;
        }
        if (node->left != NULL)  {
            if (stack_push(&stack, node->left))
                goto ERROR;
        }
    }

    stack_free(&stack, false);
    return 0;

ERROR:
    stack_free(&stack, false);
    return -1;
}

int btree_inw(struct btree *tree, btree_walkcb callback, void *arg)
{
    if (tree->root == NULL)
        return 0;

    return _btree_inw(tree->root, callback, arg);
}

int btree_postw(struct btree *tree, btree_walkcb callback, void *arg)
{
    if (tree->root == NULL)
        return 0;
    return _btree_postw(tree->root, callback, arg);
}

int btree_levelw(struct btree *tree, btree_walkcb callback, void *arg)
{
    struct queue queue;
    struct btree_node *node;
    int cb_result;

    if (queue_init(&queue))
        return -1;
    if (tree->root != NULL) {
        queue_push(&queue, tree->root);
    }

    for (; queue.size > 0;) {
        node = queue_pop(&queue);
        if (node == NULL)
            goto ERROR;
        cb_result = callback(node, arg);
        if (cb_result < 0)
            goto ERROR;
        else if (cb_result == 1)
            break;

        if (node->left != NULL) {
            if (queue_push(&queue, node->left))
                goto ERROR;
        }
        if (node->right != NULL) {
            if (queue_push(&queue, node->right))
                goto ERROR;
        }
    }

    queue_free(&queue, false);
    return 0;

ERROR:
    queue_free(&queue, false);
    return -1;
}

void btree_free(struct btree *tree)
{
}

static int 
_btree_inw(struct btree_node *node, btree_walkcb callback, void *arg)
{
    int cb_result;

    if (node->left != NULL) {
        if (_btree_inw(node->left, callback, arg))
            return -1;
    }
    
    cb_result = callback(node, arg);
    if (cb_result < 0)
        return -1;
    else if (cb_result == 1)
        return 1;

    if (node->right != NULL) {
        if (_btree_inw(node->right, callback, arg))
            return -1;
    }

    return 0;
}

static int
_btree_postw(struct btree_node *node, btree_walkcb callback, void *arg)
{
    if (node->left != NULL) {
        if (_btree_postw(node->left, callback, arg))
            return -1;
    }
    if (node->right != NULL) {
        if (_btree_postw(node->right, callback, arg))
            return -1;
    }

    return callback(node, arg);
}
