/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/gl.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __GL_180_PI_RAD 57.295780
#define __GL_PI_180_RAD 0.017453

#define __GL_DEGREES_TO_RADIANS(__x) (__x * __GL_PI_180_RAD)
#define __GL_RADIANS_TO_DEGREES(__x) (__x * __GL_180_PI_RAD)

// -------------------------------------------------------------------------------------------------
// STATIC VARIABLES
// -------------------------------------------------------------------------------------------------

static MatrixState** _gl_current_matrix    = NULL;
static MatrixState*  _gl_matrix_cache      = NULL;
static MatrixState*  _gl_modelview_matrix  = NULL;
static MatrixState*  _gl_projection_matrix = NULL;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __GL_FREE_STATE(__state) \
    while (NULL != __state) { \
        next = __state->next; \
        free(__state); \
        __state = next; \
    }

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void gl_cleanup () {
    MatrixState* next = NULL;

    __GL_FREE_STATE(_gl_modelview_matrix);
    __GL_FREE_STATE(_gl_projection_matrix);
    __GL_FREE_STATE(_gl_matrix_cache);
}

void gl_frustum (float left, float right, float bottom, float top, float near, float far) {
    memset((*_gl_current_matrix)->matrix, 0, sizeof(matrix4));

    (*_gl_current_matrix)->matrix[0]  = 2.0 * near / (right - left);
    (*_gl_current_matrix)->matrix[5]  = 2.0 * near / (top - bottom);
    (*_gl_current_matrix)->matrix[8]  = (right + left) / (right - left);
    (*_gl_current_matrix)->matrix[9]  = (top + bottom) / (top - bottom);
    (*_gl_current_matrix)->matrix[10] = -(far + near) / (far - near);
    (*_gl_current_matrix)->matrix[11] = -1.0;
    (*_gl_current_matrix)->matrix[14] = -(2.0 * far * near) / (far - near);
}

void gl_identity (matrix4 matrix) {
    memset(matrix, 0, sizeof(matrix4));

    matrix[0]  =
    matrix[5]  =
    matrix[10] =
    matrix[15] = 1.0;
}

void gl_load_identity () {
    if (NULL == *_gl_current_matrix) {
        *_gl_current_matrix = (MatrixState*) malloc(sizeof(MatrixState));

        if (NULL == *_gl_current_matrix) {
            return;
        }
    }

    memset((*_gl_current_matrix)->matrix, 0, sizeof(matrix4));

    (*_gl_current_matrix)->matrix[0]  =
    (*_gl_current_matrix)->matrix[5]  =
    (*_gl_current_matrix)->matrix[10] =
    (*_gl_current_matrix)->matrix[15] = 1.0;
    (*_gl_current_matrix)->next       = NULL;
}

void gl_matrix (matrix4 matrix) {
    assert(NULL != *_gl_current_matrix);
    assert(NULL != (*_gl_current_matrix)->matrix);

    matrix4_mult(_gl_projection_matrix->matrix, _gl_modelview_matrix->matrix, matrix);
}

void gl_matrix_mode (uint32_t mode) {
    if (mode == GL_PROJECTION) {
        _gl_current_matrix = &_gl_projection_matrix;
    } else if (mode == GL_MODELVIEW) {
        _gl_current_matrix = &_gl_modelview_matrix;
    }
}

void gl_ortho (float left, float right, float bottom, float top, float near, float far) {
    memset((*_gl_current_matrix)->matrix, 0, sizeof(matrix4));

    (*_gl_current_matrix)->matrix[0]  = 2.0 / (right - left);
    (*_gl_current_matrix)->matrix[5]  = 2.0 / (top - bottom);
    (*_gl_current_matrix)->matrix[10] = -2.0 / (far - near);
    (*_gl_current_matrix)->matrix[12] = -(right + left) / (right - left);
    (*_gl_current_matrix)->matrix[13] = -(top + bottom) / (top - bottom);
    (*_gl_current_matrix)->matrix[14] = -(far + near) / (far - near);
    (*_gl_current_matrix)->matrix[15] = 1.0;
}

void gl_pop_matrix () {
    assert(NULL != _gl_current_matrix);
    assert(NULL != *_gl_current_matrix);

    if (NULL == (*_gl_current_matrix)->next) {
        return;
    }

    MatrixState* state = *_gl_current_matrix;

    *_gl_current_matrix = state->next;
    state->next         = _gl_matrix_cache;
    _gl_matrix_cache    = state;
}

void gl_push_matrix () {
    MatrixState* state = _gl_matrix_cache;

    if (NULL == state) {
        state = (MatrixState*) malloc(sizeof(MatrixState));

        if (NULL == state) {
            return;
        }
    } else {
        _gl_matrix_cache = state->next;
    }

    memcpy(state->matrix, (*_gl_current_matrix)->matrix, sizeof(matrix4));

    state->next         = *_gl_current_matrix;
    *_gl_current_matrix = state;
}

