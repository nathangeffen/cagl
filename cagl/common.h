/*! \file Generic algorithms and common code used throughout CAGL library.

    \copyright Copyright 2014 Nathan Geffen. All rights reserved.

    \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

    \sa COPYING for the license text.

    \sa howtodev.rst to learn how this code works and how to modify it.
*/

#ifndef CAG_COMMON_H
#define CAG_COMMON_H

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define CAG_TRUE 1
#define CAG_FALSE 0

#define CAG_SUCCESS 0
#define CAG_ERROR 1

#define CAG_VERSION 0.1

#if __STDC_VERSION__ < 199901L

#define restrict
#define inline

#endif

/* TO IMPLEMENT
   Define CAG_SAFER during development to catch bugs and
   for safer use of this library.
*/

#ifdef CAG_SAFER

#define CAG_DEF_INITIALIZE int initialized;
#define CAG_SET_INITIALIZED(var) var = 1
#define CAG_CHECK_INITIALIZED(var) assert(var == 1)

#else

#define CAG_DEF_INITIALIZE
#define CAG_SET_INITIALIZED(var)
#define CAG_CHECK_INITIALIZED(var)

#endif

/* Only relevant if gda_error.h included. */

#ifdef CAG_ERROR_H

#define CAG_PROCESS_ERROR(M)                                                  \
    CAG_LOG_ERR(M);                                                           \
    goto CAG_ERROR_LABEL

#else

#define CAG_PROCESS_ERROR(M)

#endif


#ifndef CAG_MALLOC

#define CAG_MALLOC malloc

#endif

#ifndef CAG_FREE

#define CAG_FREE free

#endif

#ifndef CAG_RAND

#define CAG_RAND rand

#endif

#ifndef CAG_RAND_RANGE

/*! \brief Returns a random integer in the range [min, max).

   See:
   http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range-c
*/

#define CAG_RAND_RANGE(min, max, result)                                      \
    do {                                                                      \
        int found = 0;                                                        \
        while (!found) {                                                      \
            int base_random = CAG_RAND();                                     \
            int range, remainder, bucket;                                     \
            if (RAND_MAX == base_random)                                      \
                continue;                                                     \
            /* now guaranteed to be in [0, RAND_MAX) */                       \
            range       = max - min;                                          \
            remainder   = RAND_MAX % range;                                   \
            bucket      = RAND_MAX / range;                                   \
            /* There are range buckets, plus one smaller interval */          \
            /*   within remainder of RAND_MAX */                              \
            if (base_random < RAND_MAX - remainder) {                         \
                result = min + base_random / bucket;                          \
                found = 1;                                                    \
            }                                                                 \
        }                                                                     \
    } while(0)

#endif

/*! \brief Used to indicate whether comparison function parameters are
   by address or value.
*/

#define CAG_BYADR &

#define CAG_BYVAL *&

/*! \brief Typically use this macro for alloc_style parameter if container must
   allocate memory for its type and the type is a pointer getting memory from
   malloc, strdup or a malloc or strdup like function.
*/

#define CAG_SIMPLE_ALLOC_STYLE(to, from, alloc_func, free_code)               \
    if(! (to = alloc_func(from))) free_code;

/*! \brief Typically use this macro for alloc_style parameter if container must
   allocate memory for its type and the type is a struct.
*/

#define CAG_STRUCT_ALLOC_STYLE(to, from, alloc_func, free_code)               \
    if(! (alloc_func(&to, &from))) free_code;


/*! \brief Use this macro for alloc_style parameter if container doesn't allocate
   memory for its type.
*/

#define CAG_NO_ALLOC_STYLE(to, from, alloc_func, free_code) to = from

/*! \brief Use this macro for alloc_func parameter when container doesn't allocate
   memory for its type.
*/

#define CAG_NO_ALLOC_FUNC(var) var

/*! \brief Use this macro for free_func if container doesn't allocate memory for
  its type.
*/

#define CAG_NO_FREE_FUNC(var)

/*! All CAG container iterators can be dereferenced using this macro. */

#define CAG_VALUE(it) (it)->value

/*! \brief Helper macros. */

#define CAG_IDENTITY(x) x

#define CAG_ALWAYS_TRUE_OP_1(x) 1

#define CAG_ALWAYS_TRUE_OP_2(x, y) 1

#define CAG_ALWAYS_FALSE_OP_1(x) 0

#define CAG_NO_OP_1(x)

#define CAG_NO_OP_2(x)

#define CAG_NO_OP_3(x, y, z)

/*! \brief Macros to handle structs composed of two char pointers. Useful for
    hash tables and trees.
*/

#define CAG_STR_KEY_FROM_STRUCT(x) * (char **) &(x)

#define CAG_STRCMP_STRUCT_WITH_STR_KEY(x, y)                                  \
    strcmp(CAG_STR_KEY_FROM_STRUCT(x), CAG_STR_KEY_FROM_STRUCT(y))

#define CAG_STRLEN_STRUCT_WITH_STR_KEY(x)                                     \
    strlen(CAG_STR_KEY_FROM_STRUCT(x))

#define CAG_FREE_STRUCT_STR_STR(x) cag_free_str_str(&x)

/*! \brief Default comparison function for C primitive types */

#define CAG_CMP_DEFAULT(a, b)  ((a) < (b)  ? -1 : ((a) == (b) ? 0 : 1))

/*! \brief Default comparison function for pointers to C primitive types */

#define CAG_CMPP_DEFAULT(a, b) CAG_CMP_DEFAULT(*a, *b)

/*! \brief Generic *new_from* function declaration and definition macro. Similar
    role to C++ copy constructor.
*/

#define CAG_DEC_NEW_FROM(function, container)                                 \
    container *function(container *to, const container *from)

