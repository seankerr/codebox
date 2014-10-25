/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_BUFFER_H
#define __TEST_BUFFER_H

#include <assert.h>
#include <stdlib.h>

#include "codebox/container/buffer.h"

// -------------------------------------------------------------------------------------------------
// STATIC VARIABLES
// -------------------------------------------------------------------------------------------------

static uint8_t __BUFFER_CHUNK_SIZE = sizeof(char*) > sizeof(int32_t)
                                     ? sizeof(char*) : sizeof(int32_t);

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define buffer_get_str(__buffer) \
    ((char*) __buffer->data)

#define buffer_indexof_str(__buffer, __data) \
    buffer_indexof(__buffer, (unsigned char*) __data, strlen(__data))

#define buffer_insert_str(__buffer, __index, __data) \
    buffer_insert(__buffer, __index, (unsigned char*)  __data, strlen(__data))

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_buffer () {
    Buffer* b = buffer_new();
    Buffer* c = NULL;

    assert(NULL != b);
    assert(buffer_init(b, 1, true));

    assert(__BUFFER_CHUNK_SIZE == b->size);
    assert(buffer_append_str(b, "Buffer"));
    assert(6 == b->length);
    assert(0 == b->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Buffer", buffer_get_str(b), b->length));
    assert(buffer_insert_str(b, 0, "Fancy "));
    assert(12 == b->length);
    assert(0 == b->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Fancy Buffer", buffer_get_str(b), b->length));
    assert(buffer_insert_str(b, b->length, " Fluffer"));
    assert(20 == b->length);
    assert(0 == b->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Fancy Buffer Fluffer", buffer_get_str(b), b->length));
    assert(buffer_insert_str(b, 6, "Bopper "));
    assert(27 == b->length);
    assert(0 == b->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Fancy Bopper Buffer Fluffer", buffer_get_str(b), b->length));

    c = buffer_copy(b, 0, 5);

    assert(NULL != c);
    assert(5 == c->length);
    assert(0 == c->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Fancy", buffer_get_str(c), c->length));
    assert(buffer_cleanup(c));

    assert(buffer_remove(b, 0, 6));
    assert(21 == b->length);
    assert(0 == strncmp("Bopper Buffer Fluffer", buffer_get_str(b), b->length));

    c = buffer_copy(b, 0, 21);

    assert(NULL != c);
    assert(21 == c->length);
    assert(0 == c->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Bopper Buffer Fluffer", buffer_get_str(c), c->length));
    assert(buffer_cleanup(c));

    c = buffer_copy(b, 0, 6);

    assert(NULL != c);
    assert(6 == c->length);
    assert(0 == c->size % __BUFFER_CHUNK_SIZE);
    assert(0 == strncmp("Bopper", buffer_get_str(c), c->length));
    assert(buffer_cleanup(c));

    assert(buffer_remove(b, 7, 7));
    assert(14 == b->length);
    assert(0 == strncmp("Bopper Fluffer", buffer_get_str(b), b->length));

    assert(buffer_remove(b, 6, 8));
    assert(6 == b->length);
    assert(0 == strncmp("Bopper", buffer_get_str(b), b->length));

    assert(buffer_cleanup(b));
    free(b);
}

#endif
