/*! \file Declarations and definitions of iterator concepts supported by the
    CAGL containers.

    \copyright Copyright 2014 Nathan Geffen. All rights reserved.

    \license This code is licensed under the FreeBSD Copyright.

    \sa COPYING for the license text.

    \sa howtodev.rst to learn how this code works and how to modify it.
*/

#ifndef CAG_CONCEPTS_H
#define CAG_CONCEPTS_H

#include "cagl/common.h"

/*! \brief Generic function declarations and definitions for containers
    that support the various iterators.
*/

#define CAG_DEC_FORWARD(container, type)                                      \
    extern it_ ## container (* const beg_ ## container) (const container *);  \
    CAG_DEC_NEW_FROM(new_from_ ## container, container);                      \
    CAG_DEC_RET_MANY_ARG(new_many_ ## container, container);                  \
    CAG_DEC_APPLY_CONTAINER(distance_all_ ## container, container, size_t);   \
    CAG_DEC_SWAP(swap_ ## container, it_ ## container);                       \
    CAG_DEC_INDEX(index_ ## container, container); \
    CAG_DEC_APPLY_CONTAINER(erase_all_ ## container, container,               \
                            it_ ## container);                                \
    CAG_DEC_VOID_MANY_ARG(free_many_ ## container, container);                \
    CAG_DEC_COPY(copy_ ## container, it_ ## container, container);            \
    CAG_DEC_APPLY_IN_OUT(copy_all_ ## container, container,                   \
                         container, container);                               \
    CAG_DEC_RET_MANY_ARG(copy_many_ ## container, container);                 \
    CAG_DEC_COPY_IF(copy_if_ ## container, it_ ## container, container, type);\
    CAG_DEC_COPY_IF_ALL(copy_if_all_ ## container, container,                 \
                        container, type);                                     \
    CAG_DEC_COPY_OVER(copy_over_ ## container, it_ ## container, it_ ## container);\
    CAG_DEC_FIND(find_ ## container, it_ ## container, type);                 \
    CAG_DEC_FINDP(findp_ ## container, it_ ## container, type);               \
    CAG_DEC_FIND_ALL(find_all_ ## container, container,                       \
                     it_ ## container, type);                                 \
    CAG_DEC_FINDP_ALL(findp_all_ ## container, container,                     \
                      it_ ## container, type)                                 \


#define CAG_DEF_FORWARD(container, type)                                      \
    it_ ## container (* const beg_ ## container)                              \
    (const container *)   = begin_ ## container;                              \
    CAG_DEF_NEW_FROM(new_from_ ## container, container)                       \
    CAG_DEF_RET_MANY_1_ARG(new_many_ ## container, container,                 \
                           new_ ## container)                                 \
    CAG_DEF_APPLY_CONTAINER(distance_all_ ## container, container, size_t,    \
                            distance_ ## container, begin_ ## container,      \
                            end_ ## container)                                \
    CAG_DEF_SWAP(swap_ ## container, it_ ## container, CAG_VALUE)             \
    CAG_DEF_INDEX(index_ ## container, container) \
    CAG_DEF_VOID_MANY_ARG(free_many_ ## container, container,                 \
                          free_ ## container)                                 \
    CAG_DEF_APPLY_VAR_CONTAINER(erase_all_ ## container, container,           \
                                it_ ## container, erase_range_ ## container,  \
                                begin_ ## container, end_ ## container)       \
    CAG_DEF_COPY(copy_ ## container, it_ ## container, container,             \
                 begin_ ## container, next_ ## container,                     \
                 next_ ## container, put_ ## container)                       \
    CAG_DEF_APPLY_IN_OUT(copy_all_ ## container, container, container,        \
                         container, copy_ ## container, begin_ ## container,  \
                         end_ ## container, begin_ ## container)              \
    CAG_DEF_RET_MANY_2_ARG(copy_many_ ## container, container,                \
                           copy_all_ ## container)                            \
    CAG_DEF_COPY_IF(copy_if_ ## container, it_ ## container, container, type, \
                    begin_ ## container, next_ ## container,                  \
                    next_ ## container, put_ ## container)                    \
    CAG_DEF_COPY_IF_ALL(copy_if_all_ ## container, container, container,      \
                        type, copy_if_ ## container, begin_ ## container,     \
                        end_ ## container)                                    \
    CAG_DEF_COPY_OVER(copy_over_ ## container, it_ ## container,              \
                      it_ ## container, next_ ## container,                   \
                      next_ ## container)                                     \
    CAG_DEF_FIND(find_ ## container, it_ ## container, type, next_ ## container) \
    CAG_DEF_FINDP(findp_ ## container, it_ ## container, type, next_ ## container) \
    CAG_DEF_FIND_ALL(find_all_ ## container, container, it_ ## container, type) \
    CAG_DEF_FINDP_ALL(findp_all_ ## container, container, it_ ## container, type) \

#define CAG_DEC_CMP_FORWARD(container, type)                                  \
    CAG_DEC_CMP(cmp_ ## container,                                            \
                it_ ## container,                                             \
                it_ ## container);                                            \
    CAG_DEC_CMP_RANGE(cmp_range_ ## container,                                \
                      it_ ## container,                                       \
                      it_ ## container);                                      \
    CAG_DEC_CMP_ALL(cmp_all_ ## container, container, container);             \
    CAG_DEC_EQUAL_RANGE(equal_range_ ## container,                            \
                        it_ ## container,                                     \
                        it_ ## container);                                    \
    CAG_DEC_EQUAL_ALL(equal_all_ ## container, container, container);         \
    CAG_DEC_SEARCH(search_ ## container, it_ ## container, type);             \
    CAG_DEC_SEARCHP(searchp_ ## container, it_ ## container, type);           \
    CAG_DEC_APPLY_DATA_CONTAINER(search_all_ ## container, container,         \
                                 it_ ## container, type);                     \
    CAG_DEC_APPLY_DATA_CONTAINER(searchp_all_ ## container, container,        \
                                 it_ ## container, type *)                    \

#define CAG_DEF_CMP_FORWARD(container, type, cmp_func, val_adr)               \
    CAG_DEF_CMP(cmp_ ## container, it_ ## container, it_ ## container,        \
                cmp_func, val_adr)                                            \
    CAG_DEF_CMP_RANGE(cmp_range_ ## container,                                \
                      it_ ## container,                                       \
                      it_ ## container,                                       \
                      next_ ## container, next_ ## container,                 \
                      cmp_func, val_adr)                                      \
    CAG_DEF_CMP_ALL(cmp_all_ ## container, container, container,              \
                    it_ ## container, it_ ## container,                       \
                    begin_ ## container, end_ ## container,                   \
                    begin_ ## container, end_ ## container,                   \
                    next_ ## container, next_ ## container,                   \
                    cmp_func, val_adr)                                        \
    CAG_DEF_EQUAL_RANGE(equal_range_ ## container,                            \
                        it_ ## container,                                     \
                        it_ ## container,                                     \
                        next_ ## container, next_ ## container,               \
                        cmp_func, val_adr)                                    \
    CAG_DEF_EQUAL_ALL(equal_all_ ## container, container, container,          \
                      it_ ## container, it_ ## container,                     \
                      begin_ ## container, end_ ## container,                 \
                      begin_ ## container,                                    \
                      next_ ## container, next_ ## container,                 \
                      cmp_func, val_adr)                                      \
    CAG_DEF_LSEARCH(search_ ## container, it_ ## container,                   \
                    type, next_ ## container, CAG_VALUE, cmp_func, val_adr)   \
    CAG_DEF_LSEARCHP(searchp_ ## container,                                   \
                     it_ ## container, type,                                  \
                     next_ ## container, CAG_VALUE, cmp_func, val_adr)        \
    CAG_DEF_APPLY_DATA_CONTAINER(search_all_ ## container, container,         \
                                 it_ ## container, type, search_ ## container,\
                                 begin_ ## container, end_ ## container)      \
    CAG_DEF_APPLY_DATA_CONTAINER(searchp_all_ ## container, container,        \
                                 it_ ## container, type*, searchp_ ## container, \
                                 begin_ ## container, end_ ## container)      \

#define CAG_DEC_BIDIRECTIONAL(container, type)                                \
    CAG_DEC_FORWARD(container, type);                                         \
    extern rit_ ## container (* const rbeg_ ## container) (const container *);\
    CAG_DEC_SWAP(rswap_ ## container, rit_ ## container);                     \
    CAG_DEC_FIND(rfind_ ## container, rit_ ## container, type);               \
    CAG_DEC_FINDP(rfindp_ ## container, rit_ ## container, type);             \
    CAG_DEC_COPY(rcopy_ ## container, rit_ ## container, container);          \
    CAG_DEC_APPLY_IN_OUT(rcopy_all_ ## container, container, container, container) \

#define CAG_DEF_BIDIRECTIONAL(container, type)                                \
    CAG_DEF_FORWARD(container, type)                                          \
    rit_ ## container (* const rbeg_ ## container) (const container *) =      \
            rbegin_ ## container;                                             \
    CAG_DEF_SWAP(rswap_ ## container, rit_ ## container, CAG_VALUE)           \
    CAG_DEF_FIND(rfind_ ## container, rit_ ## container, type, rnext_ ## container)\
    CAG_DEF_FINDP(rfindp_ ## container, rit_ ## container, type,              \
                  rnext_ ## container)                                        \
    CAG_DEF_COPY(rcopy_ ## container, rit_ ## container, container,           \
                 begin_ ## container, rnext_ ## container,                    \
                 next_ ## container, put_ ## container)                       \
    CAG_DEF_APPLY_IN_OUT(rcopy_all_ ## container, container, container,       \
                         container, rcopy_ ## container, rbegin_ ## container,\
                         rend_ ## container, begin_ ## container)             \

#define CAG_DEC_CMP_BIDIRECTIONAL(container, type)                            \
    CAG_DEC_CMP_FORWARD(container, type);                                     \
    CAG_DEC_CMP(rcmp_ ## container,                                           \
                rit_ ## container,                                            \
                it_ ## container);                                            \
    CAG_DEC_CMP_RANGE(rcmp_range_ ## container,                               \
                      rit_ ## container,                                      \
                      it_ ## container);                                      \
    CAG_DEC_CMP_ALL(rcmp_all_ ## container, container, container);            \
    CAG_DEC_EQUAL_RANGE(requal_range_ ## container,                           \
                        rit_ ## container,                                    \
                        it_ ## container);                                    \
    CAG_DEC_EQUAL_ALL(requal_all_ ## container, container, container);        \
    CAG_DEC_SEARCH(rsearch_ ## container, rit_ ## container, type);           \
    CAG_DEC_SEARCHP(rsearchp_ ## container, rit_ ## container, type)

#define CAG_DEF_CMP_BIDIRECTIONAL(container, type, cmp_func, val_adr)         \
    CAG_DEF_CMP_FORWARD(container, type, cmp_func, val_adr)                   \
    CAG_DEF_CMP(rcmp_ ## container, rit_ ## container, it_ ## container,      \
                cmp_func, val_adr)                                            \
    CAG_DEF_CMP_RANGE(rcmp_range_ ## container,                               \
                      rit_ ## container,                                      \
                      it_ ## container,                                       \
                      rnext_ ## container, next_ ## container,                \
                      cmp_func, val_adr)                                      \
    CAG_DEF_CMP_ALL(rcmp_all_ ## container, container, container,             \
                    rit_ ## container, it_ ## container,                      \
                    rbegin_ ## container, rend_ ## container,                 \
                    begin_ ## container, end_ ## container,                   \
                    rnext_ ## container, next_ ## container,                  \
                    cmp_func, val_adr)                                        \
    CAG_DEF_EQUAL_RANGE(requal_range_ ## container,                           \
                        rit_ ## container,                                    \
                        it_ ## container,                                     \
                        rnext_ ## container, next_ ## container,              \
                        cmp_func, val_adr)                                    \
    CAG_DEF_EQUAL_ALL(requal_all_ ## container, container, container,         \
                      rit_ ## container, it_ ## container,                    \
                      rbegin_ ## container, rend_ ## container,               \
                      begin_ ## container,                                    \
                      rnext_ ## container, next_ ## container,                \
                      cmp_func, val_adr)                                      \
    CAG_DEF_LSEARCH(rsearch_ ## container,                                    \
                    rit_ ## container,                                        \
                    type, rnext_ ## container, CAG_VALUE, cmp_func, val_adr)  \
    CAG_DEF_LSEARCHP(rsearchp_ ## container,                                  \
                     rit_ ## container,                                       \
                     type, rnext_ ## container, CAG_VALUE, cmp_func, val_adr)

#define CAG_DEC_REORDERABLE(container, type)                                  \
    CAG_DEC_REVERSE(reverse_ ## container, it_ ## container);                 \
    CAG_DEC_APPLY_CONTAINER(reverse_all_ ## container, container,             \
                            it_ ## container);                                \
    CAG_DEC_RANDOM_SHUFFLE(random_shuffle_ ## container, it_ ## container);   \
    CAG_DEC_APPLY_CONTAINER(random_shuffle_all_ ## container, container,      \
                            it_ ## container)                                 \

#define CAG_DEF_REORDERABLE(container, type)                                  \
    CAG_DEF_REVERSE(reverse_ ## container, it_ ## container,                  \
                    prev_ ## container, next_ ## container,                   \
                    swap_ ## container)                                       \
    CAG_DEF_APPLY_CONTAINER(reverse_all_ ## container, container,             \
                            it_ ## container, reverse_ ## container,          \
                            begin_ ## container, end_ ## container)           \
    CAG_DEF_RANDOM_SHUFFLE(random_shuffle_ ## container, it_ ## container,    \
                           distance_ ## container, swap_ ## container,        \
                           at_ ## container, prev_ ## container)              \
    CAG_DEF_APPLY_CONTAINER(random_shuffle_all_ ## container, container,      \
                            it_ ## container,                                 \
                            random_shuffle_ ## container, begin_ ## container,\
                            end_ ## container)                                \

#define CAG_DEC_CMP_REORDERABLE(container, type)                              \
    CAG_DEC_SORT(sort_ ## container, it_ ## container);                       \
    CAG_DEC_SORT(rsort_ ## container, rit_ ## container);                     \
    CAG_DEC_APPLY_CONTAINER(sort_all_ ## container, container,                \
                            it_ ## container);                                \
    CAG_DEC_APPLY_CONTAINER(rsort_all_ ## container, container,                \
                            rit_ ## container);                                \
    CAG_DEC_APPLY_CONTAINER(stable_sort_all_ ## container, container,         \
                            it_ ## container)                                 \

#define CAG_DEF_CMP_REORDERABLE(container, type, cmp_func, val_adr)           \
    CAG_DEF_SORT(sort_ ## container, it_ ## container, type,                  \
                 cmp_func, val_adr, distance_ ## container,                   \
                 prev_ ## container, next_ ## container,                      \
                 at_ ## container,                                            \
                 lteq_it_ ## container, lt_it_ ## container,                  \
                 swap_ ## container, CAG_VALUE)                               \
    CAG_DEF_SORT(rsort_ ## container, rit_ ## container, type,                \
                 cmp_func, val_adr, rdistance_ ## container,                  \
                 rprev_ ## container, rnext_ ## container,                    \
                 rat_ ## container,                                           \
                 rlteq_it_ ## container, rlt_it_ ## container,                \
                 rswap_ ## container, CAG_VALUE)                              \
    CAG_DEF_APPLY_CONTAINER(sort_all_ ## container, container,                \
                            it_ ## container, sort_ ## container,             \
                            begin_ ## container, end_ ## container)           \
    CAG_DEF_APPLY_CONTAINER(rsort_all_ ## container, container,                \
                            rit_ ## container, rsort_ ## container,             \
                            rbegin_ ## container, rend_ ## container)           \
    CAG_DEF_APPLY_CONTAINER(stable_sort_all_ ## container, container,         \
                            it_ ## container, stable_sort_ ## container,      \
                            begin_ ## container, end_ ## container)           \

#define CAG_DEC_RANDOMACCESS(container, type)                                 \
    CAG_DEC_BIDIRECTIONAL(container, type)                                    \


#define CAG_DEF_RANDOMACCESS(container, type)                                 \
    CAG_DEF_BIDIRECTIONAL(container, type)                                    \

#define CAG_DEC_CMP_RANDOMACCESS(container, type)                             \
    CAG_DEC_CMP_BIDIRECTIONAL(container, type);                               \
    CAG_DEC_LOWER_BOUND(lower_bound_ ## container, it_ ## container, type);   \
    CAG_DEC_LOWER_BOUND(lower_rbound_ ## container, rit_ ## container, type); \
    CAG_DEC_LOWER_BOUNDP(lower_boundp_ ## container, it_ ## container, type); \
    CAG_DEC_LOWER_BOUNDP(lower_rboundp_ ## container,                         \
                         rit_ ## container, type);                            \
    CAG_DEC_APPLY_DATA_CONTAINER(lower_bound_all_ ## container, container,    \
                                 it_ ## container, type);                     \
    CAG_DEC_APPLY_DATA_CONTAINER(lower_boundp_all_ ## container, container,   \
                                 it_ ## container, type *);                   \
    CAG_DEC_BSEARCH(binary_search_ ## container, it_ ## container, type);     \
    CAG_DEC_BSEARCH(binary_rsearch_ ## container, rit_ ## container, type);   \
    CAG_DEC_BSEARCHP(binary_searchp_ ## container, it_ ## container, type);   \
    CAG_DEC_BSEARCHP(binary_rsearchp_ ## container, rit_ ## container, type); \
    CAG_DEC_APPLY_DATA_CONTAINER(binary_search_all_ ## container, container,  \
                                 int, type);                                  \
    CAG_DEC_APPLY_DATA_CONTAINER(binary_searchp_all_ ## container, container, \
                                 int, type *)                                 \

#define CAG_DEF_CMP_RANDOMACCESS(container, type, cmp_func, val_adr)          \
    CAG_DEF_CMP_BIDIRECTIONAL(container, type, cmp_func, val_adr)             \
    CAG_DEF_LOWER_BOUND(lower_bound_ ## container, it_ ## container,          \
                        type, at_ ## container, distance_ ## container,       \
                        prev_ ## container, next_ ## container,               \
                        CAG_VALUE, cmp_func, val_adr)                         \
    CAG_DEF_LOWER_BOUND(lower_rbound_ ## container, rit_ ## container,        \
                        type, rat_ ## container, rdistance_ ## container,     \
                        rprev_ ## container, rnext_ ## container,             \
                        CAG_VALUE, cmp_func, val_adr)                         \
    CAG_DEF_LOWER_BOUNDP(lower_boundp_ ## container, it_ ## container,        \
                         type, at_ ## container, distance_ ## container,      \
                         prev_ ## container, next_ ## container,              \
                         CAG_VALUE, cmp_func, val_adr)                        \
    CAG_DEF_LOWER_BOUNDP(lower_rboundp_ ## container, rit_ ## container,      \
                         type, rat_ ## container, rdistance_ ## container,    \
                         rprev_ ## container, rnext_ ## container,            \
                         CAG_VALUE, cmp_func, val_adr)                        \
    CAG_DEF_APPLY_DATA_CONTAINER(lower_bound_all_ ## container, container,    \
                                 it_ ## container, type,                      \
                                 lower_bound_ ## container,                   \
                                 begin_ ## container, end_ ## container)      \
    CAG_DEF_APPLY_DATA_CONTAINER(lower_boundp_all_ ## container, container,   \
                                 it_ ## container, type*,                     \
                                 lower_boundp_ ## container,                  \
                                 begin_ ## container, end_ ## container)      \
    CAG_DEF_BSEARCH(binary_search_ ## container, it_ ## container,            \
                    type, lower_boundp_ ## container, cmp_func, val_adr)      \
    CAG_DEF_BSEARCH(binary_rsearch_ ## container, rit_ ## container,          \
                    type, lower_rboundp_ ## container, cmp_func, val_adr)     \
    CAG_DEF_BSEARCHP(binary_searchp_ ## container, it_ ## container,          \
                     type, lower_boundp_ ## container, cmp_func, val_adr)     \
    CAG_DEF_BSEARCHP(binary_rsearchp_ ## container, rit_ ## container,        \
                     type, lower_rboundp_ ## container, cmp_func, val_adr)    \
    CAG_DEF_APPLY_DATA_CONTAINER(binary_search_all_ ## container, container,  \
                                 int, type,                                   \
                                 binary_search_ ## container,                 \
                                 begin_ ## container, end_ ## container)      \
    CAG_DEF_APPLY_DATA_CONTAINER(binary_searchp_all_ ## container, container, \
                                 int, type*,                                  \
                                 binary_searchp_ ## container,                \
                                 begin_ ## container, end_ ## container)


#endif /* CAG_CONCEPTS_H */
