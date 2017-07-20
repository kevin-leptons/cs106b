#include <cs106b/stack.h>

int stack_init(struct stack *stack)
{
    stack->top = NULL;
    stack->size = 0;
    return 0;
}

int stack_push(struct stack *stack, void *data)
{
    struct stack_item *new_item;

    new_item = malloc(sizeof(*new_item));
    if (new_item == NULL)
        return -1;

    new_item->data = data;
    new_item->next = stack->top;
    stack->top = new_item;
    stack->size += 1;

    return 0;
}

void *stack_pop(struct stack *stack)
{
    struct stack_item *top;
    void *data;

    if (stack->size == 0)
        return NULL;

    top = stack->top;
    stack->top = stack->top->next;
    stack->size -= 1;

    data = top->data;
    free(top);

    return data;
}

void *stack_top(struct stack *stack)
{
    if (stack->top == NULL)
        return NULL;
    return stack->top->data;
}

int stack_copy(struct stack *dest, struct stack *src)
{
    struct stack_item *item;
    struct stack_item **new_item;

    stack_free(dest, false);
    if (stack_init(dest))
        goto ERROR;

    new_item = &dest->top;
    dest->size = src->size;
    for (item = src->top; item != NULL; item = item->next) {
        *new_item = malloc(sizeof(**new_item));
        if (*new_item == NULL)
            goto ERROR;
        (*new_item)->data = item->data;
        (*new_item)->next = NULL;
        new_item = &(*new_item)->next;
    }

    return 0;

ERROR:
    if (new_item != NULL)
        free(new_item);
    stack_free(dest, false);
    return -1;
}

struct stack * stack_clone(struct stack *src)
{
    struct stack *new_stack;

    new_stack = malloc(sizeof(*new_stack));
    if (new_stack == NULL)
        return NULL;
    if (stack_init(new_stack))
        goto ERROR;

    if (stack_copy(new_stack, src))
        goto ERROR;

    return new_stack;

ERROR:
    if (new_stack != NULL)
        free(new_stack);
    return NULL;
}

void stack_free(struct stack *stack, bool free_data)
{
    void *data;

    for (data = stack_pop(stack); data != NULL; data = stack_pop(stack))
        if (free_data)
            free(data);
}