#define CAG_DEF_NEW_FROM(function, container)                                 \
    CAG_DEC_NEW_FROM(function, container)                                     \
    {                                                                         \
        if (new_ ## container(to))                                            \
            return copy_all_ ## container(from, to);                          \
        else                                                                  \
            return NULL;                                                      \
    }

/*! \brief Generic *at* function declaration and definition macro.  Given an
   iterator, find the iterator *n* places away from it.
*/

#define CAG_DEC_AT(function, container, iterator_type)                        \
    iterator_type function(iterator_type iterator, size_t n)

#define CAG_DEF_AT(function, container, iterator_type, dir)                   \
    CAG_DEC_AT(function, container, iterator_type)                            \
    {                                                                         \
        while(n--) iterator = dir(iterator);                                  \
        return iterator;                                                      \
    }

/*! \brief Generic swap macro. */

#define CAG_SWAP(op1, op2)                                                    \
    do {                                                                      \
        char t[sizeof(op1)];                                                  \
        memcpy(t, &(op1), sizeof(op1));                                       \
        (op1) = (op2);                                                        \
        memcpy(&op2, t, sizeof(op1));                                         \
    } while(0)

/*! \brief Generic *swap* function declaration and definition macro. */

#define CAG_DEC_SWAP(function, iterator_type)                                 \
    void function(iterator_type a, iterator_type b)

#define CAG_DEF_SWAP(function, iterator_type, value)                          \
    CAG_DEC_SWAP(function, iterator_type)                                     \
    {                                                                         \
        CAG_SWAP(value(a), value(b));                                         \
    }


/*! \brief Generic *reverse* algorithm. Works on bidirectional iterators. */

#define CAG_REVERSE(first, last, prev, next, swap)                            \
    do {                                                                      \
        while (first != last && first != (last = prev(last))) {               \
            swap(first, last);                                                \
            first = next(first);                                              \
        }                                                                     \
    } while(0)

/*! \brief Generic *reverse* function declaration and definition macro. */

#define CAG_DEC_REVERSE(function, iterator_type)                              \
    iterator_type function(iterator_type first, iterator_type last)

#define CAG_DEF_REVERSE(function, iterator_type, prev, next, swap)            \
    CAG_DEC_REVERSE(function, iterator_type)                                  \
    {                                                                         \
        iterator_type it = first;                                             \
        CAG_REVERSE(first, last, prev, next, swap);                           \
        return it;                                                            \
    }

/*! \brief Generic *copy_over*. Works on forward iterators. */

#define CAG_COPY_OVER(first, last, value, result, input_next, output_next)    \
    do {                                                                      \
        while (first != last) {                                               \
            result->value = first->value;                                     \
            first = input_next(first);                                        \
            result = output_next(result);                                     \
        }                                                                     \
    } while(0)


/*! \brief Generic *copy_over* function declaration and definition. */

#define CAG_DEC_COPY_OVER(function, iterator_in, iterator_out)                \
    iterator_out function(iterator_in first, iterator_in last,                \
                          iterator_out result)

#define CAG_DEF_COPY_OVER(function, iterator_in, iterator_out, input_next,    \
                          output_next)                                        \
CAG_DEC_COPY_OVER(function, iterator_in, iterator_out)                        \
{                                                                             \
    CAG_COPY_OVER(first, last, value, result, input_next, output_next);       \
    return result;                                                            \
}


/*! \brief Macro and generic *concat* function declaration and definition. */

#define CAG_CONCAT_TO_EMPTY(container_result, c_result,                       \
                            container_1, c1,                                  \
                            container_2, c2)                                  \
do {                                                                          \
    it_ ## container_1 from_1 = begin_ ## container_1(c1);                    \
    it_ ## container_2 from_2 = begin_ ## container_2(c2);                    \
    it_ ## container_1 to_1 = end_ ## container_1(c1);                        \
    it_ ## container_2 to_2 = end_ ## container_2(c2);                        \
    it_ ## container_result result = begin_ ## container_result(c_result);    \
    void *p;                                                                  \
    while(from_1 != to_1) {                                                   \
        p = put_ ## container_result(c_result, result, from_1->element);      \
        if (!p)                                                               \
            break;                                                            \
        else                                                                  \
            p = next_ ## container_2(p);                              \       \
        from_1 = next_ ## container_1(from_1);                                \
    }                                                                         \
    if (p)                                                                    \
        while(from_2 != to_2) {                                               \
            p = put_ ## container_result(c_result, result, from_2->element);  \
            if (!p)                                                           \
                break;                                                        \
            else                                                              \
                p = next_ ## container_2(p);                                  \
            from_2 = next_ ## container_2(from_2);                            \
        }                                                                     \
} while(0)

#define CAG_CONCAT(container_result, container_to)

/*! \brief Generic *random_shuffle* algorithm. Works on bidirectional iterators.
   Theta(n) on random access iterators. O(n^2) otherwise.
*/

#define CAG_RANDOM_SHUFFLE(from, to, distance, swap, at, prev)                \
    do {                                                                      \
        size_t i, r, n = distance(from, to);                                  \
        for (i = n - 1, to = prev(to); i > 0; --i, to = prev(to)) {           \
            CAG_RAND_RANGE(0, i, r);                                          \
            swap(to, at(from, r));                                            \
        }                                                                     \
    } while(0)

/*! \brief Generic *random_shuffle* function declaration and definition. */

#define CAG_DEC_RANDOM_SHUFFLE(function, iterator_type)                       \
    iterator_type function(const iterator_type from, iterator_type to)

#define CAG_DEF_RANDOM_SHUFFLE(function, iterator_type, distance,             \
                               swap, at, prev)                                \
CAG_DEC_RANDOM_SHUFFLE(function, iterator_type)                               \
{                                                                             \
    CAG_RANDOM_SHUFFLE(from, to, distance, swap, at, prev);                   \
    return from;                                                              \
}

/*! \brief Generic *distance* function declaration and definition for forward
    iterators.
*/

#define CAG_DISTANCE(from, to, next, result)                                  \
    do {                                                                      \
        result = 0;                                                           \
        for (; from != to; from = next(from))                                 \
            ++result;                                                         \
    } while (0)


#define CAG_DEC_DISTANCE(function, iterator_type)                             \
    size_t function(iterator_type from,                                       \
                    const iterator_type to)

#define CAG_DEF_DISTANCE(function, iterator_type, next)                       \
    CAG_DEC_DISTANCE(function, iterator_type)                                 \
    {                                                                         \
        size_t c;                                                             \
        CAG_DISTANCE(from, to, next, c);                                      \
        return c;                                                             \
    }



/*! \brief Generic *copy* algorithm. Works on bidirectional iterators. */

#define CAG_COPY_IF(first, last, container_var, it, value, next_in,           \
                    next_out, insert_out, cond_func, val_adr, data)           \
do {                                                                          \
    void *p = it;                                                             \
    while(first != last) {                                                    \
        if(cond_func(val_adr value(first), data))  {                          \
            p = insert_out(container_var, p, value(first));                   \
            if (!p)                                                           \
                break;                                                        \
            else                                                              \
                p = next_out(p);                                              \
        }                                                                     \
        first = next_in(first);                                               \
    }                                                                         \
} while(0)

#define CAG_COPY(first, last, container_var, it, value, next_in,              \
                 next_out, insert_out)                                        \
CAG_COPY_IF(first, last, container_var, it, value, next_in, next_out,         \
            insert_out, CAG_ALWAYS_TRUE_OP_2, CAG_BYVAL, NULL)

/*! \brief Generic *copy* function declaration and definition. */

#define CAG_DEC_COPY(function, iterator_type, container)                      \
    container *function(iterator_type first,                                  \
                        iterator_type last,                                   \
                        container *c)


