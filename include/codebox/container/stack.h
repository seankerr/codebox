/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_STACK_H
#define __CODEBOX_STACK_H

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

    /** The tail item. */
    StackItem* tail;

    /** The count of items. */
    uint32_t count;

    /** The stack type. */
    StackType type;
} Stack;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define stack_pop_str(__stack) \
        (char*) stack_pop(__stack)

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Initialize a stack.
 *
 * @param stack The stack.
 * @param type  The type.
 */
bool stack_init (Stack* stack, StackType type);

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
 * Push data onto a stack.
 *
 * @param stack The stack.
 * @param data  The data.
 */
bool stack_push (Stack* stack, void* data);

#ifdef __cplusplus
}
#endif

#endif
