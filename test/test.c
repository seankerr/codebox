/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#include <stdio.h>

#include "container/test_buffer.h"
#include "container/test_list.h"
#include "container/test_stack.h"
#include "container/test_table.h"
#include "test_io.h"

int main (int arg, char** argv) {
    printf("Testing buffer...\n");
    test_buffer();
    printf("Testing list...\n");
    test_list();
    printf("Testing stack...\n");
    test_stack();
    printf("Testing table...\n");
    test_table();
    printf("Testing io...\n");
    test_io();
}
