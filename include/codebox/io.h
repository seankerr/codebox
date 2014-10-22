/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_IO_H
#define __CODEBOX_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Read a binary file.
 *
 * @param path   The file path.
 * @param length The bytes read.
 */
unsigned char* io_file_read (char* path, uint32_t* length);

/**
 * Read an ASCII file.
 *
 * @param path The file path.
 */
char* io_file_read_str (char* path);

#ifdef __cplusplus
}
#endif

#endif
