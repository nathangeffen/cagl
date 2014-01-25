/*! \file CAGL array implementation.

  \copyright Copyright 2014 Nathan Geffen. All rights reserved.

  \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

  \sa COPYING for the license text.

  \sa howtodev.rst to learn how this code works and how to modify it.
*/


#ifndef CAG_ARRAY_H
#define CAG_ARRAY_H

#include <stdlib.h>
#include "cagl/concepts.h"

/*! \brief Default size of arrays, after which the size is redoubled
    whenever the array is full.
*/

#define CAG_QUANTUM_ARRAY 4

/*! \brief Private macro used to allocate space for array. */

#define CAG_P_MALLOC_ARRAY \
    array->begin = CAG_MALLOC( (array->capacity + 1) * \
                               sizeof(*array->objects))

/*! \brief Private macro used by new function definition. */

#define CAG_P_NEW_ARRAY \
    CAG_P_MALLOC_ARRAY; \
    if (array->begin) { \
        array->end = array->objects = array->begin + 1; \
        return array; \
    } else { \
        array->end = array->objects = array->begin = NULL; \
        return NULL; \
    }

/*! \brief Function declaration and definition to create a new array
   with a specified capacity.
*/

#define CAG_DEC_NEW_CAPACITY_ARRAY(function, container) \
    container *function(container *array, const size_t reserve)

#define CAG_DEF_NEW_CAPACITY_ARRAY(function, container) \
    CAG_DEC_NEW_CAPACITY_ARRAY(function, container) \
    { \
        if (reserve) \
            array->capacity = reserve; \
        else \
            array->capacity = CAG_QUANTUM_ARRAY; \
        CAG_P_NEW_ARRAY; \
    }


/*! \brief Function declaration and definition to create a new array
   with a specified capacity.
*/

#define CAG_DEC_NEW_ARRAY(function, container) \
    container *function(container *array)

#define CAG_DEF_NEW_ARRAY(function, container) \
    CAG_DEC_NEW_ARRAY(function, container) \
    { \
        array->capacity = CAG_QUANTUM_ARRAY; \
        CAG_P_NEW_ARRAY; \
    }

#define CAG_DEC_NEW_SIZE_ARRAY(function, container) \
    container *function(container *array, const size_t size)

#define CAG_DEF_NEW_SIZE_ARRAY(function, container) \
    CAG_DEC_NEW_SIZE_ARRAY(function, container) \
    { \
        array->capacity = CAG_QUANTUM_ARRAY < size \
                          ? size : CAG_QUANTUM_ARRAY; \
        CAG_P_MALLOC_ARRAY; \
        if (array) { \
            array->objects = array->begin + 1; \
            array->end = array->objects + size; \
            return array; \
        }  else return NULL; \
    }

/*! \brief Function declaration and definition to set the minimum size of the
   array. Useful for users who wish to use *copy_over* and want to ensure there
   is enough space in the copied over array.
*/

#define CAG_DEC_SET_MIN_SIZE_ARRAY(function, container, iterator_type) \
    iterator_type function(container *array, iterator_type it, \
                           const size_t size)

#define CAG_DEF_SET_MIN_SIZE_ARRAY(function, container, iterator_type) \
    CAG_DEC_SET_MIN_SIZE_ARRAY(function, container, iterator_type) \
    { \
        iterator_type it_new; \
        size_t size_right = array->end - it; \
        size_t size_total; \
        if (size_right > size) return it + size; \
        size_total = it - array->objects + size; \
        if (array->capacity >= size_total) { \
            array->end = it + size; \
            return it + size; \
        } \
        array->capacity = size_total; \
        it_new = realloc(array->begin, (array->capacity + 1) * \
                         sizeof ( *it ) ); \
        if (it_new) { \
            array->begin = it_new; \
            array->objects = array->begin + 1; \
            array->end = array->objects + size_total; \
            return array->end; \
        }  else return NULL; \
    }

/*! \brief Function declaration and definition to set the exact size of the
   array. Useful before calling *copy_over*.
*/

#define CAG_DEC_SET_EXACT_SIZE_ARRAY(function, container, iterator_type) \
    iterator_type function(container *array, const size_t size)

#define CAG_DEF_SET_EXACT_SIZE_ARRAY(function, container, iterator_type, \
                                     set_min_size, erase_range) \
