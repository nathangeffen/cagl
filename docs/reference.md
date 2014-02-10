# Appendix: Function blueprint reference {#functions -}

**WORK IN PROGRESS ON THIS PART OF THE MANUAL**

*This part of the manual is going to be a long work in progress. The aim is to document every CAGL function blueprint.*

In this reference:

- A function *blueprint* describes the form of a CAGL function that is generated with one of the CAGL container macros that begin *CAG_DEC_*.

- Substitute *C* with the name of the container type.

- Substitute *T* with the container's element type.

- The words *array*, *dlist*, *hash*, *tree* and *slist* refer to the CAGL container types array, doubly-linked list, hash table, binary tree and singly-linked list respectively.

- A container is comprised of *elements*. These are the data (or values) of a container. An element is a type, specified by the user when the container is defined.

- For simplicity we talk about iterators pointing to an element. Strictly speaking iterators do not point to elements. Instead iterators are pointers to structs which have a variable called *value*. It is *value* that stores the element. However an element can have a pointer to it and some functions return pointers to elements that are not elements. Compare *front_C* with *begin_C*. The former returns a pointer to an element. The latter returns an iterator that we say, arguably a little loosely, points to an element. The type for _*front_C(c)*_ is the same type as _begin_C(c)->value_.

- The word *user* refers to a programmer using CAGL.

- A function blueprint often has several variations for which the differences are subtle. In most cases it won't matter which version you use so long as you get the minor differences in syntax right. But sometimes, one variation will offer a significant optimisation for a particular task at hand or significant convenience. As an example, consider *search_C*, which does a linear search through a range in a container for a specified element, passed by value. It has these variations:

    - search_all_C: Identical to search_C except it searches the entire container, not a sub-range of it. *search_C(beg_C(c), end_C(c), element)* is identical to *search_all_C(c, element)*.
	- searchp_C: Identical to search_C except that the element is passed by address. For large structs this is more efficient than the pass by value version (search_C). But if the element type is a primitive type, e.g. an int, or a  pointer, e.g. _char *_, then it makes more sense to use search_C.
	- searchp_all_C: Identical to search_all_C but the element is passed by address.

    Then there are reverse iterator versions of search_C and searchp_C called rsearch_C and rsearchp_C. They can be used to search backward through a range.

	Note there are no rsearch_all_C or rsearchp_all_C function blueprints because the *_all_* blueprints do not take iterator parameters and a reverse version would offer no additional functionality. If you need a reverse iterator returned, simply cast the return value of search_all_C or searchp_all_C to a *rit_C* type, or alternatively call rsearch_all_C(rbeg_C(c), rend_C(c)). However for some function blueprints there are reverse iterator function blueprints for the whole container. For example, *rsort_all_C* sorts an entire container in reverse order.


------


#### append_C {#append_C-ad - }

Append an element to the end of a container. Pass by value version.

```C
it_C append_C(C *c, T const element);
```

Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to append to.
element
  ~ Element to append.

##### Return value {-}

Iterator pointing to the prepended element if successful, else NULL.

##### Example {-}

```C
/* Appends elements to a list and then prints them out. */

#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	it_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (append_int_dlist(&l, i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
	for (it = beg_int_dlist(&l); it != end_int_dlist(&l);
	     it = next_int_dlist(it))
		printf("%d\n", it->value);
	free_int_dlist(&l);
	return 0;
}
```

The program will output this:


    0
    1
    2
    3
    4
    5
    6
    7
    8
    9


##### Complexity {-}

This is a constant time operation for both arrays and doubly-linked lists.

##### Data races {-}

The container is modified.

##### See also {-}

