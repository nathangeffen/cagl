/*! \file CAGL doubly-linked list implementation.

  \copyright Copyright 2014 Nathan Geffen. All rights reserved.

  \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

  \sa COPYING for the license text.

  \sa howtodev.rst to learn how this code works and how to modify it.

  The doubly linked list is implemented conventionally, but for one detail.  The
  container has two variables, begin_ and end_, which are nodes one before and
  after the list respectively. These are used for implementing the *begin* and
  *end* functions and supporting macros. Cf. the different approach used for the
  balanced tree. In time, perhaps both implementations should be made
  consistent, though these details are irrelevant to users.

*/

#ifndef CAG_DLIST_H
#define CAG_DLIST_H

#include <stdlib.h>
#include "cagl/concepts.h"

/*! \brief Macro and function declaration and definition to initialize
    doubly linked list.
*/

#define CAG_DEC_NEW_DLIST(function, container) \
    container *function(container *dlist)

#define CAG_DEF_NEW_DLIST(function, container) \
    CAG_DEC_NEW_DLIST(function, container) \
    { \
        dlist->end_ = NULL;  /* Prevents uninitialized warning. */ \
        dlist->begin_ = malloc(sizeof(*dlist->begin_)); \
        if (!dlist->begin_) return NULL; \
        dlist->end_ = malloc(sizeof(*dlist->end_)); \
        if (!dlist->end_) { \
            free(dlist->begin_); \
            return NULL; \
        } \
        dlist->begin_->next = dlist->end_; \
        dlist->begin_->prev = NULL; \
        dlist->end_->prev = dlist->begin_; \
        dlist->end_->next = NULL; \
        return dlist; \
    }


/*! \brief Experimental *new* modelled on C++11 move semantics. Not ready
    for implementation yet.
*/

#define CAG_DEC_NEW_MOVE(function, container) \
    container *function(container *to, container *from)

#define CAG_DEF_NEW_MOVE(function, container) \
    CAG_DEC_NEW_MOVE(function, container) \
    { \
        if (new_ ## container(to) == NULL) return NULL; \
        to->begin_->next = from->begin_->next; \
        to->begin_->next->prev = to->begin_; \
        to->end_->prev = from->end_->prev; \
        to->end_->prev->next  = to->end_; \
        from->begin_->next = from->end_; \
        from->begin_->prev = NULL; \
        from->end_->prev = from->begin_; \
        from->end_->next = NULL; \
        return to; \
    }

/*! \brief Macros and function declaration and definition to iterate one
    element forward or backward  in the list.
*/

#define CAG_NEXT_DLIST(iterator) (iterator)->next

#define CAG_PREV_DLIST(iterator) (iterator)->prev

#define CAG_DEC_STEP_DLIST(function, dir, iterator_type) \
    iterator_type function(iterator_type const it)

#define CAG_DEF_STEP_DLIST(function, dir, iterator_type) \
    CAG_DEC_STEP_DLIST(function, dir, iterator_type) \
    { \
        return it->dir; \
    }

/*! \brief Function declaration and definition to set the minimum size of the
   list. Useful for users who wish to use *copy_over* and want to ensure there
   is enough space in the copied over list.
*/

#define CAG_DEC_SET_MIN_SIZE_DLIST(function, container, iterator_type) \
    iterator_type function(container *dlist, size_t size)

#define CAG_DEF_SET_MIN_SIZE_DLIST(function, container, iterator_type) \
    CAG_DEC_SET_MIN_SIZE_DLIST(function, container, iterator_type) \
    { \
        size_t i = 0; \
        iterator_type it = beg_ ## container(dlist);	 \
        while (it != CAG_END_DLIST(dlist) && i < size) { \
            it = it->next; \
            ++i; \
        } \
        while (i < size) { \
            it = CAG_MALLOC(sizeof(*it)); \
            if (!it) return NULL; \
            it->prev = dlist->end_->prev; \
            it->prev->next = it; \
            it->next = dlist->end_; \
            dlist->end_->prev = it; \
            ++i; \
        } \
        return it; \
    }

/*! \brief Function declaration and definition to set the exact size of the
   list. Useful before calling *copy_over*.
*/

#define CAG_DEC_SET_EXACT_SIZE_DLIST(function, container, iterator_type) \
    iterator_type function(container *dlist, const size_t size)

#define CAG_DEF_SET_EXACT_SIZE_DLIST(function, container, iterator_type, \
                                     set_min_size, erase_range) \
