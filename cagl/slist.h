/*! \file CAGL singly-linked list implementation.

  \copyright Copyright 2014 Nathan Geffen. All rights reserved.

  \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

  \sa COPYING for the license text.

  \sa howtodev.rst to learn how this code works and how to modify it.

  The singly linked list is implemented conventionally. In contrast to the other
  containers in CAGL, it is only a forward iterator -- users cannot step
  backwards through it. Therefore since *prev_[container](end_[container])* is
  not supported, there is no reason for *end_[container]* to return anything
  other than NULL. This is convenient for users because they can choose to test
  for *end* or NULL as they iterate through a list, whichever they prefer.

  Also, because of the simplicity and ubiquity of singly linked lists, the CAGL
  implementation offers a few macros so that users can use iterators instead of
  containers to manipulate lists. See *cag_test_list.c* for examples.

*/


#ifndef CAG_SLIST_H
#define CAG_SLIST_H

#include <stdlib.h>
#include "cagl/concepts.h"

/*! \brief Macro and function declaration and definition to initialize
    singly linked list.
*/

#define CAG_DEC_NEW_SLIST(function, container)                                \
    container *function(container *slist)

#define CAG_DEF_NEW_SLIST(function, container)                                \
    CAG_DEC_NEW_SLIST(function, container)                                    \
    {                                                                         \
        slist->header = NULL;                                                 \
        return slist;                                                         \
    }

/*! \brief Macro and function declaration and definition to iterate forward. */

#define CAG_NEXT_SLIST(iterator) (iterator)->next

#define CAG_DEC_NEXT_SLIST(function, iterator_type)                           \
    iterator_type function(iterator_type const it)

#define CAG_DEF_NEXT_SLIST(function, iterator_type)                           \
    CAG_DEC_NEXT_SLIST(function, iterator_type)                               \
    {                                                                         \
        return CAG_NEXT_SLIST(it);                                            \
    }


/*! \brief Macro and function declarations and definitions to return the
   beginning and end (NULL) of the list.
*/

#define CAG_DEC_BEGIN_SLIST(function, container, iterator_type)               \
    iterator_type function(const container *slist)

#define CAG_DEF_BEGIN_SLIST(function, container, iterator_type)               \
    CAG_DEC_BEGIN_SLIST(function, container, iterator_type)                   \
    {                                                                         \
        return (iterator_type) slist->header;                                 \
    }

#define CAG_END_SLIST(slist) NULL

#define CAG_DEC_END_SLIST(function, container, iterator_type)                 \
    iterator_type function(const container *slist)

#define CAG_DEF_END_SLIST(function, container, iterator_type)                 \
    CAG_DEC_END_SLIST(function, container, iterator_type)                     \
    {                                                                         \
        assert(slist);                                                        \
        return NULL;                                                          \
    }


/*! \brief Macros to return the last iterator in the list. A slow $\theta(n)$
    operation for singly-linked lists.
*/

#define CAG_LAST_SLIST(it)                                                    \
    do {                                                                      \
        void *p = it->next;                                                   \
        while (p != NULL) {                                                   \
            it = it->next;                                                    \
            p = it->next;                                                     \
        }                                                                     \
    } while (0);

#define CAG_DEC_LAST_SLIST(function, container, iterator_type)                \
    iterator_type function(const container *slist)

#define CAG_DEF_LAST_SLIST(function, container, iterator_type)                \
    CAG_DEC_LAST_SLIST(function, container, iterator_type)                    \
    {                                                                         \
        iterator_type it = slist->header;                                     \
        CAG_LAST_SLIST(it);                                                   \
        return it;                                                            \
    }

/*! \brief Public macro for users of slist iterators to initialize the iterator
    with its first element. Also, this macro can be called to stuff an iterator
    with an element before calls to *CAG_PREPEND_SLIST*.
*/

#define CAG_MAKE_SLIST(it, element)                                           \
    do {                                                                      \
        it = CAG_MALLOC(sizeof (*it) );                                       \
        if (it) {                                                             \
            it->value = element;                                              \
            it->next = NULL;                                                  \
        }                                                                     \
    } while (0)


