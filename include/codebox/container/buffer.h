/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef struct {
    /** The data. */
    unsigned char* data;

    /** The length of the data. */
    int32_t length;

    /** The size of buffer. */
    int32_t size;
} Buffer;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define buffer_append_str(__buffer, __data) \
    buffer_append(__buffer, (unsigned char*) __data, strlen(__data))

#define buffer_get_str(__buffer) \
    ((char*) __buffer->data)

#define buffer_indexof_str(__buffer, __data) \
    buffer_indexof(__buffer, (unsigned char*) __data, strlen(__data))

#define buffer_insert_str(__buffer, __index, __data) \
    buffer_insert(__buffer, __index, (unsigned char*)  __data, strlen(__data))

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Append data onto the end of a buffer.
 *
 * @param buffer The buffer.
 * @param data   The data.
 * @param length The length of the data.
 */
bool buffer_append (Buffer* buffer, unsigned char* data, int32_t length);

/**
 * Cleanup a buffer.
 *
 * @param buffer The buffer.
 */
bool buffer_cleanup (Buffer* buffer);

/**
 * Copy a slice of a buffer.
 *
 * @param buffer The buffer.
 * @param start  The starting position.
 * @param length The length.
 */
Buffer* buffer_copy (Buffer* buffer, int32_t start, int32_t length);

/**
 * Find the starting index of data in a buffer.
 *
 * @param buffer The buffer.
 * @param data   The data.
 * @param length The length of the data.
 */
int32_t buffer_indexof (Buffer* buffer, unsigned char* data, int32_t length);

/**
 * Initialize a buffer.
 *
 * @param buffer The buffer.
 * @param size   The initial size.
 */
bool buffer_init (Buffer* buffer, int32_t size);

/**
 * Insert data into a buffer.
 *
 * @param buffer The buffer.
 * @param index  The index.
 * @param data   The data.
 * @param length The length of the data.
 */
bool buffer_insert (Buffer* buffer, int32_t index, unsigned char* data, int32_t length);

/**
 * Create a new buffer.
 */
Buffer* buffer_new ();

/**
 * Remove a slice of a buffer.
 *
 * @param buffer The buffer.
 * @param start  The starting position.
 * @param length The length.
 */
bool buffer_remove (Buffer* buffer, int32_t start, int32_t length);

/**
 * Resize a buffer.
 *
 * @param buffer The buffer.
 * @param size   The expected size.
 */
bool buffer_resize (Buffer* buffer, int32_t size);

#ifdef __cplusplus
}
#endif

#endif
