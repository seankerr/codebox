/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#include <assert.h>
#include <stdlib.h>

#include "codebox/util.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

Token* split (unsigned char* data, uint32_t data_length, unsigned char* delimiter,
              uint32_t delimiter_length) {
    assert(NULL != data);
    assert(NULL != delimiter);
    assert(0 < data_length);
    assert(0 < delimiter_length);
    assert(delimiter_length <= data_length);

    for (int32_t i = 0, s = 0; i < data_length; i++) {
        if (*(data + i) == *(delimiter + s)) {
            if (++s == delimiter_length) {
                Token* token = (Token*) malloc(sizeof(Token));

                if (NULL == token) {
                    return NULL;
                }

                token->data   = data;
                token->length = ++i - delimiter_length;
                token->next   = split(data + i, data_length - i, delimiter, delimiter_length);

                return token;
            }
        } else {
            s = 0;
        }
    }

    // didn't find the delimiter
    Token* token = (Token*) malloc(sizeof(Token));

    if (NULL == token) {
        return NULL;
    }

    token->data   = data;
    token->length = data_length;
    token->next   = NULL;

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
