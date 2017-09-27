#include <cs106b/stack.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

void stack_init(struct stack *stack)
{
    stack->top = NULL;
    stack->size = 0;
}

void stack_free(struct stack *stack)
{
    for (; stack->size > 0;)
        stack_pop(stack, NULL);
}

int stack_push(struct stack *stack, void *value)
{
    struct stack_item *new_item;

    if (cs106b_malloc((void *) &new_item, sizeof(*new_item)))
        return -1;

    new_item->value = value;
    new_item->next = stack->top;
    stack->top = new_item;
    stack->size += 1;

    return 0;
}

int stack_pop(struct stack *stack, void **value)
{
    struct stack_item *top;

    if (stack->size == 0)  {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    top = stack->top;
    stack->top = stack->top->next;
    stack->size -= 1;

    if (value != NULL)
        *value = top->value;
    free(top);
    return 0;
}

int stack_top(struct stack *stack, void **value)
{
    if (stack->top == NULL) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    *value = stack->top->value;
    return 0;
}

int stack_copy(struct stack *dest, struct stack *src)
{
    struct stack_item *src_item;
    struct stack_item **dest_item;

    stack_free(dest);
    dest_item = &dest->top;
    for (src_item = src->top; src_item != NULL; src_item = src_item->next) {
        if (cs106b_malloc((void *) dest_item, sizeof(struct stack_item))) {
            stack_free(dest);
            return -1;
        }
        (*dest_item)->value = src_item->value;
        (*dest_item)->next = NULL;
        dest_item = &(*dest_item)->next;
    }

    dest->size = src->size;
    return 0;
}

int stack_clone(struct stack **dest, struct stack *src)
{
    if (cs106b_malloc((void *) dest, sizeof(struct stack)))
        return -1;
    stack_init(*dest);

    if (stack_copy(*dest, src))
        return -1;

    return 0;
}