/*! \brief Macro and function declaration and definition to prepend to a list.
    Users of iterators (instead of the slist container) must call
    *CAG_MAKE_SLIST* with the first element of the list and only subsequently
    *CAG_PREPEND_SLIST*, else they will dereference a NULL pointer.
    Implementing this as two macros instead of one is done for speed purposes.
    If implemented as one macro, every call to it would need to check if the
    iterator was NULL.
*/

#define CAG_PREPEND_SLIST(head, it)                                           \
    do {                                                                      \
        it->next = head;                                                      \
        head = it;                                                            \
    }                                                                         \
    while(0)


/*! \brief Private macro and declaration and definition for prepending
    to slist. By value and by address versions supplied.
*/

#define CAG_P_PREPEND_SLIST(iterator_type, slist,                             \
                            item, alloc_style, alloc_func)                    \
do {                                                                          \
    iterator_type node = CAG_MALLOC(sizeof (*node) );                         \
    if (node == NULL) return NULL;                                            \
    alloc_style(node->value, (item), alloc_func,                              \
                {                                                             \
                        CAG_FREE(node);                                       \
                        return NULL;                                          \
                });                                                           \
    if (slist->header)                                                        \
        CAG_PREPEND_SLIST(slist->header, node);                               \
    else {                                                                    \
        node->next = NULL;                                                    \
        slist->header = node;                                                 \
    }                                                                         \
    return node;                                                              \
} while(0)


#define CAG_DEC_PREPEND_SLIST(function, container, iterator_type, type)       \
    iterator_type function(container *slist, type element)

#define CAG_DEF_PREPEND_SLIST(function, container, iterator_type, type,       \
                              alloc_style, alloc_func)                        \
CAG_DEC_PREPEND_SLIST(function, container, iterator_type, type)               \
{                                                                             \
    CAG_P_PREPEND_SLIST(iterator_type, slist,                                 \
                        element, alloc_style, alloc_func);                    \
}


#define CAG_DEC_PREPENDP_SLIST(function, container, iterator_type, type)      \
    iterator_type function(container *slist, type *element)

#define CAG_DEF_PREPENDP_SLIST(function, container, iterator_type, type,      \
                               alloc_style, alloc_func)                       \
CAG_DEC_PREPENDP_SLIST(function, container, iterator_type, type)              \
{                                                                             \
    CAG_P_PREPEND_SLIST(iterator_type, slist,                                 \
                        *element, alloc_style, alloc_func);                   \
}


/*! \brief Macro for inserting an iterator after a node in an slist. */

#define CAG_INSERT_AFTER_SLIST(it_already_in_list, it_to_insert)              \
    do {                                                                      \
        it_to_insert->next = it_already_in_list->next;                        \
        it_already_in_list->next = it_to_insert;                              \
    }                                                                         \
    while(0)

/*! \brief Macro and function declaration and definition to insert into slist.
    By value and by address versions supplied.
*/

#define CAG_P_INSERT_AFTER_SLIST(iterator_type, slist, it,                    \
                                 item, alloc_style, alloc_func)               \
do {                                                                          \
    iterator_type node = CAG_MALLOC(sizeof (*node));                          \
    if (node == NULL) return NULL;                                            \
    alloc_style(node->value, (item), alloc_func,                              \
                {                                                             \
                        CAG_FREE(node);                                       \
                        return NULL;                                          \
                });                                                           \
    if (it)                                                                   \
        CAG_INSERT_AFTER_SLIST(it, node);                                     \
    else { /* Empty list */                                                   \
        node->next = NULL;                                                    \
        slist->header = node;                                                 \
    }                                                                         \
    return node;                                                              \
} while(0)

#define CAG_DEC_INSERT_AFTER_SLIST(function, container, iterator_type, type)  \
    iterator_type function(container *slist, iterator_type it, type item)

#define CAG_DEF_INSERT_AFTER_SLIST(function, container, iterator_type, type,  \
                                   alloc_style, alloc_func)                   \
CAG_DEC_INSERT_AFTER_SLIST(function, container, iterator_type, type)          \
{                                                                             \
    CAG_P_INSERT_AFTER_SLIST(iterator_type, slist, it,                        \
                             item, alloc_style, alloc_func);                  \
}

#define CAG_DEC_INSERTP_AFTER_SLIST(function, container, iterator_type, type) \
    iterator_type function(container *slist, iterator_type it, type *item)