#define CAG_DEF_COPY(function, iterator_type, container, begin,               \
                     next_in, next_out, insert_out)                           \
CAG_DEC_COPY(function, iterator_type, container)                              \
{                                                                             \
    void *it = begin(c);                                                      \
    CAG_COPY(first, last, c, it, CAG_VALUE, next_in, next_out,                \
             insert_out);                                                     \
    return c;                                                                 \
}

#define CAG_DEC_COPY_IF(function, iterator_type, container, type)             \
    container *function(iterator_type first,                                  \
                        iterator_type last,                                   \
                        container *c,                                         \
                        int (*cond_func) (type*, void *),                     \
                        void *data)


#define CAG_DEF_COPY_IF(function, iterator_type, container, type, begin,      \
                        next_in, next_out, insert_out)                        \
CAG_DEC_COPY_IF(function, iterator_type, container, type)                     \
{                                                                             \
    void *it = begin(c);                                                      \
    CAG_COPY_IF(first, last, c, it, CAG_VALUE, next_in, next_out,             \
                insert_out, cond_func, CAG_BYADR, data);                      \
    return c;                                                                 \
}

#define CAG_DEC_COPY_IF_ALL(function, container_in, container_out, type)      \
    container_out *function(const container_in *c1, container_out *c2,        \
                            int (*cond_func) (type*, void *),                 \
                            void *data)


#define CAG_DEF_COPY_IF_ALL(function, container_in, container_out, type,      \
                            copy_func, begin_in, end_in)                      \
CAG_DEC_COPY_IF_ALL(function, container_in, container_out, type)              \
{                                                                             \
    return copy_func(begin_in(c1),                                            \
                     end_in(c1),                                              \
                     c2, cond_func, data);                                    \
}


/*! \brief Simple generic macro and function declaration and definition to
    compare the value of two iterators.
*/

#define CAG_CMP(it1, it2, cmp_func, val_adr)                                  \
    cmp_func(val_adr CAG_VALUE(it1), val_adr CAG_VALUE(it2))

#define CAG_DEC_CMP(function, iterator_type_1, iterator_type_2)               \
    int function(const iterator_type_1 it1, const iterator_type_2 it2)

#define CAG_DEF_CMP(function, iterator_type_1, iterator_type_2,               \
                    cmp_func, val_adr)                                        \
CAG_DEC_CMP(function, iterator_type_1, iterator_type_2)                       \
{                                                                             \
    return CAG_CMP(it1, it2, cmp_func, val_adr);                              \
}

/*! \brief Generic macro and function declaration and definition to compare two
   sequences. Returns -1 if a smaller value is encountered first in the first
   sequence, 0 if the sequences are precisely equal and 1 otherwise.
*/

#define CAG_CMP_RANGE(from_1, to_1, from_2, to_2,                             \
                      next_1, next_2, result, cmp_func, val_adr)              \
do {                                                                          \
    while (from_1 != to_1 && from_2 != to_2) {                                \
        result = CAG_CMP(from_1, from_2, cmp_func, val_adr);                  \
        if (result)                                                           \
            goto __cag__label__cmp__;                                         \
        from_1 = next_1(from_1);                                              \
        from_2 = next_2(from_2);                                              \
    }                                                                         \
    if (from_1 != to_1)                                                       \
        result = 1;                                                           \
    else                                                                      \
        if (from_2 != to_2)                                                   \
            result = -1;                                                      \
        else                                                                  \
            result = 0;                                                       \
__cag__label__cmp__:;                                                         \
} while (0)

#define CAG_DEC_CMP_RANGE(function, iterator_type_1, iterator_type_2)         \
    int function(iterator_type_1 from_1, const iterator_type_1 to_1,          \
                 iterator_type_2 from_2, const iterator_type_2 to_2)

#define CAG_DEF_CMP_RANGE(function, iterator_type_1, iterator_type_2,         \
                          next_1, next_2, cmp_func, val_adr)                  \
CAG_DEC_CMP_RANGE(function, iterator_type_1, iterator_type_2)                 \
{                                                                             \
    int i = 0;                                                                \
    CAG_CMP_RANGE(from_1, to_1, from_2, to_2,                                 \
                  next_1, next_2, i, cmp_func, val_adr);                      \
    return i;                                                                 \
}

/*! \brief Generic *cmp_all* function declaration and definition. */

#define CAG_DEC_CMP_ALL(function, container_1, container_2)                   \
    int function(const container_1 *c1, const container_2 *c2)

#define CAG_DEF_CMP_ALL(function, container_1, container_2,                   \
                        iterator_type_1, iterator_type_2,                     \
                        begin_1, end_1, begin_2, end_2,                       \
                        next_1, next_2, cmp_func, val_adr)                    \
CAG_DEC_CMP_ALL(function, container_1, container_2)                           \
{                                                                             \
    int i = 0;                                                                \
    iterator_type_1 from_1, to_1;                                             \
    iterator_type_2 from_2, to_2;                                             \
    from_1 = begin_1(c1);                                                     \
    from_2 = begin_2(c2);                                                     \
    to_1 = end_1(c1);                                                         \
    to_2 = end_2(c2);                                                         \
    CAG_CMP_RANGE(from_1, to_1, from_2, to_2,                                 \
                  next_1, next_2, i, cmp_func, val_adr);                      \
    return i;                                                                 \
}


#define CAG_EQUAL !CAG_CMP

/*! \brief Similar to *cmp_range* algorithm except returns CAG_TRUE (1) when two
   sequences have identical values, else CAG_FALSE (0). Also *to* only is only
   specified for first range. Same behaviour as STL *equal*.

*/

#define CAG_EQUAL_RANGE(from_1, to, from_2, next_1, next_2,                   \
                        result, cmp_func, val_adr)                            \
do {                                                                          \
    while (from_1 != to_1) {                                                  \
        result = CAG_EQUAL(from_1, from_2, cmp_func, val_adr);                \
        if (!result)                                                          \
            break;                                                            \
        from_1 = next_1(from_1);                                              \
        from_2 = next_2(from_2);                                              \
    }                                                                         \
} while (0)


/*! \brief Generic *equal* function declaration and definition. */

#define CAG_DEC_EQUAL_RANGE(function, iterator_type_1, iterator_type_2)       \
    int function(iterator_type_1 from_1, const iterator_type_1 to_1,          \
                 iterator_type_2 from_2)

#define CAG_DEF_EQUAL_RANGE(function, iterator_type_1, iterator_type_2,       \
                            next_1, next_2, cmp_func, val_adr)                \
