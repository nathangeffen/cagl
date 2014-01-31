# Appendix: Function blueprint reference {#functions -}

**WORK IN PROGRESS ON THIS PART OF THE MANUAL**

*This part of the manual is going to be a long work in progress. The aim is to document every CAGL function blueprint.*

In this reference:

- A function *blueprint* describes the form of a CAGL function that is generated with one of the CAGL container macros that begin *CAG_DEC_*.

- Substitute *C* with the name of the container type.

- Substitute *T* with the container's element type.

- The words *array*, *dlist*, *hash*, *tree* and *slist* refer to the CAGL container types array, doubly-linked list, hash table, binary tree and singly-linked list respectively.

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

------


#### back_C {#back_C-adt - }

Returns pointer to last element of a container.

```C
T *back_C(const C *C);
```


Containers:
array	dlist	tree


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

This is a constant time operation.

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

TODO

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

TODO

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

TODO

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
int binary_search_all_C(C *c, T key);
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

TODO

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

TODO

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
int binary_searchp_all_C(C *c, T * key);
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

TODO.

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

TODO

#### Complexity {-}

This is an $\theta(n)$ operation where $n$ is the number of elements in the tree.

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

TODO

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

TODO

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

TODO

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_all_C](#copy_all_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)

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

TODO.

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)

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

TODO

#### Complexity {-}


##### Data races {-}


#### See also {-}

