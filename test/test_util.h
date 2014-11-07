/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_UTIL_H
#define __TEST_UTIL_H
#endif

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/util.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_util () {
    Token* t = split((unsigned char*) "The | quick | brown | fox | jumped | over | the | lazy | dog",
                     60, (unsigned char*) " | ", 3);
    Token* first = t;

    assert(NULL != t);
    assert(9 == token_count(t));
    assert(3 == t->length);
    assert(0 == strncmp("The", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(5 == t->length);
    assert(0 == strncmp("quick", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(5 == t->length);
    assert(0 == strncmp("brown", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("fox", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(6 == t->length);
    assert(0 == strncmp("jumped", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(4 == t->length);
    assert(0 == strncmp("over", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("the", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(4 == t->length);
    assert(0 == strncmp("lazy", (char*) t->data, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("dog", (char*) t->data, t->length));

    t = t->next;

    assert(NULL == t);
    token_cleanup(first);

    t = split((unsigned char*) "The | quick | brown | fox | jumped | over | the | lazy | dog",
              60, (unsigned char*) "XXX", 3);

    assert(NULL != t);
    assert(1 == token_count(t));
    assert(60 == t->length);
    assert(0 == strncmp("The | quick | brown | fox | jumped | over | the | lazy | dog",
                        (char*) t->data, t->length));
    token_cleanup(t);
}