CAG_DEC_EQUAL_RANGE(function, iterator_type_1, iterator_type_2)               \
{                                                                             \
    int i = CAG_TRUE;                                                         \
    CAG_EQUAL_RANGE(from_1, to_1, from_2,                                     \
                    next_1, next_2, i, cmp_func, val_adr);                    \
    return i;                                                                 \
}

/*! \brief Generic *equal_all* function declaration and definition. */

#define CAG_DEC_EQUAL_ALL(function, container_1, container_2)                 \
    int function(const container_1 *c1, const container_2 *c2)

#define CAG_DEF_EQUAL_ALL(function, container_1, container_2,                 \
                          iterator_type_1, iterator_type_2,                   \
                          begin_1, end_1, begin_2,                            \
                          next_1, next_2, cmp_func, val_adr)                  \
CAG_DEC_EQUAL_ALL(function, container_1, container_2)                         \
{                                                                             \
    int i = 0;                                                                \
    iterator_type_1 from_1, to_1;                                             \
    iterator_type_2 from_2;                                                   \
    from_1 = begin_1(c1);                                                     \
    from_2 = begin_2(c2);                                                     \
    to_1 = end_1(c1);                                                         \
    CAG_EQUAL_RANGE(from_1, to_1, from_2,                                     \
                    next_1, next_2, i, cmp_func, val_adr);                    \
    return i;                                                                 \
}

/*! \brief Generic *linear_search* algorithm. */

#define CAG_LSEARCH(first, last, dir, cmp_func, val_adr, value, key)          \
    do {                                                                      \
        while (first != last)   {                                             \
            if (cmp_func(val_adr value(first), val_adr (key)) == 0)           \
                break;                                                        \
            first = dir(first);                                               \
        }                                                                     \
    } while(0)

/*! \brief Generic *linear_search* function declaration and definition.
   Works on forward iterators.
*/

#define CAG_DEC_SEARCH(function, iterator_type, type)                         \
    iterator_type  function(iterator_type first,                              \
                            const iterator_type last,                         \
                            type const key)

#define CAG_DEF_LSEARCH(function, iterator_type, type,                        \
                        direction, value, cmp_func, val_adr)                  \
CAG_DEC_SEARCH(function, iterator_type, type)                                 \
{                                                                             \
    CAG_LSEARCH(first, last, direction, cmp_func, val_adr, value, key);       \
    return first;                                                             \
}

/*! \brief Generic *linear_search* function declaration and definition, but used with
   comparison functions whose comparators are by address not by value.
*/

#define CAG_DEC_SEARCHP(function, iterator_type, type)                        \
    iterator_type  function(iterator_type first,                              \
                            const iterator_type last,                         \
                            type const *key)

#define CAG_DEF_LSEARCHP(function, iterator_type, type,                       \
                         direction, value, cmp_func, val_adr)                 \
CAG_DEC_SEARCHP(function, iterator_type, type)                                \
{                                                                             \
    CAG_LSEARCH(first, last, direction, cmp_func, val_adr, value, *key);      \
    return first;                                                             \
}


/*! \brief Generic *lower_bound* algorithm.  Works on sorted bidirectional
   iterators.  Based on the STL lower_bound algorithm.
   O(log n) for random access iterators. O(n) otherwise.
*/

#define CAG_P_LOWER_BOUND(iterator_type, first, last, at, distance,           \
                          prev, next, value, key, cmp_func, val_adr)          \
do {                                                                          \
    iterator_type it;                                                         \
    size_t count, step;                                                       \
    count = distance(first, last);                                            \
    while (count > 0)  {                                                      \
        it = first;                                                           \
        step = count/2;                                                       \
        it = at (it, step);                                                   \
        if ((cmp_func(val_adr value(it), val_adr (key)) < 0)) {               \
            it = next(it);                                                    \
            first = it;                                                       \
            count -= step+1;                                                  \
        }                                                                     \
        else count=step;                                                      \
    }                                                                         \
    return first;                                                             \
} while (0)


#define CAG_DEC_LOWER_BOUND(function, iterator_type, type)                    \
    iterator_type  function(iterator_type first,                              \
                            iterator_type last,                               \
                            type const key)

#define CAG_DEC_LOWER_BOUNDP(function, iterator_type, type)                   \
    iterator_type  function(iterator_type first,                              \
                            iterator_type last,                               \
                            type const *key)

#define CAG_DEF_LOWER_BOUND(function, iterator_type, type,                    \
                            at, distance, prev, next, value, cmp_func, val_adr)   \
CAG_DEC_LOWER_BOUND(function, iterator_type, type)                            \
{                                                                             \
    CAG_P_LOWER_BOUND(iterator_type, first, last, at, distance,               \
                      prev, next, value, key, cmp_func, val_adr);             \
}

#define CAG_DEF_LOWER_BOUNDP(function, iterator_type, type,                   \
                             at, distance, prev, next, value, cmp_func, val_adr)  \
CAG_DEC_LOWER_BOUNDP(function, iterator_type, type)                           \
{                                                                             \
    CAG_P_LOWER_BOUND(iterator_type, first, last, at, distance,               \
                      prev, next, value, *key, cmp_func, val_adr);            \
}



/*! \brief Generic *binary_search* algorithm.  Works on sorted bidirectional
   iterators.  O(log n) for random access iterators. O(n) otherwise.
*/

#define CAG_P_BSEARCH(first, last, key, lower_bound, cmp_func, val_adr)       \
    {                                                                         \
        first = lower_bound(first, last, &key);                               \
        return (first != last &&                                              \
                !(cmp_func(val_adr (key), val_adr first->value) < 0));        \
    }

#define CAG_DEC_BSEARCH(function, iterator_type, type)                        \
    int  function(iterator_type first,                                        \
                  iterator_type last,                                         \
                  type const key)

#define CAG_DEC_BSEARCHP(function, iterator_type, type)                       \
    int  function(iterator_type first,                                        \
                  iterator_type last,                                         \
                  type const *key)

#define CAG_DEF_BSEARCH(function, iterator_type, type,                        \
                        lower_bound, cmp_func, val_adr)                       \
CAG_DEC_BSEARCH(function, iterator_type, type)                                \
{                                                                             \
    CAG_P_BSEARCH(first, last, key, lower_bound, cmp_func, val_adr);          \
}

#define CAG_DEF_BSEARCHP(function, iterator_type, type,                       \
                         lower_bound, cmp_func, val_adr)                      \
CAG_DEC_BSEARCHP(function, iterator_type, type)                               \
{                                                                             \
    CAG_P_BSEARCH(first, last, *key, lower_bound, cmp_func, val_adr);         \
}


/*! \brief Generic *find* macro, function declaration and definition. Used to
    search an unordered container or by a non-key field in an ordered one.
*/