CAG_DEC_SET_EXACT_SIZE_DLIST(function, container, iterator_type) \
{ \
    iterator_type it; \
    it = set_min_size(dlist, size); \
    if (it) { \
        if (it->next) \
            erase_range(dlist, it->next, CAG_END_DLIST(dlist)); \
    } else { \
        return NULL; \
    } \
    return CAG_END_DLIST(dlist)->prev; \
}

/*! \brief Algorithm and function declaration and definition to calculate
   distance between two iterators in a list.
*/

#define CAG_DISTANCE_DLIST(iterator_from, iterator_to, next, count) \
    do { \
        count = 0; \
        while (iterator_from != iterator_to) { \
            ++count; \
            iterator_from = iterator_from->next; \
        } \
    } while(0)

#define CAG_DEC_DISTANCE_DLIST(function, iterator_type) \
    size_t function(iterator_type from, \
                    const iterator_type to)

#define CAG_DEF_DISTANCE_DLIST(function, iterator_type) \
    CAG_DEC_DISTANCE_DLIST(function, iterator_type) \
    { \
        size_t count; \
        CAG_DISTANCE_DLIST(from, to, \
                           next, count); \
        return count; \
    }

/*! \brief Algorithms and function declarations and definitions to compare
    iterators.  These are used, for example, by the *sort* algorithm.
*/

#define CAG_LTEQ_IT_DLIST(it1, it2, next, end, result) \
    do { \
        while (it1 != it2 && it1 != end) \
            it1 = next(it1); \
        if (it1 == it2) \
            result = 1; \
        else \
            result = 0; \
    } while(0)

#define CAG_LT_IT_DLIST(it1, it2, next, end, result) \
    do { \
        if (it1 == it2) \
            result = 0; \
        else \
            CAG_LTEQ_IT_DLIST(it1, it2, \
                              next, end, result); \
    } while(0)


#define CAG_DEC_LT_IT_DLIST(function, iterator_type) \
    int function(iterator_type it1, const iterator_type it2)

#define CAG_DEF_LT_IT_DLIST(function, iterator_type, next) \
    CAG_DEC_LT_IT_DLIST(function, iterator_type) \
    { \
        int result; \
        CAG_LT_IT_DLIST(it1, it2, next, NULL, result); \
        return result; \
    }

#define CAG_DEC_LTEQ_IT_DLIST(function, iterator_type) \
    int function(iterator_type it1, const iterator_type it2)

#define CAG_DEF_LTEQ_IT_DLIST(function, iterator_type, next) \
    CAG_DEC_LTEQ_IT_DLIST(function, iterator_type) \
    { \
        int result; \
        CAG_LTEQ_IT_DLIST(it1, it2, next, NULL, result); \
        return result; \
    }

/*! \brief Macros, function declaration and definition for *begin* and
    *end*.
*/

#define CAG_BEGIN_DLIST(dlist) (dlist)->begin_->next

#define CAG_RBEGIN_DLIST(dlist) (dlist)->end_->prev

#define CAG_END_DLIST(dlist) (dlist)->end_

#define CAG_REND_DLIST(dlist) dlist->begin_

#define CAG_DEC_BEGIN_DLIST(function, container) \
    it_ ## container function(const container *dlist)

#define CAG_DEF_BEGIN_DLIST(function, container) \
    CAG_DEC_BEGIN_DLIST(function, container) \
    { \
        return CAG_BEGIN_DLIST(dlist); \
    }

#define CAG_DEC_RBEGIN_DLIST(function, container) \
    rit_ ## container function(const container *dlist)

