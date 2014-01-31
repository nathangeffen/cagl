# Appendix: Macro reference {#macros -}

**WORK IN PROGRESS ON THIS PART OF THE MANUAL**

*This part of the manual is going to be a long work in progress. The aim is to document every CAGL macro.*

## Container declaration and definition macros {#decdef -}

----

#### CAG_DEC_ARRAY {-}

Declares an array container type called *container* with elements of type *type*. Also declares associated structs, typedefs and function prototypes.

```C
CAG_DEC_ARRAY(container, type)
```

----

#### CAG_DEC_DLIST {-}

Declares a doubly-linked list container type called *container* with elements of type *type*. Also declares associated structs, typedefs and function prototypes.

```C
CAG_DEC_DLIST(container, type)
```

#### CAG_DEC_SLIST {-}

Declares a singly-linked list container type called *container* with elements of type *type*. Also declares associated structs, typedefs and function prototypes.

```C
CAG_DEC_SLIST(container, type)
```

#### CAG_DEF_ARRAY {-}

Defines the functions for a CAGL array container type called *container*, which has elements of type *type*. Usually used in conjunction with *CAG_DEC_ARRAY*.

```C
CAG_DEF_ARRAY(container, type);
```

#### CAG_DEF_DLIST {-}

Defines the functions for a CAGL doubly-linked list container type called *container*, which has elements of type *type*. Usually used in conjunction with *CAG_DEC_DLIST*.

```C
CAG_DEF_DLIST(container, type);
```


#### CAG_DEF_SLIST {-}

Defines the functions for a CAGL singly-linked list container type called *container*, which has elements of type *type*. Usually used in conjunction with *CAG_DEC_SLIST*.

```C
CAG_DEF_SLIST(container, type);
```

#### CAG_DEC_DEF_ARRAY {-}


This is equivalent to calling *CAG_DEC_ARRAY* followed by *CAG_DEF_ARRAY*.

```C
CAG_DEC_DEF_ARRAY(container, type)
```

#### CAG_DEC_DEF_DLIST {-}

This is equivalent to calling *CAG_DEC_DLIST* followed by *CAG_DEF_DLIST*.

```C
CAG_DEC_DEF_DLIST(container, type)
```

#### CAG_DEC_DEF_SLIST {-}

This is equivalent to calling *CAG_DEC_SLIST* followed by *CAG_DEF_SLIST*.

```C
CAG_DEC_DEF_SLIST(container, type)
```

#### CAG_DEC_CMP_ARRAY {-}

Declares a type called *container* which is a CAGL array container with elements of type *type*. In addition to declaring the same prototypes as *CAG_DEC_ARRAY* it also declares prototypes that facilitate ordering the container.

```C
CAG_DEC_CMP_ARRAY(container, type)
```


#### CAG_DEC_CMP_DLIST {-}

#### CAG_DEC_CMP_HASH {-}

#### CAG_DEC_CMP_SLIST {-}

#### CAG_DEC_CMP_TREE {-}

#### CAG_DEF_CMP_ARRAY {-}

Defines the functions for an orderable CAGL array container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMP_ARRAY*.

```C
CAG_DEF_CMP_ARRAY(container, type, cmp_func);
```

The *cmp_func* function is of the form:

```C
int cmp_func(type e1, type e2);
```


#### CAG_DEF_CMP_DLIST {-}

#### CAG_DEF_CMP_HASH {-}

#### CAG_DEF_CMP_SLIST {-}

#### CAG_DEF_CMP_TREE {-}

#### CAG_DEC_DEF_CMP_ARRAY {-}

This is equivalent to calling *CAG_DEC_CMP_ARRAY* then *CAG_DEF_CMP_ARRAY*.

```C
CAG_DEC_DEF_CMP_ARRAY(container, type, cmp_func);
```


#### CAG_DEC_DEF_CMP_DLIST {-}

#### CAG_DEC_DEF_CMP_HASH {-}

#### CAG_DEC_DEF_CMP_SLIST {-}

#### CAG_DEC_DEF_CMP_TREE {-}

#### CAG_DEC_CMPP_ARRAY {-}

This is identical to *CAG_DEC_CMP_ARRAY* but provided for consistent naming with *CAG_DEF_CMP_ARRAY*.

```C
CAG_DEC_CMPP_ARRAY(container, type)
```

#### CAG_DEC_CMPP_DLIST {-}

#### CAG_DEC_CMPP_HASH {-}