#define CAG_P_FIND(type, from, to, next, element, cmp_func)                   \
    do {                                                                      \
        while (from != to &&                                                  \
                cmp_func((const type *) &from->value, &element))              \
            from = next(from);                                                \
    } while(0)

#define CAG_DEC_FIND(function, iterator_type, type)                           \
    iterator_type function(iterator_type from, const iterator_type to,        \
                           const type element,                                \
                           int (*cmp_func) (const type*, const type*))

#define CAG_DEF_FIND(function, iterator_type, type, next)                     \
    CAG_DEC_FIND(function, iterator_type, type)                               \
    {                                                                         \
        CAG_P_FIND(type, from, to, next, element, cmp_func);                  \
        return from;                                                          \
    }

#define CAG_DEC_FINDP(function, iterator_type, type)                          \
    iterator_type function(iterator_type from, const iterator_type to,        \
                           const type *element,                               \
                           int (*cmp_func)(const type*, const type*))

#define CAG_DEF_FINDP(function, iterator_type, type, next)                    \
    CAG_DEC_FINDP(function, iterator_type, type)                              \
    {                                                                         \
        CAG_P_FIND(type, from, to, next, *element, cmp_func);                 \
        return from;                                                          \
    }

#define CAG_DEC_FIND_ALL(function, container, iterator_type, type)            \
    iterator_type function(const container *c, const type element,            \
                           int (*cmp_func) (const type*, const type*))

#define CAG_DEF_FIND_ALL(function, container, iterator_type, type)            \
    CAG_DEC_FIND_ALL(function, container, iterator_type, type)                \
    {                                                                         \
        return find_ ## container(begin_ ## container(c),                     \
                                  end_ ## container(c),                       \
                                  element, cmp_func);                         \
    }

#define CAG_DEC_FINDP_ALL(function, container, iterator_type, type)           \
    iterator_type function(container *c, const type* element,                 \
                           int (*cmp_func) (const type*, const type*))

#define CAG_DEF_FINDP_ALL(function, container, iterator_type, type)           \
    CAG_DEC_FINDP_ALL(function, container, iterator_type, type)               \
    {                                                                         \
        return findp_ ## container(begin_ ## container(c),                    \
                                   end_ ## container(c),                      \
                                   element, cmp_func);                        \
    }


/*! \brief Macro, declaration and definition of *index*, which is a convenient
    way of saying *at_[container](beg_[container](var), n)*.
*/


#define CAG_INDEX(container, var, n) \
    at_ ## container(beg_ ## container(var), n)

#define CAG_DEC_INDEX(function, container) \
    it_ ## container function(container *c, size_t n)

#define CAG_DEF_INDEX(function, container) \
    CAG_DEC_INDEX(function, container) \
    { \
        return CAG_INDEX(container, c, n); \
    }

/*! \brief Declaration and definition of *erase_range*. Works on forward
    iterators.
*/

#define CAG_DEC_ERASE_RANGE(function, container, iterator_type)               \
    iterator_type function(container *c, iterator_type from, iterator_type to)

#define CAG_DEF_ERASE_RANGE(function, container, iterator_type,               \
                            erase_function, post_erase_hook)                  \
CAG_DEC_ERASE_RANGE(function, container, iterator_type)                       \
{                                                                             \
    while(from != to) {                                                       \
        from = erase_function(c, from);                                       \
        post_erase_hook(c, from, to);                                         \
    }                                                                         \
    return from;                                                              \
}

/*! \brief Declaration and definition of a void function that applies a function
    to multiple containers.
*/

#define CAG_DEC_VOID_MANY_ARG(function, container)                            \
    void function(int max, container *c, ...)

#define CAG_DEF_VOID_MANY_ARG(function, container, func)                      \
    CAG_DEC_VOID_MANY_ARG(function, container)                                \
    {                                                                         \
        container *t;                                                         \
        va_list ap;                                                           \
        if (max < 0) max = -max;                                              \
        func(c);                                                              \
        va_start(ap, c);                                                      \
        while( (t = va_arg(ap, container *) ) ) {                             \
            if (max > 0) {                                                    \
                func(t);                                                      \
                --max;                                                        \
            }                                                                 \
        }                                                                     \
        va_end(ap);                                                           \
    }

/*! \brief Declaration and definition of a function that applies a function with
   one argument to multiple containers. If the function returns non-NULL, a
   counter is incremented, else upon NULL returns the current count. Useful for
   error management on batch operations.
*/


#define CAG_DEC_RET_MANY_ARG(function, container)                             \
    int function(container *c, ...)

#define CAG_DEF_RET_MANY_1_ARG(function, container, func)                     \
    CAG_DEC_RET_MANY_ARG(function, container)                                 \
    {                                                                         \
        int i = 0;                                                            \
        int num_args = 0;                                                     \
        unsigned good = 1;                                                    \
        container *t;                                                         \
        va_list ap;                                                           \
        if ( !func(c) )                                                       \
            good = 0;                                                         \
        else                                                                  \
            ++i;                                                              \
        ++num_args;                                                           \
        va_start(ap, c);                                                      \
        while( (t = va_arg(ap, container *) ) ) {                             \
            if ( good ) {                                                     \
                if (!func(t))                                                 \
                    good = 0;                                                 \
                else                                                          \
                    ++i;                                                      \
                ++num_args;                                                   \
            }                                                                 \
        }                                                                     \
        va_end(ap);                                                           \
        return i < num_args ? -i : i;                                         \
    }

/*! \brief Definition of a function that applies a function with two arguments
   to multiple containers. If the function returns non-NULL, a counter is
   incremented, else upon NULL returns the current count. Useful for error
   management on batch operations.
*/

#define CAG_DEF_RET_MANY_2_ARG(function, container, func)                     \
    CAG_DEC_RET_MANY_ARG(function, container)                                 \
    {                                                                         \
        int i = 0;                                                            \
        int num_args = 0;                                                     \
        unsigned good = 1;                                                    \
        container *t;                                                         \
        va_list ap;                                                           \
        va_start(ap, c);                                                      \
        while( (t = va_arg(ap, container *) ) ) {                             \
            if ( good ) {                                                     \
                if (!func(c, t))                                              \
                    good = 0;                                                 \
                else                                                          \
                    ++i;                                                      \
                ++num_args;                                                   \
            }                                                                 \
        }                                                                     \
        va_end(ap);                                                           \
        return i < num_args ? -i : i;                                         \
    }


/*! \brief Supporting private macros for CAG_SORT. */

#define CAG_P_INSERTION_SORT_LIMIT 32

/*! \brief Private algorithm to do insertion sort. */

#define CAG_P_INSERTION_SORT(from, to, i, j, p, value, prev, next, lteq,      \
                             cmp, val_adr)                                    \
