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

bool stack_init (Stack* stack, StackType type, bool thread_safe) {
    assert(NULL != stack);
    assert(0 == stack->count);

    stack->count = 0;
    stack->head  = NULL;
    stack->mutex = NULL;
    stack->tail  = NULL;
    stack->type  = type;

    if (thread_safe) {
        pthread_mutex_init(stack->mutex, NULL);
    }

    return true;
}

void stack_lock (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);
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

void* stack_pop_ts (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);

    void* ret = stack_pop(stack);

    pthread_mutex_unlock(stack->mutex);

    return ret;
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

bool stack_push_ts (Stack* stack, void* data) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);

    bool ret = stack_push(stack, data);

    pthread_mutex_unlock(stack->mutex);

    return ret;
}

void stack_unlock (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_unlock(stack->mutex);
}