#define CAG_DEF_INSERTP_AFTER_SLIST(function, container, iterator_type, type, \
                                    alloc_style, alloc_func)                  \
CAG_DEC_INSERTP_AFTER_SLIST(function, container, iterator_type, type)         \
{                                                                             \
    CAG_P_INSERT_AFTER_SLIST(iterator_type, slist, it,                        \
                             *item, alloc_style, alloc_func);                 \
}


#define CAG_DEC_PUT_SLIST(function, container, iterator_type, type)           \
    iterator_type function(container *slist, iterator_type it,                \
                           type const item)


#define CAG_DEF_PUT_SLIST(function, container, iterator_type, type)           \
    CAG_DEC_PUT_SLIST(function, container, iterator_type, type)               \
    {                                                                         \
        if (it) {                                                             \
            if(insert_after_ ## container(slist, it, item))                   \
                return it;                                                    \
            else                                                              \
                return NULL;                                                  \
        } else {                                                              \
            if(insert_after_ ## container(slist, it, item))                   \
                return (iterator_type) &slist->header;                        \
            else                                                              \
                return NULL;                                                  \
        }                                                                     \
    }


/*! \brief Macro and function declaration and definition to calculate
   distance between two iterators in a list.
*/

#define CAG_DISTANCE_SLIST(it_from, it_to, count)                             \
    do {                                                                      \
        void *p = it_from;                                                    \
        count = 0;                                                            \
        while (p != it_to) {                                                  \
            ++count;                                                          \
            memmove(&p, p, sizeof(p));                                        \
        }                                                                     \
    } while(0)

#define CAG_DEC_DISTANCE_SLIST(function, iterator_type)                       \
    size_t function(iterator_type iterator_from,                              \
                    iterator_type iterator_to)

#define CAG_DEF_DISTANCE_SLIST(function, iterator_type)                       \
    CAG_DEC_DISTANCE_SLIST(function, iterator_type)                           \
    {                                                                         \
        size_t count;                                                         \
        CAG_DISTANCE_SLIST(iterator_from, iterator_to,  count);               \
        return count;                                                         \
    }


/*! \brief Function declaration and definition to retrieve front element. */

#define CAG_DEC_FRONT_SLIST(function, container, type)                        \
    type *function(container *slist)

#define CAG_DEF_FRONT_SLIST(function, container, type)                        \
    CAG_DEC_FRONT_SLIST(function, container, type)                            \
    {                                                                         \
        return &slist->header->value;                                         \
    }


/*! \brief Function declaration and definition to set the minimum size of the
   list. Useful for users who wish to use *copy_over* and want to ensure there
   is enough space in the copied over list.
*/


#define CAG_DEC_SET_MIN_SIZE_SLIST(function, container, iterator_type)        \
    iterator_type function(container *slist, iterator_type it, size_t size)

#define CAG_DEF_SET_MIN_SIZE_SLIST(function, container, iterator_type)        \
    CAG_DEC_SET_MIN_SIZE_SLIST(function, container, iterator_type)            \
    {                                                                         \
        size_t i = 0;                                                         \
        if (it) {                                                             \
            while (it->next != NULL && i < size) {                            \
                it = it->next;                                                \
                ++i;                                                          \
            }                                                                 \
        }  else {                                                             \
            it = CAG_MALLOC(sizeof(*it));                                     \
            if (!it) return NULL;                                             \
            slist->header = it;                                               \
            it->next = NULL;                                                  \
        }                                                                     \
        while (i < size) {                                                    \
            void *p = CAG_MALLOC(sizeof(*it));                                \
            if (!p)return NULL;                                               \
            it->next = p;                                                     \
            ++i;                                                              \
        }                                                                     \
        return it;                                                            \
    }

/*! \brief Function declaration and definition to set the exact size of the
   list. Useful before calling *copy_over*.
*/

#define CAG_DEC_SET_EXACT_SIZE_SLIST(function, container, iterator_type)      \
    iterator_type function(container *slist, size_t size)

#define CAG_DEF_SET_EXACT_SIZE_SLIST(function, container, iterator_type,      \
                                     set_min_size, erase_range)               \
CAG_DEC_SET_EXACT_SIZE_SLIST(function, container, iterator_type)              \
{                                                                             \
    iterator_type it;                                                         \
    it = set_min_size(slist, slist->header, size);                            \
    if (it)                                                                   \
        erase_range(slist, it->next, NULL);                                   \
    return it;                                                                \
}

/*! \brief Macros and function declaration and definitions to erase element from
    list. The macro can be used by users on iterators, but users are responsible
    for maintainin the integrity of the list if the node to delete is in the
    middle of the list.

    \sa *CAG_ERASE_AFTER_SLIST*.
*/

#define CAG_P_ERASE_SLIST(it, free_func, val_adr)                             \
    do {                                                                      \
        free_func(val_adr (it)->value);                                       \
        CAG_FREE(it);                                                         \
    } while(0)

#define CAG_ERASE_SLIST(it, free_func)                                        \
    CAG_P_ERASE_SLIST(it, free_func, CAG_BYVAL)


/*! \brief Declaration and definition of function to erase first element of
    list.
*/

#define CAG_DEC_ERASE_FRONT_SLIST(function, container, iterator_type)         \
    iterator_type function(container *slist)

#define CAG_DEF_ERASE_FRONT_SLIST(function, container,                        \
                                  iterator_type, free_func, val_adr)          \
