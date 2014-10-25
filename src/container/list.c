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

#include "codebox/container/list.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __DLIST_FIND(__var, __list, __index) \
    if (__index == 0) { \
        __var = __list->head; \
    } else if (__index == __list->count - 1) { \
        __var = __list->tail; \
    } else if (__index < __list->count / 2) { \
        __var = __list->head; \
        for (uint32_t __i = 0; \
             __i < __list->count && NULL != __var && __i != __index; \
             __i++, __var = __var->next); \
    } else { \
        __var = __list->tail; \
        for (uint32_t __i = 0; \
             __i < __list->count && NULL != __var && list->count - 1 - __i != __index; \
             __i++, __var = __var->prev); \
    }

#define __LIST_FIND(__var, __list, __index) \
    if (__index == 0) { \
        __var = __list->head; \
    } else if (__index == __list->count - 1) { \
        __var = __list->tail; \
    } else { \
        __var = __list->head; \
        for (uint32_t __i = 0; \
             __i < __list->count && NULL != __var && __i != __index; \
             __i++, __var = __var->next); \
    }

// -------------------------------------------------------------------------------------------------
// DOUBLY LINKED LIST FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool dlist_cleanup (DList* list) {
    assert(NULL != list);
    assert(0 == list->count);

    if (NULL != list->mutex) {
        pthread_mutex_destroy(list->mutex);
    }

    return true;
}

uint32_t dlist_count (DList* list) {
    assert(NULL != list);

    return list->count;
}

uint32_t dlist_count_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    uint32_t ret = list->count;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* dlist_get (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(0 < list->count);

    DListItem* item = NULL;

    __DLIST_FIND(item, list, index);

    return NULL != item ? item->data : NULL;
}