#define CAG_DEF_RBEGIN_DLIST(function, container) \
    CAG_DEC_RBEGIN_DLIST(function, container) \
    { \
        return (rit_ ## container) CAG_RBEGIN_DLIST(dlist); \
    }

#define CAG_DEC_END_DLIST(function, container) \
    it_ ## container function(const container *dlist)

#define CAG_DEF_END_DLIST(function, container) \
    CAG_DEC_END_DLIST(function, container) \
    { \
        return CAG_END_DLIST(dlist); \
    }

#define CAG_DEC_REND_DLIST(function, container) \
    rit_ ## container function(const container *dlist)

#define CAG_DEF_REND_DLIST(function, container) \
    CAG_DEC_REND_DLIST(function, container) \
    { \
        return (rit_ ## container) CAG_REND_DLIST(dlist); \
    }

/*! \brief Function declarations and definitions to find the front and back
    elements of the list.
*/

#define CAG_DEC_FRONT_DLIST(function, container, type) \
    type *function(const container *dlist)

#define CAG_DEF_FRONT_DLIST(function, container, type) \
    CAG_DEC_FRONT_DLIST(function, container, type) \
    { \
        return &dlist->begin_->next->value; \
    }

#define CAG_DEC_BACK_DLIST(function, container, type) \
    type *function(const container *dlist)

#define CAG_DEF_BACK_DLIST(function, container, type) \
    CAG_DEC_BACK_DLIST(function, container, type) \
    { \
        return &dlist->end_->prev->value; \
    }


/*! \brief Function declaration, definition and supporting macros to insert
   element into list. By value and address versions for element supplied. Also
   insert in specified order versions supplied, which iterate through a sorted
   list until the place for the element to insert is found.
*/

#define CAG_P_INSERT_DLIST(iterator, node, prev, next, item, \
                           alloc_style, alloc_func) \
do { \
    alloc_style(node->value, (item), alloc_func, \
                { \
                        CAG_FREE(node); \
                        return NULL; \
                }); \
    node->next = iterator; \
    node->prev = iterator->prev; \
    iterator->prev->next = node; \
    iterator->prev = node; \
} while(0)

#define _CAG_ALLOC_INSERT_DLIST(iterator, node, prev, next, item, \
                                alloc_style, alloc_func) \
do { \
    node = CAG_MALLOC(sizeof *node); \
    if (node) \
        CAG_P_INSERT_DLIST(iterator, node, prev, next, item, \
                           alloc_style, alloc_func); \
} while(0)

#define CAG_DEC_INSERT_DLIST(function, container, iterator_type, type) \
    iterator_type function(iterator_type it, \
                           type const element)

#define CAG_DEF_INSERT_DLIST(function, container, iterator_type, type, \
                             prev, next, alloc_style, alloc_func) \
CAG_DEC_INSERT_DLIST(function, container, iterator_type, type) \
{ \
    iterator_type node; \
    _CAG_ALLOC_INSERT_DLIST(it, node, prev, next, \
                            element, alloc_style, alloc_func); \
    return node; \
}

#define CAG_DEC_INSERTP_DLIST(function, container, iterator_type, type) \
    iterator_type function(iterator_type it, \
                           type const *element)

#define CAG_DEF_INSERTP_DLIST(function, container, iterator_type, type, \
                              prev, next, alloc_style, alloc_func) \
CAG_DEC_INSERTP_DLIST(function, container, iterator_type, type) \
{ \
    iterator_type node; \
    _CAG_ALLOC_INSERT_DLIST(it, node, prev, next, \
                            *element, alloc_style, alloc_func); \
    return node; \
}

#define CAG_DEF_INSERTP_ORDER_DLIST(function, container, \
                                    iterator_type, type, \
                                    prev, next, cmp_func, val_adr, comparator, \
                                    alloc_style, alloc_func) \
CAG_DEC_INSERTP_DLIST(function, container, iterator_type, \
                      type) \
{ \
    iterator_type node; \
    while (it->next != NULL && \
            cmp_func(val_adr (*element), val_adr it->value) comparator 0) \
        it = it->next; \
    _CAG_ALLOC_INSERT_DLIST(it, node, prev, next, \
                            *element, alloc_style, alloc_func); \
    return node; \
}

/*! \brief Function declaration and definition for *put* and putp. */

#define CAG_DEC_PUT_DLIST(function, container, iterator_type, type) \
    iterator_type function(container *dlist, iterator_type it, \
                           type const element)


#define CAG_DEF_PUT_DLIST(function, container, iterator_type, type) \
    CAG_DEC_PUT_DLIST(function, container, iterator_type, type) \
    { \
        assert(dlist); \
        return insert_ ## container(it, element); \
    }

