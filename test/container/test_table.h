/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_TABLE_H
#define __TEST_TABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "codebox/container/table.h"

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_table () {
    Table* t = table_new();

    assert(NULL != t);
    assert(table_init_defaults(t));

    assert(table_put_str(t, "Key1", "Value1"));
    assert(table_put_str(t, "Key2", "Value2"));
    assert(table_put_str(t, "Key3", "Value3"));
    assert(table_put_str(t, "Key4", "Value4"));
    assert(table_put_str(t, "Key5", "Value5"));
    assert(table_put_str(t, "Key6", "Value6"));
    assert(table_put_str(t, "Key7", "Value7"));
    assert(table_put_str(t, "Key8", "Value8"));
    assert(table_put_str(t, "Key9", "Value9"));
    assert(9 == t->key_count);
    assert(0 == strcmp("Value1", table_get_str(t, "Key1")));
    assert(0 == strcmp("Value2", table_get_str(t, "Key2")));
    assert(0 == strcmp("Value3", table_get_str(t, "Key3")));
    assert(0 == strcmp("Value4", table_get_str(t, "Key4")));
    assert(0 == strcmp("Value5", table_get_str(t, "Key5")));
    assert(0 == strcmp("Value6", table_get_str(t, "Key6")));
    assert(0 == strcmp("Value7", table_get_str(t, "Key7")));
    assert(0 == strcmp("Value8", table_get_str(t, "Key8")));
    assert(0 == strcmp("Value9", table_get_str(t, "Key9")));
    assert(53 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(97 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(193 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(389 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(769 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(1543 == t->bucket_count);
    assert(1 == table_resize(t, t->bucket_count + 1));
    assert(3079 == t->bucket_count);
    assert(1 == table_has_key_str(t, "Key1"));
    assert(1 == table_has_key_str(t, "Key2"));
    assert(1 == table_has_key_str(t, "Key3"));
    assert(1 == table_has_key_str(t, "Key4"));
    assert(1 == table_has_key_str(t, "Key5"));
    assert(1 == table_has_key_str(t, "Key6"));
    assert(1 == table_has_key_str(t, "Key7"));
    assert(1 == table_has_key_str(t, "Key8"));
    assert(1 == table_has_key_str(t, "Key9"));
    assert(9 == t->key_count);
    assert(0 == strcmp("Value9", (char*) table_remove_str(t, "Key9")));
    assert(8 == t->key_count);
    assert(0 == strcmp("Value8", (char*) table_remove_str(t, "Key8")));
    assert(7 == t->key_count);
    assert(0 == strcmp("Value7", (char*) table_remove_str(t, "Key7")));
    assert(6 == t->key_count);
    assert(0 == strcmp("Value6", (char*) table_remove_str(t, "Key6")));
    assert(5 == t->key_count);
    assert(0 == strcmp("Value5", (char*) table_remove_str(t, "Key5")));
    assert(4 == t->key_count);
    assert(0 == strcmp("Value4", (char*) table_remove_str(t, "Key4")));
    assert(3 == t->key_count);
    assert(0 == strcmp("Value3", (char*) table_remove_str(t, "Key3")));
    assert(2 == t->key_count);
    assert(0 == strcmp("Value2", (char*) table_remove_str(t, "Key2")));
    assert(1 == t->key_count);
    assert(0 == strcmp("Value1", (char*) table_remove_str(t, "Key1")));
    assert(0 == t->key_count);
    assert(0 == table_has_key_str(t, "Key1"));
    assert(0 == table_has_key_str(t, "Key2"));
    assert(0 == table_has_key_str(t, "Key3"));
    assert(0 == table_has_key_str(t, "Key4"));
    assert(0 == table_has_key_str(t, "Key5"));
    assert(0 == table_has_key_str(t, "Key6"));
    assert(0 == table_has_key_str(t, "Key7"));
    assert(0 == table_has_key_str(t, "Key8"));
    assert(0 == table_has_key_str(t, "Key9"));
    assert(table_cleanup(t));
    free(t);
}

#endif
