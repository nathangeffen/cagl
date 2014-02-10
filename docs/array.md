# Arrays

CAGL arrays (ARRAY) are random access containers that grow automatically. Appending to and access elements in arrays is efficient. Inserting elements at arbitrary positions in the array is increasingly less efficient, the closer to the front of the array the insertion takes place (worst case is O(n), where n is the number of elements in the array).

Iterators for these container types are random access. ARRAYs can be unordered  or ordered. Ordered ARRAYs should be declared and defined with *CMP* macros. It is ok for an ordered array to become unordered, but certain functions (like *binary_search_C*) will then give undefined results. An array can be ordered again by calling one of the sorting functions, e.g. *sort_C* or *stable_sort_C*.

ARRAYs are intended to provide similar functionality to the C++ STL vector.

### ARRAY declaration and definition macros {-}

- [CAG_DEC_ARRAY](#cag_dec_array)
- [CAG_DEF_ARRAY](#cag_def_array)
- [CAG_DEC_DEF_ARRAY](#cag_dec_def_array)
- [CAG_DEC_CMP_ARRAY](#cag_dec_cmp_array)
- [CAG_DEF_CMP_ARRAY](#cag_def_cmp_array)
- [CAG_DEC_DEF_CMP_ARRAY](#cag_dec_def_cmp_array)
- [CAG_DEC_CMPP_ARRAY](#cag_dec_cmpp_array)
- [CAG_DEF_CMPP_ARRAY](#cag_def_cmpp_array)
- [CAG_DEC_DEF_CMPP_ARRAY](#cag_dec_def_cmpp_array)
- [CAG_DEF_ALL_ARRAY](#cag_def_all_array)
- [CAG_DEF_ALL_CMP_ARRAY](#cag_def_all_cmp_array)

### ARRAY other useful macros {-}

- [CAG_SORT_ARRAY](#cag_sort_array)

### ARRAY function blueprints {-}

- [append_C](#append_C-ad)
- [appendp_C](#appendp_C-ad)
- [at_C](#at_C-adhst)
- [back_C](#back_C-adst)
- [begin_C](#begin_C-adhst)
- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)
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
- [lower_bound_C](#lower_bound_C-a)
- [lower_bound_all_C](#lower_bound_all_C-a)
- [lower_boundp_C](#lower_boundp_C-a)
- [lower_boundp_all_C](#lower_boundp_all_C-a)
- [lt_it_C](#lt_it_C-ad)
- [lteq_it_C](#lteq_it_C-ad)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [new_with_capacity_C](#new_with_capacity_C-a)
- [new_with_size_C](#new_with_size_C-a)
- [next_C](#next_C-adhst)
- [prepend_C](#prepend_C-ads)
- [prependp_C](#prependp_C-ads)
- [prev_C](#prev_C-adt)
- [put_C](#put_C-adhst)
- [putp_C](#putp_C-ad)
- [random_shuffle_C](#random_shuffle_C-ad)
- [random_shuffle_all_C](#random_shuffle_all_C-ad)
- [rappend_C](#rappend_C-a)
- [rat_C](#rat_C-adt)
- [rback_C](#rback_C-a)
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
- [rfront_C](#rfront_C-a)
- [rinsert_C](#rinsert_C-a)
- [rit_C](#rit_C-adt)
- [rlt_it_C](#rlt_it_C-ad)
- [rlteq_it_C](#rlteq_it_C-ad)
- [rnext_C](#rnext_C-adt)
- [rprepend_C](#rprepend_C-a)
- [rprependp_C](#rprependp_C-a)
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
- [size_C](#size_C-a)
- [sort_C](#sort_C-ad)
- [sort_all_C](#sort_all_C-ad)
- [stable_sort_C](#stable_sort_C-ad)
- [stable_sort_all_C](#stable_sort_all_C-ads)
- [swap_C](#swap_C-adhst)


## ARRAY structs and functions {-}

#### Iterator structs and typedefs {-}

```C
struct iterator_C {
    T value;
};
struct reverse_iterator_C {
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
    size_t capacity; /* Treat as read-only */
    ... /* internal variables */
};
typedef struct C C;
```



### New and free functions {-}

There are several functions to create (new) and destroy (free) arrays. Usually users would use *new_C* and *free_C*.

Every CAGL container must have a *new* function executed on it before any other operations. Failure to do this will result in writing to uninitialized or unallocated memory.

Every CAGL container should have a *free* function executed on it when it is no longer needed. Failure to do this will result in memory leaks.

```C
/* Initialize *array* and return pointer to it. */
C *new_C(C * array);

/* Initialize *array* and allocate space for *capacity* elements.
   Returns pointer to *array*. */
C *new_with_capacity_C(C * array, const size_t capacity);

/* Initialize array and allocate space for at least *size* new elements.
   Fill the array with size elements.
*/
C *new_with_size_C(C * array, const size_t size);

/* Applies new to *to* and copies the contents of *from* into it.
   Essential for containers that are the elements of containers and whose
   memory must be managed by the parent container.
   Returns a pointer to *to*.
   O(n) operation, where n is the number of elements in *from*.
*/
C *new_from_C(C * to, const C * from);

/* Call *new_C* on every argument (which must all be pointers to container
   variables) except the last which must be NULL. Returns immediately if an
   error occurs (i.e. no further container variables will be initialized).
   Returns the number of successfully initialized container variables.
   While marginally less efficient than calling *new_C* yourself on every
   container, this is convenient in functions that have to initialize many
   container variables. Works well in conjunction with *free_many_C*.
*/
int new_many_C(C * c, ...);

/* Destroys *array* and returns its memory to heap.
   This should be called when the array is no longer used.
*/
void free_C(C * array);

/* Calls *free_C* on a maximum of *max* container variables specified by the
   second argument onwards. The last argument must be NULL. This is slightly
   less  efficient than calling *free_C* yourself on each variable, but
   this can be convenient in functions that need to use many containers.
*/
void free_many_C(int max, C * c1, ...);
```

### Iterator functions {-}

```C
/* Set the array to have a minimum number of *size* elements.
   Useful before calling *copy_over*.
*/
it_C set_min_size_C(C * array, it_C it, const size_t size);

/* Set the array to have exactly *size* elements. If there are already more than
   size elements, erase the excess ones.  Useful before calling *copy_over*,
   but beware that data can be erased.
*/
it_C set_exact_size_C(C * array, const size_t size);

/* Return the next iterator in the array.
   Forward and reverse iterator versions supplied.
   For arrays these are equivalent to ++it and --it respectively.
*/
it_C next_C(it_C const it);
rit_C rnext_C(rit_C const it);


/* Return the previous iterator in the array.
   Forward and reverse iterator versions supplied.
   For arrays these are equivalent to --it and ++it respectively.
*/
it_C prev_C(it_C const it);
rit_C rprev_C(rit_C const it);


/* Return the iterator *n* elements away from *it*.
   Forward and reverse iterator versions supplied.
   For arrays Equivalent to  it + n and it - n respectively.
*/
it_C at_C(it_C it, const size_t n);
rit_C rat_C(rit_C it, const size_t n);

/* Returns an iterator to the *n*th element of the array. */
it_C index_C(C * array, size_t n);

/* Return the number of elements from *first* to *last* excluding *last*.
   Forward and reverse iterator versions supplied, as well as *all* version
   over entire container. *size_C* is equivalent to *distance_all* but is
   only available for arrays.

   For arrays, equivalent to: last - first and first - last for forward and
   reverse iterator versions respectively.
*/
size_t distance_C(const it_C first, const it_C last);
size_t rdistance_C(const rit_C first, const rit_C last);
size_t distance_all_C(const C * c);
size_t size_C(const C * array);

/* Indicates whether *it1* comes before *it2*. If it does then
   returns 1 else 0.
   Forward and reverse iterator versions supplied.
*/
int lt_it_C(const it_C it1, const it_C it2);
int rlt_it_C(const rit_C it1, const rit_C it2);

/* Indicates whether *it1* comes before or is the same position as *it2*.
   If it is then returns 1 else 0.
   Forward and reverse iterator versions supplied.
*/
int lteq_it_C(const it_C it1, const it_C it2);
int rlteq_it_C(const rit_C it1, const rit_C it2);

/* Returns an iterator pointing to the first element in the array (or last
   element for reverse iterator version).
   Forward and reverse iterator versions supplied.
   *beg_C* and *rbeg_C* are synonyms for *begin_C* and *rbegin_C* respectively.
*/
it_C begin_C(const C * array);
it_C beg_C = begin_C;
rit_C rbegin_C(const C * array);
rit_C rbeg_C = rbegin_C;


/* Returns an iterator pointing one past the last element in the array (or
   one before the first element for reverse iterator version).
   Forward and reverse iterator versions supplied.
*/
it_C end_C(const C * array);
rit_C rend_C(const C * array);
```


### Element insertion functions {-}


/* Prepends *element* to the beginning of *array*. Versions for passing the
   element by value and by address are provided.
   Returns iterator pointing to the prepended element if successful, else NULL.
   This is a slow O(n) time way of adding elements to an array.
*/
it_C prepend_C(C * array, T const element);
it_C prependp_C(C * array, T const *element);


/* Inserts *element* before *position* into *array*. Versions for passing the
   element by value and by address are provided.
   Returns iterator pointing to the prepended element if successful, else NULL.
   Every container is required to have a *put_C* function which inserts an
   element. Unless you're writing new containers, you're unlikely to need to
   use it.
   The efficiency of these functions is proportional to the size of the array
   minus *position*. The higher the value of *position*, the quicker the
   operation.
*/
it_C insert_C(C * array, it_C position, T const element);
it_C insertp_C(C * array, it_C position, T const *element);
it_C put_C(C * array, it_C position, T const element);
```

#### Ordered insertions {-}

```C
/* Ordered insertion functions are only available to containers defined with a
   *CMP* macro (i.e. have a user-supplied *cmp_func*), support bidirectional
   iterators and are reorderable.  Currently the only CAGL containers that
   support these are arrays and doubly linked lists.

   Returns an iterator to the inserted element.
*/

/* Insert *element* at the first position encountered for which a comparison
   is true, starting from *position*.

   These are only defined for reorderable CAGL containers (currently only arrays
   and doubly linked lists) for which a *cmp_func* has been supplied by the
   user via a container declaration macro with CMP in it.

   By value and address versions are supplied.

   The comparisons are determined by the second name of the function after
   *insert_* and *insertp_*:

   - gt - the element is inserted at the first position at which it would
     be  greater than all elements starting at *position*.

   - gteq - the element is inserted at the first position at which it would
     be  greater than or equal to all elements starting at *position*.

   - lt - the element is inserted at the first position at which it would
     be  less than all elements starting at *position*.

   - lteq - the element is inserted at the first position at which it would
     be  less than or equal to all elements starting at *position*.

     Returns an iterator pointing to the element that has been inserted.
*/
it_C insert_gt_C(C * array, it_C position, T const element);
it_C insert_gteq_C(C * array, it_C position, T const element);
it_C insert_lt_C(C * array, it_C position, T const element);
it_C insert_lteq_C(C * array, it_C position, T const element);
it_C insertp_gt_C(C * array, it_C position, T const *element);
it_C insertp_gteq_C(C * array, it_C position, T const *element);
it_C insertp_lt_C(C * array, it_C position, T const *element);
it_C insertp_lteq_C(C * array, it_C position, T const *element);
```

### Element retrieval functions {-}

```C
/* Returns pointer to first element of *array*. */
T *front_C(const C * array);

/* Returns pointer to last element of array. */
T *back_C(const C * array);
```

### Comparison functions {-}

```C
/* Comparison functions are only defined for container types declared with a
   definition macro that has CMP in it's name. They make use of the *cmp_func*
   function that the user has supplied.
*/

/* Compares the element in each of two iterators by calling the user-supplied
   *cmp_func* function and returning its return value.

   Return value:

   - If *cmp_func* returns a non-zero value for elements at corresponding
     positions in the two ranges being compared, then this is the value
     returned.

   - If all corresponding elements are equal (i.e. *cmp_func* returns 0 for all
     comparisons) and both ranges (or containers) have exactly the same number
     of elements, then 0 is returned (i.e. the ranges compared are precisely
     equal).

   - If all corresponding elements are equal (i.e. *cmp_func* returns 0 for all
     comparisons) and the first range (or container) has more elements than the
     second one, then 1 is returned (i.e. the first range is greater than the
     second range).

   - If all corresponding elements are equal (i.e. *cmp_func* returns 0 for all
     comparisons) and the second range (or container) has more elements than the
     first one, then 01 is returned (i.e. the first range is less than the
     second range).

   The reverse iterator versions compare the ranges or containers by moving in
   opposite directions, i.e. they test to see if the one range is exactly equal
   to the reverse of the other.

   See also *equal_range_C*.
*/
int cmp_C(const it_C it1, const it_C it2);
int cmp_range_C(it_C from_1, const it_C to_1, it_C from_2,
		const it_C to_2);
int cmp_all_C(const C * c1, const C * c2);
int rcmp_C(const rit_C it1, const it_C it2);
int rcmp_range_C(rit_C from_1, const rit_C to_1, it_C from_2,
		 const it_C to_2);
int rcmp_all_C(const C * c1, const C * c2);

/* Compares the elements in a range by calling the user-supplied
   *cmp_func* function and returning its return value. The comparisons
   stop once the first range or container has been iterated.

   If there are fewer elements in the second range or container than the first,
   then behaviour is undefined.

   Returns 0 if all calls to *cmp_func* return zero, else returns the first
   non-zero value of cmp_func.

   The reverse iterator versions compare the ranges or containers by moving in
   opposite directions, i.e. they test to see if the one range is exactly equal
   to the reverse of the other.
*/
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
int equal_all_C(const C * c1, const C * c2);
int requal_range_C(rit_C from_1, const rit_C to_1, it_C from_2);
int requal_all_C(const C * c1, const C * c2);
```

### Erase functions {-}

```C
/* Erase the element pointed to by *it* from *array*.

   Returns iterator pointing to element that will now occupy the space
   where the erased element was.

   Time complexity: O(n) on average where n is the number of elements in
   the array.

*/
it_C erase_C(C * array, it_C it);

/* Erases all elements in the range [first, last).
   Returns *last*.
*/
it_C erase_range_C(C * c, it_C first, it_C last);

/* Empties *array*. The size (*distance_all_C*) of *array* will be zero after
   this.
   Returns an iterator equivalent to *end_C(array)*.
*/
it_C erase_all_C(C * array);
```

### Reordering functions {-}

#### Sorting {-}

```C
/* Sort the elements [first, last). Only defined for containers with comparison
   functions. Reverse iterator and *all* versions supplied.
   To sort in reverse order, use the *rsort* versions.
   Average efficiency O(n log n). While worst case is O(n^2), it is extremely
   unlikely to occur. Implemented as a highly optimized Quicksort.
*/
it_C sort_C(it_C first, it_C last);
it_C sort_all_C(C * c);
rit_C rsort_C(rit_C first, rit_C last);
rit_C rsort_all_C(C * c);

/* Stable sorts the elements [first, last). In contrast to *sort_C* these
   functions maintain the same order of elements whose keys are equal.

   Only defined for containers with comparison functions. Reverse iterator and
   *all* versions supplied.

   To sort in reverse order, use the *rsort* versions.
   Efficiency is O(n log n). Implemented as a Mergesort.
*/
it_C stable_sort_C(it_C first, it_C last);
rit_C rstable_sort_C(rit_C first, rit_C last);
it_C stable_sort_all_C(C * c);
```

#### Reversing {-}

```C
/* Reverses elements in the range[first, last). *range* and *all* versions
   supplied.

   Returns an iterator to the new first element in the range.

   O(n) operation, where n is the size of the range.
*/
it_C reverse_C(it_C first, it_C last);
it_C reverse_all_C(C * array);
```

#### Shuffling {-}

```C
/* Shuffles the elements in the range [first, last). *range* and *all* versions
   supplied.
   Returns an iterator to the new first element in the range.

   Time complexity: This is an O(n) operation, where n is the size of the range.
*/
it_C random_shuffle_C(const it_C first, it_C last);
it_C random_shuffle_all_C(C * c);
```

### Swap functions {-}

```C
/* Swaps the elements of two iterators.*/
void swap_C(it_C a, it_C b);
void rswap_C(rit_C a, rit_C b);
```

### Copy functions {-}

CAGL supports two types of copying functions. The *copy_over_C* functions are
similar to the C++ STL *copy* template function.  For these you must have
sufficient space in the container to which you're copying. On the other hand
*copy_C* and *copy_all_C* are similar to C++ copy constructors. When they copy,
they allocate memory for each element in the copied to array.


```C
/* Copies the elements in the range [first, last) into *c*, which must be
   initialized but is generally empty. Space is allocated for *c*.

   The reverse iterator version copies the elements in reverse to *c*.

   Returns pointer to *c* upon success, else NULL.
*/
C *copy_C(it_C first, it_C last, C * c);
C *rcopy_C(rit_C first, rit_C last, C * c);

/* Copies all the elements in *c1* to *c2*,  which must be
   initialized but is generally empty. Space is allocated for *c2*.

   The reverse iterator version copies the elements in reverse to *c2*.

   Returns pointer to *c2* upon success, else NULL.
*/
C *copy_all_C(const C * c1, C * c2);
C *rcopy_all_C(const C * c1, C * c2);

/* Copies *c* to each of its subsequent arguments, which must be containers
   of the same type as *c*. Last parameter must be NULL. Space is allocated
   for each of the recipient containers.
   Returns the number of containers which have been successfully copied to.
*/
int copy_many_C(C * c, ...);

/* Copies the elements in the range [first, last) into *c*, which must be
   initialized but is generally empty, for those elements for which *cond_func*
   returns true.
   Space is allocated for *c*.
   Returns pointer to *c* upon success, else NULL.
*/
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);

/* Copies elements in *c1*, for which *cond_func* returns TRUE,
   to *c2*,  which must be initialized but is generally empty.
   Space is allocated for *c2*.
   Returns pointer to *c2* upon success, else NULL.
*/
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);

/* Copies elements in the range [first, last) over result for as many elements
   as there are from *first* to *last*.
   There must be sufficient space in the remainder of the container to which
   *result* points.
*/
it_C copy_over_C(it_C first, const it_C last, it_C result);
```

### Searching {-}

#### Linear search {-}

```C
/* Linear search for *element* in the range [first, last).

   This differs from the *search_C* functions in that these are available to all
   containers, but in contrast to the *search_C* functions, a *cmp_func*
   function must be passed as a parameter to these functions.

   By value, by address, *all*, forward and reverse iterator versions supplied.

   The *cmp_func* function, which is passed as a parameter, should return 0 if
   its two parameters are equal. It takes both its operands by address.

   Returns an iterator pointing to an element if found, else *to*.

   Time complexity: O(n) where n is the number of elements in the range.
*/
it_C find_C(it_C first, const it_C last, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C first, const it_C last, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
rit_C rfind_C(rit_C first, const rit_C last, const T element,
	      int (*cmp_func) (const T *, const T *));
rit_C rfindp_C(rit_C first, const rit_C last, const T * element,
	       int (*cmp_func) (const T *, const T *));

/* Linear search for *element* in the range [first, last).

   This differs from the *find_C* functions in that these are only available
   to types declared with container declaration macros that have *CMP* in them.
   They make use of the supplied *cmp_func* function.

   By value, by address, *all*, forward and reverse iterator versions supplied.

   Returns an iterator pointing to an element if found, else *to*.
   In the *all* versions, if the keys are not *end_C(c)* is returned.

   Time complexity: O(n) where n is the number of elements in the range.
*/
it_C search_C(it_C first, const it_C last, T const key);
it_C searchp_C(it_C first, const it_C last, T const *key);
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);
it_C search_all_C(C * c, T d);
it_C searchp_all_C(C * c, T * d);
```

#### Binary search {-}

```C
/* Binary search for the lower bound of *key*. The lower bound is the first
   element less than or equal to the key.

   These are only available to ordered types declared with container declaration
   macros that have *CMP* in them. They make use of the supplied *cmp_func*
   function. Currently these functions are supported for arrays, doubly-linked
   lists and trees.

   Returns an iterator pointing to the lower bound element. If all the elements
   in the range compare less than *element*, the function returns *last* (as
   with the C++ STL).
*/
it_C lower_bound_C(it_C first, it_C last, T const key);
it_C lower_boundp_C(it_C first, it_C last, T const *key);
it_C lower_bound_all_C(C * c, T d);
it_C lower_boundp_all_C(C * c, T * d);

/* Binary search for an element equal to *key*.

   These are only available to ordered types declared with container declaration
   macros that have *CMP* in them. They make use of the supplied *cmp_func*
   function. Currently these functions are supported for arrays, doubly-linked
   lists and trees.

   Returns true if the element is found else false.
*/
int binary_search_C(it_C first, it_C last, T const key);
int binary_searchp_C(it_C first, it_C last, T const *key);
int binary_search_all_C(C * c, T d);
int binary_searchp_all_C(C * c, T * d);
```
