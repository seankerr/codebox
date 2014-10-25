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

typedef struct __dlist_item {
    /** The data. */
    void* data;

    /** The next item. */
    struct __dlist_item* next;

    /** The previous item. */
    struct __dlist_item* prev;
} DListItem;

typedef struct {
    /** The head item. */
    DListItem* head;

    /** The mutex. */
    pthread_mutex_t* mutex;

    /** The tail item. */
    DListItem* tail;

    /** The count of items. */
    uint32_t count;
} DList;

typedef struct __list_item {
    /** The data. */
    void* data;

    /** The next item. */
    struct __list_item* next;
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
// DOUBLY LINKED LIST FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Cleanup a list.
 *
 * @param list The list.
 */
bool dlist_cleanup (DList* list);

/**
 * Retrieve the count of items in a list.
 *
 * @param list The list.
 */
uint32_t dlist_count (DList* list);

/**
 * Retrieve the count of items in a list using thread safety.
 *
 * @param list The list.
 */
uint32_t dlist_count_ts (DList* list);

/**
 * Retrieve data from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
void* dlist_get (DList* list, uint32_t index);

/**
 * Retrieve data from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
void* dlist_get_ts (DList* list, uint32_t index);

/**
 * Retrieve an item from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
DListItem* dlist_get_item (DList* list, uint32_t index);

/**
 * Retrieve an item from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
DListItem* dlist_get_item_ts (DList* list, uint32_t index);

/**
 * Retrieve the head of a list.
 *
 * @param list The list.
 */
void* dlist_head (DList* list);

/**
 * Retrieve the head of a list using thread safety.
 *
 * @param list The list.
 */
void* dlist_head_ts (DList* list);

/**
 * Retrieve the head item of a list.
 *
 * @param list The list.
 */
DListItem* dlist_head_item (DList* list);

/**
 * Retrieve the head item of a list using thread safety.
 *
 * @param list The list.
 */
DListItem* dlist_head_item_ts (DList* list);

/**
 * Initialize a list.
 *
 * @param list        The list.
 * @param thread_safe Indicates that a mutex will be initialized.
 */
bool dlist_init (DList* list, bool thread_safe);

/**
 * Insert data into a list.
 *
 * @param list  The list.
 * @param index The index.
 * @param data  The data.
 */
bool dlist_insert (DList* list, uint32_t index, void* data);

/**
 * Insert data into a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 * @param data  The data.
 */
bool dlist_insert_ts (DList* list, uint32_t index, void* data);

/**
 * Lock a list if it was initialized as thread-safe.
 *
 * @param list The list.
 */
void dlist_lock (DList* list);

/**
 * Create a new list.
 */
DList* dlist_new ();

/**
 * Pop data off the head of a list.
 *
 * @param list The list.
 */
void* dlist_pop_head (DList* list);

/**
 * Pop data off the head of a list using thread safety.
 *
 * @param list The list.
 */
void* dlist_pop_head_ts (DList* list);

/**
 * Pop data off the tail of a list.
 *
 * @param list The list.
 */
void* dlist_pop_tail (DList* list);

/**
 * Pop data off the tail of a list using thread safety.
 *
 * @param list The list.
 */
void* dlist_pop_tail_ts (DList* list);

/**
 * Push data onto the head of a list.
 *
 * @param list The list.
 * @param data The data.
 */
bool dlist_push_head (DList* list, void* data);

/**
 * Push data onto the head of a list using thread safety.
 *
 * @param list The list.
 * @param data The data.
 */
bool dlist_push_head_ts (DList* list, void* data);

/**
 * Push data onto the tail of a list.
 *
 * @param list The list.
 * @param data The data.
 */
bool dlist_push_tail (DList* list, void* data);

/**
 * Push data onto the tail of a list using thread safety.
 *
 * @param list The list.
 * @param data The data.
 */
bool dlist_push_tail_ts (DList* list, void* data);

/**
 * Remove data from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
void* dlist_remove (DList* list, uint32_t index);

/**
 * Remove data from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
void* dlist_remove_ts (DList* list, uint32_t index);

/**
 * Retrieve the tail of a list.
 *
 * @param list The list.
 */
void* dlist_tail (DList* list);

/**
 * Retrieve the tail of a list using thread safety.
 *
 * @param list The list.
 */
void* dlist_tail_ts (DList* list);

/**
 * Retrieve the tail item of a list.
 *
 * @param list The list.
 */
DListItem* dlist_tail_item (DList* list);

/**
 * Retrieve the tail item of a list using thread safety.
 *
 * @param list The list.
 */
DListItem* dlist_tail_item_ts (DList* list);

/**
 * Unlock a list if it was initialized as thread-safe.
 *
 * @param list The list.
 */
void dlist_unlock (DList* list);

// -------------------------------------------------------------------------------------------------
// SINGLY LINKED LIST FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Cleanup a list.
 *
 * @param list The list.
 */
bool list_cleanup (List* list);

/**
 * Retrieve the count of items in a list.
 *
 * @param list The list.
 */
uint32_t list_count (List* list);

/**
 * Retrieve the count of items in a list using thread safety.
 *
 * @param list The list.
 */
uint32_t list_count_ts (List* list);

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
 * Retrieve an item from a list.
 *
 * @param list  The list.
 * @param index The index.
 */
ListItem* list_get_item (List* list, uint32_t index);

/**
 * Retrieve an item from a list using thread safety.
 *
 * @param list  The list.
 * @param index The index.
 */
ListItem* list_get_item_ts (List* list, uint32_t index);

/**
 * Retrieve the head of a list.
 *
 * @param list The list.
 */
void* list_head (List* list);

/**
 * Retrieve the head of a list using thread safety.
 *
 * @param list The list.
 */
void* list_head_ts (List* list);

/**
 * Retrieve the head item of a list.
 *
 * @param list The list.
 */
ListItem* list_head_item (List* list);

/**
 * Retrieve the head item of a list using thread safety.
 *
 * @param list The list.
 */
ListItem* list_head_item_ts (List* list);

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
 * Retrieve the tail of a list.
 *
 * @param list The list.
 */
void* list_tail (List* list);

/**
 * Retrieve the tail of a list using thread safety.
 *
 * @param list The list.
 */
void* list_tail_ts (List* list);

/**
 * Retrieve the tail item of a list.
 *
 * @param list The list.
 */
ListItem* list_tail_item (List* list);

/**
 * Retrieve the tail item of a list using thread safety.
 *
 * @param list The list.
 */
ListItem* list_tail_item_ts (List* list);

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