CAG_DEC_ERASE_FRONT_SLIST(function, container, iterator_type)                 \
{                                                                             \
    iterator_type it = slist->header;                                         \
    if (slist->header) {                                                      \
        slist->header = slist->header->next;                                  \
        CAG_P_ERASE_SLIST(it, free_func, val_adr);                            \
    }                                                                         \
    return slist->header;                                                     \
}

/*! \brief Macro to erase element after the given iterator.
*/

#define CAG_P_ERASE_AFTER_SLIST(it, free_func, val_adr)                       \
    do {                                                                      \
        void *p = it->next->next ;                                            \
        CAG_P_ERASE_SLIST(it->next, free_func, val_adr);                      \
        it->next = p;                                                         \
    } while(0)

#define CAG_ERASE_AFTER_SLIST(it, free_func)                                  \
    CAG_P_ERASE_AFTER_SLIST(it, free_func, CAG_BYVAL)

/*! \brief Declaration and definition of function to erase element of
     iterator.
*/

#define CAG_DEC_ERASE_AFTER_SLIST(function, iterator_type)                    \
    iterator_type function(iterator_type it)

#define CAG_DEF_ERASE_AFTER_SLIST(function,                                   \
                                  iterator_type, free_func, val_adr)          \
CAG_DEC_ERASE_AFTER_SLIST(function, iterator_type)                            \
{                                                                             \
    CAG_P_ERASE_AFTER_SLIST(it, free_func, val_adr);                          \
    return it;                                                                \
}

/*! \brief Macro and function declaration and definition to erase range of
    elements after iterator.
*/

#define CAG_P_ERASE_AFTER_RANGE_SLIST(from, to, free_func, val_adr)           \
    do {                                                                      \
        void *p = from->next, *q;                                             \
        while (p != to) {                                                     \
            memmove(&q, p, sizeof(p));                                        \
            CAG_ERASE_SLIST(p, free_func);                                    \
            p = q;                                                            \
        }                                                                     \
        from->next = to;                                                      \
    } while (0)


#define CAG_ERASE_AFTER_RANGE_SLIST(from, to, free_func)                      \
    CAG_P_ERASE_AFTER_RANGE_SLIST(from, to, free_func, CAG_BYVAL)


#define CAG_DEC_ERASE_AFTER_RANGE_SLIST(function, iterator_type)              \
    iterator_type function(iterator_type from, iterator_type to)

#define CAG_DEF_ERASE_AFTER_RANGE_SLIST(function,                             \
                                        iterator_type, free_func, val_adr)    \
CAG_DEC_ERASE_AFTER_RANGE_SLIST(function, iterator_type)                      \
{                                                                             \
    CAG_P_ERASE_AFTER_RANGE_SLIST(from, to, free_func, val_adr);              \
    return from;                                                              \
}


#define CAG_P_ERASE_RANGE_SLIST(from, to, free_func, val_adr)                 \
    do {                                                                      \
        if (from->next) {                                                     \
            CAG_ERASE_AFTER_RANGE_SLIST(from, to, free_func);                 \
        }                                                                     \
        CAG_ERASE_SLIST(from, free_func);                                     \
    } while (0)


