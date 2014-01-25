/*! \file CAGL hash table implementation.

    \copyright Copyright 2014 Nathan Geffen. All rights reserved.

    \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

    \sa COPYING for the license text.

    \sa howtodev.rst to learn how this code works and how to modify it.
*/


#ifndef CAG_HASH_H
#define CAG_HASH_H

#include <stdlib.h>
#include "cagl/concepts.h"

/*! \brief A table of the sizes the hash table is resized to. Used in the rehash
    code.

    This could do with optimising. Also, rehashing has not been tested with the
    very large entries in this table.

    The last entry in the table should be set to zero because the current
    implementation will not rehash once it gets to the last entry. So setting it
    to anything other than zero would be deceptive.
 */

#define CAG_P_HTABLE_SIZES 16

static const size_t cag_p_htable_sizes[] = {
    23, 47, 101,
    509, 1009, 10993,
    109987, 412849, 970704,
    1299989, 3562115, 5999993,
    12999997, 99999989, 3999999979,
    0
};

/*! \brief Two string hash algorithms are currently provided: Kernighan &
   Ritchie and Oat. See cag_common.c for implementations and references.

   The two macros below can be used with structs that are passed by value and
   whose first element is a string (char *) used as the key into the hash table.
*/

#define CAG_KR_HASH_STRUCT_WITH_STR_KEY(x, y)                                 \
    cag_kr_hash( CAG_STR_KEY_FROM_STRUCT(x), y)

#define CAG_OAT_HASH_STRUCT_WITH_STR_KEY(x, y)                                \
    cag_oat_hash( CAG_STR_KEY_FROM_STRUCT(x), y)

#define CAG_STRING_HASH(x, y) CAG_OAT_HASH_STRUCT_WITH_STR_KEY(x, y)


/*! \brief Macro for hash table of integers.  See cag_common.c for
    implementations of this as a function. Ridiculously simple
*/

#define CAG_INT_HASH(i, len) i

/*! \brief New hash table function declaration and definitions.

   The with_buckets version allows the user to specify the number of buckets
   when the hash table is initialized.
*/

#define CAG_DEC_NEW_HASH_WITH_BUCKETS(function, container)                    \
    container *function(container *hash, const size_t buckets)

#define CAG_DEF_NEW_HASH_WITH_BUCKETS(function, container)                    \
    CAG_DEC_NEW_HASH_WITH_BUCKETS(function, container)                        \
    {                                                                         \
        hash->buckets = buckets;                                              \
        hash->size = 0;                                                       \
        hash->rehash = 1;                                                     \
        hash->objects = calloc(hash->buckets + 1, sizeof(hash->objects));     \
        if (!hash->objects)                                                   \
            return NULL;                                                      \
        else {                                                                \
            hash->objects[hash->buckets] =                                    \
                    calloc(1, sizeof(*hash->objects[hash->buckets]));         \
            return hash;                                                      \
        }                                                                     \
    }

#define CAG_DEC_NEW_HASH(function, container)                                 \
    container *function(container *hash)

#define CAG_DEF_NEW_HASH(function, container)                                 \
    CAG_DEC_NEW_HASH(function, container)                                     \
    {                                                                         \
        return new_with_buckets_ ## container(hash, cag_p_htable_sizes[0]);   \
    }

/*! \brief Algorithm and function declaration and definition to get (lookup) a
   hash entry based on the key. Pass by value and address versions implemented.
*/

#define CAG_P_GET_HASH(iterator_type, hash, key, cmp_func, val_adr,           \
                       hash_func, length_func)                                \
do {                                                                          \
    iterator_type it;                                                         \
    size_t i = hash_func(key,  length_func(key)) % hash->buckets;             \
    for (it = hash->objects[i]; it != NULL; it = it->next)                    \
        if (cmp_func(val_adr (key), val_adr it->value) == 0)                  \
            return it;                                                        \
    return NULL;                                                              \
} while(0)

#define CAG_DEC_GET_HASH(function, container, iterator_type, type)            \
    iterator_type function(const container *hash, const type element)

#define CAG_DEF_GET_HASH(function, container, iterator_type,                  \
                         type, cmp_func, val_adr, hash_func, length_func)     \
