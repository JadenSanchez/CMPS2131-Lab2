#include "stack.h"

#include <stdlib.h>

Stack stack_create(void) {
    Stack stack = {.top = NULL, .size = 0};
    return stack;
}

void stack_destroy(Stack *stack) {
    // GIVEN: study this carefully. It repeatedly removes the top node until
    // there are no nodes left. Notice that the Stack header itself is not
    // heap-allocated; only the nodes are.
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
    stack->size = 0;
}

bool stack_push(Stack *stack, long value) {
    // TODO 1: implement PUSH.
    // Algorithm:
    //   1. allocate a new node
    StackNode *new_node = malloc(sizeof *new_node);

    //   2. if allocation fails, return false
    if (new_node == NULL) {
        return false;
    }

    //   3. store value in the node
    new_node->value = value;

    //   4. point new_node->next at the CURRENT top
    new_node->next = stack->top;

    //   5. move stack->top to the new node
    stack->top = new_node;

    //   6. increment size
    stack->size++;

    //   7. return true
    return true;
}

bool stack_pop(Stack *stack, long *out) {
    // TODO 2: implement POP.
    // Algorithm:

    //   1. if empty, return false
    if (stack_is_empty(stack)) {
        return false;
    }

    //   2. save stack->top in a temporary pointer
    StackNode *temp = stack->top;

    //   3. copy its value into *out
    *out = temp->value;

    //   4. move stack->top to temp->next
    stack->top = temp->next;

    //   5. free temp
    free(temp);

    //   6. decrement size
    stack->size--;

    //   7. return true
    return true;
}

bool stack_peek(const Stack *stack, long *out) {
    // TODO 3: if empty return false; otherwise copy the top value to *out.
    // IMPORTANT: peek must NOT remove or free anything.

    // 1. Check whether the stack is empty.
    if (stack_is_empty(stack)) {
        return false;
    }

    // 2. Copy the top node's value into *out.
    *out = stack->top->value;

    // 3. Return true to indicate success.
    return true;
}

bool stack_is_empty(const Stack *stack) {
    return stack->top == NULL;
}

size_t stack_size(const Stack *stack) {
    return stack->size;
}

void stack_print(const Stack *stack, FILE *out) {
    // GIVEN: for learning/debugging only. Because our linked stack points from
    // top downward, this first copies values into a small temporary array so we
    // can display bottom -> top like a textbook stack.
    if (stack->size == 0) {
        fprintf(out, "[]");
        return;
    }

    long *values = malloc(stack->size * sizeof *values);
    if (values == NULL) {
        fprintf(out, "[print unavailable: out of memory]");
        return;
    }

    size_t i = stack->size;
    for (const StackNode *node = stack->top; node != NULL; node = node->next) {
        values[--i] = node->value;
    }

    fputc('[', out);
    for (i = 0; i < stack->size; i++) {
        if (i > 0)
            fprintf(out, ", ");
        fprintf(out, "%ld", values[i]);
    }
    fputc(']', out);
    free(values);
}

bool stack_check_invariant(const Stack *stack) {
    // TODO 4: count the reachable nodes and verify the two invariant rules in
    // stack.h. This is deliberately a little different from push/pop: it makes
    // you reason about what a *valid* stack must always mean.

    // 1. Check that top is NULL exactly when size is 0.
    if ((stack->top == NULL) != (stack->size == 0)) {
        return false;
    }

    // 2. Start counting the nodes from the top.
    size_t count = 0;

    // 3. Traverse the linked stack and count each node.
    for (const StackNode *node = stack->top; node != NULL; node = node->next) {
        count++;
    }

    // 4. Return true if the count matches the recorded size.
    return count == stack->size;
}