void* dlist_get_ts (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = dlist_get(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

DListItem* dlist_get_item (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(0 < list->count);

    DListItem* item = NULL;

    __DLIST_FIND(item, list, index);

    return item;
}

DListItem* dlist_get_item_ts (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    DListItem* ret = dlist_get_item(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* dlist_head (DList* list) {
    assert(NULL != list);
    assert(NULL != list->head);

    return list->head->data;
}

void* dlist_head_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->head);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list->head->data;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

DListItem* dlist_head_item (DList* list) {
    assert(NULL != list);

    return list->head;
}

DListItem* dlist_head_item_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    DListItem* ret = list->head;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool dlist_init (DList* list, bool thread_safe) {
    assert(NULL != list);
    assert(0 == list->count);

    list->count = 0;
    list->head  = NULL;
    list->mutex = NULL;
    list->tail  = NULL;

    if (thread_safe) {
        list->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

        pthread_mutex_init(list->mutex, NULL);
    }

    return true;
}

bool dlist_insert (DList* list, uint32_t index, void* data) {
    assert(NULL != list);
    assert(0 <= index && index <= list->count);

    if (0 == index) {
        return dlist_push_head(list, data);
    } else if (index == list->count) {
        return dlist_push_tail(list, data);
    } else if (index > list->count) {
        return false;
    }

    DListItem* item     = NULL;
    DListItem* new_item = (DListItem*) malloc(sizeof(DListItem));

    if (NULL == new_item) {
        return false;
    }

    __DLIST_FIND(item, list, index);

    new_item->data       = data;
    new_item->next       = item;
    new_item->prev       = item->prev;
    new_item->next->prev = new_item;
    new_item->prev->next = new_item;
    list->count++;

    return true;
}

bool dlist_insert_ts (DList* list, uint32_t index, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = dlist_insert(list, index, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void dlist_lock (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);
}

DList* dlist_new () {
    DList* list = (DList*) malloc(sizeof(DList));

    if (NULL == list) {
        return NULL;
    }

    memset(list, 0, sizeof(DList));

    return list;
}

void* dlist_pop_head (DList* list) {
    assert(NULL != list);
    assert(NULL != list->head);
    assert(0 < list->count);

    DListItem* item = list->head;
    void*      data = item->data;

    list->head = item->next;
    list->count--;

    if (NULL != list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(item);

    return data;
}

void* dlist_pop_head_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = dlist_pop_head(list);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* dlist_pop_tail (DList* list) {
    assert(NULL != list);
    assert(NULL != list->tail);
    assert(0 < list->count);

    DListItem* item = list->tail;
    void*      data = item->data;

    list->tail = item->prev;
    list->count--;

    if (NULL != list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(item);

    return data;
}

void* dlist_pop_tail_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = dlist_pop_tail(list);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool dlist_push_head (DList* list, void* data) {
    assert(NULL != list);

    DListItem* item = (DListItem*) malloc(sizeof(DListItem));

    if (NULL == item) {
        return false;
    }

    item->data = data;
    item->next = list->head;
    item->prev = NULL;
    list->head = item;
    list->count++;

    if (NULL != item->next) {
        item->next->prev = item;
    } else {
        list->tail = item;
    }

    return true;
}

bool dlist_push_head_ts (DList* list, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = dlist_push_head(list, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool dlist_push_tail (DList* list, void* data) {
    assert(NULL != list);

    DListItem* item = (DListItem*) malloc(sizeof(DListItem));

    if (NULL == item) {
        return false;
    }

    item->data = data;
    item->next = NULL;
    item->prev = list->tail;
    list->tail = item;
    list->count++;

    if (NULL != item->prev) {
        item->prev->next = item;
    } else {
        list->head = item;
    }

    return true;
}

bool dlist_push_tail_ts (DList* list, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = dlist_push_tail(list, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* dlist_remove (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(0 <= index && index < list->count);

    if (0 == index) {
        return dlist_pop_head(list);
    } else if (index == list->count - 1) {
        return dlist_pop_tail(list);
    } else if (index >= list->count) {
        return false;
    }

    DListItem* item = NULL;

    __DLIST_FIND(item, list, index);

    void*     data      = item->data;
    DListItem* rem_item = item;

    rem_item->next->prev = rem_item->prev;
    rem_item->prev->next = rem_item->next;
    list->count--;

    free(rem_item);

    return data;
}

void* dlist_remove_ts (DList* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = dlist_remove(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* dlist_tail (DList* list) {
    assert(NULL != list);
    assert(NULL != list->tail);

    return list->tail->data;
}

void* dlist_tail_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->tail);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list->tail->data;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

DListItem* dlist_tail_item (DList* list) {
    assert(NULL != list);

    return list->tail;
}

DListItem* dlist_tail_item_ts (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    DListItem* ret = list->tail;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void dlist_unlock (DList* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_unlock(list->mutex);
}

// -------------------------------------------------------------------------------------------------
// SINGLY LIST FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool list_cleanup (List* list) {
    assert(NULL != list);
    assert(0 == list->count);

    if (NULL != list->mutex) {
        pthread_mutex_destroy(list->mutex);
    }

    return true;
}

uint32_t list_count (List* list) {
    assert(NULL != list);

    return list->count;
}

uint32_t list_count_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    uint32_t ret = list->count;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* list_get (List* list, uint32_t index) {
    assert(NULL != list);
    assert(0 < list->count);

    ListItem* item = NULL;

    __LIST_FIND(item, list, index);

    return NULL != item ? item->data : NULL;
}

void* list_get_ts (List* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list_get(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

ListItem* list_get_item (List* list, uint32_t index) {
    assert(NULL != list);
    assert(0 < list->count);

    ListItem* item = NULL;

    __LIST_FIND(item, list, index);

    return item;
}

ListItem* list_get_item_ts (List* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    ListItem* ret = list_get_item(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* list_head (List* list) {
    assert(NULL != list);
    assert(NULL != list->head);

    return list->head->data;
}

void* list_head_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->head);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list->head->data;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

ListItem* list_head_item (List* list) {
    assert(NULL != list);

    return list->head;
}

ListItem* list_head_item_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    ListItem* ret = list->head;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool list_init (List* list, bool thread_safe) {
    assert(NULL != list);
    assert(0 == list->count);

    list->count = 0;
    list->head  = NULL;
    list->mutex = NULL;
    list->tail  = NULL;

    if (thread_safe) {
        list->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

        pthread_mutex_init(list->mutex, NULL);
    }

    return true;
}

bool list_insert (List* list, uint32_t index, void* data) {
    assert(NULL != list);
    assert(0 <= index && index <= list->count);

    if (0 == index) {
        return list_push_head(list, data);
    } else if (index == list->count) {
        return list_push_tail(list, data);
    } else if (index > list->count) {
        return false;
    }

    ListItem* item     = NULL;
    ListItem* new_item = (ListItem*) malloc(sizeof(ListItem));

    if (NULL == new_item) {
        return false;
    }

    // we need the item previous to the insertion index
    __LIST_FIND(item, list, index - 1);

    new_item->data = data;
    new_item->next = item->next;
    item->next     = new_item;
    list->count++;

    return true;
}

bool list_insert_ts (List* list, uint32_t index, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = list_insert(list, index, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void list_lock (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);
}

List* list_new () {
    List* list = (List*) malloc(sizeof(List));

    if (NULL == list) {
        return NULL;
    }

    memset(list, 0, sizeof(List));

    return list;
}

void* list_pop_head (List* list) {
    assert(NULL != list);
    assert(NULL != list->head);
    assert(0 < list->count);

    ListItem* item = list->head;
    void*     data = item->data;

    list->head = item->next;
    list->count--;

    free(item);

    if (0 == list->count) {
        list->tail = NULL;
    }

    return data;
}

void* list_pop_head_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list_pop_head(list);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* list_pop_tail (List* list) {
    assert(NULL != list);
    assert(NULL != list->tail);
    assert(0 < list->count);

    ListItem* item = list->tail;
    void*     data = item->data;

    if (1 == list->count) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        // we need the item previous to tail
        ListItem* prev_item = NULL;

        __LIST_FIND(prev_item, list, list->count - 2);

        list->tail      = prev_item;
        prev_item->next = NULL;
    }

    list->count--;

    free(item);

    return data;
}

void* list_pop_tail_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list_pop_tail(list);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool list_push_head (List* list, void* data) {
    assert(NULL != list);

    ListItem* item = (ListItem*) malloc(sizeof(ListItem));

    if (NULL == item) {
        return false;
    }

    item->data = data;
    item->next = list->head;
    list->head = item;
    list->count++;

    if (1 == list->count) {
        list->tail = item;
    }

    return true;
}

bool list_push_head_ts (List* list, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = list_push_head(list, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

bool list_push_tail (List* list, void* data) {
    assert(NULL != list);

    ListItem* item = (ListItem*) malloc(sizeof(ListItem));

    if (NULL == item) {
        return false;
    }

    item->data = data;
    item->next = NULL;
    list->count++;

    if (1 == list->count) {
        list->head = item;
        list->tail = item;
    } else {
        list->tail->next = item;
        list->tail       = item;
    }

    return true;
}

bool list_push_tail_ts (List* list, void* data) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    bool ret = list_push_tail(list, data);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* list_remove (List* list, uint32_t index) {
    assert(NULL != list);
    assert(0 <= index && index < list->count);

    if (0 == index) {
        return list_pop_head(list);
    } else if (index == list->count - 1) {
        return list_pop_tail(list);
    } else if (index >= list->count) {
        return false;
    }

    ListItem* item = NULL;

    // we need the item previous to the removal index
    __LIST_FIND(item, list, index - 1);

    ListItem* rem_item = item->next;
    void*     data     = rem_item->data;

    item->next = rem_item->next;
    list->count--;

    free(rem_item);

    return data;
}

void* list_remove_ts (List* list, uint32_t index) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list_remove(list, index);

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void* list_tail (List* list) {
    assert(NULL != list);
    assert(NULL != list->tail);

    return list->tail->data;
}

void* list_tail_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->tail);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    void* ret = list->tail->data;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

ListItem* list_tail_item (List* list) {
    assert(NULL != list);

    return list->tail;
}

ListItem* list_tail_item_ts (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_lock(list->mutex);

    ListItem* ret = list->tail;

    pthread_mutex_unlock(list->mutex);

    return ret;
}

void list_unlock (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_unlock(list->mutex);
}