- [copy_C](#copy_C-adhst)
- [copy_if_C](#copy_if_C-adhst)
- [copy_if_all_C](#copy_if_all_C-adhst)
- [copy_many_C](#copy_many_C-adhst)
- [copy_over_C](#copy_over_C-adhst)


------


#### copy_many_C {#copy_many_C-adhst - }


```C
int copy_many_C(C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### copy_over_C {#copy_over_C-adhst - }


```C
it_C copy_over_C(it_C first, const it_C last, it_C result);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### distance_C {#distance_C-adhst - }


```C
size_t distance_C(const it_C from, const it_C to);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### distance_all_C {#distance_all_C-adhst - }


```C
size_t distance_all_C(const C *c);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### end_C {#end_C-adhst - }


```C
it_C end_C(const C *array);
```


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


```C
int equal_all_C(const C *c1, const C *c2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### equal_range_C {#equal_range_C-adst - }


```C
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_C {#erase_C-adht - }


```C
it_C erase_C(C *array, it_C it);
```


Containers:
array	dlist	hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_after_C {#erase_after_C-s - }


```C
it_C erase_after_C(it_C it);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_after_range_C {#erase_after_range_C-s - }


```C
it_C erase_after_range_C(it_C from, it_C to);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_all_C {#erase_all_C-adhst - }


```C
it_C erase_all_C(C *c);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_front_C {#erase_front_C-s - }


```C
it_C erase_front_C(C *slist);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### erase_range_C {#erase_range_C-adhst - }


```C
it_C erase_range_C(C *array, it_C from, it_C to);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### find_C {#find_C-adhst - }


```C
it_C find_C(it_C from, const it_C to, const T element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### find_all_C {#find_all_C-adhst - }


```C
it_C find_all_C(const C *c, const T element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### findp_C {#findp_C-adhst - }


```C
it_C findp_C(it_C from, const it_C to, const T *element, int (*cmp_func)(const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### findp_all_C {#findp_all_C-adhst - }


```C
it_C findp_all_C(C *c, const T* element, int (*cmp_func) (const T*, const T*));
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### free_C {#free_C-adhst - }


```C
void free_C(C *array);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### free_many_C {#free_many_C-adhst - }


```C
void free_many_C(int max, C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### free_node_C {#free_node_C-t - }


```C
void free_node_C(C *tree, it_C it);
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### front_C {#front_C-adst - }


```C
T *front_C(const C *array);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### get_C {#get_C-ht - }


```C
it_C get_C(const it_C root, const T element);
```


Containers:
hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### getp_C {#getp_C-ht - }


```C
it_C getp_C(const it_C root, const T *element);
```


Containers:
hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### index_C {#index_C-adhst - }


```C
it_C index_C(C *c, size_t n);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### inorder_C {#inorder_C-t - }


```C
void inorder_C(it_C root, void *data, void (*action)(it_C, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_C {#insert_C-adht - }


```C
it_C insert_C(C *array, it_C position, T const element);
```


Containers:
array	dlist	hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_after_C {#insert_after_C-s - }


```C
it_C insert_after_C(C *slist, it_C it, T item);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insert_gt_C {#insert_gt_C-ad - }


```C
it_C insert_gt_C(C *c, it_C position, T element);
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


#### insert_gteq_C {#insert_gteq_C-ad - }


```C
it_C insert_gteq_C(C *c, it_C position, T element);
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


#### insert_lt_C {#insert_lt_C-ad - }


```C
it_C insert_lt_C(C *c, it_C position, T element);
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


#### insert_lteq_C {#insert_lteq_C-ad - }


```C
it_C insert_lteq_C(C *c, it_C position, T element);
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


#### insertp_C {#insertp_C-adht - }


```C
it_C insertp_C(C *array, it_C position, T const *element);
```


Containers:
array	dlist	hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_after_C {#insertp_after_C-s - }


```C
it_C insertp_after_C(C *slist, it_C it, T *item);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### insertp_gt_C {#insertp_gt_C-ad - }


```C
it_C insertp_gt_C(C *c, it_C position, T *element);
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


#### insertp_gteq_C {#insertp_gteq_C-ad - }


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


```C
it_C insertp_lt_C(C *c, it_C position, T *element);
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


#### insertp_lteq_C {#insertp_lteq_C-ad - }


```C
it_C insertp_lteq_C(C *c, it_C position, T *element);
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


#### it_C {#it_C-adhst - }


```C
typedef iterator_C * it_C;
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### last_C {#last_C-s - }


```C
it_C last_C(const C *slist);
```


Containers:
slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### levelorder_C {#levelorder_C-t - }


```C
void levelorder_C(C *tree, void *data, void (*action)(it_C, size_t, int, int, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### lower_bound_C {#lower_bound_C-a - }


```C
it_C lower_bound_C(it_C first, it_C last, T const key);
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


#### lower_bound_all_C {#lower_bound_all_C-a - }


```C
it_C lower_bound_all_C(C *c, T d);
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


#### lower_boundp_C {#lower_boundp_C-a - }


```C
it_C lower_boundp_C(it_C first, it_C last, T const *key);
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


#### lower_boundp_all_C {#lower_boundp_all_C-a - }


```C
it_C lower_boundp_all_C(C *c, T * d);
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


#### lower_rbound_C {#lower_rbound_C-a - }


```C
rit_C lower_rbound_C(rit_C first, rit_C last, T const key);
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


#### lower_rboundp_C {#lower_rboundp_C-a - }


```C
rit_C lower_rboundp_C(rit_C first, rit_C last, T const *key);
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


#### lt_it_C {#lt_it_C-ad - }


```C
int lt_it_C(const it_C it1, const it_C it2);
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


#### lteq_it_C {#lteq_it_C-ad - }


```C
int lteq_it_C(const it_C it1, const it_C it2);
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
/* Demonstrates different uses of *new* on an array.

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
		free_many_int_arr(i, &a4, &a5, NULL);
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
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5, NULL);
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


```C
C *new_from_C(C *to, const C *from);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### new_many_C {#new_many_C-adhst - }


```C
int new_many_C(C *c, ...);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### new_with_buckets_C {#new_with_buckets_C-h - }


```C
C *new_with_buckets_C(C *hash, const size_t buckets);
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


```C
C *new_with_size_C(C *array, const size_t size);
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


#### next_C {#next_C-adhst - }


```C
it_C next_C(it_C const it);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### postorder_C {#postorder_C-t - }


```C
void postorder_C(it_C root, void *data, void (*action)(it_C, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### preorder_C {#preorder_C-t - }


```C
void preorder_C(it_C root, void *data, void (*action)(it_C, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### prepend_C {#prepend_C-ads - }


```C
it_C prepend_C(C *array, T const element);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### prependp_C {#prependp_C-ads - }


```C
it_C prependp_C(C *array, T const *element);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### prev_C {#prev_C-adt - }


```C
it_C prev_C(it_C const it);
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


#### put_C {#put_C-adhst - }


```C
it_C put_C(C *array, it_C position, T const element);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### putp_C {#putp_C-ad - }


```C
it_C putp_C(C *array, it_C position, T const *element);
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


#### random_shuffle_C {#random_shuffle_C-ad - }


```C
it_C random_shuffle_C(const it_C from, it_C to);
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


#### random_shuffle_all_C {#random_shuffle_all_C-ad - }


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


```C
rit_C rat_C(rit_C it, const size_t n);
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


#### rback_C {#rback_C-a - }


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


#### rdistance_all_C {#rdistance_all_C-t - }


```C
size_t rdistance_all_C(C *c);
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rehash_C {#rehash_C-h - }


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


```C
it_C remove_C(C *tree, const T element);
```


Containers:
hash	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### removep_C {#removep_C-h - }


```C
it_C removep_C(C *hash, const T *element);
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


#### rend_C {#rend_C-adt - }


```C
rit_C rend_C(const C *array);
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


```C
it_C reverse_C(it_C first, it_C last);
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


#### reverse_all_C {#reverse_all_C-ads - }


```C
it_C reverse_all_C(C *c);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### reverseorder_C {#reverseorder_C-t - }


```C
void reverseorder_C(rit_C root, void *data, void (*action)(rit_C, void *));
```


Containers:
tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### rfind_C {#rfind_C-adt - }


```C
rit_C rfind_C(rit_C from, const rit_C to, const T element, int (*cmp_func) (const T*, const T*));
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


#### rfindp_C {#rfindp_C-adt - }


```C
rit_C rfindp_C(rit_C from, const rit_C to, const T *element, int (*cmp_func)(const T*, const T*));
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


#### rfront_C {#rfront_C-a - }


```C
T *rfront_C(const C *array);
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


#### rinsert_C {#rinsert_C-a - }


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


```C
typedef reverse_iterator_C * rit_C;
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


#### rlt_it_C {#rlt_it_C-ad - }


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


```C
int rlteq_it_C(const rit_C it1, const rit_C it2);
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


#### rnext_C {#rnext_C-adt - }


```C
rit_C rnext_C(rit_C const it);
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


#### rprepend_C {#rprepend_C-a - }


```C
rit_C rprepend_C(C *array, T const element);
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


#### rprependp_C {#rprependp_C-a - }


```C
rit_C rprependp_C(C *array, T const *element);
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


#### rprev_C {#rprev_C-adt - }


```C
rit_C rprev_C(rit_C const it);
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


#### rsearch_C {#rsearch_C-adt - }


```C
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
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


#### rsearchp_C {#rsearchp_C-adt - }


```C
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);
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


#### rsort_C {#rsort_C-ad - }


```C
rit_C rsort_C(rit_C from, rit_C to);
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


#### rsort_all_C {#rsort_all_C-ad - }


```C
rit_C rsort_all_C(C *c);
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


#### rstable_sort_C {#rstable_sort_C-ad - }


```C
rit_C rstable_sort_C(rit_C from, rit_C to);
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


#### rswap_C {#rswap_C-adt - }


```C
void rswap_C(rit_C a, rit_C b);
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


#### search_C {#search_C-adst - }


```C
it_C search_C(it_C first, const it_C last, T const key);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### search_all_C {#search_all_C-adst - }


```C
it_C search_all_C(C *c, T d);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### searchp_C {#searchp_C-adst - }


```C
it_C searchp_C(it_C first, const it_C last, T const *key);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### searchp_all_C {#searchp_all_C-adst - }


```C
it_C searchp_all_C(C *c, T * d);
```


Containers:
array	dlist	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### set_exact_size_C {#set_exact_size_C-ads - }


```C
it_C set_exact_size_C(C *array, const size_t size);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### set_min_size_C {#set_min_size_C-ads - }


```C
it_C set_min_size_C(C *array, it_C it, const size_t size);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### size_C {#size_C-a - }


```C
C *new_with_size_C(C *array, const size_t size);
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


#### sort_C {#sort_C-ad - }


```C
it_C stable_sort_C(it_C from, it_C to);
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


#### sort_all_C {#sort_all_C-ad - }


```C
it_C sort_all_C(C *c);
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


#### stable_sort_C {#stable_sort_C-ad - }


```C
it_C stable_sort_C(it_C from, it_C to);
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


#### stable_sort_all_C {#stable_sort_all_C-ads - }


```C
it_C stable_sort_all_C(C *c);
```


Containers:
array	dlist	slist


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


#### See also {-}


------


#### swap_C {#swap_C-adhst - }


```C
void swap_C(it_C a, it_C b);
```


Containers:
array	dlist	hash	slist	tree


##### Parameters {-}


#### Return value {-}


##### Example {-}


#### Complexity {-}


##### Data races {-}


##### See also {-}


------
