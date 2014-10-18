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

#include "codebox/container/table.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __TABLE_FIND(__bucket, __table, __key, __length) \
    { \
        uint32_t __hash = __table->hash_func(__key, __length); \
        __bucket = *(__table->buckets + (__hash % __table->bucket_count)); \
        for (; NULL != __bucket; __bucket = __bucket->next) { \
            if (__bucket->hashcode == __hash && \
                __table->comp_func(__bucket->key, __bucket->length, __key, __length)) { \
                break; \
            } \
        } \
    }

// -------------------------------------------------------------------------------------------------
// STATIC VARIABLES
// -------------------------------------------------------------------------------------------------

static uint32_t primes[26] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
                               196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917,
                               25165843, 50331653, 100663319, 201326611, 402653189, 805306457,
                               1610612741 };

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

bool compare_binary (unsigned char* key1, uint32_t length1,
                       unsigned char* key2, uint32_t length2) {
    for (int i = 0; i < length1 && i < length2; i++, key1++, key2++) {
        if (*key1 != *key2) {
            return false;
        }
    }

    return true;
}

uint32_t hash_djb2 (unsigned char* bytes, uint32_t length) {
    uint32_t hash = 5381;

    for (int i = 0; i < length; i++, bytes++) {
        hash = ((hash << 5) + hash) + (*bytes);
    }

    return hash;
}

bool table_cleanup (Table* table) {
    assert(NULL != table);
    assert(NULL != table->buckets);

    free(table->buckets);

    return true;
}

void* table_get (Table* table, unsigned char* key, uint32_t length) {
    assert(NULL != table);
    assert(NULL != key);
    assert(0 < length);

    Bucket* bucket = NULL;

    __TABLE_FIND(bucket, table, key, length);

    return NULL != bucket ? bucket->value : NULL;
}

bool table_has_key (Table* table, unsigned char* key, uint32_t length) {
    assert(NULL != table);
    assert(NULL != key);
    assert(0 < length);

    Bucket* bucket = NULL;

    __TABLE_FIND(bucket, table, key, length);

    return NULL != bucket;
}

bool table_init (Table* table, uint32_t bucket_count, float load_factor,
                 bool (*comp_func) (unsigned char* key1, uint32_t length1,
                                    unsigned char* key2, uint32_t length2),
                 uint32_t (*hash_func) (unsigned char* key, uint32_t length)) {
    assert(NULL != table);
    assert(NULL == table->buckets);
    assert(NULL != comp_func);
    assert(NULL != hash_func);

    for (int8_t i = 0, length = sizeof(primes) / sizeof(primes[0]); i < length; i++) {
        if (bucket_count <= primes[i] || i + 1 == length) {
            table->bucket_count = primes[i];
            break;
        }
    }

    table->buckets = (Bucket**) malloc(table->bucket_count * sizeof(Bucket*));

    if (NULL == table->buckets) {
        return false;
    }

    memset(table->buckets, 0, table->bucket_count * sizeof(Bucket*));

    table->comp_func    = comp_func;
    table->hash_func    = hash_func;
    table->key_count    = 0;
    table->load_factor  = load_factor;
    table->resize_count = (uint32_t) (table->bucket_count * table->load_factor);

    return true;
}

bool table_init_defaults (Table* table) {
    return table_init(table,
                      __TABLE_DEFAULT_BUCKET_COUNT,
                      __TABLE_DEFAULT_LOAD_FACTOR,
                      __TABLE_DEFAULT_COMP_FUNC,
                      __TABLE_DEFAULT_HASH_FUNC);
}

Table* table_new () {
    Table* table = (Table*) malloc(sizeof(Table));

    if (NULL == table) {
        return NULL;
    }

    memset(table, 0, sizeof(Table));

    return table;
}

bool table_put (Table* table, unsigned char* key, uint32_t length, void* value) {
    assert(NULL != table);
    assert(NULL != key);
    assert(0 < length);

    if (table->key_count == table->resize_count) {
        table_resize(table, table->bucket_count + 1);
    }

    uint32_t hash   = table->hash_func(key, length);
    Bucket** bucket = table->buckets + (hash % table->bucket_count);

    for (; NULL != *bucket; bucket = &((*bucket)->next));

    *bucket = (Bucket*) malloc(sizeof(Bucket));

    if (NULL == *bucket) {
        return false;
    }

    (*bucket)->key      = key;
    (*bucket)->length   = length;
    (*bucket)->hashcode = hash;
    (*bucket)->next     = NULL;
    (*bucket)->value    = (void*) value;

    table->key_count++;

    return true;
}

void* table_remove (Table* table, unsigned char* key, uint32_t length) {
    uint32_t hash   = table->hash_func(key, length);
    Bucket** bucket = table->buckets + (hash % table->bucket_count);

    for (; NULL != *bucket; bucket = &((*bucket)->next)) {
        if ((*bucket)->hashcode == hash && table->comp_func((*bucket)->key, (*bucket)->length, key, length)) {
            Bucket* removed = *bucket;
            void*   value   = removed->value;

            *bucket = removed->next;
            table->key_count--;

            free(removed);

            return value;
        }
    }

    return NULL;
}

bool table_resize (Table* table, uint32_t bucket_count) {
    for (int8_t i = 0, length = sizeof(primes) / sizeof(primes[0]); i < length; i++) {
        if (bucket_count <= primes[i] || i + 1 == length) {
            bucket_count = primes[i];
            break;
        }
    }

    if (bucket_count == primes[(sizeof(primes) / sizeof(primes[0])) - 1]) {
        return false;
    }

    Bucket** buckets = (Bucket**) malloc(bucket_count * sizeof(Bucket*));

    if (NULL == buckets) {
        return false;
    }

    memset(buckets, 0, bucket_count * sizeof(Bucket*));

    Bucket*  old_bucket  = *(table->buckets);
    Bucket** new_bucket  = NULL;

    for (uint32_t i = 0; i < table->bucket_count; old_bucket = *(table->buckets + (++i))) {
        while (NULL != old_bucket) {
            new_bucket = buckets + (old_bucket->hashcode % bucket_count);

            for (; NULL != *new_bucket; new_bucket = &((*new_bucket)->next));

            *new_bucket         = old_bucket;
            old_bucket          = old_bucket->next;
            (*new_bucket)->next = NULL;
        }
    }

    free(table->buckets);

    table->buckets      = buckets;
    table->bucket_count = bucket_count;
    table->resize_count = (uint32_t) (table->bucket_count * table->load_factor);

    return true;
}