- [appendp_C](#appendp_C-ad)
- [rappend_C](#rappend_C-a)
- [prepend_C](#prepend_C-ads)
- [prependp_C](#prependp_C-ads)
- [rprepend_C](#rprepend_C-a)
- [rprependp_C](#rprependp_C-a)

------


#### appendp_C {#appendp_C-ad - }

Append an element to the end of a container. Pass by address version.

```C
it_C appendp_C(C *c, T const *element);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to append to.

element
  ~ Element to append.


##### Return value {-}

Iterator pointing to the prepended element if successful, else NULL.

##### Example {-}

```C
#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	it_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (appendp_int_dlist(&l, &i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
	for (it = beg_int_dlist(&l); it != end_int_dlist(&l);
	     it = next_int_dlist(it))
		printf("%d\n", it->value);
	free_int_dlist(&l);
	return 0;
}
```

The program will output this:


    0
    1
    2
    3
    4
    5
    6
    7
    8
    9


##### Complexity {-}

This is a constant time operation for both arrays and doubly-linked lists.

##### Data races {-}

The container is modified.

##### See also {-}

- [append_C](#append__C-ad)
- [rappend_C](#rappend_C-a)
- [prepend_C](#prepend_C-ads)
- [prependp_C](#prependp_C-ads)
- [rprepend_C](#rprepend_C-a)
- [rprependp_C](#rprependp_C-a)

------


#### at_C {#at_C-adhst - }

Get an iterator at a specified position.

```C
it_C at_C(it_C it, const size_t n);
```

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

it
  ~ Iterator pointing to an element in a container

n
  ~ Number of elements (distance) offset from the position of *it*. Must not be larger than the distance to the end of the container.

##### Return value {-}

Iterator that is *n* elements from *it*.

##### Example {-}

```C
#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	it_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (append_int_dlist(&l, i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
	it = beg_int_dlist(&l);
	while(it != end_int_dlist(&l)) {
		printf("%d\n", it->value);
		it = at_int_dlist(it, 2);
	}
	free_int_dlist(&l);
	return 0;
}

```

The output is:

```
0
2
4
6
8
```

##### Complexity {-}

For arrays, this is a random access constant time operation.

For trees, lists and hash tables, this will take O(n) time where *n* is the second parameter.

##### Data races {-}

This is thread-safe.

For trees, lists and hash tables, the iterator parameter is modified *n* times but never the values to which it points.

For arrays, only local storage is modified.

##### See also {-}

- [index_C](#index_C-adhst)
- [rat_C](#rat_C-adt)

------


#### back_C {#back_C-adst - }

Returns pointer to last element of a container.

```C
T *back_C(const C *C);
```


Containers:
array dlist slist tree


##### Parameters {-}

c
  ~ Container from which the last element will be retrieved.

#### Return value {-}

Pointer to last element in *c*.

##### Example {-}

```C
/* Populate a tree from 10 down to 0 then retrieve back element which should be
   10.
*/

#include <stdio.h>
#include <cagl/tree.h>

CAG_DEC_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);

int main(void)
{
	int_tree t;
	int i;

	if(new_int_tree(&t) == NULL) {
		fprintf(stderr, "Error creating new tree\n");
		exit(1);
	}
	for (i = 10; i > 0; --i)
		if (insert_int_tree(&t, i) == NULL) {
			fprintf(stderr, "Error creating new tree\n");
			exit(1);
		}
    /* Last element is pointer to highest value. */
	i = *back_int_tree(&t);
	printf("Last element in tree is: %d\n", i);
	free_int_tree(&t);
	return 0;
}
```

The output is:

```
10
```

#### Complexity {-}

For ARRAY, DLIST and TREE, this is a constant time operation. For SLIST, it is $\theta(n)$ where $n$ is the number of elements in the container.

##### Data races {-}

No data is modified and no elements are accessed.

#### See also {-}

- [front_C](#front_C-adst)
- [rfront_C](#rfront_C-a)
- [rback_C](#rback_C-a)

------


#### begin_C {#begin_C-adhst - }

Returns an iterator pointing to the first element in a container.

Note that *beg_C* is an abbreviated synonym for this.

```C
it_C begin_C(const C *c);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container from which the beginning will be retrieved.

#### Return value {-}

Iterator pointing to first element in *c*.

##### Example {-}

```C
/* Appends elements to a list and then prints them out. */

#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	it_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (append_int_dlist(&l, i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
    /* Either begin_int_dlist or beg_int_dlist can be used here.
	   They are synonyms.
	*/
	for (it = beg_int_dlist(&l); it != end_int_dlist(&l);
	     it = next_int_dlist(it))
		printf("%d\n", it->value);
	free_int_dlist(&l);
	return 0;
}
```


#### Complexity {-}

This is a constant time operation for all container types.

##### Data races {-}

No data is modified and no element data is accessed.

#### See also {-}

- [rbegin_C](#rbegin_C-adt)
- [end_C](#end_C-adhst)
- [rend_C](#rend_C-adt)

------


#### binary_rsearch_C {#binary_rsearch_C-a - }

Binary search for an element by key in reverse.

The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro. This is a reverse search so the element pointed to by *first* must be greater than or equal to the one pointed to by *last*.

```C
int binary_rsearch_C(rit_C first, rit_C last, T const key);
```

Containers:
array (only if declared with macro containing CMP)


##### Parameters {-}

first
  ~ Reverse iterator pointing to element to start search.

last
  ~ Reverse iterator pointing to element to end search

key
  ~ Element to search for, passed by value.

#### Return value {-}

Returns true if the element is in the semi-open range [first, last) else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of in the range [first, last).

##### Data races {-}

The iterator parameters *first* and *last* are modified, but not their elements. Each accessed element is read only once.

#### See also {-}

- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### binary_rsearchp_C {#binary_rsearchp_C-a - }

Binary search for an element by key in reverse. The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro. This is a reverse search so the element pointed to by *first* must be greater than or equal to the one pointed to by *last*.

```C
int binary_rsearchp_C(rit_C first, rit_C last, T const *key);
```


Containers:
array (only if declared with macro containing CMP)


##### Parameters {-}


first
  ~ Reverse iterator pointing to element to start search.

last
  ~ Reverse iterator pointing to element to end search

key
  ~ Element to search for, passed by address.

#### Return value {-}

Returns true if the element is in the semi-open range [first, last) else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of in the range [first, last).


##### Data races {-}

The iterator parameters *first* and *last* are modified, but not their elements. Each accessed element is read only once.

#### See also {-}

- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### binary_search_C {#binary_search_C-a - }

Binary search for an element, with a specified key. The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro.


```C
int binary_search_C(it_C first, it_C last, T const key);
```


Containers:
array (only if declared with macro containing CMP)


##### Parameters {-}

first
  ~ Iterator pointing to element to start search. This must come before *last* in the container.

last
  ~ Iterator pointing to element to end search. This must come after *first* in the container.

key
  ~ Element to search for, passed by value.

#### Return value {-}

True if the element is in the semi-open range [first, last) else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of in the range [first, last).

##### Data races {-}

The iterator parameters *first* and *last* are modified, but not their elements. Each accessed element is read only once.

#### See also {-}

- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)


------


#### binary_search_all_C {#binary_search_all_C-a - }

Binary search an entire container for a key. The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro.


```C
int binary_search_all_C(C *c, T const key);
```


Containers:
array


##### Parameters {-}

c
  ~ Container to search.

key
  ~ Element to search for, passed by value.


#### Return value {-}

True if the element is in the container, else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of elements in the container.

##### Data races {-}

Each accessed element is read only once.

#### See also {-}

- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_C](#binary_search_C)
- [binary_searchp_C](binary_searchp_C)
- [binary_searchp_all_C](#binary_searchp_all_C-a)


------


#### binary_searchp_C {#binary_searchp_C-a - }

Binary search for an element, with a specified key. The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro.


```C
int binary_searchp_C(it_C first, it_C last, T const *key);
```


Containers:
array


##### Parameters {-}

first
  ~ Iterator pointing to element to start search. This must come before *last* in the container.

last
  ~ Iterator pointing to element to end search. This must come after *first* in the container.

key
  ~ Element to search for, passed by address.


#### Return value {-}

True if the element is in the semi-open range [first, last) else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of in the range [first, last).

##### Data races {-}

The iterator parameters *first* and *last* are modified, but not their elements. Each accessed element is read only once.

#### See also {-}

- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_search_C](#binary_search_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### binary_searchp_all_C {#binary_searchp_all_C-a - }

Binary search an entire container for a key. The container should be sorted in the order specified by the *cmp_func* function passed to the definition macro.

```C
int binary_searchp_all_C(C *c, T * const key);
```


Containers:
array


##### Parameters {-}

c
  ~ Container to search.

key
  ~ Element to search for, passed by address.


#### Return value {-}

True if the element is in the container, else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(log n) operation, where n is the number of elements in the container.

##### Data races {-}

Each accessed element is read only once.

#### See also {-}

- [binary_rsearch_C](#binary_rsearch_C-a)
- [binary_rsearchp_C](#binary_rsearchp_C-a)
- [binary_search_C](#binary_search_C)
- [binary_searchp_C](binary_searchp_C)
- [binary_search_all_C](#binary_search_all_C-a)


------


#### check_integrity_C {#check_integrity_C-t - }

Checks that a tree container is not corrupted. Users should seldom need to use this function.

```C
int check_integrity_C(const C *tree, it_C it);
```


Containers:
tree


##### Parameters {-}

tree
  ~ Tree container whose integrity must be checked.

it
  ~ Pointer to element in tree to begin checking from. For the whole tree, set this to *tree->root*.


#### Return value {-}

True if the tree is a valid red-black one, else false.

##### Example {-}

TO DO.

#### Complexity {-}

This is a $\theta(n)$ operation where $n$ is the number of elements in the tree.

##### Data races {-}

Every element is read once.

#### See also {-}

None.

------


#### cmp_C {#cmp_C-adst - }

Compare if the elements pointed to by two iterators are less than, equal to or greater than each other. This is only defined for container types declared with a declaration macro containing *CMP* in it.

```C
int cmp_C(const it_C it1, const it_C it2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

it1
  ~ Iterator pointing to element to compare.

it2
  ~ Iterator pointing to element to compare.


#### Return value {-}

If the two elements are equal, less

##### Example {-}

```C
#include <stdio.h>
#include <cagl/hash.h>

CAG_DEC_DEF_STR_HASH(string_hash);

int main(void)
{
	string_hash h1, h2;
	it_string_hash it1, it2;

	if(new_string_hash(&h1) == NULL) {
		fprintf(stderr, "Error creating new first hash table.\n");
		exit(1);
	}
	if (new_string_hash(&h2) == NULL) {
		fprintf(stderr, "Error creating second hash table.\n");
		exit(1);
	}
	insert_string_hash(&h1, "cow");
	insert_string_hash(&h1, "mouse");
	insert_string_hash(&h1, "dog");
	insert_string_hash(&h2, "cat");
	insert_string_hash(&h2, "giraffe");
	insert_string_hash(&h2, "dog");

	it1 = get_string_hash(&h1, "dog");
	it2 = get_string_hash(&h2, "dog");

	if (cmp_string_hash(it1, it2) == 0)
		printf("A dog is a dog\n");
	else
		printf("Whoops. That shouldn't happen. %s != %s.\n",
		       it1->value, it2->value);

	it2 = get_string_hash(&h2, "giraffe");

	if (cmp_string_hash(it1, it2) != 0)
		printf("A dog is indeed not a giraffe.\n.");
	else
		printf("Whoops. That shouldn't happen. %s == %s.\n",
		       it1->value, it2->value);

	free_string_hash(&h1);
	free_string_hash(&h2);
	return 0;
}
```

#### Complexity {-}

This is a constant time operation.

##### Data races {-}

Entirely thread-local. Each iterator's element is read once.

#### See also {-}

- [cmp_all_C](#cmp_all_C-adst)
- [cmp_range_C](#cmp_range_C-adst)

------


#### cmp_all_C {#cmp_all_C-adst - }

Compares corresponding elements in two containers, stopping if unequal elements are encountered. This is only defined for container types declared with a declaration macro containing *CMP* in it.

```C
int cmp_all_C(const C *c1, const C *c2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

c1
  ~ Container to be compared.
c2
  ~ Container to be compared.

#### Return value {-}

These rules determine the integer value returned:

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
  first one, then -1 is returned (i.e. the first range is less than the
  second range).

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(n) operation.

##### Data races {-}

Each element in each container is read at most once.

#### See also {-}

- [cmp_C](#cmp_C-adst)
- [cmp_range_C](#cmp_range_C-adst)
- [equal_all_C](#equal_all_C-adst)
- [equal_range_C](#equal_range_C-adst)

------


#### cmp_range_C {#cmp_range_C-adst - }

Compares corresponding elements in two semi-open ranges, [first_1, last_1) and [first_2, last_2), stopping if unequal elements are encountered. This is only defined for container types declared with a declaration macro containing *CMP* in it.


```C
int cmp_range_C(it_C first_1, const it_C last_1,
                it_C first_2, const it_C last_2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

first_1
  ~ Iterator to first element in first range to start comparing. Must come before last_1.
last_1
  ~ Iterator to last element in semi-open first range. Must come after first_1.
first_2
  ~ Iterator to first element in second range to start comparing
last_2
  ~ Iterator to last element in semi-open second range.


#### Return value {-}

These rules determine the integer value returned:

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
  first one, then -1 is returned (i.e. the first range is less than the
  second range).

##### Example {-}

TO DO.

#### Complexity {-}

This is an O(n) operation.


##### Data races {-}

Each element in each container is read at most once.

#### See also {-}

- [cmp_C](#cmp_C-adst)
- [cmp_all_C](#cmp_range_C-adst)
- [equal_all_C](#equal_all_C-adst)
- [equal_range_C](#equal_range_C-adst)

------


#### copy_C {#copy_C-adhst - }

Copies elements in the semi-open range [first, last) to a container. The container being copied to should be empty. Space will be allocated, if necessary, for the new elements.

```C
C *copy_C(it_C first, it_C last, C *c);
```

Note that only copying between containers of the same type is supported by the copy function blueprints. To copy between different types of containers that store the same element type, use the [copy macros](#copy-macros).

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

first
  ~ Iterator to first element in range that must be copied.
last
  ~ Iterator one past last element in range that must be copied.
c
  ~ Container to be copied to.

#### Return value {-}

Pointer to container copied to if successful else NULL.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [Copy macros](#copy-macros)

------


#### copy_all_C {#copy_all_C-adhst - }

Copies all the elements in one container to another. The copied to container is expected to be empty. The function allocates sufficient memory for the copy to succeed.

```C
C *copy_all_C(const C *c1, C *c2);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c1
  ~ Container to copy from.
c2
  ~ Container to copy to.

#### Return value {-}

Pointer to C2 if the operation succeeds, else NULL.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [Copy macros](#copy-macros)

------


#### copy_if_C {#copy_if_C-adhst - }

Copies elements in a range that meet specified criteria to an empty container. Space is allocated for the copy

```C
C *copy_if_C(it_C first, it_C last, C *c, int (*cond_func) (T*, void *), void *data);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

first
  ~ Iterator to first element in range that must be copied.
last
  ~ Iterator one past last element in range that must be copied.
c
  ~ Container to be copied to.
cond_func
  ~ Function that takes two parameters, an element by address and a void pointer solely for the user's use. The function must return true if the element must be copied, else false.
data
  ~ This is solely for the user's use, generally to be able to maintain state between calls to cond_func. Set to NULL if this is not needed.

#### Return value {-}

Pointer to container copied to if successful else NULL.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [Copy macros](#copy-macros)

------


#### copy_if_all_C {#copy_if_all_C-adhst - }

Copies all the elements meeting user-specified criteria in a container to another container.

```C
C *copy_if_all_C(const C *c1, C *c2, int (*cond_func) (T*, void *), void *data);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c1
  ~ Container to copy from.
c2
  ~ Container to copy to.
cond_func
  ~ Function that takes two parameters, an element by address and a void pointer solely for the user's use. The function must return true if the element must be copied, else false.
data
  ~ This is solely for the user's use, generally to be able to maintain state between calls to cond_func. Set to NULL if this is not needed.

#### Return value {-}

Pointer to container copied to if successful else NULL.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [Copy macros](#copy-macros)

------


#### copy_many_C {#copy_many_C-adhst - }

Copy a container to multiple containers.

```C
int copy_many_C(C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ The container to copy from.

The remaining parameters are one or more containers, c1, c2, ... cn, to copy to. The very last parameter must be NULL.

#### Return value {-}

The number of containers successfully copied to. If this is less than the number of containers expected, an error occurred on the return value + 1th container.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_over_C](#copy_over_C-adhst)
- [Copy macros](#copy-macros)

------


#### copy_over_C {#copy_over_C-adhst - }

Copies a range over another range. For *copy_over* functions, there must be sufficient space already in the container. This is similar to the behaviour of the of the C++ STL copy.

```C
it_C copy_over_C(it_C first, const it_C last, it_C result);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

first
  ~ Iterator pointing to The first element to begin copying from.
last
  ~ Iterator pointing one past the last element to copy.


#### Return value {-}

Returns iterator one past the last copied element in the copied to container.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [Copy macros](#copy-macros)

------


#### distance_C {#distance_C-adhst - }

Calculates the number of elements in a semi-open range, [first, last).

```C
size_t distance_C(const it_C first, const it_C last);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}

This is a constant time operation for arrays. It is $\theta(n)$ for all the other containers, where $n$ is the number of elements in the range.

##### Data races {-}


#### See also {-}

- [distance_all_C](#distance_all_C-adhst)

------


#### distance_all_C {#distance_all_C-adhst - }

Calculates the number of elements in a container.

```C
size_t distance_all_C(const C *c);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}

This is a constant time operation for arrays. It is $\theta(n)$ for all other containers. If you frequently need to determine the size of a non-array container, it might be more efficient to track the size yourself.

##### Data races {-}


#### See also {-}

- [distance_C](#distance_C-adhst)

------


#### end_C {#end_C-adhst - }

Returns an iterator pointing one past the last element in a container. Accessing the element of this iterator is undefined.

```C
it_C end_C(const C *c);
```

Note: For SLISTs this function always returns NULL. For all other containers, the operation *prev_C(end_C(c)* is valid.

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### equal_all_C {#equal_all_C-adst - }

Compares corresponding elements in two containers, stopping if unequal elements are encountered. This is only defined for container types declared with a declaration macro containing *CMP* in it. If there are fewer elements in the container than the first, then behaviour is undefined.

```C
int equal_all_C(const C *c1, const C *c2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

c1
  ~ First container to compare. This container must have the same or fewer elements than c2.
c2
  ~ Second container to compare. This container must have the same or more elements than c1.

#### Return value {-}

Returns 0 if all calls to *cmp_func* return zero, else returns the first non-zero value of cmp_func.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### equal_range_C {#equal_range_C-adst - }

Compares corresponding elements in two ranges, stopping if unequal elements are encountered. This is only defined for container types declared with a declaration macro containing *CMP* in it. If there are fewer elements in the second range than the first, then behaviour is undefined. The first range [first_1, last_1), is semi-open.

```C
int equal_range_C(it_C first_1, const it_C last_1, it_C first_2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

first_1
  ~ Iterator to first element in range.
last_1
  ~ Iterator one past last element in range to be compared
first_2
  ~ Iterator to first element in second range.

#### Return value {-}

Returns 0 if all calls to *cmp_func* return zero, else returns the first non-zero value of cmp_func.


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_C {#erase_C-adht - }

Erases the element pointed to by an iterator.

```C
it_C erase_C(C *c, it_C it);
```


Containers:
array	dlist	hash	tree


##### Parameters {-}


#### Return value {-}

Iterator pointing to the element that was immediately after the erased element.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_after_C {#erase_after_C-s - }

Erases the element after the one pointed to by an iterator.

```C
it_C erase_after_C(it_C it);
```


Containers:
slist


##### Parameters {-}

it
  ~ Iterator pointing to an element. It is the next that will be erased. it->next should not be NULL.

#### Return value {-}

The parameter iterator, *it*, is returned.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [erase_front_C](#erase_front_C-s)
- [erase_after_range_C](#erase_after_range_C-s)

------


#### erase_after_range_C {#erase_after_range_C-s - }

Erases all elements in the open range (first, last).

```C
it_C erase_after_range_C(it_C first, it_C last);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}

The parameter iterator, *first*, is returned.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [erase_front_C](#erase_front_C-s)
- [erase_after_C](#erase_after_C-s)


------


#### erase_all_C {#erase_all_C-adhst - }

Erases all the elements in a container.

```C
it_C erase_all_C(C *c);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [erase_front_C](#erase_front_C-s)
- [erase_after_C](#erase_after_C-s)
- [erase_after_range_C](#erase_after_range_C-s)

------


#### erase_front_C {#erase_front_C-s - }

Erases the first element in a singly linked list.

```C
it_C erase_front_C(C *slist);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}

Iterator pointing to what was previously the second element in the list, but is now the head.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_range_C {#erase_range_C-adhst - }

Erases all the elements in the semi-open range [first, last).

```C
it_C erase_range_C(C *array, it_C first, it_C last);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}

Returns the parameter iterator, *last*.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### find_C {#find_C-adhst - }

Finds the first occurrence of *element* in the semi-open range [first, last) using a linear search. This is useful for containers which have no ordering function defined (e.g. hash tables, container types that were not declared with a macro containing CMP) or for ordered containers that need to search on a non-primary key.

```C
it_C find_C(it_C first, const it_C last, const T element, int (*cmp_func) (const T*, const T*));
```

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

first
  ~ Iterator pointing to first element in range to search.
last
  ~ Iterator pointing one past the last element in range to search.
element
  ~ Element to search for, passed by value.
cmp_func
  ~ User function that compares two elements. It returns zero if they are equal, else non-zero.

#### Return value {-}

Iterator pointing to found element. If the element was not found, it returns *last*.

##### Example {-}


#### Complexity {-}

This is an $O(n)$ operation where $n$ is the number of elements in the range.

##### Data races {-}


#### See also {-}

- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### find_all_C {#find_all_C-adhst - }

Finds the first occurrence of *element* in a container using a linear search. This is useful for containers which have no ordering function defined (e.g. hash tables, container types that were not declared with a macro containing CMP) or for ordered containers that need to search on a non-primary key.

```C
it_C find_all_C(const C *c, const T element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to search.
element
  ~ Element to search for, passed by value.
cmp_func
  ~ User function that compares two elements. It returns zero if they are equal, else non-zero.

#### Return value {-}

Iterator pointing to found element. If the element was not found, it returns *end_C(c)*.

##### Example {-}


#### Complexity {-}

This is a $O(n)$ operation where $n$ is the number of elements in the container.

##### Data races {-}


#### See also {-}

- [find_C](#find_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### findp_C {#findp_C-adhst - }

Finds the first occurrence of *element* in the semi-open range [first, last) using a linear search. This is useful for containers which have no ordering function defined (e.g. hash tables, container types that were not declared with a macro containing CMP) or for ordered containers that need to search on a non-primary key.

```C
it_C findp_C(it_C from, const it_C to, const T *element, int (*cmp_func)(const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

first
  ~ Iterator pointing to first element in range to search.
last
  ~ Iterator pointing one past the last element in range to search.
element
  ~ Element to search for, passed by address.
cmp_func
  ~ User function that compares two elements. It returns zero if they are equal, else non-zero.


#### Return value {-}

Iterator pointing to found element. If the element was not found, it returns *last*.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}

- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### findp_all_C {#findp_all_C-adhst - }

Finds the first occurrence of *element* in a container using a linear search. This is useful for containers which have no ordering function defined (e.g. hash tables, container types that were not declared with a macro containing CMP) or for ordered containers that need to search on a non-primary key.

```C
it_C findp_all_C(C *c, const T* element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to search.
element
  ~ Element to search for, passed by address.
cmp_func
  ~ User function that compares two elements. It returns zero if they are equal, else non-zero.


#### Return value {-}

Iterator pointing to found element. If the element was not found, it returns *end_C(c)*.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)

------


#### free_C {#free_C-adhst - }

Destroys a container, erases all its elements and returns all memory to the heap. Every container variable should be freed once it is no longer needed. For every container variable a *new_C* function should be applied to it before it is used and *free_C* function should be applied to it once it is no longer used. For every call to a *new_C* function there should be one and only one corresponding call to a *free_C* function.

```C
void free_C(C *c);
```

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to free.

#### Return value {-}

None. This is one of the few void CAGL functions.

##### Example {-}

```C
/* Demonstrates different uses of *new* and *free* on an array.

   While in many environment it isn't generally necessary to free memory upon
   exit of a program this demonstration does show how to exit the program
   without leaving any memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr a1, a2, a3, a4, a5;
	int i;

        /* Simplest use of new. Most users should use this version
	   most of the time.
	*/
	if (!new_int_arr(&a1)) {
		fprintf(stderr, "Error initialising with simple new.\n");
		/* No need to free something that wasn't successfully
		   initialized.
		 */
		exit(1);
	}

        /* Override CAGL default capacity with space for 10 elements. */
	if (!new_with_capacity_int_arr(&a2, 10)) {
		fprintf(stderr, "Error initialising with new_with_capacity.\n");
		free_int_arr(&a1);
		exit(1);
	}

        /* Insert 10 elements into a new array */
	if (!new_with_size_int_arr(&a3, 10)) {
		fprintf(stderr, "Error initialising with new_with_size.\n");
		free_int_arr(&a1);
		free_int_arr(&a2);
		exit(1);
	}

	/* Use the new_many, free_many idiom. */
	i = new_many_int_arr(&a4, &a5, NULL); /* Initialize multiple arrays. */
	if (i <= 0) {
		fprintf(stderr, "Only %d successfully initialized\n", i);
		free_many_int_arr(i, &a4, &a5);
		exit(1);
	}

	printf("Capacity of a1 is: %lu\n", a1.capacity);
	printf("Size of a1 is: %lu\n", size_int_arr(&a1));
	printf("Capacity of a2 is: %lu\n", a2.capacity);
	printf("Size of a2 is: %lu\n", size_int_arr(&a2));
	printf("Capacity of a3 is: %lu\n", a3.capacity);
	printf("Size of a3 is: %lu\n", size_int_arr(&a3));
	printf("Capacity of a4 is: %lu\n", a4.capacity);
	printf("Size of a4 is: %lu\n", size_int_arr(&a4));
	printf("Capacity of a5 is: %lu\n", a5.capacity);
	printf("Size of a5 is: %lu\n", size_int_arr(&a5));

        /* Return arrays to heap. */
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5);
	return 0;
}
```

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [free_many_C](#free_many_C-adhst)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [new_with_capacity_C](#new_with_capacity_C-a)
- [new_with_size_C](#new_with_size_C-a)

------


#### free_many_C {#free_many_C-adhst - }

Destroys many containers, erases all their elements and returns all memory used by them to the heap. This is usually used in conjunction with a [new_many_C](#new_many_C-adhst) function.

```C
void free_many_C(int num_to_free, C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

num_to_free
  ~ The number of container variables to free. This must be equal to or less than the containers used in the rest of the parameter list. You shouldn't have to manually determine the value of num_to_free. Instead, assign it to the return value when you call [new_many_C](#new_many_C-adhst). This is thethe number of containers that were successfully created. Then by passing the same value to *free_many_C*, you can be sure that only the successfully allocated containers will be freed. (It's a mistake to free containers that aren't successfully allocated.)

c
  ~ First container to free.
Remaining parameters (...)
  ~ All the containers, in the same order, that were passed to *new_many_C*.

Note in contrast to *new_many_C*, there is no need to pass NULL as the last parameter, but no harm results from doing so either.

#### Return value {-}

None.

##### Example {-}

```C
/* Demonstrates different uses of *new* and *free* on an array.

   While in many environment it isn't generally necessary to free memory upon
   exit of a program this demonstration does show how to exit the program
   without leaving any memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr a1, a2, a3, a4, a5;
	int i;

        /* Simplest use of new. Most users should use this version
	   most of the time.
	*/
	if (!new_int_arr(&a1)) {
		fprintf(stderr, "Error initialising with simple new.\n");
		/* No need to free something that wasn't successfully
		   initialized.
		 */
		exit(1);
	}

        /* Override CAGL default capacity with space for 10 elements. */
	if (!new_with_capacity_int_arr(&a2, 10)) {
		fprintf(stderr, "Error initialising with new_with_capacity.\n");
		free_int_arr(&a1);
		exit(1);
	}

        /* Insert 10 elements into a new array */
	if (!new_with_size_int_arr(&a3, 10)) {
		fprintf(stderr, "Error initialising with new_with_size.\n");
		free_int_arr(&a1);
		free_int_arr(&a2);
		exit(1);
	}

	/* Use the new_many, free_many idiom. */
	i = new_many_int_arr(&a4, &a5, NULL); /* Initialize multiple arrays. */
	if (i <= 0) {
		fprintf(stderr, "Only %d successfully initialized\n", i);
		free_many_int_arr(i, &a4, &a5);
		exit(1);
	}

	printf("Capacity of a1 is: %lu\n", a1.capacity);
	printf("Size of a1 is: %lu\n", size_int_arr(&a1));
	printf("Capacity of a2 is: %lu\n", a2.capacity);
	printf("Size of a2 is: %lu\n", size_int_arr(&a2));
	printf("Capacity of a3 is: %lu\n", a3.capacity);
	printf("Size of a3 is: %lu\n", size_int_arr(&a3));
	printf("Capacity of a4 is: %lu\n", a4.capacity);
	printf("Size of a4 is: %lu\n", size_int_arr(&a4));
	printf("Capacity of a5 is: %lu\n", a5.capacity);
	printf("Size of a5 is: %lu\n", size_int_arr(&a5));

        /* Return arrays to heap. */
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5);
	return 0;
}
```

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [free_C](#free_C-adhst)
- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_many_C](#new_many_C-adhst)
- [new_with_capacity_C](#new_with_capacity_C-a)
- [new_with_size_C](#new_with_size_C-a)

------


#### front_C {#front_C-adst - }

Returns pointer to first element in container.

```C
T *front_C(const C *c);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

c
  ~ Container to retrieve the front element for.

#### Return value {-}

Pointer to first element in the container.

##### Example {-}

TO DO.

#### Complexity {-}

This is a constant time operation for all containers.

##### Data races {-}

The address of the first element is retrieved.

#### See also {-}


------


#### get_C {#get_C-ht - }

Retrieves the element from the container with the given key.

```C
it_C get_C(const C *c, const T key);
```


Containers:
hash	tree


##### Parameters {-}

c
  ~ Container to retrieve element from.
key
  ~ Element to search for, passed by value.

#### Return value {-}

Iterator pointing to element if a match is found, else NULL.

##### Example {-}

```C
#include <stdio.h>
#include <stdlib.h>

#include <cagl/tree.h>
#include <cagl/hash.h>

CAG_DEC_DEF_STR_TREE(string_tree);

CAG_DEC_DEF_STR_HASH(string_hash);

int main(void)
{
	string_tree t;
	string_hash h;
	it_string_tree itt;
	it_string_hash ith;

	if (!new_string_tree(&t)) {
		fprintf(stderr, "Error allocating tree.\n");
		exit(1);
	}
	if (!new_string_hash(&h)) {
		fprintf(stderr, "Error allocating hash.\n");
		exit(1);
	}

	insert_string_tree(&t, "cat");
	insert_string_tree(&t, "dog");
	insert_string_tree(&t, "mouse");

	insert_string_hash(&h, "cat");
	insert_string_hash(&h, "dog");
	insert_string_hash(&h, "mouse");

	itt = get_string_tree(&t, "dog");
	if (!itt)
		printf("Dog not found.\n");
	else
		printf("%s barks woof!\n", itt->value);

	ith = get_string_hash(&h, "cat");
	if (!ith)
		printf("Cat not found.\n");
	else
		printf("%s say meeow!\n", ith->value);

	free_string_tree(&t);
	free_string_hash(&h);
	return 0;
}
```

The output is:

```
dog barks woof!
cat say meeow!
```

#### Complexity {-}

For hash tables on average this is a constant time operation. But it can degrade in the worst case to $O(n)$ where $n$ is the number of elements in the table.

For trees this is an $O(\log n)$ operation on average and in the worst case.

##### Data races {-}

For hash tables it is possible that every element will be read once.

For trees approximately $\log n$ elements will be read where $n$ is the number of elements in the tree.

#### See also {-}

- [getp_C](#getp_C-ht)

------


#### getp_C {#getp_C-ht - }

Retrieves the element from the container with the given key.

```C
it_C getp_C(const it_C root, const T *element);
```


Containers:
hash	tree

##### Parameters {-}

c
  ~ Container to retrieve element from.
key
  ~ Element to search for, passed by address.

#### Return value {-}

Iterator pointing to element if a match is found, else NULL.

##### Example {-}

```C
#include <stdio.h>
#include <stdlib.h>

#include <cagl/tree.h>
#include <cagl/hash.h>

struct dictionary {
	char *word;
	char *definition;
};

CAG_DEC_DEF_STR_STR_TREE(tree_dict, struct dictionary);

CAG_DEC_DEF_STR_STR_HASH(hash_dict, struct dictionary);

int main(void)
{
	tree_dict t;
	hash_dict h;
	it_tree_dict itt;
	it_hash_dict ith;
	struct dictionary dict;
	struct dictionary key;

	if (!new_tree_dict(&t)) {
		fprintf(stderr, "Error allocating tree.\n");
		exit(1);
	}
	if (!new_hash_dict(&h)) {
		fprintf(stderr, "Error allocating hash.\n");
		exit(1);
	}

	dict.word = "cat";
	dict.definition = "Meeowing domestic pet";
	insertp_tree_dict(&t, &dict);

	dict.word = "dog";
	dict.definition = "Barking domestic pet";
	insertp_hash_dict(&h, &dict);

	key.word = "cat";
	itt = getp_tree_dict(&t, &key);
	if (!itt)
		printf("Cat not found.\n");
	else
		printf("%s - %s\n", itt->value.word, itt->value.definition);

	key.word = "dog";
	ith = getp_hash_dict(&h, &key);
	if (!ith)
		printf("Dog not found.\n");
	else
		printf("%s - %s\n", ith->value.word, ith->value.definition);

	free_tree_dict(&t);
	free_hash_dict(&h);
	return 0;
}
```

The output is:

```
cat - Meeowing domestic pet
dog - Barking domestic pet
```

#### Complexity {-}

For hash tables on average this is a constant time operation. But it can degrade in the worst case to $O(n)$ where $n$ is the number of elements in the table.

For trees this is an $O(\log n)$ operation on average and in the worst case.

##### Data races {-}

For hash tables it is possible that every element will be read once.

For trees approximately $\log n$ elements will be read where $n$ is the number of elements in the tree.

#### See also {-}

- [get_C](#get_C-ht)

------


#### index_C {#index_C-adhst - }

Retrieves an iterator to an element a specified number of positions from the beginning of the container.

```C
it_C index_C(C *c, size_t n);
```

Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to get the iterator from.
n
  ~ It is the *n*th element that will be retrieved.

It is undefined if *n* is greater than the number of elements in the container.

#### Return value {-}

Iterator pointing to *n*th element in *c*.

##### Example {-}

TO DO.

#### Complexity {-}

This is a constant time operation for arrays.

It is an O(n) time operation for all other containers, where *n* is the index of the element to retrieve.

##### Data races {-}


#### See also {-}


------


#### inorder_C {#inorder_C-t - }

Traverses a tree in the order defined by the *cmp_func* function with which the tree was defined.

```C
void inorder_C(it_C root, void *data, void (*action_func)(it_C, void *));
```


Containers:
tree


##### Parameters {-}

root
  ~ Iterator pointing to the node in the tree from which to start traversing from. Usually tree->root.
data
  ~ User defined data. Set to NULL if not needed, else set to the address of a structure that you intend to modify in *action_func*.
action
  ~ User defined function that will be exected on an iterator pointing to each element that is traversed. The second parameter to the function is *data*.

#### Return value {-}

None.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_C {#insert_C-adht - }

Inserts an element into a container.

The syntax differs for different container types.

```C

/* ARRAY */
it_C insert_C(C *array, it_C position, T const element);

/* DLIST */
it_C insert_C(it_C position, T const element);

/* HASH */
it_C  insert_C(C *hash, T const element);

/* TREE */
it_C  insert_C(C *tree, T const element);

```


Containers:
array	dlist	hash	tree


##### Parameters {-}

c
  ~ Container to insert into. Applies to arrays, trees and hash tables only.
position
  ~ Place where element must be inserted. The current element at position, and if necessary, all subsequent elements are shifted up. Applies to arrays and doubly-linked lists only.
element
  ~ Element to insert, passed by value.

#### Return value {-}

Iterator pointing to newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}

ARRAY: $O(n - position)$ where $n$ is the number of elements in the array.

DLIST: Constant time. Inserting into lists is very efficient.

TREE: $O(\log n)$ where $n$ is the number of elements in the tree.

HASH: Constant time on average, but can degrade to $O(n)$.

##### Data races {-}


#### See also {-}


------


#### insert_after_C {#insert_after_C-s - }

Inserts an element after a specified position in a singly-linked list.

```C
it_C insert_after_C(C *c, it_C position, T item);
```


Containers:
slist


##### Parameters {-}

c
  ~ Container to insert into.
position
  ~ Place where element must be inserted after.
element
  ~ Element to insert, passed by value.


#### Return value {-}

Iterator pointing to newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_gt_C {#insert_gt_C-ad - }

Inserts an element at the first position in a container, such that it is bigger than all preceding elements.

```C
it_C insert_gt_C(C *c, it_C position, T element);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to insert into.
element
  ~ Element to insert, passed by value.

#### Return value {-}

Iterator pointing to the newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_gteq_C {#insert_gteq_C-ad - }

Inserts an element at the first position in a container, such that it is bigger than or equal to all preceding elements.

```C
it_C insert_gteq_C(C *c, it_C position, T element);
```

Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to insert into.
element
  ~ Element to insert, passed by value.

#### Return value {-}

Iterator pointing to the newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_lt_C {#insert_lt_C-ad - }

Inserts an element at the first position in a container, such that it is less than all preceding elements.


```C
it_C insert_lt_C(C *c, it_C position, T element);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to insert into.
element
  ~ Element to insert, passed by value.

#### Return value {-}

Iterator pointing to the newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_lteq_C {#insert_lteq_C-ad - }

Inserts an element at the first position in a container, such that it is less than or equal to all preceding elements.

```C
it_C insert_lteq_C(C *c, it_C position, T element);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to insert into.
element
  ~ Element to insert, passed by value.

#### Return value {-}

Iterator pointing to the newly inserted element upon success. Null upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_C {#insertp_C-adht - }

Identical to [insert_C](#insert_C-adht) except that the *element* parameter is passed by address.

```C
it_C insertp_C(C *array, it_C position, T const *element);
```


Containers:
array	dlist	hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_after_C {#insertp_after_C-s - }

Identical to [insert_after_C](#insert_after_C-s) except that the *element* parameter is passed by address.

```C
it_C insertp_after_C(C *slist, it_C it, T *item);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_gt_C {#insertp_gt_C-ad - }

Identical to [insert_gt_C](#insert_gt_C-ad) except that the *element* parameter is passed by address.

```C
it_C insertp_gt_C(C *c, it_C position, T *element);
```

Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_gteq_C {#insertp_gteq_C-ad - }

Identical to [insert_gteq_C](#insert_gteq_C-ad) except that the *element* parameter is passed by address.

```C
it_C insertp_gteq_C(C *c, it_C position, T *element);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_lt_C {#insertp_lt_C-ad - }

Identical to [insert_lt_C](#insert_lt_C-ad) except that the *element* parameter is passed by address.

```C
it_C insertp_lt_C(C *c, it_C position, T *element);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_lteq_C {#insertp_lteq_C-ad - }

Identical to [insert_lteq_C](#insert_lteq_C-ad) except that the *element* parameter is passed by address.


```C
it_C insertp_lteq_C(C *c, it_C position, T *element);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### it_C {#it_C-adhst - }

Iterator typedef. Every container type has an associated forward iterator typedef called *it_C* where *C* is the container type name.


```C
typedef iterator_C * it_C;
```

Containers:
array	dlist	hash	slist	tree


Containers that support bidirectional iterators (ARRAY, DLIST and TREE) also have an associated reverse iterator typedef called *rit_C*. You can convert a reverse iterator to a forward iterator and back again simply by casting from one to the other. E.g.

```C
it_C it;
rit_C rit;

...

it = (it_C) rit;

...

rit = (rit_C) it;

```

#### See also {-}


------


#### last_C {#last_C - }

Get an iterator pointing to the last element in the container.

```C
it_C last_C(const C *c);
```


Containers:
array dlist slist tree


##### Parameters {-}

c
  ~ Container to get last element from.

#### Return value {-}

Iterator pointing to last element.

##### Example {-}

TO DO.

#### Complexity {-}

Constant time except SLIST, for which it is $\theta(n)$ where $n$ is the number of elements in the list.

##### Data races {-}


#### See also {-}


------


#### levelorder_C {#levelorder_C-t - }

Performs a breadth-first traversal of a binary tree, applying a user-provided function to an iterator to each element.

```C
void levelorder_C(C *tree, void *data, void (*action_func)(it_C, size_t, int, int, void *));
```


Containers:
tree


##### Parameters {-}

tree
  ~ Tree to traverse.
data
  ~ Pointer to user provided data field that will be passed to *action_func* each time it is called. Set to NULL if unneeded.
action_func
  ~ User provided function that takes four parameters:
      - An iterator pointing to the element being traversed
	  - Current level of the tree
	  - Boolean indicating whether the level has changed since last call to *action_func*
	  - Boolean indicating whether the parent has changed since the last call to *action_func*
	  - User provided data, which will be NULL if *data* was set to NULL.

#### Return value {-}

None.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### lower_bound_C {#lower_bound_C-a - }

Uses a binary search to retrieve an iterator pointing to the smallest element that is bigger than or equal to the key in the semi-open range [first, last). This is only available to containers declared with a CMP macro. The *cmp_func* function provided to the definition macro is used to do the comparisons.

```C
it_C lower_bound_C(it_C first, it_C last, T const key);
```


Containers:
array


##### Parameters {-}

first
  ~ Beginning of range to search.
last
  ~ One past the end of the range to search.
key
  ~ Element to search for, passed by value.

#### Return value {-}

Iterator pointing to the lower bound element. If all elements are less than *key* then *last* is returned.

##### Example {-}

TO DO.

#### Complexity {-}

This is an $O(n)$ operation, where $n$ is the number of elements in the range.

##### Data races {-}


#### See also {-}


------


#### lower_bound_all_C {#lower_bound_all_C-a - }

Uses a binary search to retrieve an iterator pointing to the smallest element that is bigger than or equal to the key in a container. This is only available to containers declared with a CMP macro. The *cmp_func* function provided to the definition macro is used to do the comparisons.

```C
it_C lower_bound_all_C(C *c, const T key);
```


Containers:
array


##### Parameters {-}

c
  ~ Container to search.
key
  ~ Key to search for.

#### Return value {-}

Iterator pointing to the lower bound element. If all elements are less than *key* then *last* is returned.

##### Example {-}

TO DO.

#### Complexity {-}

This is an $O(n)$ operation, where $n$ is the number of elements in the range.

##### Data races {-}


#### See also {-}


------


#### lower_boundp_C {#lower_boundp_C-a - }

Identical to [lower_bound_C](#lower_bound_C-a) except the key is passed by address.

```C
it_C lower_boundp_C(it_C first, it_C last, T const *key);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### lower_boundp_all_C {#lower_boundp_all_C-a - }

Identical to [lower_bound_all_C](#lower_bound_all_C-a) except the key is passed by address.

```C
it_C lower_boundp_all_C(C *c, T * const d);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------



#### lt_it_C {#lt_it_C-ad - }

Compares two iterators and returns true if and only if the first one is pointing to a lower position in the container than the second one.

```C
int lt_it_C(const it_C it1, const it_C it2);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### lteq_it_C {#lteq_it_C-ad - }

Compares two iterators and returns true if and only if the first one is pointing to a lower or same position in the container as the second one.


```C
int lteq_it_C(const it_C it1, const it_C it2);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### new_C {#new_C-adhst - }

Initializes a container variable.

```C
C *new_C(C *c);
```

Containers:
array	dlist	hash	slist	tree

Every CAGL container variable must be initialized with a call to a new function before any other operation is performed on the container. It is undefined to call a *new* function on a container more than once before calling a *free* function. For every call of *new* on a container variable there should be a corresponding call to *free*.

##### Parameters {-}

array
  ~ The array to initialize.

##### Return value {-}

On success, the initialized container, else NULL.

##### Example {-}

```C
/* Demonstrates different uses of *new* and *free* on an array.

   While in many environment it isn't generally necessary to free memory upon
   exit of a program this demonstration does show how to exit the program
   without leaving any memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr a1, a2, a3, a4, a5;
	int i;

        /* Simplest use of new. Most users should use this version
	   most of the time.
	*/
	if (!new_int_arr(&a1)) {
		fprintf(stderr, "Error initialising with simple new.\n");
		/* No need to free something that wasn't successfully
		   initialized.
		 */
		exit(1);
	}

        /* Override CAGL default capacity with space for 10 elements. */
	if (!new_with_capacity_int_arr(&a2, 10)) {
		fprintf(stderr, "Error initialising with new_with_capacity.\n");
		free_int_arr(&a1);
		exit(1);
	}

        /* Insert 10 elements into a new array */
	if (!new_with_size_int_arr(&a3, 10)) {
		fprintf(stderr, "Error initialising with new_with_size.\n");
		free_int_arr(&a1);
		free_int_arr(&a2);
		exit(1);
	}

	/* Use the new_many, free_many idiom. */
	i = new_many_int_arr(&a4, &a5, NULL); /* Initialize multiple arrays. */
	if (i <= 0) {
		fprintf(stderr, "Only %d successfully initialized\n", i);
		free_many_int_arr(i, &a4, &a5);
		exit(1);
	}

	printf("Capacity of a1 is: %lu\n", a1.capacity);
	printf("Size of a1 is: %lu\n", size_int_arr(&a1));
	printf("Capacity of a2 is: %lu\n", a2.capacity);
	printf("Size of a2 is: %lu\n", size_int_arr(&a2));
	printf("Capacity of a3 is: %lu\n", a3.capacity);
	printf("Size of a3 is: %lu\n", size_int_arr(&a3));
	printf("Capacity of a4 is: %lu\n", a4.capacity);
	printf("Size of a4 is: %lu\n", size_int_arr(&a4));
	printf("Capacity of a5 is: %lu\n", a5.capacity);
	printf("Size of a5 is: %lu\n", size_int_arr(&a5));

        /* Return arrays to heap. */
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5);
	return 0;
}
```

##### Complexity {-}

This is a constant time operation for all containers.

For arrays, CAG_QUANTUM_ARRAY * sizeof(type) bytes are allocated from the heap. For hash tables, the default dynamic sizing values are in the *cag_p_htable_sizes* table defined in hash.h. For trees and slists space for at most two elements is allocated.

##### Data races {-}

The container parameter, *c*, is modified.

##### See also {-}


------


#### new_from_C {#new_from_C-adhst - }

Initializes a container and copies the elements in another container into it. Serves a similar (though much less flexible) purpose to a C++ copy constructor. This is very useful if you have a container whose elements are containers.

```C
C *new_from_C(C *to, const C *from);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

to
  ~ Container to initialize and copy to.
from
  ~ Container to copy from.

#### Return value {-}

Pointer to the container that is initialized upon success else NULL.

##### Example {-}

TO DO.

#### Complexity {-}

This is an $\theta(n)$ operation where $n$ is the number of elements in the second container (the one being copied from).

##### Data races {-}


#### See also {-}


------


#### new_many_C {#new_many_C-adhst - }

Initialize several containers in one function call. Use in conjunction with [free_many_C](#free_many_C-adhst)

```C
int new_many_C(C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to initialize
Remaining parameters (...)
  Zero or more container variables of type C,  followed by NULL. The NULL is essential else results are undefined.

#### Return value {-}

Number of successfully initialized containers.

##### Example {-}

```C
/* Demonstrates different uses of *new* and *free* on an array.

   While in many environment it isn't generally necessary to free memory upon
   exit of a program this demonstration does show how to exit the program
   without leaving any memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr a1, a2, a3, a4, a5;
	int i;

        /* Simplest use of new. Most users should use this version
	   most of the time.
	*/
	if (!new_int_arr(&a1)) {
		fprintf(stderr, "Error initialising with simple new.\n");
		/* No need to free something that wasn't successfully
		   initialized.
		 */
		exit(1);
	}

        /* Override CAGL default capacity with space for 10 elements. */
	if (!new_with_capacity_int_arr(&a2, 10)) {
		fprintf(stderr, "Error initialising with new_with_capacity.\n");
		free_int_arr(&a1);
		exit(1);
	}

        /* Insert 10 elements into a new array */
	if (!new_with_size_int_arr(&a3, 10)) {
		fprintf(stderr, "Error initialising with new_with_size.\n");
		free_int_arr(&a1);
		free_int_arr(&a2);
		exit(1);
	}

	/* Use the new_many, free_many idiom. */
	i = new_many_int_arr(&a4, &a5, NULL); /* Initialize multiple arrays. */
	if (i <= 0) {
		fprintf(stderr, "Only %d successfully initialized\n", i);
		free_many_int_arr(i, &a4, &a5);
		exit(1);
	}

	printf("Capacity of a1 is: %lu\n", a1.capacity);
	printf("Size of a1 is: %lu\n", size_int_arr(&a1));
	printf("Capacity of a2 is: %lu\n", a2.capacity);
	printf("Size of a2 is: %lu\n", size_int_arr(&a2));
	printf("Capacity of a3 is: %lu\n", a3.capacity);
	printf("Size of a3 is: %lu\n", size_int_arr(&a3));
	printf("Capacity of a4 is: %lu\n", a4.capacity);
	printf("Size of a4 is: %lu\n", size_int_arr(&a4));
	printf("Capacity of a5 is: %lu\n", a5.capacity);
	printf("Size of a5 is: %lu\n", size_int_arr(&a5));

        /* Return arrays to heap. */
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5);
	return 0;
}
```

#### Complexity {-}

For each container this is a constant time operation.

##### Data races {-}

Containers are modified when initialized. No other threads should access a container when it is being initialized.

#### See also {-}

- [new_C](#new_C-adhst)
- [new_from_C](#new_from_C-adhst)
- [new_with_capacity_C](#new_with_capacity_C-a)
- [new_with_size_C](#new_with_size_C-a)

------


#### new_with_buckets_C {#new_with_buckets_C-h - }

Initialize a hash table with a specified number of buckets.

```C
C *new_with_buckets_C(C *hash, const size_t buckets);
```


Containers:
hash


##### Parameters {-}

hash
  ~ Hash table to initialize.
buckets
  ~ Number of buckets to use. This must be greater than zero. This does not fix the number of buckets permanently. If the table becomes crowded, more buckets will be added.


#### Return value {-}

Pointer to initialized hash table upon success, else NULL.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### new_with_capacity_C {#new_with_capacity_C-a - }


Initializes an array and sets its capacity to a user-specified amount.

```C
C *new_with_capacity_C(C *array, const size_t reserve);
```

Containers:
array

The *capacity* specifies the number of elements to make space for, but the array size is initialized to zero. By default, once the capacity is reached, CAGL functions that add further elements will call realloc to attempt to get additional capacity.

##### Parameters {-}

array
  ~ The array to initialize.

capacity
  ~ Enough memory is allocated to accommodate *capacity* elements in the array.

##### Return value {-}

Return value
  ~ On success, the initialized container, else NULL.


##### Example {-}

See the example in [new_C](#new_C-adhst).

##### Complexity {-}

This is a constant time operation.

##### Data races {-}

The parameter *array* is modified.

##### See also {-}

- [new_C](#new_C-adhst)

------


#### new_with_size_C {#new_with_size_C-a - }

Initializes an array and adds a specified number of uninitialized elements to it.

```C
C *new_with_size_C(C *array, const size_t size);
```


Containers:
array


##### Parameters {-}

array
  ~ Array to initialize.
size
  ~ Number of elements to place in the array.

#### Return value {-}

Pointer to initialized array upon success, else NULL.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### next_C {#next_C-adhst - }

Increment an iterator so as to point to the next element in a container.

```C
it_C next_C(it_C const it);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

it
  Iterator to increment.

#### Return value {-}

Iterator pointing to next element.

##### Example {-}

TO DO.

#### Complexity {-}

On average this is a constant time operation in all containers.

##### Data races {-}


#### See also {-}


------


#### postorder_C {#postorder_C-t - }

Does a [post-order](http://en.wikipedia.org/wiki/Tree_traversal#Post-order) traversal of a binary tree.

```C
void postorder_C(it_C root, void *data, void (*action)(it_C, void *));
```


Containers:
tree


##### Parameters {-}

root
  ~ Iterator pointing to the node in the tree from which to start traversing from. Usually tree->root.
data
  ~ User defined data. Set to NULL if not needed, else set to the address of a structure that you intend to modify in *action_func*.
action
  ~ User defined function that will be exected on an iterator pointing to each element that is traversed. The second parameter to the function is *data*.

#### Return value {-}

None.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### preorder_C {#preorder_C-t - }

Does a [pre-order](http://en.wikipedia.org/wiki/Tree_traversal#Pre-order) traversal of a binary tree.

```C
void preorder_C(it_C root, void *data, void (*action)(it_C, void *));
```


Containers:
tree


##### Parameters {-}

root
  ~ Iterator pointing to the node in the tree from which to start traversing from. Usually tree->root.
data
  ~ User defined data. Set to NULL if not needed, else set to the address of a structure that you intend to modify in *action_func*.
action
  ~ User defined function that will be exected on an iterator pointing to each element that is traversed. The second parameter to the function is *data*.


#### Return value {-}

None.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### prepend_C {#prepend_C-ads - }

Prepend an element to the beginning of a container.

```C
it_C prepend_C(C *c, T const element);
```


Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to prepend to.
element
  ~ Element to prepend, passed by value.

#### Return value {-}

Iterator to new element if successful, else NULL.

##### Example {-}


#### Complexity {-}

Constant time operation for lists, but $\theta(n)$ for arrays, where $n$ is the number of elements in the array.

##### Data races {-}


#### See also {-}

- [prependp_C](#prependp_C-ads)

------


#### prependp_C {#prependp_C-ads - }

Prepend an element to the beginning of a container.

```C
it_C prependp_C(C *c, T const *element);
```


Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to prepend to.
element
  ~ Element to prepend, passed by address.

#### Return value {-}

Iterator to new element if successful, else NULL.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### prev_C {#prev_C-adt - }

Decrement an iterator so as to point to the next element in a container.

```C
it_C prev_C(it_C const it);
```


Containers:
array	dlist	tree


##### Parameters {-}

it
  ~ Iterator to decrement.

#### Return value {-}

Iterator pointing to previous element.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### put_C {#put_C-adhst - }

Inserts an element into a container. The function blueprints *put_C* and *putp_C* provide a uniform interface to all containers for inserting an element. Users are welcome to use these functions, but they're primarily designed for the internal workings of CAGL.

```C
it_C put_C(C *c, it_C position, T const element);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

c
  ~ Container to insert into.
position
  ~ Place in container to put the new element.
element
  ~ Element to insert, passed by value.


#### Return value {-}

Iterator pointing to newly inserted element.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### putp_C {#putp_C-ad - }

Inserts an element into a container. The function blueprints *put_C* and *putp_C* provide a uniform interface to all containers for inserting an element. Users are welcome to use these functions, but they're primarily designed for the internal workings of CAGL.

```C
it_C putp_C(C *array, it_C position, T const *element);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to insert into.
position
  ~ Place in container to put the new element.
element
  ~ Element to insert, passed by address.

#### Return value {-}

Iterator pointing to newly inserted element.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### random_shuffle_C {#random_shuffle_C-ad - }

Randomly shuffles the elements in a semi-open range [first, last). This function is defined for reorderable containers that support bidirectional iterators (ARRAY and DLIST).

```C
it_C random_shuffle_C(const it_C first, it_C last);
```


Containers:
array	dlist


##### Parameters {-}

first
  ~ Iterator pointing to beginning of semi-open range.
last
  ~ Iterator pointing one past end of semi-open range.

#### Return value {-}

Iterator pointing to first element in range.

##### Example {-}


#### Complexity {-}

This is a $\theta(n)$ operation where $n$ is the number of elements in the range.

##### Data races {-}


#### See also {-}


------


#### random_shuffle_all_C {#random_shuffle_all_C-ad - }

Randomly shuffles the elements in a semi-open range [first, last). This function is defined for reorderable containers that support bidirectional iterators (ARRAY and DLIST).

```C
it_C random_shuffle_all_C(C *c);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rappend_C {#rappend_C-a - }

Reverse iterator version of append. This function blueprint is experimental and might be removed in future versions.

```C
rit_C rappend_C(C *array, T const element);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rat_C {#rat_C-adt - }

Get an iterator at a specified position. Reverse iterator version of [at_C](#at_C-adhst).

```C
rit_C rat_C(rit_C it, const size_t n);
```


Containers:
array	dlist	tree


##### Parameters {-}

it
  ~ Reverse iterator pointing to an element in the container.
n
  ~ Number of positions from *it* moving towards the front of the container to retrieve.

#### Return value {-}

Iterator that is *n* elements before *it*.

##### Example {-}

```C
/* Example of rat_C function blueprint.
   Prints out odd numbers in reverse.
*/
#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	rit_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (append_int_dlist(&l, i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
	it = rbeg_int_dlist(&l);
	while(it != rend_int_dlist(&l)) {
		printf("%d\n", it->value);
		it = rat_int_dlist(it, 2);
	}
	free_int_dlist(&l);
	return 0;
}
```

This is the output:

```
9
7
5
3
1
```

#### Complexity {-}

For arrays, this is a random access constant time operation.

For trees and lists this will take O(n) time where *n* is the second parameter.

##### Data races {-}

This is thread-safe.

For trees, lists and hash tables, the iterator parameter is modified *n* times but never the values to which it points.

For arrays, only local storage is modified.

#### See also {-}

- [at_C](#at_C-adhst)
- [index_C](#index_C-adhst)

------


#### rback_C {#rback_C-a - }

Retrieves a reverse iterator to the front of a container. This is experimental and might be removed from a future version of CAGL.

```C
T *rback_C(const C *array);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rbegin_C {#rbegin_C-adt - }

Returns a reverse iterator pointing to the last element in a container.

```C
rit_C rbegin_C(const C *array);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rcmp_C {#rcmp_C-adt - }

Compares an element pointed to by a  reverse iterator with one pointed to by a forward iterator. Seldom that users would need this. Mainly used by other CAGL algorithms.

```C
int rcmp_C(const rit_C it1, const it_C it2);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rcmp_all_C {#rcmp_all_C-adt - }

Compares two containers in opposite directions. Similar to [cmp_all_C](#cmp_all_C-adst) except that one of the containers is traversed in reverse. Can be used, for example, to check for a palindrome. The two containers may be the same.

```C
int rcmp_all_C(const C *c1, const C *c2);
```

Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rcmp_range_C {#rcmp_range_C-adt - }

Compares two ranges. Similar to [cmp_range_C](#cmp_range_C-adst) except that one of the ranges is traversed in reverse using reverse iterators. Can be used, for example, to check for a palindrome. The two ranges must be of the same container type, but do not have to be from the same container.

```C
int rcmp_range_C(rit_C from_1, const rit_C to_1, it_C from_2, const it_C to_2);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rcopy_C {#rcopy_C-adt - }

Copies a range in reverse to a container. The container is presumed to be empty and memory is allocated for it.


```C
C *rcopy_C(rit_C first, rit_C last, C *c);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rcopy_all_C {#rcopy_all_C-adt - }

Copies a container in reverse into another container. The second container is presumed to be empty.

```C
C *rcopy_all_C(const C *c1, C *c2);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rdistance_C {#rdistance_C-adt - }

Calculates the distance between two iterators in a container. Reverse iterator version of [distance_C](#rdistance_C-adhst)

```C
size_t rdistance_C(const rit_C from, const rit_C to);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rehash_C {#rehash_C-h - }

Regenerates a hash table. Primarily for internal use, but users can safely call it if they, for example, they are optimising the number of buckets.

```C
C *rehash_C(C *hash, size_t buckets);
```


Containers:
hash


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### remove_C {#remove_C-ht - }

Removes a given element from a container.

```C
it_C remove_C(C *c, const T element);
```


Containers:
hash	tree


##### Parameters {-}

c
  ~ Container to remove element from.
element
  ~ Element containing search key, passed by value.

#### Return value {-}

If the element is found, returns the next element in the container, else NULL.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### removep_C {#removep_C-h - }

Removes a given element from a container.

```C
it_C removep_C(C *hash, const T *element);
```


Containers:
hash


##### Parameters {-}

c
  ~ Container to remove element from.
element
  ~ Element containing search key, passed by address.


#### Return value {-}

If the element is found, returns the next element in the container, else NULL.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rend_C {#rend_C-adt - }

Reverse iterator version of [end_C](#end_C-adhst). Returns the last element of a container.

```C
rit_C rend_C(const C *c);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### requal_all_C {#requal_all_C-adt - }

Compares two containers in opposite directions. Similar to [equal_all_C](#equal_all_C-adst) except that one of the containers is traversed in reverse. Can be used, for example, to check for a palindrome. The two containers may be the same.

```C
int requal_all_C(const C *c1, const C *c2);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### requal_range_C {#requal_range_C-adt - }

Compares two ranges. Similar to [equal_range_C](#cmp_range_C-adst) except that one of the ranges is traversed in reverse using reverse iterators. Can be used, for example, to check for a palindrome. The two ranges must be of the same container type, but do not have to be from the same container.

```C
int requal_range_C(rit_C from_1, const rit_C to_1, it_C from_2);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### reverse_C {#reverse_C-ad - }

Reverses the elements in the semi-open range [first, last).

```C
it_C reverse_C(it_C first, it_C last);
```


Containers:
array	dlist


##### Parameters {-}

first
  ~ Iterator pointing to first element in range.
last
  ~ Iterator pointing one past the last element in the range.

#### Return value {-}

Iterator pointing to first element in the range which was previously the last element in the range.

##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### reverse_all_C {#reverse_all_C-ads - }

Reverses the elements in a container.

```C
it_C reverse_all_C(C *c);
```


Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to reverse.

#### Return value {-}

Iterator pointing to first element (previously last) of the container.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### reverseorder_C {#reverseorder_C-t - }

Traverses a tree in the reverse order defined by the *cmp_func* function with which the tree was defined. Reverse iterator version of [inorder_C](#inorder_C-t).

```C
void reverseorder_C(rit_C root, void *data, void (*action)(rit_C, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rfind_C {#rfind_C-adt - }

Reverse iterator version of [find_C](#find_C-adhst).

```C
rit_C rfind_C(rit_C from, const rit_C to, const T element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rfindp_C {#rfindp_C-adt - }

Reverse iterator version of [findp_C](#findp_C-adhst).

```C
rit_C rfindp_C(rit_C from, const rit_C to, const T *element, int (*cmp_func)(const T*, const T*));
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [find_C](#find_C-adhst)
- [find_all_C](#find_all_C-adhst)
- [findp_C](#findp_C-adhst)
- [findp_all_C](#findp_all_C-adhst)
- [rfind_C](#rfind_C-adt)
- [rfindp_C](#rfindp_C-adt)
- [search_C](#search_C-adst)
- [search_all_C](#search_all_C-adst)
- [searchp_C](#searchp_C-adst)
- [searchp_all_C](#searchp_all_C-adst)
- [rsearch_C](#rsearch_C-adt)
- [rsearchp_C](#rsearchp_C-adt)
- [binary_search_C](#binary_search_C-a)
- [binary_search_all_C](#binary_search_all_C-a)
- [binary_searchp_C](#binary_searchp_C-a)
- [binary_searchp_all_C](#binary_searchp_all_C-a)



------


#### rfront_C {#rfront_C-a - }

Reverse iterator version of [front_C](#front_C-adst).
This is experimental and might be removed from future versions.

```C
T *rfront_C(const C *array);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rinsert_C {#rinsert_C-a - }

Reverse iterator version of [insert_C](#insert_C-adht).
This is experimental and might be removed from future versions.

```C
rit_C rinsert_C(C *array, rit_C position, T const element);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rit_C {#rit_C-adt - }

Reverse iterator typedef. Containers that support bidirectional iterators have an associated reverse iterator typedef called *rit_C* where *C* is the container type name.

```C
typedef reverse_iterator_C * rit_C;
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rlt_it_C {#rlt_it_C-ad - }

Reverse iterator version of [lt_it_C](#lt_it_C-ad).

```C
int rlt_it_C(const rit_C it1, const rit_C it2);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rlteq_it_C {#rlteq_it_C-ad - }

Reverse iterator version of [lteq_it_C](#lteq_it_C-ad).

```C
int rlteq_it_C(const rit_C it1, const rit_C it2);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rnext_C {#rnext_C-adt - }

Reverse iterator version of [next_C](#next_C-adhst).

```C
rit_C rnext_C(rit_C const it);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rprepend_C {#rprepend_C-a - }

Reverse iterator version of [prepend_C](#prepend_C-ads). Appends an element to the back of a container.

```C
rit_C rprepend_C(C *array, T const element);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rprependp_C {#rprependp_C-a - }

Reverse iterator version of [prependp_C](#prependp_C-ads). Appends an element to the back of a container.

```C
rit_C rprependp_C(C *array, T const *element);
```


Containers:
array


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rprev_C {#rprev_C-adt - }

Reverse iterator version of [prev_C](#prev_C-adt).

```C
rit_C rprev_C(rit_C const it);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rsearch_C {#rsearch_C-adt - }

Reverse iterator version of [search_C](#search_C-adst).

```C
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rsearchp_C {#rsearchp_C-adt - }

Reverse iterator version of [searchp_C](#searchp_C-adst).

```C
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rsort_C {#rsort_C-ad - }

Reverse iterator version of [sort_C](#sort_C-ad). Sorts a semi-open range [first, last) in reverse order.

```C
rit_C rsort_C(rit_C from, rit_C to);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rsort_all_C {#rsort_all_C-ad - }

Reverse iterator version of [sort_all_C](#sort_all_C-ad). Sorts a container in reverse order.


```C
rit_C rsort_all_C(C *c);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rstable_sort_C {#rstable_sort_C-ad - }

Reverse iterator version of [stable_sort_all_C](#stable_sort_all_C-ads). Sorts a container in reverse order.

```C
rit_C rstable_sort_C(rit_C from, rit_C to);
```


Containers:
array	dlist


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rswap_C {#rswap_C-adt - }

Reverse iterator version of [swap_C](#swap_C-adhst).

```C
void rswap_C(rit_C a, rit_C b);
```


Containers:
array	dlist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### search_C {#search_C-adst - }

Search for a specified value in a semi-open range [first, last). The search is linear and uses the *cmp_func* function provided by the user when the container type was declared. Only available to container types declared with a *CMP* macro.


```C
it_C search_C(it_C first, const it_C last, T const key);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

first
  ~ Iterator pointing to first element in range.
last
  ~ Iterator pointing to one past the last element in range.
key
  ~ Element containing key that is being searched for, passed by address.

#### Return value {-}

Iterator pointing to matching element if found, else *end_C(c)*.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### search_all_C {#search_all_C-adst - }

Search for a specified value in a container. The search is linear and uses the *cmp_func* function provided by the user when the container type was declared. Only available to container types declared with a *CMP* macro.


```C
it_C search_all_C(C *c, T const d);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

c
  ~ Container to search.
key
  ~ Element containing key that is being searched for, passed by value.


#### Return value {-}

Iterator pointing to matching element if found, else *end_C(c)*.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### searchp_C {#searchp_C-adst - }

Search for a specified value in a semi-open range [first, last). The search is linear and uses the *cmp_func* function provided by the user when the container type was declared. Only available to container types declared with a *CMP* macro.


```C
it_C searchp_C(it_C first, const it_C last, T const *key);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}

first
  ~ Iterator pointing to first element in range.
last
  ~ Iterator pointing to one past the last element in range.
key
  ~ Element containing key that is being searched for, passed by address.


#### Return value {-}

Iterator pointing to matching element if found, else *end_C(c)*.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### searchp_all_C {#searchp_all_C-adst - }

Search for a specified value in a container. The search is linear and uses the *cmp_func* function provided by the user when the container type was declared. Only available to container types declared with a *CMP* macro.

```C
it_C searchp_all_C(C *c, T * const key);
```

Containers:
array	dlist	slist	tree


##### Parameters {-}

c
  ~ Container to search.
key
  ~ Element containing key that is being searched for, passed by address.


#### Return value {-}

Iterator pointing to matching element if found, else *end_C(c)*.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### set_exact_size_C {#set_exact_size_C-ads - }

Allocates an exact number of elements to a container. If the container has fewer elements than requested, more will be allocated. If it has more than the number of requested elements, the excess ones are deleted. If new elements have to be allocated, their values are not initialized. Useful sometimes before using *copy_over_C*.

```C
it_C set_exact_size_C(C *c, const size_t size);
```


Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to allocate an exact number of elements to.
size
  ~ Number of elements to allocate.

#### Return value {-}

Iterator pointing to last element allocated upon success. NULL upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### set_min_size_C {#set_min_size_C-ads - }

Allocates a minimum number of elements to a container. If the container already has the minimum number of elements, the function has no effect. If new elements have to be allocated, their values are not initialized. Useful sometimes before using *copy_over_C*.

```C
it_C set_min_size_C(C *c, const size_t size);
```

Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to allocate a minimum number of elements to.
size
  ~ Number of elements to allocate.

#### Return value {-}

Iterator pointing to last element allocated upon success. NULL upon failure.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### size_C {#size_C-a - }

Determines the number of elements in an array. Identical functionally to *distance_all*.

```C
size_t size_C(const C *array);
```


Containers:
array


##### Parameters {-}

array
  ~ Array for which size is needed.


#### Return value {-}

Number of elements in the array.

##### Example {-}

TO DO.

#### Complexity {-}

Constant size operation.

##### Data races {-}

Internal array variables are read but no elements are.

#### See also {-}

- [distance_all_C](#distance_all_C-adhst)

------


#### sort_C {#sort_C-ad - }

Sorts a semi-open range, [first, last). The order is defined by the *cmp_func* function provided by the user when declaring the container type. This is only available to containers declared with macros containing *CMP*.

```C
it_C stable_sort_C(it_C first, it_C last);
```


Containers:
array	dlist


##### Parameters {-}

first
  ~ Iterator pointing to first element in range.
last
  ~ Iterator pointing one past last element in range.

#### Return value {-}

Iterator pointing to first element in range.

##### Example {-}


#### Complexity {-}

The algorithm is implemented as an optimised Quicksort. It is $O(n \log n)$ on average, where $n$ is the number of comparisons. With extremely rare data sets it can degrade to O($n^2)$, but in practice such a data set would have to be designed.

The following optimisations have been implemented

- Both random and median of three pivoting are used.
- With small subsets of data, the algorithm switches to Insertion Sort.
- No recursion is used. A stack is manually maintained.

##### Data races {-}


#### See also {-}


------


#### sort_all_C {#sort_all_C-ad - }

Sorts a container. The order is defined by the *cmp_func* function provided by the user when declaring the container type. This is only available to containers declared with macros containing *CMP*.

```C
it_C sort_all_C(C *c);
```


Containers:
array	dlist


##### Parameters {-}

c
  ~ Container to sort.

#### Return value {-}

Iterator pointing to first element in container.

##### Example {-}

TO DO.

#### Complexity {-}

The algorithm is implemented as an optimised Quicksort. It is $O(n \log n)$ on average, where $n$ is the number of comparisons. With extremely rare data sets it can degrade to O($n^2)$, but in practice such a data set would have to be designed.

The following optimisations have been implemented

- Both random and median of three pivoting are used.
- With small subsets of data, the algorithm switches to Insertion Sort.
- No recursion is used. A stack is manually maintained.

##### Data races {-}


#### See also {-}


------


#### stable_sort_C {#stable_sort_C-ad - }

Sorts a semi-open range [first, last) in the order specified by the user-supplied *cmp_func* function. It is only defined for containers declared with a CMP containing macro. This sort is stable which means it retains two elements in the same original order if they have the same key. For arrays, this is not as fast as *sort_C*, but it is a good way to sort lists even if stability is not a requirement.


```C
it_C stable_sort_C(it_C first, it_C last);
```


Containers:
array	dlist


##### Parameters {-}

first
  ~ Iterator pointing to first element in range.
last
  ~ Iterator pointing one past last element in range.

#### Return value {-}

Iterator pointing to the beginning of the list. If a memory allocation error occurs, NULL is returned.

##### Example {-}

TO DO.

#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### stable_sort_all_C {#stable_sort_all_C-ads - }

Sorts a container in the order specified by the user-supplied *cmp_func* function. It is only defined for containers declared with a CMP containing macro. This sort is stable which means it retains two elements in the same original order if they have the same key. For arrays, this is not as fast as *sort_C*, but it is a good way to sort lists even if stability is not a requirement.

```C
it_C stable_sort_all_C(C *c);
```


Containers:
array	dlist	slist


##### Parameters {-}

c
  ~ Container to sort.

#### Return value {-}

Iterator pointing to the beginning of the list. If a memory allocation error occurs, NULL is returned.

##### Example {-}

TO DO.

#### Complexity {-}

This is an $O(n \log n)$ operation for number of comparisons where $n$ is the number of elements in the container.

##### Data races {-}


#### See also {-}


------


#### swap_C {#swap_C-adhst - }

Swaps the positions of two elements in a container.

```C
void swap_C(it_C a, it_C b);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}

a
  ~ Iterator pointing to first element to swap.
b
  ~ Iterator pointing to second element to swap.

#### Return value {-}


##### Example {-}

TO DO.

#### Complexity {-}

This is a constant time operation.

##### Data races {-}


##### See also {-}


------