CAG_DEC_GET_HASH(function, container, iterator_type, type)                    \
{                                                                             \
    CAG_P_GET_HASH(iterator_type, hash, element, cmp_func, val_adr,           \
                   hash_func, length_func);                                   \
}

#define CAG_DEC_GETP_HASH(function, container, iterator_type, type)           \
    iterator_type function(const container *hash, const type *element)

#define CAG_DEF_GETP_HASH(function, container, iterator_type,                 \
                          type, cmp_func, val_adr, hash_func, length_func)    \
CAG_DEC_GETP_HASH(function, container, iterator_type, type)                   \
{                                                                             \
    CAG_P_GET_HASH(iterator_type, hash, *element, cmp_func, val_adr,          \
                   hash_func, length_func);                                   \
}


/*! \brief Algorithm and function declaration and definition to insert into a
   hash table. Pass by value and address versions implemented.
*/


#define CAG_P_INSERT_HASH(container, iterator_type,                           \
                          type, cmp_func, hash_func,                          \
                          length_func, get,                                   \
                          alloc_style, alloc_func, free_func, val_adr, val)   \
do {                                                                          \
    iterator_type it;                                                         \
    type replace_val;                                                         \
    size_t hashval;                                                           \
    if ((it = get(hash, val)) == NULL) {                                      \
        if ( (float) hash->size / (float) hash->buckets > 0.75) {             \
            rehash_ ## container(hash, 0);                                    \
        }                                                                     \
        it = CAG_MALLOC(sizeof(*it));                                         \
        if (it == NULL)                                                       \
            return NULL;                                                      \
        hashval = hash_func(val, length_func(val)) % hash->buckets;           \
        it->next = hash->objects[hashval];                                    \
        it->bucket = &hash->objects[hashval];                                 \
        alloc_style(it->value, val, alloc_func,                               \
                    {                                                         \
                            CAG_FREE(it);                                     \
                            return NULL;                                      \
                    });                                                       \
        hash->objects[hashval] = it;                                          \
        ++hash->size;                                                         \
    } else {                                                                  \
        alloc_style(replace_val, val, alloc_func,                             \
                    {return NULL;});                                          \
        free_func(val_adr it->value);                                         \
        it->value = replace_val;                                              \
    }                                                                         \
    return it;                                                                \
} while (0)


#define CAG_DEC_INSERT_HASH(function, container, iterator_type,               \
                            type)                                             \
iterator_type function(container *hash, type const element)

#define CAG_DEF_INSERT_HASH(function, container, iterator_type,               \
                            type, cmp_func, hash_func,                        \
                            length_func, get,                                 \
                            alloc_style, alloc_func, free_func, val_adr)      \
CAG_DEC_INSERT_HASH(function, container, iterator_type, type)                 \
{                                                                             \
    CAG_P_INSERT_HASH(container, iterator_type,                               \
                      type, cmp_func, hash_func,                              \
                      length_func, get,                                       \
                      alloc_style, alloc_func, free_func, val_adr, element);  \
}

#define CAG_DEC_INSERTP_HASH(function, container, iterator_type,              \
                             type)                                            \
iterator_type function(container *hash, type const *element)

#define CAG_DEF_INSERTP_HASH(function, container, iterator_type,              \
                             type, cmp_func, hash_func,                       \
                             length_func, get,                                \
                             alloc_style, alloc_func, free_func, val_adr)     \
CAG_DEC_INSERTP_HASH(function, container, iterator_type, type)                \
{                                                                             \
    CAG_P_INSERT_HASH(container, iterator_type,                               \
                      type, cmp_func, hash_func,                              \
                      length_func, get,                                       \
                      alloc_style, alloc_func, free_func, val_adr, *element); \
}

/*! \brief Function declaration and definition for *put*.
*/


#define CAG_DEC_PUT_HASH(function, container, iterator_type, type)            \
    iterator_type function(container *hash, iterator_type it,                 \
                           type const element)


#define CAG_DEF_PUT_HASH(function, container, iterator_type, type)            \
    CAG_DEC_PUT_HASH(function, container, iterator_type, type)                \
    {                                                                         \
        assert(it);                                                           \
        return insert_ ## container(hash, element);                           \
    }

/*! \brief Function declaration and definition for *begin* and *end*.
*/


#define CAG_DEC_BEGIN_HASH(function, container, iterator_type)                \
    iterator_type function(const container *hash)