do {                                                                          \
    for (i = next(from); lteq(i, to); i = next(i)) {                          \
        p = value(i);                                                         \
        j = i;                                                                \
        while (j != from && cmp(val_adr value(prev(j)),                       \
                                val_adr p) > 0) {                             \
            value(j) = value(prev(j));                                        \
            j = prev(j);                                                      \
        }                                                                     \
        value(j) = p;                                                         \
    }                                                                         \
} while (0)

/*! \brief Private algorithm to select pivot randomly in Quicksort. */

#define CAG_P_RANDOM_PIVOT(from, to, dist, at, value, pivot, swap, result)    \
    do {                                                                      \
        size_t i = CAG_RAND() % dist; /* better randomness unnecessary */     \
        pivot = at(from, i);                                                  \
        swap(from, pivot);                                                    \
        result = value(from);                                                 \
    } while(0)

/*! \brief Private algorithm to select median of 3 pivot in Quicksort. */

#define CAG_P_MEDIAN_3_SELECT(from, to, dist, at, value, median,              \
                              swap, cmp, val_adr, result)                     \
do {                                                                          \
    median = at(from, dist / 2);                                              \
    if (cmp(val_adr value(from), val_adr value(median)) > 0) {                \
        if (cmp(val_adr value(median),                                        \
                val_adr value(to)) > 0) {                                     \
            swap(from, median);                                               \
        } else if (cmp(val_adr value(from),                                   \
                       val_adr value(to)) > 0) {                              \
            swap(from, to);                                                   \
        }                                                                     \
    } else {                                                                  \
        if (cmp(val_adr value(to), val_adr value(median)) > 0) {              \
            swap(from, median);                                               \
        }  else if (cmp(val_adr value(to), val_adr value(from)) > 0) {        \
            swap(from, to);                                                   \
        }                                                                     \
    }                                                                         \
    result = value(from);                                                     \
} while(0)

/* \brief Private Lomuto partitioning algorithm that can be used by Quicksort. */

#define CAG_P_LOMUTO_PARTITION(from, to, i, j, p, next, value, lteq, swap,    \
                               cmp, val_adr)                                  \
do {                                                                          \
    j = from;                                                                 \
    for (i = next(from); lteq(i, to); i = next(i)) {                          \
        if (cmp(val_adr value(i), val_adr p) < 0) {                           \
            j = next(j);                                                      \
            swap(i, j);                                                       \
        }                                                                     \
    }                                                                         \
    swap(from, j);                                                            \
} while(0)


/* \brief Private Hoare partitioning algorithm that can be used by Quicksort. */

#define CAG_P_HOARE_PARTITION(from, to, i, j, p, prev, next,                  \
                              value, lt, swap, cmp, val_adr)                  \
do {                                                                          \
    i = from;                                                                 \
    j = next(to);                                                             \
    do {                                                                      \
        do {                                                                  \
            i = next(i);                                                      \
        } while(cmp(val_adr value(i), val_adr p) < 0 && i != to);             \
        do {                                                                  \
            j = prev(j);                                                      \
        } while(cmp(val_adr value(j), val_adr p) > 0);                        \
        swap(i, j);                                                           \
    } while(lt(i, j));                                                        \
    swap(i, j);                                                               \
    swap(from, j);                                                            \
} while(0)

/*! Very fast sorting algorithm for random access iterators. Also works on
   bidirectional iterators. Uses Quicksort with random pivot selection followed
   by median of 3 pivot selection, Hoare partitioning and Insertion sort for
   small sub arrays. A manually maintained stack is used instead of
   recursion. The average time efficiency class is O(n log n), including sorted,
   reverse-sorted and randomly generated sequences. There are theoretically some
   sequences which result in O(n^2) behaviour. I'm not sure if these can be
   generated using adversarial methods. The odds of such a sequence arising
   naturally are vanishingly small. Perhaps at some point this algorithm might
   be enhanced to use Introsort, but for now it seems unnecessary.
*/

#define CAG_SORT(iterator_type, type, cmp, val_adr, distance, prev,           \
                 next, at, lteq, lt, swap, value, from, to)                   \
do {                                                                          \
    int ip = 0;                                                               \
    iterator_type beg[sizeof(type *) * CHAR_BIT];                             \
    iterator_type end[sizeof(type *) * CHAR_BIT];                             \
    iterator_type i;                                                          \
    iterator_type j;                                                          \
    iterator_type m;                                                          \
    iterator_type _from;                                                      \
    iterator_type _to;                                                        \
    type p;                                                                   \
    beg[0] = (iterator_type) from;                                            \
    end[0] = (iterator_type) prev(to);                                        \
    while(ip >= 0) {                                                          \
        _from = beg[ip];                                                      \
        _to = end[ip];                                                        \
        if (lt(_from, _to)) {                                                 \
            size_t d = distance(_from, _to) + 1;                              \
            if (d < CAG_P_INSERTION_SORT_LIMIT) {                             \
                CAG_P_INSERTION_SORT(_from, _to, i, j, p, value, prev, next,  \
                                     lteq, cmp, val_adr);                     \
                --ip;                                                         \
            } else {                                                          \
                CAG_P_RANDOM_PIVOT(_from, _to, d, at, value, m, swap, p);     \
                CAG_P_MEDIAN_3_SELECT(_from, _to, d, at, value, m, swap,      \
                                      cmp, val_adr, p);                       \
                CAG_P_HOARE_PARTITION(_from, _to, i, j, p, prev, next, value, \
                                      lt, swap, cmp, val_adr);                \
                if (distance(_from,j) < distance(j,_to) + 1) {                \
                    beg[ip] = next(j);                                        \
                    end[ip++] = _to;                                          \
                    beg[ip] = _from;                                          \
                    end[ip] = prev(j);                                        \
                } else {                                                      \
                    beg[ip] =_from;                                           \
                    end[ip++] = prev(j);                                      \
                    beg[ip] = next(j);                                        \
                    end[ip] = _to;                                            \
                }                                                             \
            }                                                                 \
        } else {                                                              \
            --ip;                                                             \
        }                                                                     \
    }                                                                         \
} while(0)


/*! \brief Declaration and definition of very fast generic sort. */

#define CAG_DEC_SORT(function, iterator_type)                                 \
    iterator_type function(iterator_type from, iterator_type to)

#define CAG_DEF_SORT(function, iterator_type, type, cmp, val_adr,             \
                     distance, prev,                                          \
                     next, at, lteq, lt, swap, value)                         \
CAG_DEC_SORT(function, iterator_type)                                         \
{                                                                             \
    CAG_SORT( iterator_type, type, cmp, val_adr, distance, prev,              \
              next, at, lteq, lt, swap, value, from, to);                     \
    return from;                                                              \
}


