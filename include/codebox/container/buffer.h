/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_BUFFER_H
#define __CODEBOX_BUFFER_H

#include <pthread.h>
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

    /** The mutex. */
    pthread_mutex_t* mutex;

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
 * Append data onto the end of a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param data   The data.
 * @param length The length of the data.
 */
bool buffer_append_ts (Buffer* buffer, unsigned char* data, int32_t length);

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
 * Copy a slice of a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param start  The starting position.
 * @param length The length.
 */
Buffer* buffer_copy_ts (Buffer* buffer, int32_t start, int32_t length);

/**
 * Find the starting index of data in a buffer.
 *
 * @param buffer The buffer.
 * @param data   The data.
 * @param length The length of the data.
 */
int32_t buffer_indexof (Buffer* buffer, unsigned char* data, int32_t length);

/**
 * Find the starting index of data in a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param data   The data.
 * @param length The length of the data.
 */
int32_t buffer_indexof_ts (Buffer* buffer, unsigned char* data, int32_t length);

/**
 * Initialize a buffer.
 *
 * @param buffer      The buffer.
 * @param size        The initial size.
 * @param thread_safe Indicates that a mutex will be initialized.
 */
bool buffer_init (Buffer* buffer, int32_t size, bool thread_safe);

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
 * Insert data into a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param index  The index.
 * @param data   The data.
 * @param length The length of the data.
 */
bool buffer_insert_ts (Buffer* buffer, int32_t index, unsigned char* data, int32_t length);

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
 * Remove a slice of a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param start  The starting position.
 * @param length The length.
 */
bool buffer_remove_ts (Buffer* buffer, int32_t start, int32_t length);

/**
 * Resize a buffer.
 *
 * @param buffer The buffer.
 * @param size   The expected size.
 */
bool buffer_resize (Buffer* buffer, int32_t size);

/**
 * Resize a buffer using thread safety.
 *
 * @param buffer The buffer.
 * @param size   The expected size.
 */
bool buffer_resize_ts (Buffer* buffer, int32_t size);

/**
 * Truncate a buffer.
 *
 * @param buffer The buffer.
 */
void buffer_truncate (Buffer* buffer);

/**
 * Truncate a buffer using thread safety.
 *
 * @param buffer The buffer.
 */
void buffer_truncate_ts (Buffer* buffer);

#ifdef __cplusplus
}
#endif

#endif