#define CAG_DEF_BEGIN_HASH(function, container, iterator_type)                \
    CAG_DEC_BEGIN_HASH(function, container, iterator_type)                    \
    {                                                                         \
        iterator_type *it = hash->objects;                                    \
        while (*it == NULL) ++it;                                             \
        return *it;                                                           \
    }

#define CAG_DEC_END_HASH(function, container, iterator_type)                  \
    iterator_type function(const container *hash)

#define CAG_DEF_END_HASH(function, container, iterator_type)                  \
    CAG_DEC_END_HASH(function, container, iterator_type)                      \
    {                                                                         \
        return hash->objects[hash->buckets];                                  \
    }

/*! \brief Function declaration and definition for *next*.
*/


#define CAG_DEC_NEXT_HASH(function, iterator_type)                            \
    iterator_type function(iterator_type const it)

#define CAG_DEF_NEXT_HASH(function, iterator_type)                            \
    CAG_DEC_NEXT_HASH(function, iterator_type)                                \
    {                                                                         \
        iterator_type const *mit;                                             \
        if (it->next)                                                         \
            return it->next;                                                  \
        else {                                                                \
            mit = it->bucket + 1;                                             \
            while ( *mit == NULL)                                             \
                ++mit;                                                        \
            return *mit;                                                      \
        }                                                                     \
    }

/*! \brief Function declaration and definition for *at*. Not sure that this has
   any useful purpose for hash tables.
*/


#define CAG_DEC_AT_HASH(function, iterator_type)                              \
    iterator_type function(iterator_type it, size_t n)

#define CAG_DEF_AT_HASH(function, iterator_type, next)                        \
    CAG_DEC_AT_HASH(function, iterator_type)                                  \
    {                                                                         \
        while(n > 0) {                                                        \
            it = next(it);                                                    \
            --n;                                                              \
        }                                                                     \
        return it;                                                            \
    }

/*! \brief Function declaration and definition for *erase*.
*/


#define CAG_DEC_ERASE_HASH(function, container, iterator_type)                \
    iterator_type function(container *hash, iterator_type it)

#define CAG_DEF_ERASE_HASH(function, container, iterator_type,                \
                           next_func, free_func, val_adr)                     \
CAG_DEC_ERASE_HASH(function, container, iterator_type)                        \
{                                                                             \
    iterator_type t, next, prev = NULL;                                       \
    t = it;                                                                   \
    for (it = it->bucket[0]; it != t; prev = it, it = it->next);              \
    assert (it == t);                                                         \
    --hash->size;                                                             \
    if (prev) {                                                               \
        prev->next = it->next;                                                \
        free_func(val_adr it->value);                                         \
        CAG_FREE(it);                                                         \
        return next_func(prev);                                               \
    } else {                                                                  \
        next = next_func(it);                                                 \
        it->bucket[0] = it->next;                                             \
        free_func(val_adr it->value);                                         \
        CAG_FREE(it);                                                         \
        return next;                                                          \
    }                                                                         \
}

/*! \brief Algorithm, function declaration and definition for *remove*. Both by
   value and by address versions are provided.
*/

#define CAG_P_REMOVE_HASH(iterator_type, next_func, hash_func, length_func,   \
                          cmp_func, val_adr, free_func, key)                  \
do {                                                                          \
    iterator_type it, next, prev = NULL;                                      \
    size_t i = hash_func(key, length_func(key)) % hash->buckets;              \
    for (it = hash->objects[i]; it != NULL; prev = it, it = it->next)         \
        if (cmp_func(val_adr (key), val_adr it->value) == 0) {                \
            --hash->size;                                                     \
            if (prev) {                                                       \
                prev->next = it->next;                                        \
                free_func(val_adr it->value);                                 \
                CAG_FREE(it);                                                 \
                return next_func(prev);                                       \
            } else {                                                          \
                next = next_func(it);                                         \
                hash->objects[i] = it->next;                                  \
                free_func(val_adr it->value);                                 \
                CAG_FREE(it);                                                 \
                return next;                                                  \
            }                                                                 \
        }                                                                     \
    return NULL;                                                              \
} while (0)

#define CAG_DEC_REMOVE_HASH(function, container, iterator_type, type)         \
    iterator_type function(container *hash, const type element)

