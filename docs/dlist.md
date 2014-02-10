# Doubly-linked lists

CAGL doubly-linked lists (DLIST) are containers that support efficiently inserting and removing elements at any point in the container, if there is an iterator already pointing to a place near to the place where the item must be inserted.

Iterators for these container types are bidirectional. DLISTs can be unordered  or ordered. Ordered DLISTs should be declared and defined with *CMP* macros. It is ok for an ordered list to become unordered, but certain functions (like *search_C*) will then give undefined results. A list can be ordered again by calling one of the sorting functions, e.g. *sort_C* or *stable_sort_C*.

### DLIST declaration and definition macros {-}

- [CAG_DEC_DLIST](#cag_dec_dlist)
- [CAG_DEF_DLIST](#cag_def_dlist)
- [CAG_DEC_DEF_DLIST](#cag_dec_def_dlist)
- [CAG_DEC_CMP_DLIST](#cag_dec_cmp_dlist)
- [CAG_DEF_CMP_DLIST](#cag_def_cmp_dlist)
- [CAG_DEC_DEF_CMP_DLIST](#cag_dec_def_cmp_dlist)
- [CAG_DEC_CMPP_DLIST](#cag_dec_cmpp_dlist)
- [CAG_DEF_CMPP_DLIST](#cag_def_cmpp_dlist)
- [CAG_DEC_DEF_CMPP_DLIST](#cag_dec_def_cmpp_dlist)
- [CAG_DEF_ALL_DLIST](#cag_def_all_dlist)
- [CAG_DEF_ALL_CMP_DLIST](#cag_def_all_cmp_dlist)

### DLIST function blueprints {-}

- [append_C](#append_C-ad)
- [appendp_C](#appendp_C-ad)
- [at_C](#at_C-adhst)
- [back_C](#back_C-adst)
- [begin_C](#begin_C-adhst)
- [cmp_C](#cmp_C-adst)
- [cmp_all_C](#cmp_all_C-adst)
- [cmp_range_C](#cmp_range_C-adst)
- [copy_C](#copy_C-adhst)
- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [distance_C](#distance_C-adhst)
- [distance_all_C](#distance_all_C-adhst)
- [end_C](#end_C-adhst)
- [equal_all_C](#equal_all_C-adst)
- [equal_range_C](#equal_range_C-adst)
- [erase_C](#erase_C-adht)
- [erase_all_C](#erase_all_C-adhst)
- [erase_range_C](#erase_range_C-adhst)
- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [free_C](#free_C-adhst)
- [free_many_C](#free_many_C-adhst)
- [front_C](#front_C-adst)
- [index_C](#index_C-adhst)
- [insert_C](#insert_C-adht)
- [insert_gt_C](#insert_gt_C-ad)
- [insert_gteq_C](#insert_gteq_C-ad)
- [insert_lt_C](#insert_lt_C-ad)
- [insert_lteq_C](#insert_lteq_C-ad)
- [insertp_C](#insertp_C-adht)
- [insertp_gt_C](#insertp_gt_C-ad)
- [insertp_gteq_C](#insertp_gteq_C-ad)
- [insertp_lt_C](#insertp_lt_C-ad)
- [insertp_lteq_C](#insertp_lteq_C-ad)
- [it_C](#it_C-adhst)
- [last_C](#last_C)
- [lt_it_C](#lt_it_C-ad)
- [lteq_it_C](#lteq_it_C-ad)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [next_C](#next_C-adhst)
- [prepend_C](#prepend_C-ads)
- [prependp_C](#prependp_C-ads)
- [prev_C](#prev_C-adt)
- [put_C](#put_C-adhst)
- [putp_C](#putp_C-ad)
- [random_shuffle_C](#random_shuffle_C-ad)
- [random_shuffle_all_C](#random_shuffle_all_C-ad)
- [rat_C](#rat_C-adt)
- [rbegin_C](#rbegin_C-adt)
- [rcmp_C](#rcmp_C-adt)
- [rcmp_all_C](#rcmp_all_C-adt)
- [rcmp_range_C](#rcmp_range_C-adt)
- [rcopy_C](#rcopy_C-adt)
- [rcopy_all_C](#rcopy_all_C-adt)
- [rdistance_C](#rdistance_C-adt)
- [rend_C](#rend_C-adt)
- [requal_all_C](#requal_all_C-adt)
- [requal_range_C](#requal_range_C-adt)
- [reverse_C](#reverse_C-ad)
- [reverse_all_C](#reverse_all_C-ads)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [rit_C](#rit_C-adt)
- [rlt_it_C](#rlt_it_C-ad)
- [rlteq_it_C](#rlteq_it_C-ad)
- [rnext_C](#rnext_C-adt)
- [rprev_C](#rprev_C-adt)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [rsort_C](#rsort_C-ad)
- [rsort_all_C](#rsort_all_C-ad)
- [rstable_sort_C](#rstable_sort_C-ad)
- [rswap_C](#rswap_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [set_exact_size_C](#set_exact_size_C-ads)
- [set_min_size_C](#set_min_size_C-ads)
- [sort_C](#sort_C-ad)
- [sort_all_C](#sort_all_C-ad)
- [stable_sort_C](#stable_sort_C-ad)
- [stable_sort_all_C](#stable_sort_all_C-ads)
- [swap_C](#swap_C-adhst)


## DLIST structs and functions {-}

#### Iterator structs and typedefs {-}

```C
struct iterator_C {
    struct iterator_C *prev;
    struct iterator_C *next;
    T value;
};
struct reverse_iterator_C {
    struct reverse_iterator_C *next;
    struct reverse_iterator_C *prev;
    T value;
};
typedef struct iterator_C iterator_C;
typedef struct reverse_iterator_C reverse_iterator_C;
typedef iterator_C *it_C;
typedef reverse_iterator_C *rit_C;
```

#### Container structs and typedefs {-}

```C
struct C {
    it_C begin_;
    it_C end_;
};
typedef struct C C;
```
