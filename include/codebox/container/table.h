/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 *
 * @author Sean Kerr: sean@code-box.org
 */

#ifndef __CODEBOX_TABLE_H
#define __CODEBOX_TABLE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------------------------
// TYPEDEFS
// -------------------------------------------------------------------------------------------------

typedef struct __bucket {
    /** The key. */
    unsigned char* key;

    /** The next bucket. */
    struct __bucket* next;

    /** The value. */
    void* value;

    /** The hashcode. */
    uint32_t hashcode;

    /** The key length. */
    uint32_t length;
} Bucket;

typedef struct {
    /** The key comparision function. */
    bool (*comp_func) (unsigned char* key1, uint32_t length1,
                       unsigned char* key2, uint32_t length2);

    /** The hash function. */
    uint32_t (*hash_func) (unsigned char* key, uint32_t length);

    /** The buckets. */
    Bucket** buckets;

    /** The mutex. */
    pthread_mutex_t* mutex;

    /** The bucket count. */
    uint32_t bucket_count;

    /** The key count. */
    uint32_t key_count;

    /** The resize load factor. */
    float load_factor;

    /** The resize count. */
    uint32_t resize_count;
} Table;

typedef struct {
    /** The current bucket. */
    Bucket* bucket;

    /** The table. */
    Table* table;

    /** The current bucket index. */
    uint32_t bucket_index;
} TableIterator;

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#define __TABLE_DEFAULT_BUCKET_COUNT 53
#define __TABLE_DEFAULT_COMP_FUNC    compare_binary
#define __TABLE_DEFAULT_HASH_FUNC    hash_djb2
#define __TABLE_DEFAULT_LOAD_FACTOR  0.75

// -------------------------------------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------------------------------------

/**
 * Compare two pointers.
 *
 * @param key1    The first key.
 * @param length1 The first key length.
 * @param key2    The second key.
 * @param length2 The second key length.
 */
bool compare_binary (unsigned char* key1, uint32_t length1,
                     unsigned char* key2, uint32_t length2);

/**
 * The djb2 hash function.
 *
 * @param bytes  The bytes.
 * @param length The length.
 */
uint32_t hash_djb2 (unsigned char* bytes, uint32_t length);

/**
 * Cleanup a hash table.
 *
 * @param table The hash table.
 */
bool table_cleanup (Table* table);

/**
 * Retrieve a value from a hash table.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 */
void* table_get (Table* table, unsigned char* key, uint32_t length);

/**
 * Retrieve a value from a hash table using thread safety.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 */
void* table_get_ts (Table* table, unsigned char* key, uint32_t length);

/**
 * Indicates whether or not a table contains a key.
 *
 * @param table  The table.
 * @param key    The key.
 * @param length The key length.
 */
bool table_has_key (Table* table, unsigned char* key, uint32_t length);

/**
 * Indicates whether or not a table contains a key using thread safety.
 *
 * @param table  The table.
 * @param key    The key.
 * @param length The key length.
 */
bool table_has_key_ts (Table* table, unsigned char* key, uint32_t length);

/**
 * Initialize a hash table.
 *
 * @param table        The hash table.
 * @param bucket_count The initial bucket count.
 * @param load_factor  The resize load factor.
 * @param comp_func    The comparison function.
 * @param hash_func    The hash function.
 * @param thread_safe  Indicates that a mutex will be initialized.
 */
bool table_init (Table* table, uint32_t bucket_count, float load_factor,
                 bool (*comp_func) (unsigned char* key1, uint32_t length1,
                                    unsigned char* key2, uint32_t length2),
                 uint32_t (*hash_func) (unsigned char* key, uint32_t length),
                 bool thread_safe);

/**
 * Initialize a hash table with default settings.
 *
 * Defaults:
 *   * bucket_count = 53
 *   * load_factor  = 0.75
 *   * comp_func    = binary
 *   * hash_func    = djb2
 *   * thread_safe  = false
 *
 * @param table The hash table.
 */
bool table_init_defaults (Table* table);

/**
 * Initialize a thread-safe hash table with default settings.
 *
 * Defaults:
 *   * bucket_count = 53
 *   * load_factor  = 0.75
 *   * comp_func    = binary
 *   * hash_func    = djb2
 *   * thread_safe  = true
 *
 * @param table The hash table.
 */
bool table_init_defaults_ts (Table* table);

/**
 * Initialize a table iterator.
 *
 * Note: The table must be locked prior to iterating in multithreaded environments.
 *
 * @param iter  The table iterator.
 * @param table The table.
 */
void table_iter_init (TableIterator* iter, Table* table);

/**
 * Retrieve the key for the current table iteration.
 *
 * Note: The table must be locked prior to iterating in multithreaded environments.
 *
 * @param iter The table iterator.
 */
void* table_iter_key (TableIterator* iter);

/**
 * Skip to the next key/value pair.
 *
 * Note: The table must be locked prior to iterating in multithreaded environments.
 *
 * @param iter The table iterator.
 */
bool table_iter_next (TableIterator* iter);

/**
 * Retrieve the value for the current table iteration.
 *
 * Note: The table must be locked prior to iterating in multithreaded environments.
 *
 * @param iter The table iterator.
 */
void* table_iter_value (TableIterator* iter);

/**
 * Retrieve the count of keys in a table.
 *
 * @param table The table.
 */
uint32_t table_key_count (Table* table);

/**
 * Retrieve the count of keys in a table using thread safety.
 *
 * @param table The table.
 */
uint32_t table_key_count_ts (Table* table);

/**
 * Lock a table if it was initialized as thread-safe.
 *
 * @param table The table.
 */
void table_lock (Table* table);

/**
 * Create a new hash table.
 */
Table* table_new ();

/**
 * Put an item into a hash table.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 * @param value  The value.
 */
bool table_put (Table* table, unsigned char* key, uint32_t length, void* value);

/**
 * Put an item into a hash table using thread safety.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 * @param value  The value.
 */
bool table_put_ts (Table* table, unsigned char* key, uint32_t length, void* value);

/**
 * Remove an item from a hash table.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 */
void* table_remove (Table* table, unsigned char* key, uint32_t length);

/**
 * Remove an item from a hash table using thread safety.
 *
 * @param table  The hash table.
 * @param key    The key.
 * @param length The key length.
 */
void* table_remove_ts (Table* table, unsigned char* key, uint32_t length);

/**
 * Resize a hash table.
 *
 * @param table        The table.
 * @param bucket_count The estimated bucket count.
 */
bool table_resize (Table* table, uint32_t bucket_count);

/**
 * Resize a hash table using thread safety.
 *
 * @param table        The table.
 * @param bucket_count The estimated bucket count.
 */
bool table_resize_ts (Table* table, uint32_t bucket_count);

/**
 * Unlock a table if it was initialized as thread-safe.
 *
 * @param table The table.
 */
void table_unlock (Table* table);

#ifdef __cplusplus
}
#endif

#endif