#define CAG_DEF_REMOVE_HASH(function, container, iterator_type,               \
                            type, next_func, hash_func,                       \
                            length_func, cmp_func, val_adr, free_func)        \
CAG_DEC_REMOVE_HASH(function, container, iterator_type, type)                 \
{                                                                             \
    CAG_P_REMOVE_HASH(iterator_type, next_func, hash_func, length_func,       \
                      cmp_func, val_adr, free_func, element);                 \
}

#define CAG_DEC_REMOVEP_HASH(function, container, iterator_type, type)        \
    iterator_type function(container *hash, const type *element)

#define CAG_DEF_REMOVEP_HASH(function, container, iterator_type,              \
                             type, next_func, hash_func,                      \
                             length_func, cmp_func, val_adr, free_func)       \
CAG_DEC_REMOVEP_HASH(function, container, iterator_type, type)                \
{                                                                             \
    CAG_P_REMOVE_HASH(iterator_type, next_func, hash_func, length_func,       \
                      cmp_func, val_adr, free_func, *element);                \
}


/*! \brief Function declaration and definition to rebuild the hash table.
*/


#define CAG_DEC_REHASH(function, container)                                   \
    container *function(container *hash, size_t buckets)

#define CAG_DEF_REHASH(function, container, iterator_type)                    \
    CAG_DEC_REHASH(function, container)                                       \
    {                                                                         \
        container tmp;                                                        \
        iterator_type it, result;                                             \
        size_t c = 1;                                                         \
        if (!hash->rehash)                                                    \
            return hash;                                                      \
        if (buckets == 0) {                                                   \
            while (c < CAG_P_HTABLE_SIZES &&                                  \
                    cag_p_htable_sizes[c++] <= hash->buckets);                \
            if (c == CAG_P_HTABLE_SIZES) {                                    \
                hash->rehash = 0;                                             \
                return hash;                                                  \
            }                                                                 \
            buckets = cag_p_htable_sizes[c-1];                                \
        }                                                                     \
        if (!new_with_buckets_ ## container(&tmp, buckets)) {                 \
            hash->rehash = 0;                                                 \
            return hash;                                                      \
        }                                                                     \
        CAG_FOR_ALL(container, hash, it,                                      \
                    {                                                         \
                            result =                                          \
                                    insert_ ## container(&tmp, it->value);    \
        if(!result) {                                                         \
        free_ ## container(&tmp);                                             \
            hash->rehash = 0;                                                 \
            return hash;                                                      \
        }                                                                     \
                    });                                                       \
        free_ ## container(hash);                                             \
        hash->objects = tmp.objects;                                          \
        hash->buckets = tmp.buckets;                                          \
        hash->size = tmp.size;                                                \
        return hash;                                                          \
    }

/*! \brief Function declaration and definition for *free*, to return the
   container to the heap.
*/


#define CAG_DEC_FREE_HASH(function, container)                                \
    void function(container *hash)

#define CAG_DEF_FREE_HASH(function, container, iterator_type,                 \
                          free_func, val_adr)                                 \
CAG_DEC_FREE_HASH(function, container)                                        \
{                                                                             \
    size_t i;                                                                 \
    iterator_type prev, curr;                                                 \
    for (i = 0; i < hash->buckets; ++i) {                                     \
        prev = hash->objects[i];                                              \
        while(prev) {                                                         \
            free_func(val_adr prev->value);                                   \
            curr = prev->next;                                                \
            CAG_FREE(prev);                                                   \
            prev = curr;                                                      \
        }                                                                     \
    }                                                                         \
    CAG_FREE(hash->objects[hash->buckets]);                                   \
    CAG_FREE(hash->objects);                                                  \
}

/*! \brief Declaration of hash functions and data structures. */