#define CAG_ERASE_RANGE_SLIST(from, to, free_func)                            \
    CAG_P_ERASE_RANGE_SLIST(from, to, free_func, CAG_BYVAL)

#define CAG_DEC_ERASE_RANGE_SLIST(function, container, iterator_type)         \
    iterator_type function(container *slist,                                  \
                           iterator_type from, iterator_type to)

#define CAG_DEF_ERASE_RANGE_SLIST(function, container,                        \
                                  iterator_type, free_func, val_adr)          \
CAG_DEC_ERASE_RANGE_SLIST(function, container, iterator_type)                 \
{                                                                             \
    if (slist->header) {                                                      \
        if (slist->header == from)                                            \
            slist->header = to;                                               \
        CAG_P_ERASE_RANGE_SLIST(from, to, free_func, val_adr);                \
        return to;                                                            \
    } else {                                                                  \
        return NULL;                                                          \
    }                                                                         \
}

#define CAG_P_ERASE_ALL_SLIST(from, free_func, val_adr)                       \
    do {                                                                      \
        CAG_ERASE_RANGE_SLIST(from, NULL, free_func);                         \
    } while (0)

#define CAG_ERASE_ALL_SLIST(from, free_func)                                  \
    CAG_P_ERASE_ALL_SLIST(from, free_func, CAG_BYVAL)

#define CAG_DEC_ERASE_ALL_SLIST(function, container)                          \
    iterator_type function(container *slist)

#define CAG_DEF_ERASE_ALL_SLIST(function, container,                          \
                                iterator_type, free_func, val_adr)            \
CAG_DEC_ERASE_ALL_SLIST(function, container)                                  \
{                                                                             \
    CAG_P_ERASE_RANGE_SLIST(slist, from, to, free_func, val_adr);             \
    return to;                                                                \
}

/*! \brief Macro and function declaration and definition to reverse slist. */

#define CAG_REVERSE_SLIST(iterator_type, from)                                \
    do {                                                                      \
        iterator_type p = from, q = NULL;                                     \
        while (p != NULL) {                                                   \
            from = p;                                                         \
            p = from->next;                                                   \
            from->next = q;                                                   \
            q = from;                                                         \
        }                                                                     \
    } while (0)


#define CAG_DEC_REVERSE_SLIST(function, container)                            \
    container *function(container *slist)