CAG_DEC_SET_EXACT_SIZE_ARRAY(function, container, iterator_type) \
{ \
    iterator_type it; \
    if ( (it = set_min_size(array, CAG_P_BEG_ARRAY(*array), size))) { \
        if ( it != CAG_P_END_ARRAY(*array)) { \
            erase_range(array, it, CAG_P_END_ARRAY(*array)); \
        } \
    }  else return NULL; \
    return CAG_P_END_ARRAY(*array); \
}

/*! \brief Algorithm and function declaration and definition to calculate
   distance between two iterators in an array.
*/

#define CAG_DISTANCE_ARRAY(from, to) ((to) - (from))

#define CAG_DEC_DISTANCE_ARRAY(function, iterator_type) \
    size_t function(const iterator_type from, \
                    const iterator_type to)

#define CAG_DEF_DISTANCE_ARRAY(function, iterator_type) \
    CAG_DEC_DISTANCE_ARRAY(function, iterator_type) \
    { \
        return CAG_DISTANCE_ARRAY(from, to); \
    }

#define CAG_DEF_RDISTANCE_ARRAY(function, iterator_type) \
    CAG_DEC_DISTANCE_ARRAY(function, iterator_type) \
    { \
        return CAG_DISTANCE_ARRAY(to, from); \
    }

/*! \brief Algorithms, supporting macros and function declarations and
    definitions to move an iterator forward or backward one position.
*/

#define CAG_PREV_ARRAY(i) ((i) - 1)

#define CAG_NEXT_ARRAY(i) ((i) + 1)

#define CAG_DEC_STEP_ARRAY(function, iterator_type) \
    iterator_type function(iterator_type const it)

#define CAG_DEF_STEP_ARRAY(function, dir, iterator_type) \
    CAG_DEC_STEP_ARRAY(function, iterator_type) \
    { \
        return dir(it); \
    }

/*! \brief Algorithms and function declarations and definitions to retrieve an
    iterator a specified number of positions from the current iterator.
*/

#define CAG_AT_ARRAY(iterator, n)  ( (iterator) + (n) )

#define CAG_DEC_AT_ARRAY(function, container, iterator_type) \
    iterator_type function(iterator_type it, const size_t n)

#define CAG_DEF_AT_ARRAY(function, container, iterator_type, sign) \
    CAG_DEC_AT_ARRAY(function, container, iterator_type) \
    { \
        return it sign n; \
    }

/*! \brief Algorithms and function declarations and definitions to retrieve an
    iterator a specified number of positions from the current iterator.
*/

#define CAG_P_BEG_ARRAY(array) (array).objects

#define CAG_P_RBEG_ARRAY(array) (array).end - 1

#define CAG_P_END_ARRAY(array) (array).end

#define CAG_P_REND_ARRAY(array) (array).begin

/*! \brief Function declaration and definition for *begin* and *end*.*/

#define CAG_DEC_BEGIN_ARRAY(function, container, iterator_type) \
    iterator_type function(const container *array)

#define CAG_DEF_BEGIN_ARRAY(function, container, iterator_type, begin) \
    CAG_DEC_BEGIN_ARRAY(function, container, iterator_type) \
    { \
        return (iterator_type) begin(*array); \
    }

#define CAG_DEC_END_ARRAY(function, container, iterator_type) \
    iterator_type function(const container *array)

#define CAG_DEF_END_ARRAY(function, container, iterator_type, end, pos) \
    CAG_DEC_END_ARRAY(function, container, iterator_type) \
    { \
        return  (iterator_type) (CAG_END_ARRAY(*array) + pos); \
    }

/*! \brief Algorithms and function declarations and definitions to compare
    iterators.  These are used, for example, by the *sort* algorithm.
*/

#define CAG_LT_ARRAY(i, j) ( (i) < (j) )

#define CAG_DEC_LT_IT_ARRAY(function, iterator_type) \
    int function(const iterator_type it1, const iterator_type it2)

#define CAG_DEF_LT_IT_ARRAY(function, iterator_type) \
    CAG_DEC_LT_IT_ARRAY(function, iterator_type) \
    { \
        return CAG_LT_ARRAY(it1, it2); \
    }

#define CAG_DEC_RLT_IT_ARRAY(function, iterator_type) \
    CAG_DEC_LT_IT_ARRAY(function, iterator_type)

#define CAG_DEF_RLT_IT_ARRAY(function, iterator_type) \
    CAG_DEC_RLT_IT_ARRAY(function, iterator_type) \
    { \
        return CAG_LT_ARRAY(it2, it1); \
    }


#define CAG_LTEQ_ARRAY(i, j) ( (i) <= (j) )

