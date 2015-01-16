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

#include "codebox/string.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

int32_t chr_indexof (unsigned char* data, int32_t data_length, int32_t start,
                     unsigned char* sequence, int32_t sequence_length) {
    assert(NULL != data);
    assert(NULL != sequence);
    assert(0 <= start);
    assert(0 < data_length);
    assert(0 < sequence_length);

    for (int32_t i = start, s = 0; i < data_length; i++) {
        if (*(data + i) == *(sequence + s)) {
            if (++s == sequence_length) {
                return i - (sequence_length - 1);
            }
        } else {
            s = 0;
        }
    }

    return -1;
}

Token* chr_split (unsigned char* data, int32_t data_length, unsigned char* delimiter,
                  int32_t delimiter_length) {
    assert(NULL != data);
    assert(NULL != delimiter);
    assert(0 < delimiter_length);

    Token* token = (Token*) malloc(sizeof(Token));

    if (NULL == token) {
        return NULL;
    }

    for (int32_t i = 0, s = 0; i < data_length; i++) {
        if (*(data + i) == *(delimiter + s)) {
            if (++s == delimiter_length) {
                token->data.cval = data;
                token->length    = ++i - delimiter_length;
                token->next      = chr_split(data + i, data_length - i, delimiter, delimiter_length);

                return token;
            }
        } else {
            s = 0;
        }
    }

    // didn't find the delimiter
    token->data.cval = data;
    token->length    = data_length;
    token->next      = NULL;

    return token;
}

int32_t str_indexof (char* data, int32_t start, char* sequence) {
    assert(NULL != data);
    assert(NULL != sequence);
    assert(0 <= start);

    uint32_t data_length     = strlen(data);
    uint32_t sequence_length = strlen(sequence);

    assert(0 < data_length);
    assert(0 < sequence_length);

    for (int32_t i = start, s = 0; i < data_length; i++) {
        if (*(data + i) == *(sequence + s)) {
            if (++s == sequence_length) {
                return i - (sequence_length - 1);
            }
        } else {
            s = 0;
        }
    }

    return -1;
}

Token* str_split (char* data, char* delimiter) {
    assert(NULL != data);
    assert(NULL != delimiter);

    uint32_t data_length      = strlen(data);
    uint32_t delimiter_length = strlen(delimiter);

    assert(0 < delimiter_length);

    Token* token = (Token*) malloc(sizeof(Token));

    if (NULL == token) {
        return NULL;
    }

    for (int32_t i = 0, s = 0; i < data_length; i++) {
        if (*(data + i) == *(delimiter + s)) {
            if (++s == delimiter_length) {
                token->data.sval = data;
                token->length    = ++i - delimiter_length;
                token->next      = str_split(data + i, delimiter);

                return token;
            }
        } else {
            s = 0;
        }
    }

    // didn't find the delimiter
    token->data.sval = data;
    token->length    = data_length;
    token->next      = NULL;

    return token;
}

void token_cleanup (Token* token) {
    if (NULL == token) {
        return;
    }

    Token* next = NULL;

    for (; NULL != token;) {
        next = token->next;

        free(token);

        token = next;
    }
}

uint32_t token_count (Token* token) {
    assert(NULL != token);

    uint32_t count = 0;

    for (;NULL != token;) {
        count++;

        token = token->next;
    }

    return count;
}
