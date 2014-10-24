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

#include "codebox/container/buffer.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __BUFFER_ALIGN_SIZE(__size) \
    (__size) < __BUFFER_CHUNK_SIZE \
    ? __BUFFER_CHUNK_SIZE \
    : (__size) % __BUFFER_CHUNK_SIZE == 0 \
      ? (__size) \
      : (__size) + __BUFFER_CHUNK_SIZE - ((__size) % __BUFFER_CHUNK_SIZE)

// -------------------------------------------------------------------------------------------------
// STATIC VARIABLES
// -------------------------------------------------------------------------------------------------

static uint8_t __BUFFER_CHUNK_SIZE = sizeof(char*);

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool buffer_append (Buffer* buffer, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(0 < length);

    if (buffer->size < buffer->length + length &&
        !buffer_resize(buffer, buffer->length + length)) {
        return false;
    }

    memcpy(buffer->data + buffer->length, data, length);

    buffer->length += length;

    return true;
}

bool buffer_append_ts (Buffer* buffer, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    bool ret = buffer_append(buffer, data, length);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

bool buffer_cleanup (Buffer* buffer) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);

    free(buffer->data);

    if (NULL != buffer->mutex) {
        pthread_mutex_destroy(buffer->mutex);
    }

    return true;
}

Buffer* buffer_copy (Buffer* buffer, int32_t start, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(0 <= start);
    assert(0 < length);
    assert(start + length <= buffer->length);

    Buffer* copy = buffer_new();

    if (NULL == copy) {
        return NULL;
    } else if (!buffer_init(copy, length, NULL != buffer->mutex)) {
        free(copy);

        return NULL;
    } else if (!buffer_append(copy, buffer->data + start, length)) {
        buffer_cleanup(copy);
        free(copy);

        return NULL;
    }

    return copy;
}

Buffer* buffer_copy_ts (Buffer* buffer, int32_t start, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    Buffer* ret = buffer_copy(buffer, start, length);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

int32_t buffer_indexof (Buffer* buffer, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(NULL != data);
    assert(0 < length);

    for (int32_t i = 0, s = 0; i < buffer->length; i++) {
        if (*(buffer->data + i) == *(((unsigned char*) data) + s)) {
            if (++s == length) {
                return i - (length - 1);
            }
        } else {
            s = 0;
        }
    }

    return -1;
}

int32_t buffer_indexof_ts (Buffer* buffer, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    int32_t ret = buffer_indexof(buffer, data, length);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

bool buffer_init (Buffer* buffer, int32_t size, bool thread_safe) {
    assert(NULL != buffer);
    assert(NULL == buffer->data);
    assert(0 < size);

    buffer->length = 0;
    buffer->size   = __BUFFER_ALIGN_SIZE(size);
    buffer->data   = malloc(buffer->size);
    buffer->mutex  = NULL;

    if (NULL == buffer->data) {
        return false;
    }

    if (thread_safe) {
        pthread_mutex_init(buffer->mutex, NULL);
    }

    return true;
}

bool buffer_insert (Buffer* buffer, int32_t index, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(0 < length);
    assert(0 <= index && index <= buffer->length);

    if (index == buffer->length) {
        return buffer_append(buffer, data, length);
    }

    if (buffer->size < buffer->length + length && !buffer_resize(buffer, buffer->length + length)) {
        return false;
    }

    unsigned char* tmp = malloc(__BUFFER_ALIGN_SIZE(buffer->length - index));

    if (NULL == tmp) {
        return false;
    }

    memcpy(tmp, buffer->data + index, buffer->length - index);
    memcpy(buffer->data + index + length, tmp, buffer->length - index);
    memcpy(buffer->data + index, data, length);
    free(tmp);

    buffer->length += length;

    return true;
}

bool buffer_insert_ts (Buffer* buffer, int32_t index, unsigned char* data, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    bool ret = buffer_insert(buffer, index, data, length);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

void buffer_lock (Buffer* buffer) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);
}

Buffer* buffer_new () {
    Buffer* buffer = (Buffer*) malloc(sizeof(Buffer));

    if (NULL == buffer) {
        return NULL;
    }

    memset(buffer, 0, sizeof(Buffer));

    return buffer;
}

bool buffer_remove (Buffer* buffer, int32_t start, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(0 <= start);
    assert(0 < length);
    assert(start + length <= buffer->length);

    int32_t move_length = buffer->length - (start + length);

    if (buffer->length == move_length) {
        buffer->length = 0;

        return true;
    }

    memcpy(buffer->data + start, ((char*) buffer->data) + start + length, move_length);

    buffer->length -= length;

    return true;
}

bool buffer_remove_ts (Buffer* buffer, int32_t start, int32_t length) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    bool ret = buffer_remove(buffer, start, length);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

bool buffer_resize (Buffer* buffer, int32_t size) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);
    assert(0 < size);

    if (0 == size) {
        return false;
    }

    int32_t        _size = __BUFFER_ALIGN_SIZE(size);
    unsigned char* ptr   = realloc(buffer->data, _size);

    if (NULL != ptr) {
        buffer->data   = ptr;
        buffer->length = (buffer->length < size) ? buffer->length : size;
        buffer->size   = _size;

        return true;
    }

    ptr = malloc(_size);

    if (NULL == ptr) {
        return false;
    }

    memcpy(ptr, buffer->data, buffer->length);
    free(buffer->data);

    buffer->data   = ptr;
    buffer->length = (buffer->length < size) ? buffer->length : size;
    buffer->size   = _size;

    return true;
}

bool buffer_resize_ts (Buffer* buffer, int32_t size) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    bool ret = buffer_resize(buffer, size);

    pthread_mutex_unlock(buffer->mutex);

    return ret;
}

void buffer_truncate (Buffer* buffer) {
    assert(NULL != buffer);
    assert(NULL != buffer->data);

    buffer->length = 0;
}

void buffer_truncate_ts (Buffer* buffer) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_lock(buffer->mutex);

    buffer_truncate(buffer);

    pthread_mutex_unlock(buffer->mutex);
}

void buffer_unlock (Buffer* buffer) {
    assert(NULL != buffer);
    assert(NULL != buffer->mutex);

    pthread_mutex_unlock(buffer->mutex);
}