#### CAG_DEC_CMPP_SLIST {-}

#### CAG_DEC_CMPP_TREE {-}

#### CAG_DEF_CMPP_ARRAY {-}

Defines the functions for an orderable CAGL array container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMPP_ARRAY*.

The only difference between this macro and *CAG_DEF_CMP_ARRAY* is that the comparison function takes its arguments by address.

```C
CAG_DEF_CMPP_ARRAY(container, type, cmp_func);
```

The comparison function, *cmp_func*, is of the form:

```C
int cmp_func(type *e1, type *e2);
```


#### CAG_DEF_CMPP_DLIST {-}

#### CAG_DEF_CMPP_HASH {-}

#### CAG_DEF_CMPP_SLIST {-}

#### CAG_DEF_CMPP_TREE {-}

#### CAG_DEC_DEF_CMPP_ARRAY {-}

This is equivalent to calling *CAG_DEC_CMPP_ARRAY* then *CAG_DEF_CMPP_ARRAY*.

```C
CAG_DEC_DEF_CMPP_ARRAY(container, type, cmp_func);
```


#### CAG_DEC_DEF_CMPP_DLIST {-}

#### CAG_DEC_DEF_CMPP_HASH {-}

#### CAG_DEC_DEF_CMPP_SLIST {-}

#### CAG_DEC_DEF_CMPP_TREE {-}

#### CAG_DEF_ALL_ARRAY {-}


Defines the functions for a CAGL array container type. Usually used in conjunction with *CAG_DEC_ARRAY*.

```C
CAG_DEF_ALL(container, type, alloc_style, alloc_func, free_func, val_adr)
```

##### Parameters {-}

container
  ~ Name of the container type
type
  ~ Type of the container's elements.

