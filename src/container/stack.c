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

bool stack_cleanup (Stack* stack) {
    assert(NULL != stack);

    if (NULL != stack->mutex) {
        pthread_mutex_destroy(stack->mutex);
    }

    return true;
}

int32_t stack_count (Stack* stack) {
    assert(NULL != stack);

    return stack->count;
}

int32_t stack_count_ts (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);

    int32_t ret = stack->count;

    pthread_mutex_unlock(stack->mutex);

    return ret;
}

void* stack_head (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->head);

    return stack->head->data;
}

void* stack_head_ts (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->head);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);

    void* ret = stack->head->data;

    pthread_mutex_unlock(stack->mutex);

    return ret;
}

bool stack_init (Stack* stack, StackType type, bool thread_safe) {
    assert(NULL != stack);
    assert(0 == stack->count);

    stack->count = 0;
    stack->head  = NULL;
    stack->mutex = NULL;
    stack->tail  = NULL;
    stack->type  = type;

    if (thread_safe) {
        stack->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

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
#include <stdio.h>
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

        if (0 == stack->count) {
            stack->head = item;
        } else {
            stack->tail->next = item;
        }

        stack->tail = item;
    } else {
        item->next  = stack->head;
        stack->head = item;

        if (0 == stack->count) {
            stack->tail = item;
        }
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

void* stack_tail (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->tail);

    return stack->tail->data;
}

void* stack_tail_ts (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->tail);
    assert(NULL != stack->mutex);

    pthread_mutex_lock(stack->mutex);

    void* ret = stack->tail->data;

    pthread_mutex_unlock(stack->mutex);

    return ret;
}

void stack_unlock (Stack* stack) {
    assert(NULL != stack);
    assert(NULL != stack->mutex);

    pthread_mutex_unlock(stack->mutex);
}