/*! \brief Generic stable sort. Works on bidirectional iterators. O(n log n)
   efficiency.

   Implementation is a mergesort with O(n) extra space. Instead of recursion a
   manually managed stack is used. For single-linked lists a specialised
   mergesort has been implemented. The same should perhaps be done for
   double-linked lists.
*/

#define CAG_STABLE_SORT(from, to, container, iterator_type, new_container,    \
                        free_container, begin, end,                           \
                        next, at, distance, appendp, cmp, val_adr)            \
do {                                                                          \
    size_t n, j, k, s, e;                                                     \
    container w;                                                              \
    iterator_type *it, *it2;                                                  \
    new_container(&w);                                                        \
    for (it = from; it != to; it = next(it))                                  \
        appendp(&w, &it->value);                                              \
    n = distance(from, to);                                                   \
    for (k = 1; k < n; k *= 2 ) {                                             \
        for( j = 0; j < (n - k); j += 2 * k ) {                               \
            if (j + k > n)                                                    \
                s = n;                                                        \
            else                                                              \
                s = j + k;                                                    \
            if (s + k > n)                                                    \
                e = n - s;                                                    \
            else                                                              \
                e = k;                                                        \
            {                                                                 \
                iterator_type *a = at(from, j);                               \
                iterator_type *b = at(from, s);                               \
                iterator_type *c = at(begin(&w), j);                          \
                size_t m = k;                                                 \
                size_t n = e;                                                 \
                size_t i = 0, j = 0, k = 0;                                   \
                while (i < m && j < n) {                                      \
                    if( cmp(val_adr a->value, val_adr b->value)               \
                            <= 0) {                                           \
                        c->value = a->value;                                  \
                        a = next(a);                                          \
                        k++; i++;                                             \
                    }                                                         \
                    else                                                      \
                    {                                                         \
                        c->value = b->value;                                  \
                        b = next(b);                                          \
                        k++; j++;                                             \
                    }                                                         \
                    c = next(c);                                              \
                }                                                             \
                while ( i < m ) {                                             \
                    c->value = a->value;                                      \
                    c = next(c);                                              \
                    a = next(a);                                              \
                    k++; i++;                                                 \
                }                                                             \
                while ( j < n ) {                                             \
                    c->value = b->value;                                      \
                    c = next(c);                                              \
                    b = next(b);                                              \
                    k++; j++;                                                 \
                }                                                             \
            }                                                                 \
        }                                                                     \
        for (it = from, it2 = begin(&w);                                      \
                it != to; it = next(it), it2 = next(it2))                     \
            it->value = it2->value;                                           \
    }                                                                         \
    free_container(&w);                                                       \
} while(0)


/* \brief Declaration and definition of stable sort that performs a Mergesort
   with theta(n) extra space.
*/

#define CAG_DEC_STABLE_SORT(function, iterator_type)                          \
    iterator_type function(iterator_type from, iterator_type to)

#define CAG_DEF_STABLE_SORT(function, container, iterator_type, new_container,\
                            free_container, begin, end,                       \
                            next, distance, appendp, cmp, val_adr)            \
CAG_DEC_STABLE_SORT(function, iterator_type)                                  \
{                                                                             \
    container left, right;                                                    \
    iterator_type it = from;                                                  \
    size_t d, i;                                                              \
    if ( (d = distance(from, to)) > 1) {                                      \
        new_container(&left);                                                 \
        new_container(&right);                                                \
        for (i = 0; i < d / 2; ++i, it = next(it))                            \
            appendp(&left, &it->value);                                       \
        for (;it != to; it = next(it))                                        \
            appendp(&right, &it->value);                                      \
        function(begin(&left), end(&left));                                   \
        function(begin(&right), end(&right));                                 \
        {                                                                     \
            iterator_type from1 = begin(&left);                               \
            iterator_type to1 = end(&left);                                   \
            iterator_type from2 = begin(&right);                              \
            iterator_type to2 = end(&right);                                  \
            iterator_type result = from;                                      \
            while(from1 != to1 && from2 != to2) {                             \
                if (cmp(val_adr from1->value, val_adr from2->value)           \
                        <= 0) {                                               \
                    result->value = from1->value;                             \
                    from1 = next(from1);                                      \
                }                                                             \
                else {                                                        \
                    result->value = from2->value;                             \
                    from2 = next(from2);                                      \
                }                                                             \
                result = next(result);                                        \
            }                                                                 \
            if (from1 != to1)                                                 \
                for(;from1 != to1;                                            \
                        from1 = next(from1),                                  \
                        result = next(result))                                \
                    result->value = from1->value;                             \
            else                                                              \
                for(;from2 != to2;                                            \
                        from2 = next(from2),                                  \
                        result = next(result))                                \
                    result->value = from2->value;                             \
        }                                                                     \
        free_container(&left);                                                \
        free_container(&right);                                               \
    }                                                                         \
    return from;                                                              \
}

/*!\brief Pass an entire container to a function that operates on a sequence and
   returns a value. The applying function takes two parameters, the begin and
   end of the sequence.

   E.g. used to define *distance_all*.
*/

#define CAG_DEC_APPLY_CONTAINER(function, container, return_type)             \
    return_type function(container *c)

#define CAG_DEF_APPLY_CONTAINER(function, container, return_type,             \
                                apply_func, begin, end)                       \
CAG_DEC_APPLY_CONTAINER(function, container, return_type)                     \
{                                                                             \
    return apply_func(begin(c), end(c));                                      \
}

/*!\brief Pass an entire container to a function that operates on a sequence and
   returns a value. The applying function takes three parameters, the container
   variable, the begin and end of the sequence.

   E.g. used to define *erase_all*.
*/


#define CAG_DEF_APPLY_VAR_CONTAINER(function, container, return_type,         \
                                    apply_func, begin, end)                   \
CAG_DEC_APPLY_CONTAINER(function, container, return_type)                     \
{                                                                             \
    return apply_func(c, begin(c), end(c));                                   \
}

/*!\brief Pass an entire container to a function that operates on a sequence and
   returns a value. The applying function takes three parameters the begin and
   end of the sequence, and a variable of the same type as the elements of the
   container.

   E.g. used to define *linear_search_all*.
*/

#define CAG_DEC_APPLY_DATA_CONTAINER(function, container, return_type,        \
                                     type)                                    \
return_type function(container *c, type d)

#define CAG_DEF_APPLY_DATA_CONTAINER(function, container, return_type,        \
                                     type, apply_func, begin, end)            \
CAG_DEC_APPLY_DATA_CONTAINER(function, container, return_type, type)          \
{                                                                             \
    return apply_func(begin(c), end(c), d);                                   \
}


