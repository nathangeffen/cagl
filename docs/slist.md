# Singly linked lists

CAGL singly-linked lists (SLIST) are containers that support efficiently inserting and removing elements at any point in the container, if there is an iterator already pointing to a place near and prior to the place where the item must be inserted.

SLISTs support forward iterators. SLISTs can be unordered  or ordered. Ordered SLISTs should be declared and defined with *CMP* macros. It is ok for an ordered list to become unordered, but certain functions (like *search_C*) will then give undefined results. A list can be ordered again by calling one of the sorting functions, e.g. *sort_C* or *stable_sort_C*.

In contrast to the other standard types of containers, you can declare and work solely with an SLIST iterator instead of the container itself (i.e. standalone iterators). SLIST iterators are the very simplest CAGL data structure and can be efficiently and elegantly used. Several macros have are provided to make using standalone SLIST iterators easier and safer, but working with SLIST iterators does require more programmer care to ensure memory leaks or invalid reads and writes to memory don't occur. To facilitate standard C list idioms, In contrast to other CAGL containers, the *end_C* function for an SLIST always returns NULL. Compare these similar code sketches, both of which are valid:

```C
it_C it;

... /* Omitted: Initialize *it* either by creating a new list or setting it
     * to a position in the list.
	 */

/* Iterate to the end of the list. */
while (it != NULL) {
    ... /* Omitted: Do something with it->value */
    it = it->next;
}
```

```C
C list;
it_C it;

... /* Omitted: Initialize *list* and *it*. */

/* Iterate to the end of the list. */
while (it != end_C(list)) {
    ... /* Omitted: Do something with it->value */
    it = next_C(it);
}
```


### SLIST declaration and definition macros {-}

- [CAG_DEC_SLIST](#cag_dec_slist)
- [CAG_DEF_SLIST](#cag_def_slist)
- [CAG_DEC_DEF_SLIST](#cag_dec_def_slist)
- [CAG_DEC_CMP_SLIST](#cag_dec_cmp_slist)
- [CAG_DEF_CMP_SLIST](#cag_def_cmp_slist)
- [CAG_DEC_DEF_CMP_SLIST](#cag_dec_def_cmp_slist)
- [CAG_DEC_CMPP_SLIST](#cag_dec_cmpp_slist)
- [CAG_DEF_CMPP_SLIST](#cag_def_cmpp_slist)
- [CAG_DEC_DEF_CMPP_SLIST](#cag_dec_def_cmpp_slist)
- [CAG_DEF_ALL_SLIST](#cag_def_all_slist)
- [CAG_DEF_ALL_CMP_SLIST](#cag_def_all_cmp_slist)

### SLIST function blueprints {-}

- [at_C](#at_C-adhst)
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
- [erase_after_C](#erase_after_C-s)
- [erase_after_range_C](#erase_after_range_C-s)
- [erase_all_C](#erase_all_C-adhst)
- [erase_front_C](#erase_front_C-s)
- [erase_range_C](#erase_range_C-adhst)
- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [free_C](#free_C-adhst)
- [free_many_C](#free_many_C-adhst)
- [front_C](#front_C-adst)
- [index_C](#index_C-adhst)
- [insert_after_C](#insert_after_C-s)
- [insertp_after_C](#insertp_after_C-s)
- [it_C](#it_C-adhst)
- [last_C](#last_C-s)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [next_C](#next_C-adhst)
- [prepend_C](#prepend_C-ads)
- [prependp_C](#prependp_C-ads)
- [put_C](#put_C-adhst)
- [reverse_all_C](#reverse_all_C-ads)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [set_exact_size_C](#set_exact_size_C-ads)
- [set_min_size_C](#set_min_size_C-ads)
- [stable_sort_all_C](#stable_sort_all_C-ads)
- [swap_C](#swap_C-adhst)

## SLIST structs and functions {-}

#### Iterator structs and typedefs {-}

```C
struct iterator_C {
    struct iterator_C *next;
    T value;
};
typedef struct iterator_C iterator_C;
typedef iterator_C *it_C;
```C


#### Container structs and typedefs {-}

```C
struct C {
    it_C header; /* The *new_C* functions initialize this to NULL.  */
};

typedef struct C C;
```C
