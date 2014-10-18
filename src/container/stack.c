/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/container/stack.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool stack_init (Stack* stack, StackType type) {
    assert(NULL != stack);
    assert(0 == stack->count);

    stack->count = 0;
    stack->head  = NULL;
    stack->tail  = NULL;
    stack->type  = type;

    return true;
}

Stack* stack_new () {
    Stack* stack = (Stack*) malloc(sizeof(Stack));

    if (NULL == stack) {
        return NULL;
    }

    memset(stack, 0, sizeof(Stack));

    return stack;
}

void* stack_pop (Stack* stack) {
    assert(NULL != stack);
    assert(0 < stack->count);

    StackItem* item = stack->head;
    void*      data = item->data;

    stack->head = item->next;
    stack->count--;

    free(item);

    return data;
}

bool stack_push (Stack* stack, void* data) {
    assert(NULL != stack);

    StackItem* item = (StackItem*) malloc(sizeof(StackItem));

    if (NULL == item) {
        return false;
    }

    if (stack->type == STACK_FIFO) {
        item->next = NULL;

        if (NULL != stack->tail) {
            stack->tail->next = item;
        } else {
            stack->head = item;
        }

        stack->tail = item;
    } else {
        item->next  = stack->head;
        stack->head = item;
    }

    item->data = data;
    stack->count++;

    return true;
}
