/**
 * hashmap.h
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMap
 *
 * A HashMap is an implementation of a Map with unique keys, where every key is
 * mapped to a value. The keys are not sorted. It is implemented as a hashtable
 * with robin hood hashing.
 */

#ifndef CMC_HASHMAP_H
#define CMC_HASHMAP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_hashmap = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", load:%lf, cmp:%p, hash:%p }";

#ifndef CMC_HASH_TABLE_SETUP
#define CMC_HASH_TABLE_SETUP

typedef enum cmc_entry_state_e
{
    CMC_ES_DELETED = -1,
    CMC_ES_EMPTY = 0,
    CMC_ES_FILLED = 1
} cmc_entry_state;

static const size_t cmc_hashtable_primes[] = {53, 97, 191, 383, 769, 1531,
                                              3067, 6143, 12289, 24571, 49157,
                                              98299, 196613, 393209, 786431,
                                              1572869, 3145721, 6291449,
                                              12582917, 25165813, 50331653,
                                              100663291, 201326611, 402653189,
                                              805306357, 1610612741,
                                              3221225473, 6442450939,
                                              12884901893, 25769803799,
                                              51539607551, 103079215111,
                                              206158430209, 412316860441,
                                              824633720831, 1649267441651,
                                              3298534883309, 6597069766657,
                                              13194139533299, 26388279066623,
                                              52776558133303, 105553116266489,
                                              211106232532969, 422212465066001,
                                              844424930131963,
                                              1688849860263953,
                                              3377699720527861,
                                              6755399441055731,
                                              13510798882111483,
                                              27021597764222939,
                                              54043195528445957,
                                              108086391056891903,
                                              216172782113783773,
                                              432345564227567621,
                                              864691128455135207,
                                              1729382256910270481,
                                              3458764513820540933,
                                              6917529027641081903,
                                              13835058055282163729llu};

#endif /* CMC_HASH_TABLE_SETUP */