#define CAG_DEC_PUTP_DLIST(function, container, iterator_type, type) \
    iterator_type function(container *dlist, iterator_type it, \
                           type const *element)


#define CAG_DEF_PUTP_DLIST(function, container, iterator_type, type) \
    CAG_DEC_PUTP_DLIST(function, container, iterator_type, type) \
    { \
        assert(dlist); \
        return insertp_ ## container(it, element); \
    }


/*! \brief Macros and function declaration and definitions for appending
    and prepending to list. By value and address versions supplied.
*/

#define CAG_P_ADDTO_DLIST(node, dlist, prev, next, begin, item, \
                          alloc_style, alloc_func) \
do { \
    alloc_style(node->value, item, alloc_func, \
                { \
                        CAG_FREE(node); \
                        return NULL; \
                }); \
    node->prev = dlist->begin; \
    node->next = dlist->begin->next; \
    dlist->begin->next = node; \
    node->next->prev = node; \
} while(0)

#define CAG_P_ALLOC_ADDTO_DLIST(node, dlist, prev, next, begin, value, item, \
                                alloc_style, alloc_func) \
do { \
    node = CAG_MALLOC(sizeof *node); \
    if (node) \
        CAG_P_ADDTO_DLIST(node, dlist, prev, next, begin, \
                          item, alloc_style, alloc_func); \
} while(0)

#define CAG_DEC_ADDTO_DLIST(function, container, iterator_type, type) \
    iterator_type function(container *dlist, type const item)

#define CAG_DEF_ADDTO_DLIST(function, container, iterator_type, \
                            type, begin, prev, next, \
                            alloc_style, alloc_func) \
CAG_DEC_ADDTO_DLIST(function, container, iterator_type, type) \
{ \
    iterator_type node; \
    CAG_P_ALLOC_ADDTO_DLIST(node, dlist, prev, next, \
                            begin, value, item, alloc_style, alloc_func); \
    return node; \
}

#define CAG_DEC_ADDTOP_DLIST(function, container, iterator_type, type) \
    iterator_type function(container *dlist, type const *element)

#define CAG_DEF_ADDTOP_DLIST(function, container, iterator_type, \
                             type, begin, prev, next, \
                             alloc_style, alloc_func) \
CAG_DEC_ADDTOP_DLIST(function, container, iterator_type, type) \
{ \
    iterator_type node; \
    CAG_P_ALLOC_ADDTO_DLIST(node, dlist, prev, next, \
                            begin, value, *element, alloc_style, alloc_func); \
    return node; \
}

/*! \brief Macros and function declaration and definitions to erase element
    from list.
*/


#define CAG_P_ERASE_DLIST(it, prev, next) \
    do { \
        it->prev->next = it->next; \
        it->next->prev = it->prev; \
        it = it->next; \
    } while(0)

#define CAG_P_DEALLOC_ERASE_DLIST(it, prev, next, free_func, val_adr) \
    do { \
        void *p = it; \
        free_func(val_adr it->value); \
        CAG_P_ERASE_DLIST(it, prev, next); \
        CAG_FREE(p); \
    } while(0)

#define CAG_DEC_ERASE_DLIST(function, container, iterator_type) \
    iterator_type function(container *dlist, iterator_type it)

#define CAG_DEF_ERASE_DLIST(function, container, iterator_type, prev, \
                            next, free_func, val_adr) \
CAG_DEC_ERASE_DLIST(function, container, iterator_type) \
{ \
    assert(dlist); \
    CAG_P_DEALLOC_ERASE_DLIST(it, prev, next, free_func, val_adr); \
    return it; \
}

/*! \brief Function declaration and definitions for returning list to heap. */


#define CAG_DEC_FREE_DLIST(function, container) \
    void function(container *list)

#define CAG_DEF_FREE_DLIST(function, container, free_func, val_adr) \
    CAG_DEC_FREE_DLIST(function, container) \
    { \
        it_ ## container it = list->begin_->next; \
        while (it != list->end_) { \
            list->begin_->next = it->next; \
            free_func(val_adr it->value); \
            CAG_FREE(it); \
            it = list->begin_->next; \
        } \
        CAG_FREE(list->begin_); \
        CAG_FREE(list->end_); \
    }

/*! \brief Declarations of list iterator and functions. */