#define CAG_DEC_LTEQ_IT_ARRAY(function, iterator_type) \
    CAG_DEC_LT_IT_ARRAY(function, iterator_type)

#define CAG_DEF_LTEQ_IT_ARRAY(function, iterator_type) \
    CAG_DEC_LTEQ_IT_ARRAY(function, iterator_type) \
    { \
        return CAG_LTEQ_ARRAY(it1, it2); \
    }


#define CAG_DEC_RLTEQ_IT_ARRAY(function, iterator_type) \
    CAG_DEC_LTEQ_IT_ARRAY(function, iterator_type)

#define CAG_DEF_RLTEQ_IT_ARRAY(function, iterator_type) \
    CAG_DEC_RLTEQ_IT_ARRAY(function, iterator_type) \
    { \
        return CAG_LTEQ_ARRAY(it2, it1); \
    }

/*! \brief Macros and function declarations and definitions to find the
    front and back elements of the array.
*/

#define CAG_BOUNDARY_ARRAY(array, pos, offset, value) \
    &( (array).pos + offset)->value

#define CAG_DEC_BOUNDARY_ARRAY(function, container, type) \
    type *function(const container *array)

#define CAG_DEF_BOUNDARY_ARRAY(function, container, type, pos, offset, value) \
    CAG_DEC_BOUNDARY_ARRAY(function, container, type) \
    { \
        return CAG_BOUNDARY_ARRAY(*array, pos, offset, value); \
    }

/*! \brief Algorithm to swap the values of two iterators. */

#define CAG_SWAP_ARRAY(i, j) do { \
        char t[sizeof(*i) / sizeof(char)]; \
        memcpy(t,i, sizeof(*i)); \
        *(i) = *(j); \
        memcpy(j, t, sizeof(*j)); \
    } while(0)


/*! \brief Used by the default sort to access the value of primitive
    elements.
*/

#define CAG_P_VALUE_ARRAY_PRIM(it) *it

/*! \brief Macro to sort the elements of any array.
*/

#define CAG_SORT_ARRAY_DEFAULT(type, from, to, cmp_func) \
    CAG_SORT(type *, \
             type, \
             cmp_func, \
             CAG_BYVAL, \
             CAG_DISTANCE_ARRAY, \
             CAG_PREV_ARRAY, \
             CAG_NEXT_ARRAY, \
             CAG_AT_ARRAY, \
             CAG_LTEQ_ARRAY, \
             CAG_LT_ARRAY, \
             CAG_SWAP_ARRAY, \
             CAG_P_VALUE_ARRAY_PRIM, \
             from, \
             to)

/*! \brief Algorithm and function declaration and definition to get the size of
    an array.

    This is calculated by distance, so it's not clear this adds any value to
    CAGL. Might remove.
*/

#define CAG_SIZE_ARRAY(array) (array).end - (array).objects

#define CAG_DEC_SIZE_ARRAY(function, container) \
    size_t function(const container *array)

#define CAG_DEF_SIZE_ARRAY(function, container) \
    CAG_DEC_SIZE_ARRAY(function, container) \
    { \
        return CAG_SIZE_ARRAY(*array); \
    }

/*! \brief Supporting macros and function declaration and definition to append
   to an element to the end of an array.
 */

/*! \brief Reallocate memory to array that has become full. */

#define CAG_P_REALLOC_ARRAY(array, iterator) \
    do { \
        (array).capacity *= 2; \
        iterator = realloc((array).begin, \
                           ((array).capacity + 1) * sizeof ( *(array).begin) ); \
    } while(0)

/*! \brief Reset the beginning and end of the array after it has changed size. */

#define CAG_P_SET_PTR_ARRAY(array, iterator_type, iterator) \
    do { \
        (array).begin = (iterator_type) iterator; \
        (array).objects = (array).begin + 1; \
        (array).end = (array).objects + size; \
    } while(0)


/*! \brief Allocate more space at end of array and reset the end pointer. */

#define CAG_P_APPEND_END_ARRAY(array, iterator, val, alloc_style, alloc_func) \
    do { \
        alloc_style((array).end->value, val, alloc_func, {return NULL;}); \
        iterator = (void *) (array).end; \
        ++(array).end; \
    } while (0)


/*! \brief Function declaration, definition and supporting macros to append
   element to array. By value and address versions for element supplied.
*/

#define CAG_P_APPEND_ARRAY(array, iterator_type, iterator, val, \
                           alloc_style, alloc_func) \
