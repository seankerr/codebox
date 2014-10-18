/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_IO_H
#define __TEST_IO_H
#endif

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/io.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_io () {
    void*    data;
    uint32_t length;

    data = io_file_read("/Users/sean/Downloads/bluez-5.23.tar.xz", &length);

    assert(1426660 == length);

    free(data);

    data = io_file_read_str("/Users/sean/Downloads/test.txt");

    assert(20 == strlen(data));
    assert(!strcmp((char*) data, "This is a test file!"));

    free(data);
}