#define CAG_DEC_DLIST(container, type) \
    struct iterator_ ## container { \
        struct iterator_ ## container *prev; \
        struct iterator_ ## container *next; \
        type value; \
    }; \
    struct reverse_iterator_ ## container { \
        struct reverse_iterator_ ## container *next; \
        struct reverse_iterator_ ## container *prev; \
        type value; \
    }; \
    typedef struct iterator_ ## container iterator_ ## container; \
    typedef struct reverse_iterator_ ## container \
            reverse_iterator_ ## container; \
    typedef iterator_ ## container * it_ ## container; \
    typedef reverse_iterator_ ## container * rit_ ## container; \
    struct container { \
        it_ ## container begin_; \
        it_ ## container end_; \
        CAG_DEF_INITIALIZE \
    }; \
    typedef struct container container; \
    CAG_DEC_NEW_DLIST(new_ ## container, container); \
    CAG_DEC_SET_MIN_SIZE_DLIST(set_min_size_ ## container, container, \
                               it_ ## container); \
    CAG_DEC_SET_EXACT_SIZE_DLIST(set_exact_size_ ## container, container, \
                                 it_ ## container); \
    CAG_DEC_STEP_DLIST(next_ ## container, next, it_ ## container); \
    CAG_DEC_STEP_DLIST(prev_ ## container, prev, it_ ## container); \
    CAG_DEC_STEP_DLIST(rnext_ ## container, next, rit_ ## container); \
    CAG_DEC_STEP_DLIST(rprev_ ## container, prev, rit_ ## container); \
    CAG_DEC_AT(at_ ## container, container, it_ ## container); \
    CAG_DEC_AT(rat_ ## container, container, rit_ ## container); \
    CAG_DEC_DISTANCE_DLIST(distance_ ## container, \
                           it_ ## container); \
    CAG_DEC_DISTANCE_DLIST(rdistance_ ## container, \
                           rit_ ## container); \
    CAG_DEC_LT_IT_DLIST(lt_it_ ## container, \
                        it_ ## container); \
    CAG_DEC_LTEQ_IT_DLIST(lteq_it_ ## container, \
                          it_ ## container); \
    CAG_DEC_LT_IT_DLIST(rlt_it_ ## container, \
                        rit_ ## container); \
    CAG_DEC_LTEQ_IT_DLIST(rlteq_it_ ## container, \
                          rit_ ## container); \
    CAG_DEC_BEGIN_DLIST(begin_ ## container, container); \
    CAG_DEC_END_DLIST(end_ ## container, container); \
    CAG_DEC_RBEGIN_DLIST(rbegin_ ## container, container); \
    CAG_DEC_REND_DLIST(rend_ ## container, container); \
    CAG_DEC_ADDTO_DLIST(prepend_ ## container, container, \
                        it_ ## container, type); \
    CAG_DEC_ADDTO_DLIST(append_ ## container, container, \
                        it_ ## container, type); \
    CAG_DEC_ADDTOP_DLIST(prependp_ ## container, container, \
                         it_ ## container, type); \
    CAG_DEC_ADDTOP_DLIST(appendp_ ## container, container, \
                         it_ ## container, type); \
    CAG_DEC_FRONT_DLIST(front_ ## container, container, type); \
    CAG_DEC_BACK_DLIST(back_ ## container, container, type); \
    CAG_DEC_INSERT_DLIST(insert_ ## container, container, \
                         it_ ## container, type); \
    CAG_DEC_INSERTP_DLIST(insertp_ ## container, container, \
                          it_ ## container, type); \
    CAG_DEC_PUT_DLIST(put_ ## container, container, \
                      it_ ## container, type); \
    CAG_DEC_PUTP_DLIST(putp_ ## container, container, \
                       it_ ## container, type); \
    CAG_DEC_ERASE_DLIST(erase_ ## container, container, \
                        it_ ## container); \
    CAG_DEC_ERASE_RANGE(erase_range_ ## container, container, \
                        it_ ## container); \
    CAG_DEC_FREE_DLIST(free_ ## container, container); \
    CAG_DEC_BIDIRECTIONAL(container, type); \
    CAG_DEC_REORDERABLE(container, type)

/*! \brief Definitions of dlist functions. */

#define CAG_DEF_ALL_DLIST(container, type, alloc_style, alloc_func, \
                          free_func, val_adr) \
CAG_DEF_NEW_DLIST(new_ ## container, container) \
CAG_DEF_SET_MIN_SIZE_DLIST(set_min_size_ ## container, container, \
                           it_ ## container) \
CAG_DEF_SET_EXACT_SIZE_DLIST(set_exact_size_ ## container, container, \
                             it_ ## container, \
                             set_min_size_ ## container, \
                             erase_range_ ## container) \
CAG_DEF_STEP_DLIST(next_ ## container, next, it_ ## container) \
CAG_DEF_STEP_DLIST(prev_ ## container, prev, it_ ## container) \
CAG_DEF_STEP_DLIST(rnext_ ## container, next, rit_ ## container) \
CAG_DEF_STEP_DLIST(rprev_ ## container, prev, rit_ ## container) \
CAG_DEF_AT(at_ ## container, container, \
           it_ ## container, next_ ## container) \
CAG_DEF_AT(rat_ ## container, container, \
           rit_ ## container, rnext_ ## container) \
CAG_DEF_DISTANCE_DLIST(distance_ ## container, \
                       it_ ## container) \
CAG_DEF_DISTANCE_DLIST(rdistance_ ## container, \
                       rit_ ## container) \
CAG_DEF_LT_IT_DLIST(lt_it_ ## container, \
                    it_ ## container, \
                    next_ ## container) \
CAG_DEF_LTEQ_IT_DLIST(lteq_it_ ## container, \
                      it_ ## container, \
                      next_ ## container) \
CAG_DEF_LT_IT_DLIST(rlt_it_ ## container, \
                    rit_ ## container, \
                    rnext_ ## container) \
CAG_DEF_LTEQ_IT_DLIST(rlteq_it_ ## container, \
                      rit_ ## container, \
                      rnext_ ## container) \
CAG_DEF_BEGIN_DLIST(begin_ ## container, container) \
CAG_DEF_END_DLIST(end_ ## container, container) \
CAG_DEF_RBEGIN_DLIST(rbegin_ ## container, container) \
CAG_DEF_REND_DLIST(rend_ ## container, container) \
CAG_DEF_ADDTO_DLIST(prepend_ ## container, container, \
                    it_ ## container, type, \
                    begin_, prev, next, alloc_style, alloc_func) \
CAG_DEF_ADDTO_DLIST(append_ ## container, container, \
                    it_ ## container, type, end_, \
                    next, prev, alloc_style, alloc_func) \
CAG_DEF_ADDTOP_DLIST(prependp_ ## container, container, \
                     it_ ## container, type, \
                     begin_, prev, next, alloc_style, alloc_func) \
CAG_DEF_ADDTOP_DLIST(appendp_ ## container, container, \
                     it_ ## container, type, end_, \
                     next, prev, alloc_style, alloc_func) \
CAG_DEF_FRONT_DLIST(front_ ## container, container, type) \
CAG_DEF_BACK_DLIST(back_ ## container, container, type) \
CAG_DEF_INSERT_DLIST(insert_ ## container, container, \
                     it_ ## container, type, prev, next, \
                     alloc_style, alloc_func) \
CAG_DEF_INSERTP_DLIST(insertp_ ## container, container, \
                      it_ ## container, type, prev, next, \
                      alloc_style, alloc_func) \
CAG_DEF_PUT_DLIST(put_ ## container, container, \
                  it_ ## container, type) \
CAG_DEF_PUTP_DLIST(putp_ ## container, container, \
                   it_ ## container, type) \
CAG_DEF_ERASE_DLIST(erase_ ## container, container, \
                    it_ ## container, prev, next, free_func, val_adr) \
CAG_DEF_ERASE_RANGE(erase_range_ ## container, container, \
                    it_ ## container, erase_ ## container, CAG_NO_OP_3) \
CAG_DEF_FREE_DLIST(free_ ## container, container, free_func, val_adr) \
CAG_DEF_BIDIRECTIONAL(container, type) \
CAG_DEF_REORDERABLE(container, type) \
typedef container CAG_P_CMB(container,  __LINE__)

/*! \brief Used to declare and define a doubly linked list in one macro.  Useful
   usually for small programs where the container will only be used in one
   module.
*/

#define CAG_DEC_DEF_DLIST(container, type) \
    CAG_DEC_DLIST(container, type); \
    CAG_DEF_DLIST(container, type)

#define CAG_DEC_DEF_ALL_DLIST(container, type, alloc_style, alloc_func, \
                              free_func, val_adr) \
CAG_DEC_DLIST(container, type); \
CAG_DEF_ALL_DLIST(container, type, alloc_style, alloc_func, \
                  free_func, val_adr)

#define CAG_DEC_DEF_ALL_CMP_DLIST(container, type, cmp_func, val_adr, \
				  alloc_style, alloc_func, free_func) \
CAG_DEC_CMP_DLIST(container, type); \
CAG_DEF_ALL_CMP_DLIST(container, type, cmp_func, val_adr, \
		      alloc_style, alloc_func, free_func)


/*! \brief Declaration of functions for lists that remain sorted and have a
    comparison function.
*/

#define CAG_DEC_CMP_DLIST(container, type) \
    CAG_DEC_DLIST(container, type); \
    CAG_DEC_STABLE_SORT(stable_sort_ ## container, \
                        it_ ## container); \
    CAG_DEC_STABLE_SORT(rstable_sort_ ## container, \
                        rit_ ## container); \
    CAG_DEC_CMP_REORDERABLE(container, type); \
    CAG_DEC_CMP_BIDIRECTIONAL(container, type)

/*! \brief Identical to CAG_DEC_CMP_DLIST but provided for users who
    want consistent names.
*/

#define CAG_DEC_CMPP_DLIST CAG_DEC_CMP_DLIST


/* \brief Define functions for a dlist that must remain ordered and has a
   comparison function.
*/

#define CAG_DEF_ALL_CMP_DLIST(container, type, cmp_func, val_adr, \
                              alloc_style, alloc_func, free_func) \
CAG_DEF_ALL_DLIST(container, type, \
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
CAG_DEF_CMP_BIDIRECTIONAL(container, type, cmp_func, val_adr) \
typedef container CAG_P_CMB(container ## _cmp,  __LINE__)

/*! \brief Definition of most common case list functions.
   This is a list that does not manage the memory of its elements and
   is not ordered.
*/

#define CAG_DEF_DLIST(container, type) \
    CAG_DEF_ALL_DLIST(container, type, \
                      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                      CAG_NO_FREE_FUNC, CAG_BYVAL)



/*! \brief Definition of most common case functions for an ordered list.
   This is a list that does not manage the memory of its elements.
*/

#define CAG_DEF_CMP_DLIST(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_DLIST(container, type, cmp_func, CAG_BYVAL, \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                          CAG_NO_FREE_FUNC)

/*! \brief Same as CAG_DEF_CMP_DLIST but cmp_func takes parameters by address. */

#define CAG_DEF_CMPP_DLIST(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_DLIST(container, type, cmp_func, CAG_BYADR, \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, \
                          CAG_NO_FREE_FUNC)


/*! \brief Analogous to CAG_DEC_DEF_DLIST but for ordered lists. */

#define CAG_DEC_DEF_CMP_DLIST(container, type, cmp_func) \
    CAG_DEC_CMP_DLIST(container, type); \
    CAG_DEF_CMP_DLIST(container, type, cmp_func)

/*! \brief Same as CAG_DEC_DEF_CMP_DLIST but cmp_func takes its parameters
    by address.
*/

#define CAG_DEC_DEF_CMPP_DLIST(container, type, cmp_func) \
    CAG_DEC_CMPP_DLIST(container, type); \
    CAG_DEF_CMPP_DLIST(container, type, cmp_func)

/*! \brief Declare and define macros for a dlist whose elements are C
   strings.  This is a common use-case, e.g. a list of words.
*/


#define CAG_DEC_STR_DLIST(container) \
    CAG_DEC_CMP_DLIST(container, char *)

#define CAG_DEF_STR_DLIST(container) \
    CAG_DEF_ALL_CMP_DLIST(container, char *, strcmp, CAG_BYVAL, \
                         CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free)

#define CAG_DEC_DEF_STR_DLIST(container) \
    CAG_DEC_STR_DLIST(container); \
    CAG_DEF_STR_DLIST(container)

#endif                          /* CAG_DLIST */
