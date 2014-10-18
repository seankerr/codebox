/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __GLES2_H
#define __GLES2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef float matrix4[16];

typedef struct __matrix_state {
    /** The next matrix state. */
    struct __matrix_state* next;

    /** The matrix. */
    matrix4 matrix;
} MatrixState;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define GL_PROJECTION 1
#define GL_MODELVIEW  2

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Cleanup the left-over matrix data.
 */
void gl_cleanup ();

/**
 * Replace the current matrix with a perspective matrix.
 *
 * @param left   The left vertical clipping plane.
 * @param right  The right vertical clipping plane.
 * @param bottom The bottom horizontal clipping plane.
 * @param top    The top horizontal clipping plane.
 * @param near   The near clipping plane.
 * @param far    The far clipping plane.
 */
void gl_frustum (float left, float right, float bottom, float top, float near, float far);

/**
 * Retrieve an identity matrix.
 *
 * @param matrix The matrix.
 */
void gl_identity (matrix4 matrix);

/**
 * Replace the current matrix with an identity matrix.
 */
void gl_load_identity ();

/**
 * Retrieve the projection matrix multiplied by the modelview matrix.
 *
 * @param matrix The matrix.
 */
void gl_matrix (matrix4 matrix);

/**
 * Replace the current matrix with an orthographic matrix.
 *
 * @param left   The left vertical clipping plane.
 * @param right  The right vertical clipping plane.
 * @param bottom The bottom horizontal clipping plane.
 * @param top    The top horizontal clipping plane.
 * @param near   The near clipping plane.
 * @param far    The far clipping plane.
 */
void gl_ortho (float left, float right, float bottom, float top, float near, float far);

/**
 * Set the matrix mode.
 *
 * @param mode The matrix mode.
 */
void gl_matrix_mode (uint32_t mode);

/**
 * Pop the current matrix off the matrix stack.
 */
void gl_pop_matrix ();

/**
 * Push the current matrix onto the matrix stack.
 */
void gl_push_matrix ();

/**
 * Rotate the current matrix.
 *
 * @param x The x rotation.
 * @param y The y rotation.
 * @param z The z rotation.
 */
void gl_rotate (float x, float y, float z);

/**
 * Scale the current matrix.
 *
 * @param x The x scale.
 * @param y The y scale.
 * @param z The z scale.
 */
void gl_scale (float x, float y, float z);

/**
 * Translate the current matrix.
 *
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param z The z coordinate.
 */
void gl_translate (float x, float y, float z);

/**
 * Multiply two matrices in column-major form.
 *
 * @param m1     The first matrix.
 * @param m2     The second matrix.
 * @param result The resulting matrix.
 */
void matrix4_mult (matrix4 m1, matrix4 m2, matrix4 result);

#ifdef __cplusplus
}
#endif

#endif
