/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_STRING_H
#define __TEST_STRING_H
#endif

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/string.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_string () {
    assert(6 == str_indexof("Hello, world", 0, " "));
    assert(2 == str_indexof("Hello, world", 0, "l"));
    assert(3 == str_indexof("Hello, world", 3, "l"));
    assert(7 == str_indexof("Hello, world", 0, "world"));
    assert(-1 == str_indexof("Hello, world", 0, "World"));
    assert(12 == str_indexof("Hello, world ", 7, " "));

    Token* t = str_split("The | quick | brown | fox | jumped | over | the | lazy | dog | ", " | ");
    Token* first = t;

    assert(NULL != t);
    assert(10 == token_count(t));
    assert(3 == t->length);
    assert(0 == strncmp("The", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(5 == t->length);
    assert(0 == strncmp("quick", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(5 == t->length);
    assert(0 == strncmp("brown", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("fox", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(6 == t->length);
    assert(0 == strncmp("jumped", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(4 == t->length);
    assert(0 == strncmp("over", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("the", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(4 == t->length);
    assert(0 == strncmp("lazy", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(3 == t->length);
    assert(0 == strncmp("dog", t->data.sval, t->length));

    t = t->next;

    assert(NULL != t);
    assert(0 == t->length);

    token_cleanup(first);

    t = str_split("The | quick | brown | fox | jumped | over | the | lazy | dog", "XXX");

    assert(NULL != t);
    assert(1 == token_count(t));
    assert(60 == t->length);
    assert(0 == strncmp("The | quick | brown | fox | jumped | over | the | lazy | dog",
                        t->data.sval, t->length));
    token_cleanup(t);
}