do { \
    size_t size = (array).end - (array).objects; \
    if ( size >= (array).capacity) { \
        CAG_P_REALLOC_ARRAY(array, iterator); \
        if (iterator) { \
            CAG_P_SET_PTR_ARRAY(array, iterator_type, iterator); \
            CAG_P_APPEND_END_ARRAY( (array) , iterator, val, alloc_style, \
                                    alloc_func); \
        }  else { \
            iterator = NULL; \
        } \
    } else { \
        CAG_P_APPEND_END_ARRAY(array, iterator, val, alloc_style, \
                               alloc_func); \
    } \
} while(0)

#define CAG_DEC_APPEND_ARRAY(function, container, iterator_type, type) \
    iterator_type function(container *array, type const element)

#define CAG_DEF_APPEND_ARRAY(function, container, iterator_type, \
                             type, alloc_style, alloc_func) \
CAG_DEC_APPEND_ARRAY(function, container, iterator_type, type) \
{ \
    iterator_type it; \
    CAG_P_APPEND_ARRAY(*array, it_ ## container, it, element, alloc_style, \
                       alloc_func); \
    return it; \
}

#define CAG_DEC_APPENDP_ARRAY(function, container, iterator_type, \
                              type) \
iterator_type function(container *array, type const *element)

#define CAG_DEF_APPENDP_ARRAY(function, container, iterator_type, \
                              type, alloc_style, alloc_func) \
CAG_DEC_APPENDP_ARRAY(function, container, iterator_type, type) \
{ \
    iterator_type it; \
    CAG_P_APPEND_ARRAY(*array, it_ ## container, it, *element, \
                       alloc_style, alloc_func); \
    return it; \
}


/*! \brief Function declaration, definition and supporting macros to insert
   element into array. By value and address versions for element supplied. Also
   insert in specified order versions supplied, which iterate through a sorted
   array until the place for the element to insert is found.
*/

/*! \brief Analagous to CAG_P_APPEND_END_ARRAY. */

#define CAG_P_INSERT_POS_ARRAY(array, position, item, alloc_style, alloc_func) \
    do { \
        if (d) \
            memmove( (position + 1), position, d * sizeof(item)); \
        alloc_style(position->value, item, alloc_func, \
                    { \
                            memmove( position, (position + 1), d * sizeof(item)); \
                            return NULL; \
                    }); \
        ++(array).end; \
    } while(0)

/*! \brief Analagous to CAG_P_APPEND_ARRAY. */

#define CAG_P_INSERT_ARRAY(array, iterator_type, iterator, position, \
                           item, alloc_style, alloc_func) \
do { \
    size_t size = (array).end - (array).objects; \
    size_t d = (array).end - position; \
    if ( size >= (array).capacity) { \
        CAG_P_REALLOC_ARRAY(array, iterator); \
        if (iterator) { \
            CAG_P_SET_PTR_ARRAY(array, iterator_type, iterator); \
            position = (array).end - d; \
            CAG_P_INSERT_POS_ARRAY(array, position, item, alloc_style, \
                                   alloc_func); \
        } else { \
            position = NULL; \
        } \
    } else { \
        CAG_P_INSERT_POS_ARRAY(array, position, item, alloc_style, \
                               alloc_func); \
    } \
} while(0)

#define CAG_DEC_INSERT_ARRAY(function, container, iterator_type, type) \
    iterator_type function(container *array, iterator_type position, \
                           type const element)

#define CAG_DEF_INSERT_ARRAY(function, container, iterator_type, \
                             type, alloc_style, alloc_func) \
CAG_DEC_INSERT_ARRAY(function, container, iterator_type, type) \
{ \
    iterator_type it; \
    CAG_P_INSERT_ARRAY(*array, iterator_type, it, position, \
                       element, alloc_style, alloc_func); \
    return position; \
}

#define CAG_DEC_INSERTP_ARRAY(function, container, iterator_type, type) \
    iterator_type function(container *array, iterator_type position, \
                           type const *element)

#define CAG_DEF_INSERTP_ARRAY(function, container, iterator_type, \
                              type, alloc_style, alloc_func) \
CAG_DEC_INSERTP_ARRAY(function, container, iterator_type, type) \
{ \
    iterator_type it; \
    CAG_P_INSERT_ARRAY(*array, iterator_type, it, position, \
                       *element, alloc_style, alloc_func); \
    return position; \
}

/*! \brief Function declaration and definition to prepend an element to an array
   which is a matter of calling the insert function with the begin position.
   By value and address versions provided.
*/

