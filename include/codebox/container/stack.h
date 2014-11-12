/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_STACK_H
#define __CODEBOX_STACK_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef enum _stack_type {
    STACK_FIFO,
    STACK_LIFO
} StackType;

typedef struct _stack_item {
    /** The data. */
    void* data;

    /** The next item. */
    struct _stack_item* next;
} StackItem;

typedef struct {
    /** The head item. */
    StackItem* head;

    /** The mutex. */
    pthread_mutex_t* mutex;

    /** The tail item. */
    StackItem* tail;

    /** The count of items. */
    int32_t count;

    /** The stack type. */
    StackType type;
} Stack;

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Cleanup a stack.
 *
 * @param stack The stack.
 */
bool stack_cleanup (Stack* stack);

/**
 * Retrieve the count of items in a stack.
 *
 * @param stack The stack.
 */
int32_t stack_count (Stack* stack);

/**
 * Retrieve the count of items in a stack using thread safety.
 *
 * @param stack The stack.
 */
int32_t stack_count_ts (Stack* stack);

/**
 * Retrieve the head of a stack.
 *
 * @param stack The stack.
 */
void* stack_head (Stack* stack);

/**
 * Retrieve the head of a stack using thread safety.
 *
 * @param stack The stack.
 */
void* stack_head_ts (Stack* stack);

/**
 * Initialize a stack.
 *
 * @param stack       The stack.
 * @param type        The type.
 * @param thread_safe Indicates that a mutex will be initialized.
 */
bool stack_init (Stack* stack, StackType type, bool thread_safe);

/**
 * Lock a stack if it was initialized as thread-safe.
 *
 * @param stack The stack.
 */
void stack_lock (Stack* stack);

/**
 * Create a new stack.
 */
Stack* stack_new ();

/**
 * Pop data off a stack.
 *
 * @param stack The stack.
 * @param data  The data.
 */
void* stack_pop (Stack* stack);

/**
 * Pop data off a stack using thread safety.
 *
 * @param stack The stack.
 * @param data  The data.
 */
void* stack_pop_ts (Stack* stack);

/**
 * Push data onto a stack.
 *
 * @param stack The stack.
 * @param data  The data.
 */
bool stack_push (Stack* stack, void* data);

/**
 * Push data onto a stack using thread safety.
 *
 * @param stack The stack.
 * @param data  The data.
 */
bool stack_push_ts (Stack* stack, void* data);

/**
 * Retrieve the tail of a stack.
 *
 * @param stack The stack.
 */
void* stack_tail (Stack* stack);

/**
 * Retrieve the tail of a stack using thread safety.
 *
 * @param stack The stack.
 */
void* stack_tail_ts (Stack* stack);

/**
 * Unlock a stack if it was initialized as thread-safe.
 *
 * @param stack The stack.
 */
void stack_unlock (Stack* stack);

#ifdef __cplusplus
}
#endif

#endif