#define CMC_GENERATE_HASHMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_HASHMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_HASHMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V)                                        \
                                                                                             \
    /* Hashmap Structure */                                                                  \
    typedef struct SNAME##_s                                                                 \
    {                                                                                        \
        /* Array of Entries */                                                               \
        struct SNAME##_entry_s *buffer;                                                      \
                                                                                             \
        /* Current array capacity */                                                         \
        size_t capacity;                                                                     \
                                                                                             \
        /* Current amount of keys */                                                         \
        size_t count;                                                                        \
                                                                                             \
        /* Load factor in range (0.0, 1.0) */                                                \
        double load;                                                                         \
                                                                                             \
        /* Key comparison function */                                                        \
        int (*cmp)(K, K);                                                                    \
                                                                                             \
        /* Key hash function */                                                              \
        size_t (*hash)(K);                                                                   \
                                                                                             \
        /* Function that returns an iterator to the start of the hashmap */                  \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                               \
                                                                                             \
        /* Function that returns an iterator to the end of the hashmap */                    \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                 \
                                                                                             \
    } SNAME, *SNAME##_ptr;                                                                   \
                                                                                             \
    /* Hashmap Entry */                                                                      \
    typedef struct SNAME##_entry_s                                                           \
    {                                                                                        \
        /* Entry Key */                                                                      \
        K key;                                                                               \
                                                                                             \
        /* Entry Value */                                                                    \
        V value;                                                                             \
                                                                                             \
        /* The distance of this node to its original position, used by */                    \
        /* robin-hood hashing */                                                             \
        size_t dist;                                                                         \
                                                                                             \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                                 \
        enum cmc_entry_state_e state;                                                        \
                                                                                             \
    } SNAME##_entry, *SNAME##_entry_ptr;                                                     \
                                                                                             \
    /* Hashmap Iterator */                                                                   \
    typedef struct SNAME##_iter_s                                                            \
    {                                                                                        \
        /* Target hashmap */                                                                 \
        struct SNAME##_s *target;                                                            \
                                                                                             \
        /* Cursor's position (index) */                                                      \
        size_t cursor;                                                                       \
                                                                                             \
        /* Keeps track of relative index to the iteration of elements */                     \
        size_t index;                                                                        \
                                                                                             \
        /* The index of the first element */                                                 \
        size_t first;                                                                        \
                                                                                             \
        /* The index of the last element */                                                  \
        size_t last;                                                                         \
                                                                                             \
        /* If the iterator has reached the start of the iteration */                         \
        bool start;                                                                          \
                                                                                             \
        /* If the iterator has reached the end of the iteration */                           \
        bool end;                                                                            \
                                                                                             \
    } SNAME##_iter, *SNAME##_iter_ptr;                                                       \
                                                                                             \
    /* Collection Functions */                                                               \
    /* Collection Allocation and Deallocation */                                             \
    SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K)); \
    void PFX##_clear(SNAME *_map_, void (*deallocator)(K, V));                               \
    void PFX##_free(SNAME *_map_, void (*deallocator)(K, V));                                \
    /* Collection Input and Output */                                                        \
    bool PFX##_insert(SNAME *_map_, K key, V value);                                         \
    bool PFX##_update(SNAME *_map_, K key, V new_value, V *old_value);                       \
    bool PFX##_remove(SNAME *_map_, K key, V *out_value);                                    \
    /* Conditional Input and Output */                                                       \
    bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);                      \
    bool PFX##_remove_if(SNAME *_map_, K key, V *out_value, bool condition);                 \
    /* Element Access */                                                                     \
    bool PFX##_max(SNAME *_map_, K *key, V *value);                                          \
    bool PFX##_min(SNAME *_map_, K *key, V *value);                                          \
    V PFX##_get(SNAME *_map_, K key);                                                        \
    V *PFX##_get_ref(SNAME *_map_, K key);                                                   \
    /* Collection State */                                                                   \
    bool PFX##_contains(SNAME *_map_, K key);                                                \
    bool PFX##_empty(SNAME *_map_);                                                          \
    size_t PFX##_count(SNAME *_map_);                                                        \
    size_t PFX##_capacity(SNAME *_map_);                                                     \
    /* Collection Utility */                                                                 \
    SNAME *PFX##_copy_of(SNAME *_map_, K (*key_copy_func)(K), V (*value_copy_func)(V));      \
    bool PFX##_equals(SNAME *_map1_, SNAME *_map2_, int (*value_comparator)(V, V));          \
    cmc_string PFX##_to_string(SNAME *_map_);                                                \
                                                                                             \
    /* Iterator Functions */                                                                 \
    /* Iterator Allocation and Deallocation */                                               \
    SNAME##_iter *PFX##_iter_new(SNAME *target);                                             \
    void PFX##_iter_free(SNAME##_iter *iter);                                                \
    /* Iterator Initialization */                                                            \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                                 \
    /* Iterator State */                                                                     \
    bool PFX##_iter_start(SNAME##_iter *iter);                                               \
    bool PFX##_iter_end(SNAME##_iter *iter);                                                 \
    /* Iterator Movement */                                                                  \
    void PFX##_iter_to_start(SNAME##_iter *iter);                                            \
    void PFX##_iter_to_end(SNAME##_iter *iter);                                              \
    bool PFX##_iter_next(SNAME##_iter *iter);                                                \
    bool PFX##_iter_prev(SNAME##_iter *iter);                                                \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps);                               \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps);                                \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index);                                 \
    /* Iterator Access */                                                                    \
    K PFX##_iter_key(SNAME##_iter *iter);                                                    \
    V PFX##_iter_value(SNAME##_iter *iter);                                                  \
    V *PFX##_iter_rvalue(SNAME##_iter *iter);                                                \
    size_t PFX##_iter_index(SNAME##_iter *iter);                                             \
                                                                                             \
    /* Default Key */                                                                        \
    static inline K PFX##_impl_default_key(void)                                             \
    {                                                                                        \
        K _empty_key_;                                                                       \
                                                                                             \
        memset(&_empty_key_, 0, sizeof(K));                                                  \
                                                                                             \
        return _empty_key_;                                                                  \
    }                                                                                        \
                                                                                             \
    /* Default Value */                                                                      \
    static inline V PFX##_impl_default_value(void)                                           \
    {                                                                                        \
        V _empty_value_;                                                                     \
                                                                                             \
        memset(&_empty_value_, 0, sizeof(V));                                                \
                                                                                             \
        return _empty_value_;                                                                \
    }                                                                                        \
                                                                                             \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)                                         \
                                                                                              \
    /* Implementation Detail Functions */                                                     \
    static bool PFX##_impl_grow(SNAME *_map_);                                                \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key);                          \
    static size_t PFX##_impl_calculate_size(size_t required);                                 \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_map_);                                    \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_map_);                                      \
                                                                                              \
    SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K))   \
    {                                                                                         \
        if (capacity == 0 || load <= 0 || load >= 1)                                          \
            return NULL;                                                                      \
                                                                                              \
        /* Prevent integer overflow */                                                        \
        if (capacity >= UINTMAX_MAX * load)                                                   \
            return NULL;                                                                      \
                                                                                              \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);                    \
                                                                                              \
        SNAME *_map_ = malloc(sizeof(SNAME));                                                 \
                                                                                              \
        if (!_map_)                                                                           \
            return NULL;                                                                      \
                                                                                              \
        _map_->buffer = malloc(sizeof(SNAME##_entry) * real_capacity);                        \
                                                                                              \
        if (!_map_->buffer)                                                                   \
        {                                                                                     \
            free(_map_);                                                                      \
            return NULL;                                                                      \
        }                                                                                     \
                                                                                              \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * real_capacity);                      \
                                                                                              \
        _map_->count = 0;                                                                     \
        _map_->capacity = real_capacity;                                                      \
        _map_->load = load;                                                                   \
        _map_->cmp = compare;                                                                 \
        _map_->hash = hash;                                                                   \
                                                                                              \
        _map_->it_start = PFX##_impl_it_start;                                                \
        _map_->it_end = PFX##_impl_it_end;                                                    \
                                                                                              \
        return _map_;                                                                         \
    }                                                                                         \
                                                                                              \
    void PFX##_clear(SNAME *_map_, void (*deallocator)(K, V))                                 \
    {                                                                                         \
        if (deallocator)                                                                      \
        {                                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                                      \
            {                                                                                 \
                SNAME##_entry *entry = &(_map_->buffer[i]);                                   \
                                                                                              \
                if (entry->state == CMC_ES_FILLED)                                            \
                {                                                                             \
                    deallocator(entry->key, entry->value);                                    \
                }                                                                             \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * _map_->capacity);                    \
                                                                                              \
        _map_->count = 0;                                                                     \
    }                                                                                         \
                                                                                              \
    void PFX##_free(SNAME *_map_, void (*deallocator)(K, V))                                  \
    {                                                                                         \
        if (deallocator)                                                                      \
        {                                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                                      \
            {                                                                                 \
                SNAME##_entry *entry = &(_map_->buffer[i]);                                   \
                                                                                              \
                if (entry->state == CMC_ES_FILLED)                                            \
                {                                                                             \
                    deallocator(entry->key, entry->value);                                    \
                }                                                                             \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        free(_map_->buffer);                                                                  \
        free(_map_);                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_insert(SNAME *_map_, K key, V value)                                           \
    {                                                                                         \
        if ((double)_map_->capacity * _map_->load <= (double)_map_->count)                    \
        {                                                                                     \
            if (!PFX##_impl_grow(_map_))                                                      \
                return false;                                                                 \
        }                                                                                     \
                                                                                              \
        size_t hash = _map_->hash(key);                                                       \
        size_t original_pos = hash % _map_->capacity;                                         \
        size_t pos = original_pos;                                                            \
                                                                                              \
        SNAME##_entry *target = &(_map_->buffer[pos]);                                        \
                                                                                              \
        if (PFX##_impl_get_entry(_map_, key) != NULL)                                         \
            return false;                                                                     \
                                                                                              \
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)                 \
        {                                                                                     \
            target->key = key;                                                                \
            target->value = value;                                                            \
            target->dist = 0;                                                                 \
            target->state = CMC_ES_FILLED;                                                    \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
            while (true)                                                                      \
            {                                                                                 \
                pos++;                                                                        \
                target = &(_map_->buffer[pos % _map_->capacity]);                             \
                                                                                              \
                if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)         \
                {                                                                             \
                    target->key = key;                                                        \
                    target->value = value;                                                    \
                    target->dist = pos - original_pos;                                        \
                    target->state = CMC_ES_FILLED;                                            \
                                                                                              \
                    break;                                                                    \
                }                                                                             \
                else if (target->dist < original_pos - pos)                                   \
                {                                                                             \
                    K tmp_k = target->key;                                                    \
                    V tmp_v = target->value;                                                  \
                    size_t tmp_dist = target->dist;                                           \
                                                                                              \
                    target->key = key;                                                        \
                    target->value = value;                                                    \
                    target->dist = pos - original_pos;                                        \
                                                                                              \
                    key = tmp_k;                                                              \
                    value = tmp_v;                                                            \
                    original_pos = pos - tmp_dist;                                            \
                }                                                                             \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        _map_->count++;                                                                       \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_update(SNAME *_map_, K key, V new_value, V *old_value)                         \
    {                                                                                         \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                              \
        if (!entry)                                                                           \
            return false;                                                                     \
                                                                                              \
        if (old_value)                                                                        \
            *old_value = entry->value;                                                        \
                                                                                              \
        entry->value = new_value;                                                             \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_remove(SNAME *_map_, K key, V *out_value)                                      \
    {                                                                                         \
        SNAME##_entry *result = PFX##_impl_get_entry(_map_, key);                             \
                                                                                              \
        if (result == NULL)                                                                   \
            return false;                                                                     \
                                                                                              \
        if (out_value)                                                                        \
            *out_value = result->value;                                                       \
                                                                                              \
        result->key = PFX##_impl_default_key();                                               \
        result->value = PFX##_impl_default_value();                                           \
        result->dist = 0;                                                                     \
        result->state = CMC_ES_DELETED;                                                       \
                                                                                              \
        _map_->count--;                                                                       \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition)                        \
    {                                                                                         \
        if (condition)                                                                        \
            return PFX##_insert(_map_, key, value);                                           \
                                                                                              \
        return false;                                                                         \
    }                                                                                         \
                                                                                              \
    bool PFX##_remove_if(SNAME *_map_, K key, V *out_value, bool condition)                   \
    {                                                                                         \
        if (condition)                                                                        \
            return PFX##_remove(_map_, key, out_value);                                       \
                                                                                              \
        return false;                                                                         \
    }                                                                                         \
                                                                                              \
    bool PFX##_max(SNAME *_map_, K *key, V *value)                                            \
    {                                                                                         \
        if (PFX##_empty(_map_))                                                               \
            return false;                                                                     \
                                                                                              \
        SNAME##_iter iter;                                                                    \
                                                                                              \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))   \
        {                                                                                     \
            K result_key = PFX##_iter_key(&iter);                                             \
            V result_value = PFX##_iter_value(&iter);                                         \
            size_t index = PFX##_iter_index(&iter);                                           \
                                                                                              \
            if (index == 0)                                                                   \
            {                                                                                 \
                *key = result_key;                                                            \
                *value = result_value;                                                        \
            }                                                                                 \
            else if (_map_->cmp(result_key, *key) > 0)                                        \
            {                                                                                 \
                *key = result_key;                                                            \
                *value = result_value;                                                        \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_min(SNAME *_map_, K *key, V *value)                                            \
    {                                                                                         \
        if (PFX##_empty(_map_))                                                               \
            return false;                                                                     \
                                                                                              \
        SNAME##_iter iter;                                                                    \
                                                                                              \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))   \
        {                                                                                     \
            K result_key = PFX##_iter_key(&iter);                                             \
            V result_value = PFX##_iter_value(&iter);                                         \
            size_t index = PFX##_iter_index(&iter);                                           \
                                                                                              \
            if (index == 0)                                                                   \
            {                                                                                 \
                *key = result_key;                                                            \
                *value = result_value;                                                        \
            }                                                                                 \
            else if (_map_->cmp(result_key, *key) < 0)                                        \
            {                                                                                 \
                *key = result_key;                                                            \
                *value = result_value;                                                        \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    V PFX##_get(SNAME *_map_, K key)                                                          \
    {                                                                                         \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                              \
        if (!entry)                                                                           \
            return PFX##_impl_default_value();                                                \
                                                                                              \
        return entry->value;                                                                  \
    }                                                                                         \
                                                                                              \
    V *PFX##_get_ref(SNAME *_map_, K key)                                                     \
    {                                                                                         \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                              \
        if (!entry)                                                                           \
            return NULL;                                                                      \
                                                                                              \
        return &(entry->value);                                                               \
    }                                                                                         \
                                                                                              \
    bool PFX##_contains(SNAME *_map_, K key)                                                  \
    {                                                                                         \
        return PFX##_impl_get_entry(_map_, key) != NULL;                                      \
    }                                                                                         \
                                                                                              \
    bool PFX##_empty(SNAME *_map_)                                                            \
    {                                                                                         \
        return _map_->count == 0;                                                             \
    }                                                                                         \
                                                                                              \
    size_t PFX##_count(SNAME *_map_)                                                          \
    {                                                                                         \
        return _map_->count;                                                                  \
    }                                                                                         \
                                                                                              \
    size_t PFX##_capacity(SNAME *_map_)                                                       \
    {                                                                                         \
        return _map_->capacity;                                                               \
    }                                                                                         \
                                                                                              \
    SNAME *PFX##_copy_of(SNAME *_map_, K (*key_copy_func)(K), V (*value_copy_func)(V))        \
    {                                                                                         \
        SNAME *result = PFX##_new(_map_->capacity, _map_->load, _map_->cmp, _map_->hash);     \
                                                                                              \
        if (!result)                                                                          \
            return NULL;                                                                      \
                                                                                              \
        if (key_copy_func || value_copy_func)                                                 \
        {                                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                                      \
            {                                                                                 \
                SNAME##_entry *scan = &(_map_->buffer[i]);                                    \
                                                                                              \
                if (scan->state != CMC_ES_EMPTY)                                              \
                {                                                                             \
                    SNAME##_entry *target = &(result->buffer[i]);                             \
                                                                                              \
                    if (scan->state == CMC_ES_DELETED)                                        \
                        target->state = CMC_ES_DELETED;                                       \
                    else                                                                      \
                    {                                                                         \
                        target->state = scan->state;                                          \
                        target->dist = scan->dist;                                            \
                                                                                              \
                        if (key_copy_func)                                                    \
                            target->key = key_copy_func(scan->key);                           \
                        else                                                                  \
                            target->key = scan->key;                                          \
                                                                                              \
                        if (value_copy_func)                                                  \
                            target->value = value_copy_func(scan->value);                     \
                        else                                                                  \
                            target->value = scan->value;                                      \
                    }                                                                         \
                }                                                                             \
            }                                                                                 \
        }                                                                                     \
        else                                                                                  \
            memcpy(result->buffer, _map_->buffer, sizeof(SNAME##_entry) * _map_->capacity);   \
                                                                                              \
        result->count = _map_->count;                                                         \
                                                                                              \
        return result;                                                                        \
    }                                                                                         \
                                                                                              \
    bool PFX##_equals(SNAME *_map1_, SNAME *_map2_, int (*value_comparator)(V, V))            \
    {                                                                                         \
        if (PFX##_count(_map1_) != PFX##_count(_map2_))                                       \
            return false;                                                                     \
                                                                                              \
        SNAME##_iter iter;                                                                    \
        PFX##_iter_init(&iter, _map1_);                                                       \
                                                                                              \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))      \
        {                                                                                     \
            SNAME##_entry *entry = PFX##_impl_get_entry(_map2_, PFX##_iter_key(&iter));       \
                                                                                              \
            if (entry == NULL)                                                                \
                return false;                                                                 \
                                                                                              \
            if (value_comparator)                                                             \
            {                                                                                 \
                if (value_comparator(entry->value, PFX##_iter_value(&iter)) != 0)             \
                    return false;                                                             \
            }                                                                                 \
        }                                                                                     \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    cmc_string PFX##_to_string(SNAME *_map_)                                                  \
    {                                                                                         \
        cmc_string str;                                                                       \
        SNAME *m_ = _map_;                                                                    \
        const char *name = #SNAME;                                                            \
                                                                                              \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_hashmap,                               \
                 name, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->cmp, m_->hash); \
                                                                                              \
        return str;                                                                           \
    }                                                                                         \
                                                                                              \
    SNAME##_iter *PFX##_iter_new(SNAME *target)                                               \
    {                                                                                         \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                    \
                                                                                              \
        if (!iter)                                                                            \
            return NULL;                                                                      \
                                                                                              \
        PFX##_iter_init(iter, target);                                                        \
                                                                                              \
        return iter;                                                                          \
    }                                                                                         \
                                                                                              \
    void PFX##_iter_free(SNAME##_iter *iter)                                                  \
    {                                                                                         \
        free(iter);                                                                           \
    }                                                                                         \
                                                                                              \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                   \
    {                                                                                         \
        memset(iter, 0, sizeof(SNAME##_iter));                                                \
                                                                                              \
        iter->target = target;                                                                \
        iter->start = true;                                                                   \
        iter->end = PFX##_empty(target);                                                      \
                                                                                              \
        if (!PFX##_empty(target))                                                             \
        {                                                                                     \
            for (size_t i = 0; i < target->capacity; i++)                                     \
            {                                                                                 \
                if (target->buffer[i].state == CMC_ES_FILLED)                                 \
                {                                                                             \
                    iter->first = i;                                                          \
                    break;                                                                    \
                }                                                                             \
            }                                                                                 \
                                                                                              \
            iter->cursor = iter->first;                                                       \
                                                                                              \
            for (size_t i = target->capacity; i > 0; i--)                                     \
            {                                                                                 \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)                             \
                {                                                                             \
                    iter->last = i - 1;                                                       \
                    break;                                                                    \
                }                                                                             \
            }                                                                                 \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    bool PFX##_iter_start(SNAME##_iter *iter)                                                 \
    {                                                                                         \
        return PFX##_empty(iter->target) || iter->start;                                      \
    }                                                                                         \
                                                                                              \
    bool PFX##_iter_end(SNAME##_iter *iter)                                                   \
    {                                                                                         \
        return PFX##_empty(iter->target) || iter->end;                                        \
    }                                                                                         \
                                                                                              \
    void PFX##_iter_to_start(SNAME##_iter *iter)                                              \
    {                                                                                         \
        if (!PFX##_empty(iter->target))                                                       \
        {                                                                                     \
            iter->cursor = iter->first;                                                       \
            iter->index = 0;                                                                  \
            iter->start = true;                                                               \
            iter->end = PFX##_empty(iter->target);                                            \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    void PFX##_iter_to_end(SNAME##_iter *iter)                                                \
    {                                                                                         \
        if (!PFX##_empty(iter->target))                                                       \
        {                                                                                     \
            iter->cursor = iter->last;                                                        \
            iter->index = PFX##_count(iter->target) - 1;                                      \
            iter->start = PFX##_empty(iter->target);                                          \
            iter->end = true;                                                                 \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    bool PFX##_iter_next(SNAME##_iter *iter)                                                  \
    {                                                                                         \
        if (iter->end)                                                                        \
            return false;                                                                     \
                                                                                              \
        if (iter->index + 1 == PFX##_count(iter->target))                                     \
        {                                                                                     \
            iter->end = true;                                                                 \
            return false;                                                                     \
        }                                                                                     \
                                                                                              \
        iter->start = PFX##_empty(iter->target);                                              \
                                                                                              \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                          \
                                                                                              \
        iter->index++;                                                                        \
                                                                                              \
        while (1)                                                                             \
        {                                                                                     \
            iter->cursor++;                                                                   \
            scan = &(iter->target->buffer[iter->cursor]);                                     \
                                                                                              \
            if (scan->state == CMC_ES_FILLED)                                                 \
                break;                                                                        \
        }                                                                                     \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    bool PFX##_iter_prev(SNAME##_iter *iter)                                                  \
    {                                                                                         \
        if (iter->start)                                                                      \
            return false;                                                                     \
                                                                                              \
        if (iter->index == 0)                                                                 \
        {                                                                                     \
            iter->start = true;                                                               \
            return false;                                                                     \
        }                                                                                     \
                                                                                              \
        iter->end = PFX##_empty(iter->target);                                                \
                                                                                              \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                          \
                                                                                              \
        iter->index--;                                                                        \
                                                                                              \
        while (1)                                                                             \
        {                                                                                     \
            iter->cursor--;                                                                   \
            scan = &(iter->target->buffer[iter->cursor]);                                     \
                                                                                              \
            if (scan->state == CMC_ES_FILLED)                                                 \
                break;                                                                        \
        }                                                                                     \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    /* Returns true only if the iterator moved */                                             \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps)                                 \
    {                                                                                         \
        if (iter->end)                                                                        \
            return false;                                                                     \
                                                                                              \
        if (iter->index + 1 == PFX##_count(iter->target))                                     \
        {                                                                                     \
            iter->end = true;                                                                 \
            return false;                                                                     \
        }                                                                                     \
                                                                                              \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                   \
            return false;                                                                     \
                                                                                              \
        for (size_t i = 0; i < steps; i++)                                                    \
            PFX##_iter_next(iter);                                                            \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    /* Returns true only if the iterator moved */                                             \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps)                                  \
    {                                                                                         \
        if (iter->start)                                                                      \
            return false;                                                                     \
                                                                                              \
        if (iter->index == 0)                                                                 \
        {                                                                                     \
            iter->start = true;                                                               \
            return false;                                                                     \
        }                                                                                     \
                                                                                              \
        if (steps == 0 || iter->index < steps)                                                \
            return false;                                                                     \
                                                                                              \
        for (size_t i = 0; i < steps; i++)                                                    \
            PFX##_iter_prev(iter);                                                            \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    /* Returns true only if the iterator was able to be positioned at the given index */      \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index)                                   \
    {                                                                                         \
        if (index >= PFX##_count(iter->target))                                               \
            return false;                                                                     \
                                                                                              \
        if (iter->index > index)                                                              \
            return PFX##_iter_rewind(iter, iter->index - index);                              \
        else if (iter->index < index)                                                         \
            return PFX##_iter_advance(iter, index - iter->index);                             \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    K PFX##_iter_key(SNAME##_iter *iter)                                                      \
    {                                                                                         \
        if (PFX##_empty(iter->target))                                                        \
            return PFX##_impl_default_key();                                                  \
                                                                                              \
        return iter->target->buffer[iter->cursor].key;                                        \
    }                                                                                         \
                                                                                              \
    V PFX##_iter_value(SNAME##_iter *iter)                                                    \
    {                                                                                         \
        if (PFX##_empty(iter->target))                                                        \
            return PFX##_impl_default_value();                                                \
                                                                                              \
        return iter->target->buffer[iter->cursor].value;                                      \
    }                                                                                         \
                                                                                              \
    V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                  \
    {                                                                                         \
        if (PFX##_empty(iter->target))                                                        \
            return NULL;                                                                      \
                                                                                              \
        return &(iter->target->buffer[iter->cursor].value);                                   \
    }                                                                                         \
                                                                                              \
    size_t PFX##_iter_index(SNAME##_iter *iter)                                               \
    {                                                                                         \
        return iter->index;                                                                   \
    }                                                                                         \
                                                                                              \
    static bool PFX##_impl_grow(SNAME *_map_)                                                 \
    {                                                                                         \
        size_t new_size = _map_->capacity + _map_->capacity / 2;                              \
                                                                                              \
        SNAME *_new_map_ = PFX##_new(new_size, _map_->load, _map_->cmp, _map_->hash);         \
                                                                                              \
        if (!_new_map_)                                                                       \
            return false;                                                                     \
                                                                                              \
        SNAME##_iter iter;                                                                    \
                                                                                              \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))   \
        {                                                                                     \
            K key = PFX##_iter_key(&iter);                                                    \
            V value = PFX##_iter_value(&iter);                                                \
                                                                                              \
            PFX##_insert(_new_map_, key, value);                                              \
        }                                                                                     \
                                                                                              \
        if (_map_->count != _new_map_->count)                                                 \
        {                                                                                     \
            PFX##_free(_new_map_, NULL);                                                      \
                                                                                              \
            return false;                                                                     \
        }                                                                                     \
                                                                                              \
        SNAME##_entry *tmp_b = _map_->buffer;                                                 \
        _map_->buffer = _new_map_->buffer;                                                    \
        _new_map_->buffer = tmp_b;                                                            \
                                                                                              \
        size_t tmp_c = _map_->capacity;                                                       \
        _map_->capacity = _new_map_->capacity;                                                \
        _new_map_->capacity = tmp_c;                                                          \
                                                                                              \
        PFX##_free(_new_map_, NULL);                                                          \
                                                                                              \
        return true;                                                                          \
    }                                                                                         \
                                                                                              \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key)                           \
    {                                                                                         \
        size_t hash = _map_->hash(key);                                                       \
        size_t pos = hash % _map_->capacity;                                                  \
                                                                                              \
        SNAME##_entry *target = &(_map_->buffer[pos]);                                        \
                                                                                              \
        while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)             \
        {                                                                                     \
            if (_map_->cmp(target->key, key) == 0)                                            \
                return target;                                                                \
                                                                                              \
            pos++;                                                                            \
            target = &(_map_->buffer[pos % _map_->capacity]);                                 \
        }                                                                                     \
                                                                                              \
        return NULL;                                                                          \
    }                                                                                         \
                                                                                              \
    static size_t PFX##_impl_calculate_size(size_t required)                                  \
    {                                                                                         \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);  \
                                                                                              \
        if (cmc_hashtable_primes[count - 1] < required)                                       \
            return required;                                                                  \
                                                                                              \
        size_t i = 0;                                                                         \
        while (cmc_hashtable_primes[i] < required)                                            \
            i++;                                                                              \
                                                                                              \
        return cmc_hashtable_primes[i];                                                       \
    }                                                                                         \
                                                                                              \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_map_)                                     \
    {                                                                                         \
        SNAME##_iter iter;                                                                    \
                                                                                              \
        PFX##_iter_init(&iter, _map_);                                                        \
                                                                                              \
        return iter;                                                                          \
    }                                                                                         \
                                                                                              \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_map_)                                       \
    {                                                                                         \
        SNAME##_iter iter;                                                                    \
                                                                                              \
        PFX##_iter_init(&iter, _map_);                                                        \
        PFX##_iter_to_end(&iter);                                                             \
                                                                                              \
        return iter;                                                                          \
    }

#endif /* CMC_HASHMAP_H */