#define CAG_DEC_CMP_HASH(container, type)                                     \
    struct iterator_ ## container {                                           \
        struct iterator_ ## container *next;                                  \
        struct iterator_ ## container **bucket;                               \
        type value;                                                           \
    };                                                                        \
    typedef struct iterator_ ## container iterator_ ## container;             \
    typedef iterator_ ## container * it_ ## container;                        \
    struct container {                                                        \
        it_ ## container *objects;                                            \
        size_t buckets;                                                       \
        size_t size;                                                          \
        int rehash;                                                           \
    };                                                                        \
    typedef struct container container;                                       \
    CAG_DEC_NEW_HASH_WITH_BUCKETS(new_with_buckets_ ## container,             \
                                  container);                                 \
    CAG_DEC_NEW_HASH(new_ ## container, container);                           \
    CAG_DEC_NEW_HASH_WITH_BUCKETS(new_with_buckets_ ## container, container); \
    CAG_DEC_GET_HASH(get_ ## container, container, it_ ## container, type);   \
    CAG_DEC_GETP_HASH(getp_ ## container, container, it_ ## container, type); \
    CAG_DEC_INSERT_HASH(insert_ ## container, container, it_ ## container,    \
                        type);                                                \
    CAG_DEC_INSERTP_HASH(insertp_ ## container, container, it_ ## container,  \
                         type);                                               \
    CAG_DEC_PUT_HASH(put_ ## container, container, it_ ## container,          \
                     type);                                                   \
    CAG_DEC_BEGIN_HASH(begin_ ## container, container, it_ ## container);     \
    CAG_DEC_END_HASH(end_ ## container, container, it_ ## container);         \
    CAG_DEC_NEXT_HASH(next_ ## container, it_ ## container);                  \
    CAG_DEC_AT_HASH(at_ ## container, it_ ## container);                      \
    CAG_DEC_DISTANCE(distance_ ## container, it_ ## container);               \
    CAG_DEC_REMOVE_HASH(remove_ ## container, container, it_ ## container,    \
                        type);                                                \
    CAG_DEC_REMOVEP_HASH(removep_ ## container, container, it_ ## container,  \
                         type);                                               \
    CAG_DEC_ERASE_HASH(erase_ ## container, container, it_ ## container);     \
    CAG_DEC_ERASE_RANGE(erase_range_ ## container,                            \
                        container, it_ ## container);                         \
    CAG_DEC_REHASH(rehash_ ## container, container);                          \
    CAG_DEC_FREE_HASH(free_ ## container, container);                         \
    CAG_DEC_FORWARD(container, type)

/*! \brief Identical to CAG_DEC_CMP_HASH but provided for users who
    want consistent names.
*/

#define CAG_DEC_CMPP_HASH CAG_DEC_CMP_HASH

/*! \brief Definitions of hash functions. */

#define CAG_DEF_CMP_ALL_HASH(container, type, cmp_func, val_adr, hash_func,   \
                             length_func, alloc_style, alloc_func, free_func) \
CAG_DEF_NEW_HASH_WITH_BUCKETS(new_with_buckets_ ## container,                 \
                              container)                                      \
CAG_DEF_NEW_HASH(new_ ## container, container)                                \
CAG_DEF_GET_HASH(get_ ## container, container, it_ ## container,              \
                 type, cmp_func, val_adr, hash_func, length_func)             \
CAG_DEF_GETP_HASH(getp_ ## container, container, it_ ## container,            \
                  type, cmp_func, val_adr, hash_func, length_func)            \
CAG_DEF_INSERT_HASH(insert_ ## container, container, it_ ## container,        \
                    type, cmp_func,                                           \
                    hash_func, length_func, get_ ## container,                \
                    alloc_style, alloc_func, free_func, val_adr)              \
CAG_DEF_INSERTP_HASH(insertp_ ## container, container, it_ ## container,      \
                     type, cmp_func,                                          \
                     hash_func, length_func, get_ ## container,               \
                     alloc_style, alloc_func, free_func, val_adr)             \
CAG_DEF_PUT_HASH(put_ ## container, container, it_ ## container,              \
                 type)                                                        \
CAG_DEF_BEGIN_HASH(begin_ ## container, container, it_ ## container)          \
CAG_DEF_END_HASH(end_ ## container, container, it_ ## container)              \
CAG_DEF_NEXT_HASH(next_ ## container, it_ ## container)                       \
CAG_DEF_AT_HASH(at_ ## container, it_ ## container, next_ ## container)       \
CAG_DEF_DISTANCE(distance_ ## container, it_ ## container, next_ ## container)\
CAG_DEF_REMOVE_HASH(remove_ ## container, container, it_ ## container,        \
                    type, next_ ## container, hash_func, length_func,         \
                    cmp_func, val_adr, free_func)                             \
CAG_DEF_REMOVEP_HASH(removep_ ## container, container, it_ ## container,      \
                     type, next_ ## container, hash_func, length_func,        \
                     cmp_func, val_adr, free_func)                            \
CAG_DEF_ERASE_HASH(erase_ ## container, container, it_ ## container,          \
                   next_ ## container, free_func, val_adr)                    \
CAG_DEF_ERASE_RANGE(erase_range_ ## container, container,                     \
                    it_ ## container, erase_ ## container, CAG_NO_OP_3)       \
CAG_DEF_REHASH(rehash_ ## container, container, it_ ## container)             \
CAG_DEF_FREE_HASH(free_ ## container, container, it_ ## container,            \
                  free_func, val_adr)                                         \
CAG_DEF_FORWARD(container, type)                                              \
typedef container CAG_P_CMB(container,  __LINE__)

/*! \brief Definition of most common case hash function.
   This is a hash table that does not manage the memory of its elements.
*/

#define CAG_DEF_CMP_HASH(container, type, cmp_func, hash_func, length_func)   \
    CAG_DEF_CMP_ALL_HASH(container, type, cmp_func, CAG_BYVAL,                \
                         hash_func, length_func,                              \
                         CAG_NO_ALLOC_STYLE, CAG_ALLOC_DEFAULT, CAG_NO_FREE_FUNC)

/*! \brief Same as CAG_DEF_CMP_HASH but cmp_func takes parameters by address. */

#define CAG_DEF_CMPP_HASH(container, type, cmp_func, hash_func, length_func)   \
    CAG_DEF_CMP_ALL_HASH(container, type, cmp_func, CAG_BYADR,                \
                         hash_func, length_func,                              \
                         CAG_NO_ALLOC_STYLE, CAG_ALLOC_DEFAULT, CAG_NO_FREE_FUNC)


/*! \brief Used to declare and define a hash table in one macro.  Useful usually
   for small programs where the container will only be used in one module.
*/

#define CAG_DEC_DEF_CMP_HASH(container, type, cmp_func, hash_func, length_func)   \
    CAG_DEC_CMP_HASH(container, type);                                        \
    CAG_DEF_CMP_HASH(container, type, cmp_func, hash_func, length_func)

/*! \brief Same as CAG_DEC_DEF_CMP_ARRAY but cmp_func takes its parameters
    by address.
*/
#define CAG_DEC_DEF_CMPP_HASH(container, type, cmp_func, hash_func, length_func)   \
    CAG_DEC_CMPP_HASH(container, type);                                        \
    CAG_DEF_CMPP_HASH(container, type, cmp_func, hash_func, length_func)


/*! \brief Declare and define macros for a hash table whose elements are structs
   composed of two strings.  This is a common use-case, e.g. for a dictionary
   made up of words (the keys) and definitions.
*/


#define CAG_DEC_STR_STR_HASH(container, type)                                 \
    CAG_DEC_CMP_HASH(container, type)

#define CAG_DEF_STR_STR_HASH(container, type)                                 \
    CAG_DEF_CMP_ALL_HASH(container, type, CAG_STRCMP_STRUCT_WITH_STR_KEY,     \
                         CAG_BYVAL, CAG_OAT_HASH_STRUCT_WITH_STR_KEY,         \
                         CAG_STRLEN_STRUCT_WITH_STR_KEY, CAG_STRUCT_ALLOC_STYLE,  \
                         cag_alloc_str_str, CAG_FREE_STRUCT_STR_STR)

#define CAG_DEC_DEF_STR_STR_HASH(container, type)                             \
    CAG_DEC_STR_STR_HASH(container, type);                                    \
    CAG_DEF_STR_STR_HASH(container, type)


/*! \brief Declare and define macros for a hash table whose elements are C
   strings.  This is a common use-case, e.g. a list of words.
*/


#define CAG_DEC_STR_HASH(container)                                           \
    CAG_DEC_CMP_HASH(container, char *)

#define CAG_DEF_STR_HASH(container)                                           \
    CAG_DEF_CMP_ALL_HASH(container, char *, strcmp, CAG_BYVAL, cag_oat_hash,  \
                         strlen, CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free)

#define CAG_DEC_DEF_STR_HASH(container, type)                                 \
    CAG_DEC_STR_HASH(container, type);                                        \
    CAG_DEF_STR_HASH(container, type)


#endif /* CAG_HASH_H */