#define CAG_DEF_REVERSE_SLIST(function, container)                            \
    CAG_DEC_REVERSE_SLIST(function, container)                                \
    {                                                                         \
        CAG_REVERSE_SLIST(it_ ## container, slist->header);                   \
        return slist;                                                         \
    }


/*! \brief Macro and function declaration and definition to sort slist.
    The implementation is an in-place $O(n \log n)$ merge sort.
*/

#define CAG_STABLE_SORT_SLIST(iterator_type, list, cmp_func, val_adr)         \
    do {                                                                      \
        iterator_type left = NULL, right = NULL, middle;                      \
        iterator_type stack[65 * 2];                                          \
        int labels[65];                                                       \
        int sp = 0, i;                                                        \
    l1:                                                                       \
        if (list->next == NULL) {                                             \
            if (sp > 0) {                                                     \
                --sp;                                                         \
                if (labels[sp] == 2)                                          \
                    goto l2;                                                  \
                else                                                          \
                    goto l3;                                                  \
            } else {                                                          \
                goto lfinal;                                                  \
            }                                                                 \
        }                                                                     \
        {                                                                     \
            iterator_type slow = list, fast = list->next;                     \
            while (fast) {                                                    \
                fast = fast->next;                                            \
                if (fast) {                                                   \
                    slow = slow->next;                                        \
                    fast = fast->next;                                        \
                }                                                             \
            }                                                                 \
            middle =  slow;                                                   \
        }                                                                     \
        right = middle->next;                                                 \
        middle->next = NULL;                                                  \
        left = list;                                                          \
        /* left = mergesort(list); */                                         \
        i = sp * 2;                                                           \
        stack[i] = left;                                                      \
        stack[i + 1] = right;                                                 \
        labels[sp++] = 2;                                                     \
        goto l1;                                                              \
    l2:                                                                       \
        left = list;                                                          \
        /* right = mergesort(right); */                                       \
        i = sp * 2;                                                           \
        stack[i] = left;                                                      \
        stack[i + 1] = right;                                                 \
        labels[sp++] = 3;                                                     \
        list = right;                                                         \
        goto l1;                                                              \
    l3:                                                                       \
        right = list;                                                         \
        {                                                                     \
            iterator_type it, itl = left, itr = right, itret;                 \
            /* Position itret and it to the smaller of the */                 \
            /*  first element of the */                                       \
            /* left or right list. */                                         \
            if (left) {                                                       \
                if (right) {                                                  \
                    int result = cmp_func(val_adr left->value,                \
                                          val_adr right->value);              \
                    if (result <= 0) {                                        \
                        it = left;                                            \
                        itl = itl->next;                                      \
                    } else {                                                  \
                        it = right;                                           \
                        itr = itr->next;                                      \
                    }                                                         \
                } else {                                                      \
                    it = left;                                                \
                    itl = itl->next;                                          \
                }                                                             \
            } else {                                                          \
                it = right;                                                   \
                itr = itr->next;                                              \
            }                                                                 \
            itret = it;                                                       \
            while (itl && itr) {                                              \
                int result = cmp_func(val_adr itl->value,                     \
                                      val_adr itr->value);                    \
                if (result <= 0) {                                            \
                    it->next = itl;                                           \
                    itl = itl->next;                                          \
                } else {                                                      \
                    it->next = itr;                                           \
                    itr = itr->next;                                          \
                }                                                             \
                it = it->next;                                                \
            }                                                                 \
            if (itl)                                                          \
                it->next = itl;                                               \
            else                                                              \
                it->next = itr;                                               \
            list =  itret;                                                    \
        }                                                                     \
        if (sp > 0) {                                                         \
            --sp;                                                             \
            i = sp * 2;                                                       \
            left = stack[i];                                                  \
            right = stack[i + 1];                                             \
            if (labels[sp] == 2)                                              \
                goto l2;                                                      \
            else                                                              \
                goto l3;                                                      \
        } else {                                                              \
            goto lfinal;                                                      \
        }                                                                     \
        list = NULL;                                                          \
    lfinal: break;                                                            \
    } while (0)


/*! \brief Default macro for users only differs from the all-parameter version
    by setting the val_adr parameter to pass by value.
*/

#define CAG_STABLE_SORT_SLIST_DEFAULT(iterator_type, list, cmp_func)          \
    CAG_STABLE_SORT_SLIST(iterator_type, list, cmp_func, CAG_BYVAL);

#define CAG_DEC_SORT_SLIST(function, container, iterator_type)                \
    iterator_type function(container *list)

#define  CAG_DEF_SORT_SLIST(function, container, iterator_type,               \
                            cmp_func, val_adr)                                \
CAG_DEC_SORT_SLIST(function, container, iterator_type)                        \
{                                                                             \
    if (list->header)                                                         \
        CAG_STABLE_SORT_SLIST(iterator_type, list->header,                    \
                              cmp_func, val_adr);                             \
    return list->header;                                                      \
}

/*! \brief Function declaration and definitions for returning
    list to heap.
*/

#define CAG_DEC_FREE_SLIST(function, container)                               \
    void function(container *slist)

#define CAG_DEF_FREE_SLIST(function, container)                               \
    CAG_DEC_FREE_SLIST(function, container)                                   \
    {                                                                         \
        erase_all_ ## container(slist);                                       \
    }

/*! \brief The slist iterator type has a separate declaration for users who
    want to use slist iterators independently of the slist container.
*/

#define CAG_DEC_IT(iterator_name, type)                                       \
    struct iterator_name {                                                    \
        struct iterator_name *next;                                           \
        type value;                                                           \
    };                                                                        \
    typedef struct iterator_name *iterator_name

/*! \brief Declarations of list iterator and functions. */

