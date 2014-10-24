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

#define __LIST_FIND(__var, __list, __index) \
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

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool list_cleanup (List* list) {
    assert(NULL != list);

    if (NULL != list->mutex) {
        pthread_mutex_destroy(list->mutex);
    }

    return true;
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

    __LIST_FIND(item, list, index);

    new_item->data       = data;
    new_item->next       = item;
    new_item->prev       = item->prev;
    new_item->next->prev = new_item;
    new_item->prev->next = new_item;
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

    if (NULL != list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(item);

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

    __LIST_FIND(item, list, index);

    if (NULL == item) {
        return false;
    }

    void*     data     = item->data;
    ListItem* rem_item = item;

    rem_item->next->prev = rem_item->prev;
    rem_item->prev->next = rem_item->next;
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

void list_unlock (List* list) {
    assert(NULL != list);
    assert(NULL != list->mutex);

    pthread_mutex_unlock(list->mutex);
}
