/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_LIST_H
#define __CODEBOX_LIST_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef struct __list_item {
    /** The data. */
    void* data;

    /** The next item. */
    struct __list_item* next;

    /** The previous item. */
    struct __list_item* prev;
} ListItem;

typedef struct {
    /** The head item. */
    ListItem* head;

    /** The mutex. */
    pthread_mutex_t* mutex;

    /** The tail item. */
    ListItem* tail;

    /** The count of items. */
    uint32_t count;
} List;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define list_head(__list) \
    __list->head->data

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

#define list_tail(__list) \
    __list->tail->data

#define list_tail_str(__list) \
    ((char *) __list->tail->data)

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Cleanup a list.
 *
 * @param list The list.
 */
bool list_cleanup (List* list);

/**
 * Retrieve data from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
void* list_get (List* list, uint32_t index);

/**
 * Retrieve data from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
void* list_get_ts (List* list, uint32_t index);

/**
 * Initialize a list.
 *
 * @param list        The list.
 * @param thread_safe Indicates that a mutex will be initialized.
 */
bool list_init (List* list, bool thread_safe);

/**
 * Insert data into a list.
 *
 * @param list  The list.
 * @param index The index.
 * @param data  The data.
 */
bool list_insert (List* list, uint32_t index, void* data);

/**
 * Insert data into a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 * @param data  The data.
 */
bool list_insert_ts (List* list, uint32_t index, void* data);

/**
 * Lock a list if it was initialized as thread-safe.
 *
 * @param list The list.
 */
void list_lock (List* list);

/**
 * Create a new list.
 */
List* list_new ();

/**
 * Pop data off the head of a list.
 *
 * @param list The list.
 */
void* list_pop_head (List* list);

/**
 * Pop data off the head of a list using thread safety.
 *
 * @param list The list.
 */
void* list_pop_head_ts (List* list);

/**
 * Pop data off the tail of a list.
 *
 * @param list The list.
 */
void* list_pop_tail (List* list);

/**
 * Pop data off the tail of a list using thread safety.
 *
 * @param list The list.
 */
void* list_pop_tail_ts (List* list);

/**
 * Push data onto the head of a list.
 *
 * @param list The list.
 * @param data The data.
 */
bool list_push_head (List* list, void* data);

/**
 * Push data onto the head of a list using thread safety.
 *
 * @param list The list.
 * @param data The data.
 */
bool list_push_head_ts (List* list, void* data);

/**
 * Push data onto the tail of a list.
 *
 * @param list The list.
 * @param data The data.
 */
bool list_push_tail (List* list, void* data);

/**
 * Push data onto the tail of a list using thread safety.
 *
 * @param list The list.
 * @param data The data.
 */
bool list_push_tail_ts (List* list, void* data);

/**
 * Remove data from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
void* list_remove (List* list, uint32_t index);

/**
 * Remove data from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
void* list_remove_ts (List* list, uint32_t index);

/**
 * Unlock a list if it was initialized as thread-safe.
 *
 * @param list The list.
 */
void list_unlock (List* list);

#ifdef __cplusplus
}
#endif

#endif
