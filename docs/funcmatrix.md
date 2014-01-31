# Appendix: Function blueprint matrix {#function-matrix -}

FUNCTION | ARRAY | DLIST | HASH | TREE | SLIST |
:--------|:-----:|:-----:|:----:|:----:|:-----:|
| append_C                       | [a](#append_C-ad) | [d](#append_C-ad) |  |  |  |
| appendp_C                      | [a](#appendp_C-ad) | [d](#appendp_C-ad) |  |  |  |
| at_C                           | [a](#at_C-adhst) | [d](#at_C-adhst) | [h](#at_C-adhst) | [s](#at_C-adhst) | [t](#at_C-adhst) |
| back_C                         | [a](#back_C-adt) | [d](#back_C-adt) |  |  | [t](#back_C-adt) |
| begin_C                        | [a](#begin_C-adhst) | [d](#begin_C-adhst) | [h](#begin_C-adhst) | [s](#begin_C-adhst) | [t](#begin_C-adhst) |
| binary_rsearch_C               | [a](#binary_rsearch_C-a) |  |  |  |  |
| binary_rsearchp_C              | [a](#binary_rsearchp_C-a) |  |  |  |  |
| binary_search_C                | [a](#binary_search_C-a) |  |  |  |  |
| binary_search_all_C            | [a](#binary_search_all_C-a) |  |  |  |  |
| binary_searchp_C               | [a](#binary_searchp_C-a) |  |  |  |  |
| binary_searchp_all_C           | [a](#binary_searchp_all_C-a) |  |  |  |  |
| check_integrity_C              |  |  |  |  | [t](#check_integrity_C-t) |
| cmp_C                          | [a](#cmp_C-adst) | [d](#cmp_C-adst) |  | [s](#cmp_C-adst) | [t](#cmp_C-adst) |
| cmp_all_C                      | [a](#cmp_all_C-adst) | [d](#cmp_all_C-adst) |  | [s](#cmp_all_C-adst) | [t](#cmp_all_C-adst) |
| cmp_range_C                    | [a](#cmp_range_C-adst) | [d](#cmp_range_C-adst) |  | [s](#cmp_range_C-adst) | [t](#cmp_range_C-adst) |
| copy_C                         | [a](#copy_C-adhst) | [d](#copy_C-adhst) | [h](#copy_C-adhst) | [s](#copy_C-adhst) | [t](#copy_C-adhst) |
| copy_all_C                     | [a](#copy_all_C-adhst) | [d](#copy_all_C-adhst) | [h](#copy_all_C-adhst) | [s](#copy_all_C-adhst) | [t](#copy_all_C-adhst) |
| copy_if_C                      | [a](#copy_if_C-adhst) | [d](#copy_if_C-adhst) | [h](#copy_if_C-adhst) | [s](#copy_if_C-adhst) | [t](#copy_if_C-adhst) |
| copy_if_all_C                  | [a](#copy_if_all_C-adhst) | [d](#copy_if_all_C-adhst) | [h](#copy_if_all_C-adhst) | [s](#copy_if_all_C-adhst) | [t](#copy_if_all_C-adhst) |
| copy_many_C                    | [a](#copy_many_C-adhst) | [d](#copy_many_C-adhst) | [h](#copy_many_C-adhst) | [s](#copy_many_C-adhst) | [t](#copy_many_C-adhst) |
| copy_over_C                    | [a](#copy_over_C-adhst) | [d](#copy_over_C-adhst) | [h](#copy_over_C-adhst) | [s](#copy_over_C-adhst) | [t](#copy_over_C-adhst) |
| distance_C                     | [a](#distance_C-adhst) | [d](#distance_C-adhst) | [h](#distance_C-adhst) | [s](#distance_C-adhst) | [t](#distance_C-adhst) |
| distance_all_C                 | [a](#distance_all_C-adhst) | [d](#distance_all_C-adhst) | [h](#distance_all_C-adhst) | [s](#distance_all_C-adhst) | [t](#distance_all_C-adhst) |
| end_C                          | [a](#end_C-adhst) | [d](#end_C-adhst) | [h](#end_C-adhst) | [s](#end_C-adhst) | [t](#end_C-adhst) |
| equal_all_C                    | [a](#equal_all_C-adst) | [d](#equal_all_C-adst) |  | [s](#equal_all_C-adst) | [t](#equal_all_C-adst) |
| equal_range_C                  | [a](#equal_range_C-adst) | [d](#equal_range_C-adst) |  | [s](#equal_range_C-adst) | [t](#equal_range_C-adst) |
| erase_C                        | [a](#erase_C-adht) | [d](#erase_C-adht) | [h](#erase_C-adht) |  | [t](#erase_C-adht) |
| erase_after_C                  |  |  |  | [s](#erase_after_C-s) |  |
| erase_after_range_C            |  |  |  | [s](#erase_after_range_C-s) |  |
| erase_all_C                    | [a](#erase_all_C-adhst) | [d](#erase_all_C-adhst) | [h](#erase_all_C-adhst) | [s](#erase_all_C-adhst) | [t](#erase_all_C-adhst) |
| erase_front_C                  |  |  |  | [s](#erase_front_C-s) |  |
| erase_range_C                  | [a](#erase_range_C-adhst) | [d](#erase_range_C-adhst) | [h](#erase_range_C-adhst) | [s](#erase_range_C-adhst) | [t](#erase_range_C-adhst) |
| find_C                         | [a](#find_C-adhst) | [d](#find_C-adhst) | [h](#find_C-adhst) | [s](#find_C-adhst) | [t](#find_C-adhst) |
| find_all_C                     | [a](#find_all_C-adhst) | [d](#find_all_C-adhst) | [h](#find_all_C-adhst) | [s](#find_all_C-adhst) | [t](#find_all_C-adhst) |
| findp_C                        | [a](#findp_C-adhst) | [d](#findp_C-adhst) | [h](#findp_C-adhst) | [s](#findp_C-adhst) | [t](#findp_C-adhst) |
| findp_all_C                    | [a](#findp_all_C-adhst) | [d](#findp_all_C-adhst) | [h](#findp_all_C-adhst) | [s](#findp_all_C-adhst) | [t](#findp_all_C-adhst) |
| free_C                         | [a](#free_C-adhst) | [d](#free_C-adhst) | [h](#free_C-adhst) | [s](#free_C-adhst) | [t](#free_C-adhst) |
| free_many_C                    | [a](#free_many_C-adhst) | [d](#free_many_C-adhst) | [h](#free_many_C-adhst) | [s](#free_many_C-adhst) | [t](#free_many_C-adhst) |
| free_node_C                    |  |  |  |  | [t](#free_node_C-t) |
| front_C                        | [a](#front_C-adst) | [d](#front_C-adst) |  | [s](#front_C-adst) | [t](#front_C-adst) |
| get_C                          |  |  | [h](#get_C-ht) |  | [t](#get_C-ht) |
| getp_C                         |  |  | [h](#getp_C-ht) |  | [t](#getp_C-ht) |
| index_C                        | [a](#index_C-adhst) | [d](#index_C-adhst) | [h](#index_C-adhst) | [s](#index_C-adhst) | [t](#index_C-adhst) |
| inorder_C                      |  |  |  |  | [t](#inorder_C-t) |
| insert_C                       | [a](#insert_C-adht) | [d](#insert_C-adht) | [h](#insert_C-adht) |  | [t](#insert_C-adht) |
| insert_after_C                 |  |  |  | [s](#insert_after_C-s) |  |
| insert_gt_C                    | [a](#insert_gt_C-ad) | [d](#insert_gt_C-ad) |  |  |  |
| insert_gteq_C                  | [a](#insert_gteq_C-ad) | [d](#insert_gteq_C-ad) |  |  |  |
| insert_lt_C                    | [a](#insert_lt_C-ad) | [d](#insert_lt_C-ad) |  |  |  |
| insert_lteq_C                  | [a](#insert_lteq_C-ad) | [d](#insert_lteq_C-ad) |  |  |  |
| insertp_C                      | [a](#insertp_C-adht) | [d](#insertp_C-adht) | [h](#insertp_C-adht) |  | [t](#insertp_C-adht) |
| insertp_after_C                |  |  |  | [s](#insertp_after_C-s) |  |
| insertp_gt_C                   | [a](#insertp_gt_C-ad) | [d](#insertp_gt_C-ad) |  |  |  |
| insertp_gteq_C                 | [a](#insertp_gteq_C-ad) | [d](#insertp_gteq_C-ad) |  |  |  |
| insertp_lt_C                   | [a](#insertp_lt_C-ad) | [d](#insertp_lt_C-ad) |  |  |  |
| insertp_lteq_C                 | [a](#insertp_lteq_C-ad) | [d](#insertp_lteq_C-ad) |  |  |  |
| it_C                           | [a](#it_C-adhst) | [d](#it_C-adhst) | [h](#it_C-adhst) | [s](#it_C-adhst) | [t](#it_C-adhst) |
| last_C                         |  |  |  | [s](#last_C-s) |  |
| levelorder_C                   |  |  |  |  | [t](#levelorder_C-t) |
| lower_bound_C                  | [a](#lower_bound_C-a) |  |  |  |  |
| lower_bound_all_C              | [a](#lower_bound_all_C-a) |  |  |  |  |
| lower_boundp_C                 | [a](#lower_boundp_C-a) |  |  |  |  |
| lower_boundp_all_C             | [a](#lower_boundp_all_C-a) |  |  |  |  |
| lower_rbound_C                 | [a](#lower_rbound_C-a) |  |  |  |  |
| lower_rboundp_C                | [a](#lower_rboundp_C-a) |  |  |  |  |
| lt_it_C                        | [a](#lt_it_C-ad) | [d](#lt_it_C-ad) |  |  |  |
| lteq_it_C                      | [a](#lteq_it_C-ad) | [d](#lteq_it_C-ad) |  |  |  |
| new_C                          | [a](#new_C-adhst) | [d](#new_C-adhst) | [h](#new_C-adhst) | [s](#new_C-adhst) | [t](#new_C-adhst) |
| new_from_C                     | [a](#new_from_C-adhst) | [d](#new_from_C-adhst) | [h](#new_from_C-adhst) | [s](#new_from_C-adhst) | [t](#new_from_C-adhst) |
| new_many_C                     | [a](#new_many_C-adhst) | [d](#new_many_C-adhst) | [h](#new_many_C-adhst) | [s](#new_many_C-adhst) | [t](#new_many_C-adhst) |
| new_with_buckets_C             |  |  | [h](#new_with_buckets_C-h) |  |  |
| new_with_capacity_C            | [a](#new_with_capacity_C-a) |  |  |  |  |
| new_with_size_C                | [a](#new_with_size_C-a) |  |  |  |  |
| next_C                         | [a](#next_C-adhst) | [d](#next_C-adhst) | [h](#next_C-adhst) | [s](#next_C-adhst) | [t](#next_C-adhst) |
| postorder_C                    |  |  |  |  | [t](#postorder_C-t) |
| preorder_C                     |  |  |  |  | [t](#preorder_C-t) |
| prepend_C                      | [a](#prepend_C-ads) | [d](#prepend_C-ads) |  | [s](#prepend_C-ads) |  |
| prependp_C                     | [a](#prependp_C-ads) | [d](#prependp_C-ads) |  | [s](#prependp_C-ads) |  |
| prev_C                         | [a](#prev_C-adt) | [d](#prev_C-adt) |  |  | [t](#prev_C-adt) |
| put_C                          | [a](#put_C-adhst) | [d](#put_C-adhst) | [h](#put_C-adhst) | [s](#put_C-adhst) | [t](#put_C-adhst) |
| putp_C                         | [a](#putp_C-ad) | [d](#putp_C-ad) |  |  |  |
| random_shuffle_C               | [a](#random_shuffle_C-ad) | [d](#random_shuffle_C-ad) |  |  |  |
| random_shuffle_all_C           | [a](#random_shuffle_all_C-ad) | [d](#random_shuffle_all_C-ad) |  |  |  |
| rappend_C                      | [a](#rappend_C-a) |  |  |  |  |
| rat_C                          | [a](#rat_C-adt) | [d](#rat_C-adt) |  |  | [t](#rat_C-adt) |
| rback_C                        | [a](#rback_C-a) |  |  |  |  |
| rbegin_C                       | [a](#rbegin_C-adt) | [d](#rbegin_C-adt) |  |  | [t](#rbegin_C-adt) |
| rcmp_C                         | [a](#rcmp_C-adt) | [d](#rcmp_C-adt) |  |  | [t](#rcmp_C-adt) |
| rcmp_all_C                     | [a](#rcmp_all_C-adt) | [d](#rcmp_all_C-adt) |  |  | [t](#rcmp_all_C-adt) |
| rcmp_range_C                   | [a](#rcmp_range_C-adt) | [d](#rcmp_range_C-adt) |  |  | [t](#rcmp_range_C-adt) |
| rcopy_C                        | [a](#rcopy_C-adt) | [d](#rcopy_C-adt) |  |  | [t](#rcopy_C-adt) |
| rcopy_all_C                    | [a](#rcopy_all_C-adt) | [d](#rcopy_all_C-adt) |  |  | [t](#rcopy_all_C-adt) |
| rdistance_C                    | [a](#rdistance_C-adt) | [d](#rdistance_C-adt) |  |  | [t](#rdistance_C-adt) |
| rdistance_all_C                |  |  |  |  | [t](#rdistance_all_C-t) |
| rehash_C                       |  |  | [h](#rehash_C-h) |  |  |
| remove_C                       |  |  | [h](#remove_C-ht) |  | [t](#remove_C-ht) |
| removep_C                      |  |  | [h](#removep_C-h) |  |  |
| rend_C                         | [a](#rend_C-adt) | [d](#rend_C-adt) |  |  | [t](#rend_C-adt) |
| requal_all_C                   | [a](#requal_all_C-adt) | [d](#requal_all_C-adt) |  |  | [t](#requal_all_C-adt) |
| requal_range_C                 | [a](#requal_range_C-adt) | [d](#requal_range_C-adt) |  |  | [t](#requal_range_C-adt) |
| reverse_C                      | [a](#reverse_C-ad) | [d](#reverse_C-ad) |  |  |  |
| reverse_all_C                  | [a](#reverse_all_C-ads) | [d](#reverse_all_C-ads) |  | [s](#reverse_all_C-ads) |  |
| reverseorder_C                 |  |  |  |  | [t](#reverseorder_C-t) |
| rfind_C                        | [a](#rfind_C-adt) | [d](#rfind_C-adt) |  |  | [t](#rfind_C-adt) |
| rfindp_C                       | [a](#rfindp_C-adt) | [d](#rfindp_C-adt) |  |  | [t](#rfindp_C-adt) |
| rfront_C                       | [a](#rfront_C-a) |  |  |  |  |
| rinsert_C                      | [a](#rinsert_C-a) |  |  |  |  |
| rit_C                          | [a](#rit_C-adt) | [d](#rit_C-adt) |  |  | [t](#rit_C-adt) |
| rlt_it_C                       | [a](#rlt_it_C-ad) | [d](#rlt_it_C-ad) |  |  |  |
| rlteq_it_C                     | [a](#rlteq_it_C-ad) | [d](#rlteq_it_C-ad) |  |  |  |
| rnext_C                        | [a](#rnext_C-adt) | [d](#rnext_C-adt) |  |  | [t](#rnext_C-adt) |
| rprepend_C                     | [a](#rprepend_C-a) |  |  |  |  |
| rprependp_C                    | [a](#rprependp_C-a) |  |  |  |  |
| rprev_C                        | [a](#rprev_C-adt) | [d](#rprev_C-adt) |  |  | [t](#rprev_C-adt) |
| rsearch_C                      | [a](#rsearch_C-adt) | [d](#rsearch_C-adt) |  |  | [t](#rsearch_C-adt) |
| rsearchp_C                     | [a](#rsearchp_C-adt) | [d](#rsearchp_C-adt) |  |  | [t](#rsearchp_C-adt) |
| rsort_C                        | [a](#rsort_C-ad) | [d](#rsort_C-ad) |  |  |  |
| rsort_all_C                    | [a](#rsort_all_C-ad) | [d](#rsort_all_C-ad) |  |  |  |
| rstable_sort_C                 | [a](#rstable_sort_C-ad) | [d](#rstable_sort_C-ad) |  |  |  |
| rswap_C                        | [a](#rswap_C-adt) | [d](#rswap_C-adt) |  |  | [t](#rswap_C-adt) |
| search_C                       | [a](#search_C-adst) | [d](#search_C-adst) |  | [s](#search_C-adst) | [t](#search_C-adst) |
| search_all_C                   | [a](#search_all_C-adst) | [d](#search_all_C-adst) |  | [s](#search_all_C-adst) | [t](#search_all_C-adst) |
| searchp_C                      | [a](#searchp_C-adst) | [d](#searchp_C-adst) |  | [s](#searchp_C-adst) | [t](#searchp_C-adst) |
| searchp_all_C                  | [a](#searchp_all_C-adst) | [d](#searchp_all_C-adst) |  | [s](#searchp_all_C-adst) | [t](#searchp_all_C-adst) |
| set_exact_size_C               | [a](#set_exact_size_C-ads) | [d](#set_exact_size_C-ads) |  | [s](#set_exact_size_C-ads) |  |
| set_min_size_C                 | [a](#set_min_size_C-ads) | [d](#set_min_size_C-ads) |  | [s](#set_min_size_C-ads) |  |
| size_C                         | [a](#size_C-a) |  |  |  |  |
| sort_C                         | [a](#sort_C-ad) | [d](#sort_C-ad) |  |  |  |
| sort_all_C                     | [a](#sort_all_C-ad) | [d](#sort_all_C-ad) |  |  |  |
| stable_sort_C                  | [a](#stable_sort_C-ad) | [d](#stable_sort_C-ad) |  |  |  |
| stable_sort_all_C              | [a](#stable_sort_all_C-ads) | [d](#stable_sort_all_C-ads) |  | [s](#stable_sort_all_C-ads) |  |
| swap_C                         | [a](#swap_C-adhst) | [d](#swap_C-adhst) | [h](#swap_C-adhst) | [s](#swap_C-adhst) | [t](#swap_C-adhst) |