/*! \brief Pass an entire container to a function that operates on a sequence
   and an output container and returns a value. The applying function takes
   three parameters the begin and end of the first sequence, and an output
   container variable.

   E.g. used to define *copy_all*.
*/

#define CAG_DEC_APPLY_IN_OUT(function, container_in, container_out,           \
                             return_type)                                     \
return_type *function(const container_in *c1, container_out *c2)

#define CAG_DEF_APPLY_IN_OUT(function, container_in, container_out,           \
                             return_type, apply_func,                         \
                             begin_in, end_in, begin_out)                     \
CAG_DEC_APPLY_IN_OUT(function, container_in, container_out, return_type)      \
{                                                                             \
    return apply_func(begin_in(c1),                                           \
                      end_in(c1),                                             \
                      c2);                                                    \
}

/*! \brief Functional programming macros. Documented in user documentation.
 */

#define CAG_FOR_EACH(from, to, next, it, code)                                \
    do {                                                                      \
        for (it = (from); it != (to); it = next(it) ) {                       \
            code;                                                             \
        }                                                                     \
    } while(0)

#define CAG_FOR_ALL(container, var, it, code)                                 \
    do {                                                                      \
        for (it = beg_ ## container(var); it != end_ ## container(var);       \
                it = next_ ## container(it))                                  \
            code;                                                             \
    } while(0)


#define CAG_MAP(from1, to1, from2, next1, next2, it1, it2, code)              \
    do {                                                                      \
        for (it1 = (from1), it2 = (from2);                                    \
                it1 != (to1);                                                 \
                it1 = next1(it1), it2 = next2(it2)) {                         \
            code;                                                             \
        }                                                                     \
    } while(0)

#define CAG_MAP_ALL(container1, var1, container2, var2, it1, it2, code)       \
    CAG_MAP(begin_ ## container1(var1),                                       \
            end_ ## container1(var1),                                         \
            begin_ ## container2(var2),                                       \
            next_ ## container1,                                              \
            next_ ## container2,                                              \
            it1, it2, code);

#define CAG_MAP_SAME(container, var1, var2, it1, it2, code)                   \
    CAG_MAP_ALL(container, var1, container, var2, it1, it2, code)

#define CAG_FILTER(from1, to1, from2, next1, next2, it1, it2, expr)           \
    do {                                                                      \
        for (it1 = (from1), it2 = (from2);                                    \
                it1 != (to1);                                                 \
                it1 = next1(it1)) {                                           \
            if (expr) {                                                       \
                it2->value = it1->value;                                      \
                it2 = next2(it2);                                             \
            }                                                                 \
        }                                                                     \
    } while(0)

#define CAG_FILTER_ALL(container1, var1, container2, var2, it1, it2, expr)    \
    CAG_FILTER(begin_ ## container1(var1),                                    \
               end_ ## container1(var1),                                      \
               begin_ ## container2(var2),                                    \
               next_ ## container1,                                           \
               next_ ## container2,                                           \
               it1, it2, expr)

#define CAG_FILTER_SAME(container, var1, var2, it1, it2, expr)                \
    CAG_FILTER_ALL(container, var1, container, var2, it1, it2, expr)

#define CAG_FOLD(from, to, next, left_it, right_it, code, result)             \
    do {                                                                      \
        void *__cag__from = (from);                                           \
        void *__cag__to = (to);                                               \
        left_it = __cag__from;                                                \
        right_it = next(__cag__from);                                         \
        while (right_it != __cag__to) {                                       \
            code;                                                             \
            left_it = next(left_it);                                          \
            right_it = next(right_it);                                        \
        }                                                                     \
    } while(0)

#define CAG_FOLD_ALL(container, var, left_it, right_it, code, result)         \
    CAG_FOLD(begin_ ## container(var), end_ ## container(var),                \
             next_ ## container, left_it, right_it, code, result)

/*! \brief Macros to document container declarations.
 */

#define CAG_P_STRINGIFY_2(x) #x

#define CAG_P_STRINGIFY(x) CAG_P_STRINGIFY_2(x)

#define CAG_DOC(dec, container, type)                                         \
    {                                                                         \
        char l[10000] = CAG_P_STRINGIFY(dec(container, type));                \
        char *c = l;                                                          \
        while (*c != '\0') {                                                  \
            if (strncmp(c, "CAG_P_", 6) == 0)                                 \
                while (*c != ';' && *c != '\0') c++;                          \
            while(*c != ';' && *c != '\0') {                                  \
                printf("%c", *c);                                             \
                ++c;                                                          \
            }                                                                 \
            puts(";");                                                        \
            ++c;                                                              \
        }                                                                     \
    }


/* Not sure whether these will be kept. Not documented in version 0.1 */

#define CAG_FOR_ALL_2(container1, var1, container2, var2, it1, it2, code)     \
    do {                                                                      \
        for (it1 = beg_ ## container1(var1),                                  \
                it2 = beg_ ## container2(var2);                               \
                it1 != end_ ## container1(var1);                              \
                it1 = next_ ## container1(it1),                               \
                it2 = next_ ## container2(it2))                               \
            code;                                                             \
    } while(0)

#define CAG_FOR_SAME_2(container, var1, var2, it1, it2, code)                 \
    CAG_FOR_ALL_2(container, var1, container, var2, it1, it2, code)

#define CAG_FOR_ALL_3(container1, var1, container2, var2, container3, var3,   \
                      it1, it2, it3, code)                                    \
do {                                                                          \
    for (it1 = beg_ ## container1(var1),                                      \
            it2 = beg_ ## container2(var2);                                   \
            it3 = beg_ ## container3(var3);                                   \
            it1 != end_ ## container1(var1);                                  \
            it1 = next_ ## container1(it1),                                   \
            it2 = next_ ## container2(it2),                                   \
            it3 = next_ ## container3(it3))                                   \
        code;                                                                 \
} while(0)

#define CAG_FOR_SAME_3(container, var1, var2, var3, it1, it2, it3, code)      \
    CAG_FOR_ALL_3(container, var1, container, var2, container, var3,          \
                  it1, it2, it3, code)

/* \brief Private macros used to make a spurious typedef at the end of definitions
   so that CAG_DEF macros need a semi-colon after them when users invoke them.
*/

#define CAG_P_CMB2(x, y) cag_ ## x ## y
#define CAG_P_CMB(x, y) CAG_P_CMB2(x, y)

/* Prototypes */

size_t cag_kr_hash(const void *key, const size_t len);
size_t cag_oat_hash (const void *key, const size_t len );
size_t cag_int_hash(const int key, const size_t len);
char *cag_strdup(const char *s);
int cag_alloc_str_str(void *to, const void *from);
void cag_free_str_str(void *x);

#endif /* CAG_COMMON_H */
