# Balanced binary trees

CAGL balanced binary trees (TREE) are containers that support efficiently inserting, accessing and removing elements from anywhere in the container. All these operations are $O(\log n)$, where $n$ is the number of elements in the tree. CAGL trees, so long as nothing invalid has been done to them (e.g. by the programmer doing something undocumented), are always sorted. A comparison function must be defined for a  tree container type.

CAGL trees are implemented as red-black trees. They provide similar functionality to C++ STL maps.

Iterators for these container types are bidirectional.

### TREE declaration and definition macros {-}

- [CAG_DEC_CMP_TREE](#cag_dec_cmp_tree)
- [CAG_DEF_CMP_TREE](#cag_def_cmp_tree)
- [CAG_DEC_CMPP_TREE](#cag_dec_cmpp_tree)
- [CAG_DEF_CMPP_TREE](#cag_def_cmpp_tree)
- [CAG_DEC_DEF_CMPP_TREE](#cag_dec_def_cmpp_tree)
- [CAG_DEF_ALL_CMP_TREE](#cag_def_all_cmp_tree)
- [CAG_DEC_STR_TREE](#cag_dec_str_tree)
- [CAG_DEF_STR_TREE](#cag_def_str_tree)
- [CAG_DEC_DEF_STR_TREE](#cag_dec_def_str_tree)
- [CAG_DEC_STR_STR_TREE](#cag_dec_str_str_tree)
- [CAG_DEF_STR_STR_TREE](#cag_def_str_str_tree)
- [CAG_DEC_DEF_STR_STR_TREE](#cag_dec_def_str_str_tree)

### TREE function blueprints {-}

- [at_C](#at_C-adhst)
- [back_C](#back_C)
- [begin_C](#begin_C-adhst)
- [check_integrity_C](#check_integrity_C-t)
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
- [get_C](#get_C-ht)
- [getp_C](#getp_C-ht)
- [index_C](#index_C-adhst)
- [inorder_C](#inorder_C-t)
- [insert_C](#insert_C-adht)
- [insertp_C](#insertp_C-adht)
- [it_C](#it_C-adhst)
- [last_C](#last_C)
- [levelorder_C](#levelorder_C-t)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [next_C](#next_C-adhst)
- [postorder_C](#postorder_C-t)
- [preorder_C](#preorder_C-t)
- [prev_C](#prev_C-adt)
- [put_C](#put_C-adhst)
- [putp_C](#putp_C)
- [rat_C](#rat_C-adt)
- [rbegin_C](#rbegin_C-adt)
- [rcmp_C](#rcmp_C-adt)
- [rcmp_all_C](#rcmp_all_C-adt)
- [rcmp_range_C](#rcmp_range_C-adt)
- [rcopy_C](#rcopy_C-adt)
- [rcopy_all_C](#rcopy_all_C-adt)
- [rdistance_C](#rdistance_C-adt)
- [remove_C](#remove_C-ht)
- [removep_C](#removep_C)
- [rend_C](#rend_C-adt)
- [requal_all_C](#requal_all_C-adt)
- [requal_range_C](#requal_range_C-adt)
- [reverseorder_C](#reverseorder_C-t)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [rit_C](#rit_C-adt)
- [rnext_C](#rnext_C-adt)
- [rprev_C](#rprev_C-adt)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [rswap_C](#rswap_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [swap_C](#swap_C-adhst)


### TREE structs and functions {-}

#### Iterator structs and typedefs {-}


```C
struct iterator_C {
    T value;
    ... /* internal variables */
};
struct reverse_iterator_C {
    T value;
	    ... /* internal variables */
};
typedef struct iterator_C iterator_C;
typedef struct reverse_iterator_C reverse_iterator_C;
typedef iterator_C *it_C;
typedef reverse_iterator_C *rit_C;
```

#### Container structs and typedefs {-}

```C
struct C {
    it_C root;
    ... /* internal variables */
};
typedef struct C C;
```
