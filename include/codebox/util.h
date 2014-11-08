/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_UTIL_H
#define __CODEBOX_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef struct __token {
    /** The data. */
    unsigned char* data;

    /** The next token. */
    struct __token* next;

    /** The length. */
    uint32_t length;
} Token;

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Find the position of a character sequence.
 *
 * @param data            The data to search.
 * @param data_length     The length of the data.
 * @param start           The start position.
 * @param sequence        The character sequence.
 * @param sequence_length The length of the character sequence.
 */
int32_t indexof (unsigned char* data, int32_t data_length, int32_t start, unsigned char* sequence,
                 int32_t sequence_length);

/**
 * Split data into tokens.
 *
 * @param data             The data.
 * @param data_length      The length of the data.
 * @param delimiter        The delimiter.
 * @param delimiter_length The length of the delimiter.
 */
Token* split (unsigned char* data, int32_t data_length, unsigned char* delimiter,
              int32_t delimiter_length);

/**
 * Cleanup a token.
 *
 * @param token The token.
 */
void token_cleanup (Token* token);

/**
 * Retrieve a count of tokens.
 *
 * @param token The initial token.
 */
uint32_t token_count (Token* token);

#endif