void gl_rotate (float x, float y, float z) {
    matrix4 matrix;
    float   radians;
    matrix4 result;

    // x
    radians = __GL_DEGREES_TO_RADIANS(x);

    gl_identity(matrix);

    matrix[5]  = cosf(radians);
    matrix[6]  = -sinf(radians);
    matrix[9]  = -matrix[6];
    matrix[10] = matrix[5];

    matrix4_mult((*_gl_current_matrix)->matrix, matrix, result);
    memcpy((*_gl_current_matrix)->matrix, result, sizeof(matrix4));

    // y
    radians = __GL_DEGREES_TO_RADIANS(y);

    gl_identity(matrix);

    matrix[0]  = cosf(radians);
    matrix[2]  = sinf(radians);
    matrix[8]  = -matrix[2];
    matrix[10] = matrix[0];

    matrix4_mult((*_gl_current_matrix)->matrix, matrix, result);
    memcpy((*_gl_current_matrix)->matrix, result, sizeof(matrix4));

    // z
    radians = __GL_DEGREES_TO_RADIANS(z);

    gl_identity(matrix);

    matrix[0] = cosf(radians);
    matrix[1] = sinf(radians);
    matrix[4] = -matrix[1];
    matrix[5] = matrix[0];

    matrix4_mult((*_gl_current_matrix)->matrix, matrix, result);
    memcpy((*_gl_current_matrix)->matrix, result, sizeof(matrix4));
}

void gl_scale (float x, float y, float z) {
    matrix4 matrix;
    matrix4 result;

    gl_identity(matrix);

    matrix[0]  = x;
    matrix[5]  = y;
    matrix[10] = z;

    matrix4_mult((*_gl_current_matrix)->matrix, matrix, result);
    memcpy((*_gl_current_matrix)->matrix, result, sizeof(matrix4));
}

void gl_translate (float x, float y, float z) {
    matrix4 matrix;
    matrix4 result;

    gl_identity(matrix);

    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;

    matrix4_mult((*_gl_current_matrix)->matrix, matrix, result);
    memcpy((*_gl_current_matrix)->matrix, result, sizeof(matrix4));
}

void matrix4_mult (matrix4 m1, matrix4 m2, matrix4 result) {
    result[0]  = (m1[0] * m2[0]) + (m1[4] * m2[1]) + (m1[8] * m2[2]) + (m1[12] * m2[3]);
    result[1]  = (m1[1] * m2[0]) + (m1[5] * m2[1]) + (m1[9] * m2[2]) + (m1[13] * m2[3]);
    result[2]  = (m1[2] * m2[0]) + (m1[6] * m2[1]) + (m1[10] * m2[2]) + (m1[14] * m2[3]);
    result[3]  = (m1[3] * m2[0]) + (m1[7] * m2[1]) + (m1[11] * m2[2]) + (m1[15] * m2[3]);

    result[4]  = (m1[0] * m2[4]) + (m1[4] * m2[5]) + (m1[8] * m2[6]) + (m1[12] * m2[7]);
    result[5]  = (m1[1] * m2[4]) + (m1[5] * m2[5]) + (m1[9] * m2[6]) + (m1[13] * m2[7]);
    result[6]  = (m1[2] * m2[4]) + (m1[6] * m2[5]) + (m1[10] * m2[6]) + (m1[14] * m2[7]);
    result[7]  = (m1[3] * m2[4]) + (m1[7] * m2[5]) + (m1[11] * m2[6]) + (m1[15] * m2[7]);

    result[8]  = (m1[0] * m2[8]) + (m1[4] * m2[9]) + (m1[8] * m2[10]) + (m1[12] * m2[11]);
    result[9]  = (m1[1] * m2[8]) + (m1[5] * m2[9]) + (m1[9] * m2[10]) + (m1[13] * m2[11]);
    result[10] = (m1[2] * m2[8]) + (m1[6] * m2[9]) + (m1[10] * m2[10]) + (m1[14] * m2[11]);
    result[11] = (m1[3] * m2[8]) + (m1[7] * m2[9]) + (m1[11] * m2[10]) + (m1[15] * m2[11]);

    result[12] = (m1[0] * m2[12]) + (m1[4] * m2[13]) + (m1[8] * m2[14]) + (m1[12] * m2[15]);
    result[13] = (m1[1] * m2[12]) + (m1[5] * m2[13]) + (m1[9] * m2[14]) + (m1[13] * m2[15]);
    result[14] = (m1[2] * m2[12]) + (m1[6] * m2[13]) + (m1[10] * m2[14]) + (m1[14] * m2[15]);
    result[15] = (m1[3] * m2[12]) + (m1[7] * m2[13]) + (m1[11] * m2[14]) + (m1[15] * m2[15]);
}
