/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __TEST_LIST_H
#define __TEST_LIST_H

#include <assert.h>
#include <stdlib.h>

#include "codebox/container/list.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define list_head_str(__list) \
    ((char *) __list->head->data)

#define list_get_str(__list, __index) \
    ((char *) list_get(__list, __index))

#define list_pop_head_str(__list) \
    ((char *) list_pop_head(__list))

#define list_pop_tail_str(__list) \
    ((char *) list_pop_tail(__list))

#define list_remove_str(__list, __index) \
    ((char *) list_remove(__list, __index))

#define list_tail_str(__list) \
    ((char *) __list->tail->data)

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

void test_list () {
    List* l = list_new();

    assert(NULL != l);
    assert(list_init(l, true));

    // init check
    assert(0 == l->count);
    assert(NULL == l->head);
    assert(NULL == l->tail);

    // push head on empty list
    assert(list_push_head(l, "Head"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);
    assert(!strcmp(list_head_str(l), "Head"));

    // push head on non-empty list
    assert(list_push_head(l, "New Head"));
    assert(2 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(!strcmp(list_head_str(l), "New Head"));

    // pop head on list with more than 1 item
    assert(!strcmp(list_pop_head_str(l), "New Head"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);

    // pop head on list with 1 item
    assert(!strcmp(list_pop_head_str(l), "Head"));
    assert(0 == l->count);
    assert(NULL == l->head);
    assert(NULL == l->tail);

    // push tail on empty list
    assert(list_push_tail(l, "Tail"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);
    assert(!strcmp(list_tail_str(l), "Tail"));

    // push tail on non-empty list
    assert(list_push_tail(l, "New Tail"));
    assert(2 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(!strcmp(list_tail_str(l), "New Tail"));

    // pop tail on list with more than 1 item
    assert(!strcmp(list_pop_tail_str(l), "New Tail"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);

    // pop tail on list with 1 item
    assert(!strcmp(list_pop_tail_str(l), "Tail"));
    assert(0 == l->count);
    assert(NULL == l->head);
    assert(NULL == l->tail);

    // insert
    assert(list_insert(l, 0, "Item1"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);
    assert(!strcmp(list_head_str(l), "Item1"));
    assert(list_insert(l, 1, "Item3"));
    assert(2 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(!strcmp(list_tail_str(l), "Item3"));
    assert(list_insert(l, 1, "Item2"));
    assert(3 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(!strcmp(list_pop_tail_str(l), "Item3"));
    assert(2 == l->count);
    assert(!strcmp(list_pop_tail_str(l), "Item2"));
    assert(1 == l->count);
    assert(!strcmp(list_pop_tail_str(l), "Item1"));
    assert(0 == l->count);

    // insert and remove
    assert(list_insert(l, 0, "Item1"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head == l->tail);
    assert(list_insert(l, 1, "Item9"));
    assert(2 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item8"));
    assert(3 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item7"));
    assert(4 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item6"));
    assert(5 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item5"));
    assert(6 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item4"));
    assert(7 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item3"));
    assert(8 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(list_insert(l, 1, "Item2"));
    assert(9 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head != l->tail);
    assert(!strcmp(list_remove_str(l, 4), "Item5"));
    assert(8 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 4), "Item6"));
    assert(7 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 4), "Item7"));
    assert(6 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 4), "Item8"));
    assert(5 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 4), "Item9"));
    assert(4 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 3), "Item4"));
    assert(3 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 2), "Item3"));
    assert(2 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(!strcmp(list_remove_str(l, 1), "Item2"));
    assert(1 == l->count);
    assert(NULL != l->head);
    assert(NULL != l->tail);
    assert(l->head = l->tail);
    assert(!strcmp(list_remove_str(l, 0), "Item1"));
    assert(0 == l->count);
    assert(NULL == l->head);
    assert(NULL == l->tail);
    assert(list_cleanup(l));
    free(l);
}

#endif