#define CAG_DEC_PREPEND_ARRAY(function, container, iterator_type, type) \
    iterator_type function(container *array, type const element)

#define CAG_DEF_PREPEND_ARRAY(function, container, iterator_type, type, \
                              begin, insert) \
CAG_DEC_PREPEND_ARRAY(function, container, iterator_type, type) \
{ \
    return insert(array, begin(array),element); \
}

#define CAG_DEC_PREPENDP_ARRAY(function, container, iterator_type, type) \
    iterator_type function(container *array, type const *element)

#define CAG_DEF_PREPENDP_ARRAY(function, container, iterator_type, type, \
                               begin, insert) \
CAG_DEC_PREPENDP_ARRAY(function, container, iterator_type, type) \
{ \
    return insert(array, begin(array), element); \
}

/*! \brief Function declaration and definition to erase an element. */

#define CAG_DEC_ERASE_ARRAY(function, container, iterator_type) \
    iterator_type function(container *array, iterator_type it)

#define CAG_DEF_ERASE_ARRAY(function, container, iterator_type, \
                            free_func, val_adr) \
CAG_DEC_ERASE_ARRAY(function, container, iterator_type) \
{ \
    free_func(val_adr it->value); \
    memmove(it, it + 1, (array->end - it) * sizeof(*it)); \
    --array->end; \
    return it; \
}

/*! \brief The *erase_range* implementation uses the generic one in cag/common.h.
   However, the generic version does not handle the unique requirement of arrays
   that the pointer to the *to* position be decremented. This is done by this
   hook.
*/

#define CAG_P_POST_ERASE_ARRAY(container, from, to) --to

#define CAG_DEC_ERASE_RANGE_ARRAY(function, container, iterator_type) \
    iterator_type function(container *array, \
                           iterator_type from, iterator_type to)

#define CAG_DEF_ERASE_RANGE_ARRAY(function, container, iterator_type) \
    CAG_DEC_ERASE_RANGE_ARRAY(function, container, iterator_type) \
    { \
        while (to != from) \
            erase_ ## container(array, to--); \
        return to; \
    }

/*! \brief Function declaration and definition to return array memory to heap. */

#define CAG_DEC_FREE_ARRAY(function, container) \
    void function(container *array)

#define CAG_DEF_FREE_ARRAY(function, container, begin, free_func, val_adr) \
    CAG_DEC_FREE_ARRAY(function, container) \
    { \
        it_ ## container it; \
        for (it = begin_ ## container(array); \
                it != end_ ## container(array); \
                ++it) \
            free_func(val_adr it->value); \
        CAG_FREE(array->begin); \
    }

/*! \brief Declaration of array functions and data structures. */

