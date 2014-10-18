/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_STACK_H
#define __TEST_STACK_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/container/stack.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_stack () {
    Stack* s = stack_new();

    assert(NULL != s);

    // lifo tests
    assert(stack_init(s, STACK_LIFO));

    // init check
    assert(0 == s->count);
    assert(NULL == s->head);
    assert(NULL == s->tail);

    // push a few items
    assert(stack_push(s, "3"));
    assert(1 == s->count);
    assert(stack_push(s, "2"));
    assert(2 == s->count);
    assert(stack_push(s, "1"));
    assert(3 == s->count);
    assert(!strcmp(stack_pop_str(s), "1"));
    assert(2 == s->count);
    assert(!strcmp(stack_pop_str(s), "2"));
    assert(1 == s->count);
    assert(!strcmp(stack_pop_str(s), "3"));
    assert(0 == s->count);

    // fifo tests
    assert(stack_init(s, STACK_FIFO));

    // init check
    assert(0 == s->count);
    assert(NULL == s->head);
    assert(NULL == s->tail);

    // push a few items
    assert(stack_push(s, "3"));
    assert(1 == s->count);
    assert(stack_push(s, "2"));
    assert(2 == s->count);
    assert(stack_push(s, "1"));
    assert(3 == s->count);
    assert(!strcmp(stack_pop_str(s), "3"));
    assert(2 == s->count);
    assert(!strcmp(stack_pop_str(s), "2"));
    assert(1 == s->count);
    assert(!strcmp(stack_pop_str(s), "1"));
    assert(0 == s->count);
    free(s);
}

#endif