#define CAG_DEC_SLIST(container, type)                                        \
    struct iterator_ ## container {                                           \
        struct iterator_ ## container *next;                                  \
        type value;                                                           \
    };                                                                        \
    typedef struct iterator_ ## container iterator_ ## container;             \
    typedef iterator_ ## container * it_ ## container;                        \
    struct container {                                                        \
        it_ ## container header;                                              \
    };                                                                        \
    typedef struct container container;                                       \
    CAG_DEC_NEW_SLIST(new_ ## container, container);                          \
    CAG_DEC_NEXT_SLIST(next_ ## container, it_ ## container);                 \
    CAG_DEC_BEGIN_SLIST(begin_ ## container, container, it_ ## container);    \
    CAG_DEC_END_SLIST(end_ ## container, container, it_ ## container);        \
    CAG_DEC_LAST_SLIST(last_ ## container, container, it_ ## container);      \
    CAG_DEC_PREPEND_SLIST(prepend_ ## container, container,                   \
                          it_ ## container, type);                            \
    CAG_DEC_PREPENDP_SLIST(prependp_ ## container, container,                 \
                           it_ ## container, type);                           \
    CAG_DEC_INSERT_AFTER_SLIST(insert_after_ ## container, container,         \
                               it_ ## container, type);                       \
    CAG_DEC_INSERTP_AFTER_SLIST(insertp_after_ ## container, container,       \
                                it_ ## container, type);                      \
    CAG_DEC_PUT_SLIST(put_ ## container, container, it_ ## container, type);  \
    CAG_DEC_DISTANCE_SLIST(distance_ ## container, it_ ## container);         \
    CAG_DEC_FRONT_SLIST(front_ ## container, container, type);                \
    CAG_DEC_SET_MIN_SIZE_SLIST(set_min_size_ ## container, container,         \
                               it_ ## container);                             \
    CAG_DEC_SET_EXACT_SIZE_SLIST(set_exact_size_ ## container, container,     \
                                 it_ ## container);                           \
    CAG_DEC_ERASE_FRONT_SLIST(erase_front_ ## container, container,           \
                              it_ ## container);                              \
    CAG_DEC_ERASE_AFTER_SLIST(erase_after_ ## container,                      \
                              it_ ## container);                              \
    CAG_DEC_ERASE_AFTER_RANGE_SLIST(erase_after_range_ ## container,          \
                                    it_ ## container);                        \
    CAG_DEC_ERASE_RANGE_SLIST(erase_range_ ## container, container,           \
                              it_ ## container);                              \
    CAG_DEC_AT(at_ ## container, container, it_ ## container);                \
    CAG_DEC_REVERSE_SLIST(reverse_all_ ## container, container);              \
    CAG_DEC_FREE_SLIST(free_ ## container, container);                        \
    CAG_DEC_FORWARD(container, type)

/*! \brief Definitions of slist functions. */

#define CAG_DEF_ALL_SLIST(container, type, alloc_style, alloc_func,           \
                          free_func, val_adr)                                 \
CAG_DEF_NEW_SLIST(new_ ## container, container)                               \
CAG_DEF_NEXT_SLIST(next_ ## container, it_ ## container)                      \
CAG_DEF_BEGIN_SLIST(begin_ ## container, container,                           \
                    it_ ## container)                                         \
CAG_DEF_END_SLIST(end_ ## container, container,                               \
                  it_ ## container)                                           \
CAG_DEF_LAST_SLIST(last_ ## container, container, it_ ## container)           \
CAG_DEF_PREPEND_SLIST(prepend_ ## container, container, it_ ## container,     \
                      type, alloc_style, alloc_func)                          \
CAG_DEF_PREPENDP_SLIST(prependp_ ## container, container, it_ ## container,   \
                       type, alloc_style, alloc_func)                         \
CAG_DEF_INSERT_AFTER_SLIST(insert_after_ ## container, container,             \
                           it_ ## container, type,                            \
                           alloc_style, alloc_func)                           \
CAG_DEF_INSERTP_AFTER_SLIST(insertp_after_ ## container, container,           \
                            it_ ## container, type,                           \
                            alloc_style, alloc_func)                          \
CAG_DEF_PUT_SLIST(put_ ## container, container, it_ ## container, type)       \
CAG_DEF_DISTANCE_SLIST(distance_ ## container, it_ ## container)              \
CAG_DEF_FRONT_SLIST(front_ ## container, container, type)                     \
CAG_DEF_SET_MIN_SIZE_SLIST(set_min_size_ ## container, container,             \
                           it_ ## container)                                  \
CAG_DEF_SET_EXACT_SIZE_SLIST(set_exact_size_ ## container, container,         \
                             it_ ## container,                                \
                             set_min_size_ ## container,                      \
                             erase_range_ ## container)                       \
CAG_DEF_ERASE_FRONT_SLIST(erase_front_ ## container, container,               \
                          it_ ## container, free_func, val_adr)               \
CAG_DEF_ERASE_AFTER_SLIST(erase_after_ ## container,                          \
                          it_ ## container, free_func, val_adr)               \
CAG_DEF_ERASE_AFTER_RANGE_SLIST(erase_after_range_ ## container,              \
                                it_ ## container, free_func, val_adr)         \
CAG_DEF_ERASE_RANGE_SLIST(erase_range_ ## container, container,               \
                          it_ ## container, free_func, val_adr)               \
CAG_DEF_AT(at_ ## container, container,                                       \
           it_ ## container, next_ ## container)                              \
CAG_DEF_REVERSE_SLIST(reverse_all_ ## container, container)                   \
CAG_DEF_FREE_SLIST(free_ ## container, container)                             \
CAG_DEF_FORWARD(container, type)                                              \
typedef container CAG_P_CMB(container ## _cmp_1,  __LINE__)

/*! \brief Declare ordered slist functions. */

#define CAG_DEC_CMP_SLIST(container, type)                                    \
    CAG_DEC_SLIST(container, type);                                           \
    CAG_DEC_SORT_SLIST(stable_sort_all_ ## container, container,              \
                       it_ ## container);                                     \
    CAG_DEC_CMP_FORWARD(container, type)


/*! \brief Identical to CAG_DEC_CMP_SLIST but provided for users who
    want consistent names.
*/

#define CAG_DEC_CMPP_SLIST CAG_DEC_CMP_SLIST


/*! \brief Define ordered slist functions. */

#define CAG_DEF_ALL_CMP_SLIST(container, type, cmp_func, val_adr,             \
                              alloc_style, alloc_func, free_func)             \
CAG_DEF_SLIST(container, type);                                               \
CAG_DEF_SORT_SLIST(stable_sort_all_ ## container, container,                  \
                   it_ ## container,                                          \
                   cmp_func, val_adr)                                         \
CAG_DEF_CMP_FORWARD(container, type, cmp_func, val_adr)                       \
typedef container CAG_P_CMB(container ## _cmp_2,  __LINE__)


/*! \brief Definition of most common case functions for an unordered list.
   This is a list that does not manage the memory of its elements.
*/

#define CAG_DEF_SLIST(container, type)                                        \
    CAG_DEF_ALL_SLIST(container, type,                                        \
                      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,                  \
                      CAG_NO_FREE_FUNC, CAG_BYVAL)

/*! \brief Define singly linked list in one macro.  Useful usually for small
   programs where the container will only be used in one module.
*/

#define CAG_DEC_DEF_SLIST(container, type)                                    \
    CAG_DEC_SLIST(container, type);                                           \
    CAG_DEF_SLIST(container, type)

/*! \brief Definition of most common case functions for an ordered list.
   This is a list that does not manage the memory of its elements.
*/

#define CAG_DEF_CMP_SLIST(container, type, cmp_func)                          \
    CAG_DEF_ALL_CMP_SLIST(container, type, cmp_func, CAG_BYVAL,               \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,              \
                          CAG_NO_FREE_FUNC)

/*! \brief Same as CAG_DEF_CMP_SLIST but cmp_func takes parameters by address. */

#define CAG_DEF_CMPP_SLIST(container, type, cmp_func)                          \
    CAG_DEF_ALL_CMP_SLIST(container, type, cmp_func, CAG_BYADR,               \
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,              \
                          CAG_NO_FREE_FUNC)


/*! \brief Analogous to CAG_DEC_DEF_SLIST but for ordered lists. */

#define CAG_DEC_DEF_CMP_SLIST(container, type, cmp_func)                      \
    CAG_DEC_CMP_SLIST(container, type);                                       \
    CAG_DEF_CMP_SLIST(container, type, cmp_func)

/*! \brief Analogous to CAG_DEC_DEF_CMP_SLIST but cmp_func takes its parameters
    by address.
*/

#define CAG_DEC_DEF_CMPP_SLIST(container, type, cmp_func)                      \
    CAG_DEC_CMPP_SLIST(container, type);                                       \
    CAG_DEF_CMPP_SLIST(container, type, cmp_func)


#endif                          /* CAG_SLIST */