alloc_style
  ~ Allocation style for managing the creation of elements in the container. See [Allocation Style Macros](#allocation-style).

alloc_func
  ~ Function that allocates memory and, optionally, initializes the element.

    For an *alloc_style* of *CAG_NO_ALLOC_STYLE* set this to *CAG_NO_ALLOC_FUNC*.

	For an *alloc_style* of *CAG_SIMPLE_ALLOC_STYLE*, this should typically be set to *malloc* or *CAG_ALLOC* (which is by default equal to *malloc*).

	For an *alloc_style* of *CAG_STRUCT_ALLOC_STYLE*, this is usually a custom written function or, in the case where the element is a CAGL container, a *new_from_[container]* function.

free_func
  ~ Function to destroy an element and return its memory to the heap.

    For an *alloc_style* of *CAG_NO_ALLOC_STYLE* set this to *CAG_NO_FREE_FUNC*.

	For an *alloc_style* of *CAG_SIMPLE_ALLOC_STYLE*, this should typically be set to free or CAG_FREE (which is by default equal to free).

	For an *alloc_style* of *CAG_STRUCT_ALLOC_STYLE*, this is usually a custom written function or, in the case where the element is a CAGL container, a *free_[container]* function.

val_adr
  ~ Indicates whether the allocation function, *alloc_func* and *free_func*, takes their parameters by address or by value. Both functions have to either take all their parameters by address or by value.

	For an *alloc_func* of *CAG_NO_ALLOC_FUNC* it makes no difference what this is set to because it isn't used.

    For an *alloc_func* of *malloc* or *CAG_ALLOC* this should usually be set to *CAG_BYVAL*. This might seem counter-intuitive because *malloc* and *free* take void parameters by address. But if the element of the container is a pointer, you want to pass it as is to *malloc* and *free*; you don't want to pass it by the address of the pointer.

	If the element is a container and  *alloc_func* is a *new_from_[container]* function, then this should be set CAG_BYADR.

	If *alloc_func* and *free_func* are custom written then it is up to the programmer, but *alloc_func* and *free_func* both have to use the same parameter passing method. Also if the container has a *cmp_func*, it too must have the same parameter passing method.

#### CAG_DEF_ALL_DLIST {-}

#### CAG_DEF_ALL_HASH {-}

#### CAG_DEF_ALL_SLIST {-}

#### CAG_DEF_ALL_TREE {-}

#### CAG_DEC_DEF_ALL_ARRAY {-}

This is equivalent to calling CAG_DEC_ARRAY then CAG_DEF_ALL_ARRAY.

```C
CAG_DEC_DEF_ALL_ARRAY(container, type, alloc_style, alloc_func, free_func, val_adr);
```

#### CAG_DEC_DEF_ALL_DLIST {-}

#### CAG_DEC_DEF_ALL_HASH {-}

#### CAG_DEC_DEF_ALL_SLIST {-}

#### CAG_DEC_DEF_ALL_TREE {-}

#### CAG_DEF_ALL_CMP_ARRAY {-}

Defines the functions for an orderable CAGL array container type. Usually used in conjunction with [CAG_DEC_CMP_ARRAY].

```C
CAG_DEF_ALL_CMP_ARRAY(container, type, cmp_func, val_adr, alloc_style, alloc_func, free_func);
```

#### Parameters {-}

container
  ~ Name of the container type

type
  ~ Type of the container's elements.

cmp_func
  ~ Comparison function for ordering the container. It is of one of these forms:

    ```C
    int cmp_func(type e1, type e2);
    ```

    ```C
    int cmp_func(type *e1, type *e2);
    ```

    If the first form is used then val_adr should be set to CAG_BYVAL. For the second form it should be set to CAG_BYADR.

val_adr
  ~ Indicates whether the comparison function, *cmp_func*, allocation function, *alloc_func* and *free_func*, take their parameters by address or by value. All three functions have to either take all their parameters by address or by value.

    For an *alloc_func* of *CAG_NO_ALLOC_FUNC* it makes no difference what this is set to because it isn't used.

    For an *alloc_func* of *malloc* or *CAG_ALLOC* this should usually be set to *CAG_BYVAL*. This might seem counter-intuitive because *malloc* and *free* take void parameters by address. But if the element of the container is a pointer, you want to pass it as is to *malloc* and *free*; you don't want to pass it by the address of the pointer.

    If the element is a container and  *alloc_func* is a *new_from_[container]* function, then this should be set CAG_BYADR.

	If *alloc_func* and *free_func* are custom written then it is up to the programmer, but *alloc_func* and *free_func* both have to use the same parameter passing method. Also if the container has a *cmp_func*, it too must have the same parameter passing method.

alloc_style
  ~ Allocation style for managing the creation of elements in the container. See [Allocation Style Macros].

alloc_func
  ~ Function that allocates memory and, optionally, initializes the element.

    For an *alloc_style* of *CAG_NO_ALLOC_STYLE* set this to *CAG_NO_ALLOC_FUNC*.

    For an *alloc_style* of *CAG_SIMPLE_ALLOC_STYLE*, this should typically be set to *malloc* or *CAG_ALLOC* (which is by default equal to *malloc*).

    For an *alloc_style* of *CAG_STRUCT_ALLOC_STYLE*, this is usually a custom written function or, in the case where the element is a CAGL container, a *new_from_[container]* function.

free_func
  ~ Function to destroy an element and return its memory to the heap.

    For an *alloc_style* of *CAG_NO_ALLOC_STYLE* set this to *CAG_NO_FREE_FUNC*.

    For an *alloc_style* of *CAG_SIMPLE_ALLOC_STYLE*, this should typically be set to free or CAG_FREE (which is by default equal to free).

    For an *alloc_style* of *CAG_STRUCT_ALLOC_STYLE*, this is usually a custom written function or, in the case where the element is a CAGL container, a *free_[container]* function.


#### CAG_DEF_ALL_CMP_DLIST {-}

#### CAG_DEF_ALL_CMP_HASH {-}

#### CAG_DEF_ALL_CMP_SLIST {-}

#### CAG_DEF_ALL_CMP_TREE {-}

#### CAG_DEC_DEF_ALL_CMP_ARRAY {-}

This is equivalent to calling CAG_DEC_CMP_ARRAY then CAG_DEF_ALL_CMP_ARRAY.

```C
CAG_DEC_DEF_ALL_CMP_ARRAY(container, type, cmp_func, val_adr, alloc_style,
                          alloc_func, free_func);
```


#### CAG_DEC_DEF_ALL_CMP_DLIST {-}

#### CAG_DEC_DEF_ALL_CMP_HASH {-}

#### CAG_DEC_DEF_ALL_CMP_SLIST {-}

#### CAG_DEC_DEF_ALL_CMP_TREE {-}


## Algorithm macros {#algorithm-macros -}

#### CAG_CMP_PRIMITIVE {#cag_cmp_primitive}

Macro that can be used as a comparison function by any container whose elements are C primitives.

#### CAG_CMPP_PRIMITIVE {#cag_cmpp_primitive}

Macro that can be used as a comparison function by any container whose elements are pointers to C primitives.


#### CAG_SORT_ARRAY {#cag_sort_array -}

Macro that can be used to sort and CAGL array container type with a *cmp_func* defined or any C array. Sorts the semi-open range [first, last).

```C
CAG_SORT_ARRAY(type, first, last, cmp_func)
```

##### Parameters {-}

type
  ~ Element type of the array
first
  ~ Pointer or iterator to first element in the range to sort from
last
  ~ Pointer or iterator to last element in the semi-open range

##### Example {-}

```C
/* Demonstrates how to use the CAG_SORT_ARRAY macro. */

#include <stdio.h>
#include <stdlib.h>
#include <cagl/array.h>

CAG_DEC_DEF_CMP_ARRAY(int_array, int, CAG_CMP_PRIMITIVE);

void demo_sort_C_array()
{
	int *it;
	int iarr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	CAG_SORT_ARRAY(int, iarr, iarr + 10, CAG_CMP_PRIMITIVE);

	for (it = iarr; it != iarr + 10; ++it)
		printf("%d ", *it);
	printf("\n");
}

void demo_sort_CAGL_container()
{
	int_array arr;
	it_int_array it;
	int i;

	new_int_array(&arr);
	for (i = 9; i >= 0; --i)
		append_int_array(&arr, i);
	CAG_SORT_ARRAY(int, beg_int_array(&arr), end_int_array(&arr),
		       CAG_CMP_PRIMITIVE);
	for (it = beg_int_array(&arr); it != end_int_array(&arr);
	     it = next_int_array(it))
		printf("%d ", it->value);
	printf("\n");
	free_int_array(&arr);
}

int main(void)
{
	printf("Sorting C array of integers\n");
	demo_sort_C_array();
	printf("Sorting CAGL array of integers\n");
	demo_sort_CAGL_container();
	return 0;
}
```

The output is:

```
Sorting C array of integers
0 1 2 3 4 5 6 7 8 9
Sorting CAGL array of integers
0 1 2 3 4 5 6 7 8 9
```

##### Complexity {-}

This is an $O(n \log n)$ operation on average. Worst case behaviour is *extremely* rare and $O(n^2)$.

The implementation is a highly optimised Quicksort that changes to Insertion Sort when sub-arrays are sufficiently small. The optimizations ensure that standard Quicksort's worst behaviour, e.g. with already sorted, nearly sorted or reverse-sorted lists, doesn't occur. Theoretically, orderings will exist which will result in worst-case behaviour. However, unless adversary methods are used, the odds of such worst-case  orderings occurring are vanishingly small.

## Allocation style macros {#allocation-style -}

Containers need to know how to allocate memory for their elements. A parameter called *alloc_style* is used by the container definitions to determine this. Several predefined *allocation style* macros are provided.

**ADVANCED:** Users can write their own allocation style macros but it is rare that this should be necessary. In the case that you do wish to write an allocation style macro, see how these are used in the containers (the code in cagl/hash.h is useful to examine). All allocation style macros must take these four parameters.

to
  ~ Element being created.

from
  ~ Element from which the *to* element is being created

alloc_func
  ~ Function for allocating the memory for an element

free_code
  ~ Function for freeing the memory of allocated for a container element. This function will only be called in certain cases where an error occurred after memory for an element has been successfully allocated, but must now be freed to undo the effects of the error.

#### CAG_NO_ALLOC_STYLE {-}

```C
#define CAG_SIMPLE_ALLOC_STYLE(to, from, alloc_func, free_code)               \
    if(! (to = alloc_func(from))) free_code;
```

This allocation is typically used for containers that do not manage the memory of their elements or for elements that are primitive types.

#### CAG_SIMPLE_ALLOC_STYLE {-}

```C
#define CAG_SIMPLE_ALLOC_STYLE(to, from, alloc_func, free_code)               \
    if(! (to = alloc_func(from))) free_code;
```

This allocation style is typically used for containers whose elements are pointers and need to be managed, e.g. elements that are C strings (char *).

#### CAG_STRUCT_ALLOC_STYLE {-}

```C
#define CAG_STRUCT_ALLOC_STYLE(to, from, alloc_func, free_code)               \
    if(! (alloc_func(&to, &from))) free_code;
```

This allocation style is typically used for containers whose elements are complex structs which need custom allocation functions (analogous to C++ copy constructors) written for them.
