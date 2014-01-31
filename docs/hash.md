# Hash tables

CAGL hash tables (HASH) are containers whose elements can on average be accessed in constant time. The order of the elements is arbitrary, but hash tables do support forward iterators.

HASH container types are intended to provide similar functionality to the C++11 STL *unordered_map*.

### HASH declaration and definition macros {-}

- [CAG_DEC_CMP_HASH](#cag_dec_cmp_hash)
- [CAG_DEF_CMP_HASH](#cag_def_cmp_hash)
- [CAG_DEC_CMPP_HASH](#cag_dec_cmpp_hash)
- [CAG_DEF_CMPP_HASH](#cag_def_cmpp_hash)
- [CAG_DEC_DEF_CMPP_HASH](#cag_dec_def_cmpp_hash)
- [CAG_DEF_ALL_CMP_HASH](#cag_def_all_cmp_hash)

### HASH function blueprints {-}

- [at_C](#at_C-adhst)
- [begin_C](#begin_C-adhst)
- [copy_C](#copy_C-adhst)
- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [distance_C](#distance_C-adhst)
- [distance_all_C](#distance_all_C-adhst)
- [end_C](#end_C-adhst)
- [erase_C](#erase_C-adht)
- [erase_all_C](#erase_all_C-adhst)
- [erase_range_C](#erase_range_C-adhst)
- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [free_C](#free_C-adhst)
- [free_many_C](#free_many_C-adhst)
- [get_C](#get_C-ht)
- [getp_C](#getp_C-ht)
- [index_C](#index_C-adhst)
- [insert_C](#insert_C-adht)
- [insertp_C](#insertp_C-adht)
- [it_C](#it_C-adhst)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [new_with_buckets_C](#new_with_buckets_C-h)
- [next_C](#next_C-adhst)
- [put_C](#put_C-adhst)
- [rehash_C](#rehash_C-h)
- [remove_C](#remove_C-ht)
- [removep_C](#removep_C-h)
- [swap_C](#swap_C-adhst)


## HASH structs and functions {-}

#### Iterator structs and typedefs {-}

```C
struct iterator_C {
    T value;
    ... /* internal variables */
};
typedef struct iterator_C iterator_C;
typedef iterator_C *it_C;
```

#### Container structs and typedefs {-}

```C
struct C {
    size_t buckets; /* Number of buckets in hash table. Treat as read-only */
    size_t size; /* Number of elements in table. Treat as read-only. */
    ... /* internal variables */
};
typedef struct C C;
```
