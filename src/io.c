/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/io.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

unsigned char* io_file_read (char* path, uint32_t* length) {
    unsigned char* data    = NULL;
    FILE*          file    = fopen(path, "rb");
    uint32_t       rlength = 0;

    if (NULL == file) {
        *length = 0;

        return NULL;
    }

    fseek(file, 0, SEEK_END);

    *length = ftell(file);

    fseek(file, 0, SEEK_SET);

    data = (unsigned char*) malloc(sizeof(unsigned char) * (*length));

    if (NULL == data) {
        *length = 0;

        fclose(file);

        return NULL;
    }

    rlength = fread(data, 1, *length, file);

    if (rlength < *length) {
        *length = 0;

        fclose(file);
        free(data);

        return NULL;
    }

    fclose(file);

    return data;
}

char* io_file_read_str (char* path) {
    char*    data    = NULL;
    FILE*    file    = fopen(path, "r");
    uint32_t length  = 0;
    uint32_t rlength = 0;

    if (NULL == file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    length = ftell(file);

    fseek(file, 0, SEEK_SET);

    data = (char*) malloc(sizeof(char) * length + 1);

    if (NULL == data) {
        fclose(file);

        return NULL;
    }

    rlength = fread(data, 1, length, file);

    if (rlength < length) {
        fclose(file);
        free(data);

        return NULL;
    }

    fclose(file);

    data[length] = '\0';

    return data;
}