#define CAG_DEC_ARRAY(container, type) \
    struct iterator_ ## container { \
        type value; \
    }; \
    struct reverse_iterator_ ## container { \
        type value; \
    }; \
    typedef struct iterator_ ## container iterator_ ## container; \
    typedef struct reverse_iterator_ ## container \
            reverse_iterator_ ## container; \
    typedef iterator_ ## container * it_ ## container; \
    typedef reverse_iterator_ ## container * rit_ ## container; \
    struct container { \
        iterator_ ## container *begin; \
        iterator_ ## container *end; \
        iterator_ ## container *objects; \
        size_t capacity; \
        CAG_DEF_INITIALIZE \
    }; \
    typedef struct container container; \
    CAG_DEC_NEW_ARRAY(new_ ## container, container); \
    CAG_DEC_NEW_CAPACITY_ARRAY(new_with_capacity_ ## container, container); \
    CAG_DEC_NEW_SIZE_ARRAY(new_with_size_ ## container, container); \
    CAG_DEC_SET_MIN_SIZE_ARRAY(set_min_size_ ## container, container, \
                               it_ ## container); \
    CAG_DEC_SET_EXACT_SIZE_ARRAY(set_exact_size_ ## container, container, \
                                 it_ ## container); \
    CAG_DEC_STEP_ARRAY(next_ ## container,  it_ ## container); \
    CAG_DEC_STEP_ARRAY(prev_ ## container, it_ ## container); \
    CAG_DEC_STEP_ARRAY(rnext_ ## container, rit_ ## container); \
    CAG_DEC_STEP_ARRAY(rprev_ ## container, rit_ ## container); \
    CAG_DEC_AT_ARRAY(at_ ## container, container, it_ ## container); \
    CAG_DEC_AT_ARRAY(rat_ ## container, container, rit_ ## container); \
    CAG_DEC_DISTANCE_ARRAY(distance_ ## container, it_ ## container); \
    CAG_DEC_DISTANCE_ARRAY(rdistance_ ## container, rit_ ## container); \
    CAG_DEC_LT_IT_ARRAY(lt_it_ ## container, it_ ## container); \
    CAG_DEC_LTEQ_IT_ARRAY(lteq_it_ ## container, it_ ## container); \
    CAG_DEC_RLT_IT_ARRAY(rlt_it_ ## container, rit_ ## container); \
    CAG_DEC_RLTEQ_IT_ARRAY(rlteq_it_ ## container, rit_ ## container); \
    CAG_DEC_BEGIN_ARRAY(begin_ ## container, container, it_ ## container); \
    CAG_DEC_END_ARRAY(end_ ## container, container, it_ ## container); \
    CAG_DEC_BEGIN_ARRAY(rbegin_ ## container, container, rit_ ## container); \
    CAG_DEC_END_ARRAY(rend_ ## container, container, rit_ ## container); \
    CAG_DEC_SIZE_ARRAY(size_ ## container, container); \
    CAG_DEC_PREPEND_ARRAY(prepend_ ## container, container, it_ ## container, \
                          type); \
    CAG_DEC_APPEND_ARRAY(append_ ## container, container, \
                         it_ ## container, type); \
    CAG_DEC_PREPENDP_ARRAY(prependp_ ## container, container, it_ ## container, \
                           type); \
    CAG_DEC_APPEND_ARRAY(rprepend_ ## container, container, \
                         rit_ ## container, type); \
    CAG_DEC_APPENDP_ARRAY(appendp_ ## container, container, \
                          it_ ## container, type); \
    CAG_DEC_APPENDP_ARRAY(rprependp_ ## container, container, \
                          rit_ ## container, type); \
    CAG_DEC_INSERT_ARRAY(insert_ ## container, container, it_ ## container, \
                         type); \
    CAG_DEC_INSERT_ARRAY(put_ ## container, container, it_ ## container, \
                         type); \
    CAG_DEC_INSERTP_ARRAY(insertp_ ## container, container, it_ ## container, \
                          type); \
    CAG_DEC_INSERTP_ARRAY(putp_ ## container, container, it_ ## container, \
                          type); \
    CAG_DEC_INSERT_ARRAY(rinsert_ ## container, container, rit_ ## container, \
                         type); \
    CAG_DEC_PREPEND_ARRAY(rappend_ ## container, container, rit_ ## container, \
                          type); \
    CAG_DEC_BOUNDARY_ARRAY(front_ ## container, container, type); \
    CAG_DEC_BOUNDARY_ARRAY(back_ ## container, container, type); \
    CAG_DEC_BOUNDARY_ARRAY(rfront_ ## container, container, type); \
    CAG_DEC_BOUNDARY_ARRAY(rback_ ## container, container, type); \
    CAG_DEC_ERASE_ARRAY(erase_ ## container, container, it_ ## container); \
    CAG_DEC_ERASE_RANGE_ARRAY(erase_range_ ## container, \
                              container, it_ ## container); \
    CAG_DEC_FREE_ARRAY(free_ ## container, container); \
    CAG_DEC_REORDERABLE(container, type); \
    CAG_DEC_RANDOMACCESS(container, type) \
     

/*! \brief Definitions of array functions. */

#define CAG_DEF_ALL_ARRAY(container, type, alloc_style, alloc_func, \
                          free_func, val_adr) \
CAG_DEF_NEW_ARRAY(new_ ## container, container) \
CAG_DEF_NEW_CAPACITY_ARRAY(new_with_capacity_ ## container, container) \
CAG_DEF_NEW_SIZE_ARRAY(new_with_size_ ## container, container) \
CAG_DEF_SET_MIN_SIZE_ARRAY(set_min_size_ ## container, container, \
                           it_ ## container) \
CAG_DEF_SET_EXACT_SIZE_ARRAY(set_exact_size_ ## container, container, \
                             it_ ## container, \
                             set_min_size_ ## container, \
                             erase_range_ ## container) \
CAG_DEF_STEP_ARRAY(next_ ## container, CAG_NEXT_ARRAY, it_ ## container) \
CAG_DEF_STEP_ARRAY(prev_ ## container, CAG_PREV_ARRAY, it_ ## container) \
CAG_DEF_STEP_ARRAY(rnext_ ## container, CAG_PREV_ARRAY, rit_ ## container) \
CAG_DEF_STEP_ARRAY(rprev_ ## container, CAG_NEXT_ARRAY, rit_ ## container) \
CAG_DEF_AT_ARRAY(at_ ## container, container, it_ ## container, +) \
CAG_DEF_AT_ARRAY(rat_ ## container, container, rit_ ## container, -) \
CAG_DEF_DISTANCE_ARRAY(distance_ ## container, it_ ## container) \
CAG_DEF_RDISTANCE_ARRAY(rdistance_ ## container, rit_ ## container) \
CAG_DEF_LT_IT_ARRAY(lt_it_ ## container, it_ ## container) \
CAG_DEF_LTEQ_IT_ARRAY(lteq_it_ ## container, it_ ## container) \
CAG_DEF_RLT_IT_ARRAY(rlt_it_ ## container, rit_ ## container) \
CAG_DEF_RLTEQ_IT_ARRAY(rlteq_it_ ## container, rit_ ## container) \
CAG_DEF_BEGIN_ARRAY(begin_ ## container, container, it_ ## container, \
                    CAG_P_BEG_ARRAY) \
CAG_DEF_BEGIN_ARRAY(end_ ## container, container, it_ ## container, \
                    CAG_P_END_ARRAY) \
CAG_DEF_BEGIN_ARRAY(rbegin_ ## container, container, rit_ ## container, \
                    CAG_P_RBEG_ARRAY) \
CAG_DEF_BEGIN_ARRAY(rend_ ## container, container, rit_ ## container, \
                    CAG_P_REND_ARRAY) \
CAG_DEF_SIZE_ARRAY(size_ ## container, container) \
CAG_DEF_APPEND_ARRAY(append_ ## container, container, \
                     it_ ## container, type, alloc_style, alloc_func) \
CAG_DEF_APPEND_ARRAY(rprepend_ ## container, container, \
                     rit_ ## container, type, alloc_style, alloc_func) \
CAG_DEF_APPENDP_ARRAY(appendp_ ## container, container, \
                      it_ ## container, type, alloc_style, alloc_func) \
CAG_DEF_APPENDP_ARRAY(rprependp_ ## container, container, \
                      rit_ ## container, type, alloc_style, alloc_func) \
CAG_DEF_INSERT_ARRAY(insert_ ## container, container, it_ ## container, \
                     type, alloc_style, alloc_func) \
CAG_DEF_INSERT_ARRAY(put_ ## container, container, it_ ## container, \
                     type, alloc_style, alloc_func) \
CAG_DEF_INSERTP_ARRAY(insertp_ ## container, container, it_ ## container, \
                      type, alloc_style, alloc_func) \
CAG_DEF_INSERTP_ARRAY(putp_ ## container, container, it_ ## container, \
                      type, alloc_style, alloc_func) \
CAG_DEF_PREPEND_ARRAY(prepend_ ## container, container, it_ ## container, \
                      type, begin_ ## container, insert_ ## container) \
CAG_DEF_PREPENDP_ARRAY(prependp_ ## container, container, it_ ## container, \
                       type, begin_ ## container, insertp_ ## container) \
CAG_DEF_BOUNDARY_ARRAY(front_ ## container, container, type, \
                       begin, 1, value) \
CAG_DEF_BOUNDARY_ARRAY(back_ ## container, container, type, \
                       end, -1, value) \
CAG_DEF_BOUNDARY_ARRAY(rfront_ ## container, container, type, \
                       end, -1, value) \
CAG_DEF_BOUNDARY_ARRAY(rback_ ## container, container, type, \
                       begin, 1, value) \
CAG_DEF_ERASE_ARRAY(erase_ ## container, container, \
                    it_ ## container, free_func, val_adr) \
CAG_DEF_ERASE_RANGE_ARRAY(erase_range_ ## container, container, \
                          it_ ## container) \
CAG_DEF_FREE_ARRAY(free_ ## container, container, begin, \
                   free_func, val_adr) \
CAG_DEF_REORDERABLE(container, type) \
CAG_DEF_RANDOMACCESS(container, type) \
typedef container CAG_P_CMB(container,  __LINE__)


/*! \brief Used to declare and define an array in one macro.  Useful usually for
   small programs where the container will only be used in one module.
*/

#define CAG_DEC_DEF_ARRAY(container, type) \
    CAG_DEC_ARRAY(container, type); \
    CAG_DEF_ARRAY(container, type)

#define CAG_DEC_DEF_ALL_ARRAY(container, type, alloc_style, alloc_func, \
                              free_func, val_adr) \
CAG_DEC_ARRAY(container, type); \
CAG_DEF_ALL_ARRAY(container, type, alloc_style, alloc_func, \
                  free_func, val_adr)

/*! \brief Declaration of functions for arrays that remain sorted and have a
    comparison function.
*/

#define CAG_DEC_CMP_ARRAY(container, type) \
    CAG_DEC_ARRAY(container, type); \
    CAG_DEC_STABLE_SORT(stable_sort_ ## container, \
                        it_ ## container); \
    CAG_DEC_STABLE_SORT(rstable_sort_ ## container, \
                        rit_ ## container); \
    CAG_DEC_CMP_REORDERABLE(container, type); \
    CAG_DEC_CMP_RANDOMACCESS(container, type)

/*! \brief Identical to CAG_DEC_CMP_ARRAY but provided for users who
    want consistent names.
*/

#define CAG_DEC_CMPP_ARRAY CAG_DEC_CMP_ARRAY

/* \brief Define functions for an array that must remain ordered and has a
   comparison function.
*/

#define CAG_DEF_ALL_CMP_ARRAY(container, type, cmp_func, \
                              val_adr, alloc_style, alloc_func, \
                              free_func) \
CAG_DEF_ALL_ARRAY(container, type, \
                  alloc_style, alloc_func, free_func, val_adr); \
CAG_DEF_STABLE_SORT(stable_sort_ ## container, \
                    container, it_ ## container, \
                    new_ ## container, free_ ## container, \
                    begin_ ## container, end_ ## container, \
                    next_ ## container, distance_ ## container, \
                    appendp_ ## container, cmp_func, val_adr) \
CAG_DEF_STABLE_SORT(rstable_sort_ ## container, \
                    container, rit_ ## container, \
                    new_ ## container, free_ ## container, \
                    rbegin_ ## container, rend_ ## container, \
                    rnext_ ## container, rdistance_ ## container, \
                    prependp_ ## container, cmp_func, val_adr) \
CAG_DEF_CMP_REORDERABLE(container, type, cmp_func, val_adr) \
CAG_DEF_CMP_RANDOMACCESS(container, type, cmp_func, val_adr) \
typedef container CAG_P_CMB(container ## _cmp,  __LINE__)

/*! \brief Definition of most common case array functions.
   This is an array that does not manage the memory of its elements and
   is not ordered.
*/

#define CAG_DEF_ARRAY(container, type) \
    CAG_DEF_ALL_ARRAY(container, type, \
                      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                      CAG_NO_FREE_FUNC, CAG_BYVAL)

/*! \brief Definition of most common case functions for an ordered array.
   This is an array that does not manage the memory of its elements.
*/

#define CAG_DEF_CMP_ARRAY(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_ARRAY(container, type, cmp_func, CAG_BYVAL, \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                          CAG_NO_FREE_FUNC)

/*! \brief Same as CAG_DEF_CMP_ARRAY but cmp_fun takes parameters by address. */

#define CAG_DEF_CMPP_ARRAY(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_ARRAY(container, type, cmp_func, CAG_BYADR, \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                          CAG_NO_FREE_FUNC)

/*! \brief Analogous to CAG_DEC_DEF_ARRAY but for ordered arrays. */

#define CAG_DEC_DEF_CMP_ARRAY(container, type, cmp_func) \
    CAG_DEC_CMP_ARRAY(container, type); \
    CAG_DEF_CMP_ARRAY(container, type, cmp_func)

/*! \brief Same as CAG_DEC_DEF_CMP_ARRAY but cmp_func takes its parameters
    by address.
*/

#define CAG_DEC_DEF_CMPP_ARRAY(container, type, cmp_func) \
    CAG_DEC_CMPP_ARRAY(container, type); \
    CAG_DEF_CMPP_ARRAY(container, type, cmp_func)

/*! \brief Analogous to CAG_DEC_DEF_ALL_ARRAY but for ordered arrays. */


#define CAG_DEC_DEF_ALL_CMP_ARRAY(container, type, cmp_func, \
                                  val_adr, alloc_style, alloc_func, \
                                  free_func) \
CAG_DEC_CMP_ARRAY(container, type); \
CAG_DEF_ALL_CMP_ARRAY(container, type, cmp_func, \
                      val_adr, alloc_style, alloc_func, \
                      free_func)

#endif /* CAG_ARRAY_H */
