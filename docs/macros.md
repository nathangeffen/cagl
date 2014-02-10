# Appendix: Macro reference {#macros -}

**WORK IN PROGRESS ON THIS PART OF THE MANUAL**

*This part of the manual is going to be a long work in progress. The aim is to document every CAGL macro.*

## Container declaration and definition macros {#decdef -}

There are over 70 macros for declaring and defining container types. This might seem daunting, but much less so if the naming conventions are understood.

All these macros have either ARRAY, DLIST, HASH, SLIST or TREE as their suffix.

There are five containers and there are macros with CAG_DEC_CMP_ and CAG_DEF_CMP_ prefixes for each of them.

In addition, ARRAY, DLIST and SLIST have CAG_DEC_ and CAG_DEF_ prefix macros.

The above constitute the most commonly used declaration and definition macros.

They each have a corresponding macro prefixed CAG_DEC_DEF_ for ease of use if a container is only being used in one module.

They each also have corresponding macros prefixed CAG_DEF_ALL_ and CAG_DEC_DEF_ALL.

Some convenience macros have also been defined for frequently needed data types such as hash tables and trees whose elements are C strings or dictionaries (a struct composed of two C strings, the first string being the key).

The following code gives an example of how to invoke every macro.

```C
#include <cagl/array.h>
#include <cagl/dlist.h>
#include <cagl/slist.h>
#include <cagl/hash.h>
#include <cagl/tree.h>

CAG_DEC_ARRAY(int_a, int);
CAG_DEF_ARRAY(int_a, int);

CAG_DEC_DLIST(int_d, int);
CAG_DEF_DLIST(int_d, int);

CAG_DEC_SLIST(int_s, int);
CAG_DEF_SLIST(int_s, int);

CAG_DEC_DEF_ARRAY(int_dd_a, int);
CAG_DEC_DEF_DLIST(int_dd_d, int);
CAG_DEC_DEF_SLIST(int_dd_s, int);

CAG_DEC_CMP_ARRAY(int_ca, int);
CAG_DEF_CMP_ARRAY(int_ca, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_DLIST(int_cd, int);
CAG_DEF_CMP_DLIST(int_cd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_SLIST(int_cs, int);
CAG_DEF_CMP_SLIST(int_cs, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_HASH(int_ch, int);
CAG_DEF_CMP_HASH(int_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_CMP_TREE(int_ct, int);
CAG_DEF_CMP_TREE(int_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMP_ARRAY(int_dd_ca, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_DLIST(int_dd_cd, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_SLIST(int_dd_cs, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_HASH(int_dd_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);
CAG_DEC_DEF_CMP_TREE(int_dd_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_ARRAY(int_pca, int);
CAG_DEF_CMPP_ARRAY(int_pca, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_DLIST(int_pcd, int);
CAG_DEF_CMPP_DLIST(int_pcd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_SLIST(int_pcs, int);
CAG_DEF_CMPP_SLIST(int_pcs, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_HASH(int_pch, int);
CAG_DEF_CMPP_HASH(int_pch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_CMPP_TREE(int_pct, int);
CAG_DEF_CMPP_TREE(int_pct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMPP_ARRAY(int_pdd_ca, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMPP_DLIST(int_pdd_cd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMPP_SLIST(int_pdd_cs, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMPP_HASH(int_pdd_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_DEF_CMPP_TREE(int_pdd_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_ARRAY(int_aa, int);
CAG_DEF_ALL_ARRAY(int_aa, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_DLIST(int_ad, int);
CAG_DEF_ALL_DLIST(int_ad, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_SLIST(int_as, int);
CAG_DEF_ALL_SLIST(int_as, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_DEF_ALL_ARRAY(int_dd_aa, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);
CAG_DEC_DEF_ALL_DLIST(int_dd_ad, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);
CAG_DEC_DEF_ALL_SLIST(int_dd_as, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_CMP_ARRAY(int_caa, int);
CAG_DEF_ALL_CMP_ARRAY(int_caa, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_CMP_DLIST(int_cad, int);
CAG_DEF_ALL_CMP_DLIST(int_cad, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_SLIST(int_cas, int);
CAG_DEF_ALL_CMP_SLIST(int_cas, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_CMP_HASH(int_cas_ah, int);
CAG_DEF_ALL_CMP_HASH(int_cas_ah, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 cag_int_hash, sizeof,
		     CAG_NO_ALLOC_STYLE,
		     CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_CMP_TREE(int_cas_at, int);
CAG_DEF_ALL_CMP_TREE(int_cas_at, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		     CAG_NO_ALLOC_STYLE,
		     CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_DEF_ALL_CMP_ARRAY(int_cdd_aa, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_DLIST(int_cdd_ad, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_SLIST(int_cdd_as, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_HASH(int_cdd_ah, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 cag_int_hash, sizeof,
				 CAG_NO_ALLOC_STYLE,
				 CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_TREE(int_cdd_at, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 CAG_NO_ALLOC_STYLE,
				 CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_STR_HASH(sh);
CAG_DEF_STR_HASH(sh);
CAG_DEC_DEF_STR_HASH(ddsh);

CAG_DEC_STR_TREE(st);
CAG_DEF_STR_TREE(st);
CAG_DEC_DEF_STR_TREE(ddst);

CAG_DEC_STR_ARRAY(sa);
CAG_DEF_STR_ARRAY(sa);
CAG_DEC_DEF_STR_ARRAY(ddsa);

CAG_DEC_STR_DLIST(sd);
CAG_DEF_STR_DLIST(sd);
CAG_DEC_DEF_STR_DLIST(ddsd);

CAG_DEC_STR_SLIST(ss);
CAG_DEF_STR_SLIST(ss);
CAG_DEC_DEF_STR_SLIST(ddss);

struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_STR_STR_HASH(dh, struct dictionary);
CAG_DEF_STR_STR_HASH(dh, struct dictionary);
CAG_DEC_DEF_STR_STR_HASH(dddh, struct dictionary);

CAG_DEC_STR_STR_TREE(dt, struct dictionary );
CAG_DEF_STR_STR_TREE(dt, struct dictionary);
CAG_DEC_DEF_STR_STR_TREE(dddt, struct dictionary);


int main(void)
{
	return 0;
}
```

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

Declares a type called *container* which is a CAGL doubly-linked list container with elements of type *type*. In addition to declaring the same prototypes as *CAG_DEC_DLIST* it also declares prototypes that facilitate ordering the container.

```C
CAG_DEC_CMP_DLIST(container, type)
```

#### CAG_DEC_CMP_HASH {-}

Declares a type called *container* which is a CAGL hash table container with elements of type *type*.

```C
CAG_DEC_CMP_HASH(container, type)
```

#### CAG_DEC_CMP_SLIST {-}

Declares a type called *container* which is a CAGL singly-linked list container with elements of type *type*. In addition to declaring the same prototypes as *CAG_DEC_SLIST* it also declares prototypes that facilitate ordering the container.

CAG_DEC_CMP_SLIST(container, type)

#### CAG_DEC_CMP_TREE {-}

Declares a type called *container* which is a CAGL balanced binary tree container with elements of type *type*.

CAG_DEC_CMP_TREE(container, type)

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

Defines the functions for an orderable CAGL doubly-linked list container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMP_DLIST*.

```C
CAG_DEF_CMP_DLIST(container, type, cmp_func);
```

The *cmp_func* function is of the form:

```C
int cmp_func(type e1, type e2);
```

#### CAG_DEF_CMP_HASH {-}

Defines the functions for a CAGL hash table container type called *container*, which has elements of type *type*, a comparison function *cmp_func*, a hash function, *hash_func* and a function to calculate the number of bytes in an element, *length_func*. Usually used in conjunction with *CAG_DEC_CMP_HASH*.

```C
container, type, cmp_func, hash_func, length_func);
```

The *cmp_func* function is of the form:

```C
int cmp_func(type e1, type e2);
```

#### CAG_DEF_CMP_SLIST {-}

Defines the functions for an orderable CAGL singly-linked list container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMP_SLIST*.

```C
CAG_DEF_CMP_SLIST(container, type, cmp_func);
```

The *cmp_func* function is of the form:

```C
int cmp_func(type e1, type e2);
```


#### CAG_DEF_CMP_TREE {-}

Defines the functions for a CAGL balanced binary tree container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMP_TREE*.

```C
CAG_DEF_CMP_TREE(container, type, cmp_func);
```

The *cmp_func* function is of the form:

```C
int cmp_func(type e1, type e2);
```

#### CAG_DEC_DEF_CMP_ARRAY {-}

This is equivalent to calling *CAG_DEC_CMP_ARRAY* then *CAG_DEF_CMP_ARRAY*.

```C
CAG_DEC_DEF_CMP_ARRAY(container, type, cmp_func);
```

#### CAG_DEC_DEF_CMP_DLIST {-}

This is equivalent to calling *CAG_DEC_CMP_DLIST* then *CAG_DEF_CMP_DLIST*.

```C
CAG_DEC_DEF_CMP_DLIST(container, type, cmp_func);
```

#### CAG_DEC_DEF_CMP_HASH {-}

This is equivalent to calling *CAG_DEC_CMP_HASH* then *CAG_DEF_CMP_HASH*.

```C
CAG_DEC_DEF_CMP_HASH(container, type, cmp_func, hash_func, length_func);
```

#### CAG_DEC_DEF_CMP_SLIST {-}

This is equivalent to calling *CAG_DEC_CMP_SLIST* then *CAG_DEF_CMP_SLIST*.

```C
CAG_DEC_DEF_CMP_SLIST(container, type, cmp_func);
```

#### CAG_DEC_DEF_CMP_TREE {-}

This is equivalent to calling *CAG_DEC_CMP_TREE* then *CAG_DEF_CMP_TREE*.

```C
CAG_DEC_DEF_CMP_TREE(container, type, cmp_func);
```

#### CAG_DEC_CMPP_ARRAY {-}

This is identical to *CAG_DEC_CMP_ARRAY* but provided for consistent naming with *CAG_DEF_CMPP_ARRAY*.

```C
CAG_DEC_CMPP_ARRAY(container, type)
```

#### CAG_DEC_CMPP_DLIST {-}

This is identical to *CAG_DEC_CMP_DLIST* but provided for consistent naming with *CAG_DEF_CMPP_DLIST*.

```C
CAG_DEC_CMPP_ARRAY(container, type)
```

#### CAG_DEC_CMPP_HASH {-}

This is identical to *CAG_DEC_CMP_HASH* but provided for consistent naming with *CAG_DEF_CMPP_HASH*.

```C
CAG_DEC_CMPP_ARRAY(container, type)
```

#### CAG_DEC_CMPP_SLIST {-}

This is identical to *CAG_DEC_CMP_SLIST* but provided for consistent naming with *CAG_DEF_CMPP_SLIST*.

```C
CAG_DEC_CMPP_ARRAY(container, type)
```

#### CAG_DEC_CMPP_TREE {-}

This is identical to *CAG_DEC_CMP_TREE* but provided for consistent naming with *CAG_DEF_CMPP_TREE*.

```C
CAG_DEC_CMPP_TREE(container, type)
```

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

Defines the functions for an orderable CAGL doubly-linked list container type called *container*, which has elements of type *type* and a comparison function *cmp_func*. Usually used in conjunction with *CAG_DEC_CMPP_DLIST*.

The only difference between this macro and *CAG_DEF_CMP_DLIST* is that the comparison function takes its arguments by address.

```C
CAG_DEF_CMPP_DLIST(container, type, cmp_func);
```

The comparison function, *cmp_func*, is of the form:

```C
int cmp_func(type *e1, type *e2);
```

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

#### CAG_DEF_ALL_SLIST {-}

#### CAG_DEC_DEF_ALL_ARRAY {-}

This is equivalent to calling CAG_DEC_ARRAY then CAG_DEF_ALL_ARRAY.

```C
CAG_DEC_DEF_ALL_ARRAY(container, type, alloc_style, alloc_func, free_func, val_adr);
```

#### CAG_DEC_DEF_ALL_DLIST {-}

#### CAG_DEC_DEF_ALL_SLIST {-}

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

#### CAG_DEC_STR_ARRAY {-}

Convenience macro that declares an array of C strings. Use in conjunction with *CAG_DEF_STR_ARRAY*. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::vector<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_array* but this is arbitrary.

```C
CAG_DEC_STR_ARRAY(string_array);
```

#### CAG_DEF_STR_ARRAY {-}

Convenience macro that defines an array of C strings. Use in conjunction with *CAG_DEC_STR_ARRAY*. The element type is _char *_.

ALL memory is managed for you. This is similar to the C++ STL std::vector<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_array* but this is arbitrary.

```C
CAG_DEF_STR_ARRAY(string_array);
```

#### CAG_DEC_DEF_STR_ARRAY {-}

Convenience macro to declare and define an array of C strings. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::vector<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_array* but this is arbitrary.

```C
CAG_DEC_DEF_STR_ARRAY(string_array);
```

#### CAG_DEC_STR_DLIST {-}

Convenience macro that declares a doubly-linked list of C strings. Use in conjunction with *CAG_DEF_STR_DLIST*. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEC_STR_DLIST(string_list);
```

#### CAG_DEF_STR_DLIST {-}

Convenience macro that defines a doubly-linked list of C strings. Use in conjunction with *CAG_DEC_STR_DLIST*. The element type is _char *_.

ALL memory is managed for you. This is similar to the C++ STL std::list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEF_STR_DLIST(string_list);
```

#### CAG_DEC_DEF_STR_DLIST {-}

Convenience macro to declare and define a doubly-linked list of C strings. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEC_DEF_STR_DLIST(string_list);
```

#### CAG_DEC_STR_SLIST {-}

Convenience macro that declares a singly-linked list of C strings. Use in conjunction with *CAG_DEF_STR_SLIST*. The element type is _char *_.

All memory is managed for you. This is similar to the C++11 STL std::forward_list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEC_STR_SLIST(string_list);
```

#### CAG_DEF_STR_SLIST {-}

Convenience macro that defines a singly-linked list of C strings. Use in conjunction with *CAG_DEC_STR_SLIST*. The element type is _char *_.

ALL memory is managed for you. This is similar to the C++11 STL std::forward_list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEF_STR_SLIST(string_list);
```

#### CAG_DEC_DEF_STR_SLIST {-}

Convenience macro to declare and define a singly-linked list of C strings. The element type is _char *_.

All memory is managed for you. This is similar to the C++11 STL std::forward_list<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_list* but this is arbitrary.

```C
CAG_DEC_DEF_STR_SLIST(string_list);
```

#### CAG_DEC_STR_HASH {-}

Convenience macro that declares a hash table of C strings. Use in conjunction with *CAG_DEF_STR_HASH*. The element type is _char *_.

All memory is managed for you. This is similar to the C++11 STL std::unordered_map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_hash_table* but this is arbitrary.

```C
CAG_DEC_STR_HASH(string_hash_table);
```


#### CAG_DEF_STR_HASH {-}

Convenience macro that defines a hash table of C strings. Use in conjunction with *CAG_DEC_STR_HASH*. The element type is _char *_.

All memory is managed for you. This is similar to the C++11 STL std::unordered_map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_hash_table* but this is arbitrary.

```C
CAG_DEF_STR_HASH(string_hash_table);
```

#### CAG_DEC_DEF_STR_HASH {-}

Convenience macro to declare and define a hash table of C strings. The element type is _char *_.

All memory is managed for you. This is similar to the C++11 STL std::unordered_map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_hash_table* but this is arbitrary.

```C
CAG_DEC_DEF_STR_HASH(string_hash_table);
```

#### CAG_DEC_STR_TREE {-}

Convenience macro that declares a tree of C strings. Use in conjunction with *CAG_DEF_STR_TREE*. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_tree* but this is arbitrary.



```C
CAG_DEC_STR_TREE(string_tree);
```

#### CAG_DEF_STR_TREE {-}

Convenience macro that defines a tree of C strings. Use in conjunction with *CAG_DEC_STR_TREE*. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_tree* but this is arbitrary.


```C
CAG_DEF_STR_TREE(string_tree);
```

#### CAG_DEC_DEF_STR_TREE {-}

Convenience macro that declares and defines a tree of C strings. The element type is _char *_.

All memory is managed for you. This is similar to the C++ STL std::map<std::string>.

You can give the container any valid identifier name. In the example syntax below we've chosen *string_tree* but this is arbitrary.


```C
CAG_DEC_DEF_STR_TREE(string_tree);
```


#### CAG_DEC_STR_STR_HASH {-}

Convenience macro that declares a hash table of dictionary entries. Use in conjunction with *CAG_DEF_STR_STR_HASH*.

All memory is managed for you. This is similar to the C++11 STL *std::unordered_map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_STR_STR_HASH(dict_hash, struct dictionary);
```


#### CAG_DEF_STR_STR_HASH {-}

Convenience macro that defines a hash table of dictionary entries. Use in conjunction with *CAG_DEC_STR_STR_HASH*.

All memory is managed for you. This is similar to the C++ STL *std::unordered_map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEF_STR_STR_HASH(dict_hash, struct dictionary);
```

#### CAG_DEC_DEF_STR_STR_HASH {-}

Convenience macro that declares and defines a hash table of dictionary entries.

All memory is managed for you. This is similar to the C++ STL *std::unordered_map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_DEF_STR_STR_HASH(dict_hash, struct dictionary);
```

#### CAG_DEC_STR_STR_TREE {-}

Convenience macro that declares a tree of dictionary entries. Use in conjunction with *CAG_DEF_STR_STR_TREE*.

All memory is managed for you. This is similar to the C++ STL *std::map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_STR_STR_TREE(dict_tree, struct dictionary );
```

#### CAG_DEF_STR_STR_TREE {-}

Convenience macro that defines a tree of dictionary entries. Use in conjunction with *CAG_DEC_STR_STR_TREE*.

All memory is managed for you. This is similar to the C++ STL *std::map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEF_STR_STR_TREE(dict_tree, struct dictionary );
```

#### CAG_DEC_DEF_STR_STR_TREE {-}

Convenience macro that declares and defines a tree of dictionary entries.

All memory is managed for you. This is similar to the C++ STL *std::map< std::pair< std::string, std::string > >*.


```C
struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_DEF_STR_STR_TREE(dict_tree, struct dictionary );
```

## Copy macros {#copy-macros -}

The function blueprints for copying only work for copying between containers of the same type. This is unfortunately quite inflexible. The C++ STL, for example, supports copying between any two containers that store the same element type. To help get around this a number of macros are provided to assist with copying between different types of containers with the same types of elements.


#### CAG_COPY {#cag_copy -}

Copy all the elements from one container to another. The elements of the two containers should be the same type. The container to copy to is assumed to be empty. The macro copies using the *put_C* function which allocates memory for the copied to container.

```C
CAG_COPY(container_from, c_from, container_to, c_to, result)
```

##### Parameters {-}

container_from
  ~ Container type of container variable to copy from.
c_from
  ~ Container variable to copy from.
container_to
  ~ Container type of container variable to copy to.
c_to
  ~ Container variable to copy to.
result
  ~ Integer that will be set to false if the copy fails, else it is set to true.

#### CAG_COPY_IF {#cag-copy-if}

Copy elements meeting a user specified condition from one container to another. The elements of the two containers should be the same type. The container to be copy to is assumed to be empty. The macro copies using the *put_C* function which allocates memory for the copied to container.

```C
CAG_COPY_IF(container_from, c_from, container_to, c_to, cond_func, result)
```

##### Parameters {-}

container_from
  ~ Container type of container variable to copy from.
c_from
  ~ Container variable to copy from.
container_to
  ~ Container type of container variable to copy to.
c_to
  ~ Container variable to copy to.
cond_func
  ~ User supplied macro expression or function that takes one argument, an element by value, and returns either true or false. If it returns true, the corresponding element from *c_from* will be copied to *c_to*.
result
  ~ Integer that will be set to false if the copy fails, else it is set to true.


#### CAG_COPY_OVER {#cag-copy-over -}

Copy elements from one container to another. There should be sufficient space in the copied to container.

```C
CAG_COPY_OVER(container_from, c_from, container_to, c_to)
```

##### Parameters {-}

container_from
  ~ Container type of container variable to copy from.
c_from
  ~ Container variable to copy from.
container_to
  ~ Container type of container variable to copy to.
c_to
  ~ Container variable to copy to.

#### CAG_COPY_OVER_IF {#cag-copy-over-if -}

Copy elements meeting a specified condition from one container to another. There should be sufficient space in the copied to container.

##### Parameters {-}

container_from
  ~ Container type of container variable to copy from.
c_from
  ~ Container variable to copy from.
container_to
  ~ Container type of container variable to copy to.
c_to
  ~ Container variable to copy to.
cond_func
  ~ User supplied macro expression or function that takes one argument, an element by value, and returns either true or false. If it returns true, the corresponding element from *c_from* will be copied to *c_to*.

#### CAG_RCOPY {#cag_copy -}

Reverse copy from one container to another. Parameters are identical to [CAG_COPY](#cag_copy). The container being copied from should support bidirectional iterators.

#### CAG_RCOPY_IF {#cag-copy-if -}

Reverse copy elements meeting a user-specified condition from one container to another. Parameters are identical to [CAG_COPY_IF](#cag_copy_if). The container being copied from should support bidirectional iterators.

#### CAG_RCOPY_OVER {#cag-copy-over -}

Reverse copy elements from one container over the elements in another. Parameters are identical to [CAG_COPY_OVER](#cag_copy_over). The container being copied from should support bidirectional iterators.

#### CAG_RCOPY_OVER_IF {#cag-copy-over-if -}

Reverse copy elements meeting a user-specified condition from one container over the elements in another. Parameters are identical to [CAG_COPY_OVER_IF](#cag_copy_over_if). The container being copied from should support bidirectional iterators.


#### CAG_CONCAT {#cag-concat-if}

This is the most flexible of the copy macros (except it can't currently be used with hash tables). It concatenates all the elements from one container that meet a user-specified condition to the end of another. If the second container is empty, then this is equivalent to a copy. The *cond_func* takes an extra parameter, *data*, which is any user defined data. This allows the cond_func to track information between calls. Memory allocation is handled by CAGL.

```C
CAG_CONCAT_IF(container_from, c1, container_to, c2, cond_func, data, result)
```

##### Parameters {-}

container_from
  ~ Container type of container variable to copy elements from.
c_from
  ~ Container variable to copy elements from.
container_to
  ~ Container type of container variable to concatenate elements to.
c_to
  ~ Container variable to concatenate to.
cond_func
  ~ User supplied macro expression or function that takes two arguments, an element by value and user data (which can be NULL. It returns either true or false. If it returns true, the corresponding element from *c_from* will be copied to *c_to*.
data
  ~ User supplied data. Can be NULL if there is no need to track information between calls to *cond_func*.
result
  ~ Set to CAG_TRUE if operation is successful, else CAG_FALSE if a memory allocation error occurs.


#### CAG_CONCAT {#cag-concat}

Concatenates all the elements from one container to the end of another. Memory allocation is handled by CAGL. Works for arrays, lists and trees.

```C
CAG_CONCAT(container_from, c_from, container_to, c_to, result)
```

container_from
  ~ Container type of container variable to copy elements from.
c_from
  ~ Container variable to copy elements from.
container_to
  ~ Container type of container variable to concatenate elements to.
c_to
  ~ Container variable to concatenate to.
result
  ~ Set to CAG_TRUE if operation is successful, else CAG_FALSE if a memory allocation error occurs.



## Ordering macros {#ordering-macros -}

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

## SLIST iterator macros {#slist-iterator-macros -}

#### CAG_MAKE_SLIST {-}

Creates a new list node in *it* from *element*. The *element* parameter must be of the type that *it* holds. If memory allocation fails, *it* will be NULL.

```C
CAG_MAKE_SLIST(it, element);
```

#### CAG_PREPEND_SLIST {-}

Prepends *it* to *head* and sets *head* back to being the head of the list.

```C
CAG_PREPEND_SLIST(head, it);
```

This is equivalent to these two lines of code:

```C
it->next = head;
head = it;
```

#### CAG_INSERT_AFTER_SLIST {-}

Inserts *it_to_insert* after *it_already_in_list*.

```C
CAG_INSERT_AFTER_SLIST(it_already_in_list, it_to_insert)
```

#### CAG_DISTANCE_SLIST {-}

Calculates the distance between two iterators and places the result in *count*. Neither *it_from* nor *it_to* is modified. If *it_to* is not after *it_from* in the list, results are undefined and a crash or infinite loop is likely.

To determine the length of the whole list set *it_from* to the head of the list and *it_to* to NULL. Note this is an $\theta(n)$ operation where $n$ is the length of the list. If you frequently need the size of your whole list, it is more efficient to track it yourself.

```C
CAG_DISTANCE_SLIST(it_from, it_to, count)
```

#### CAG_ERASE_ALL_SLIST {-}

Erases the list. Set *free_func* to *CAG_NO_FREE_FUNC* if there is no need to deallocate each element's memory. Else set it to a function to do the deallocation. For elements that are pointers, this would typically just be *free*.

```C
CAG_ERASE_ALL_SLIST(head, free_func);
```

#### CAG_ERASE_SLIST {-}

Erases it from a list. The element before *it* will be linked to the element after *it*.

```C
CAG_ERASE_SLIST(it, free_func);
```

#### CAG_ERASE_RANGE_SLIST {-}

Erases a range of elements in a list.

```
CAG_ERASE_RANGE_SLIST(from, to, free_func);
```

#### CAG_ERASE_AFTER_SLIST {-}

Erases the element after *it* in a list. The list will be correctly linked after the operation. *it->next* must not be NULL.

```C
CAG_ERASE_AFTER_SLIST(it, free_func) \
```

#### CAG_ERASE_AFTER_RANGE_SLIST {-}

Erases the elements in the open range (from, to). *from* should be at least one element before *to*. The latter can be NULL. If *from->next* is NULL, then  *to* must be NULL.

```
CAG_ERASE_AFTER_RANGE_SLIST(from, to, free_func);
```

#### CAG_REVERSE_SLIST {-}

Reverses the list.

```C
CAG_REVERSE_SLIST(head, from)
```

#### CAG_STABLE_SORT_SLIST {-}

Sorts a list.

```C
CAG_STABLE_SORT_SLIST(iterator_type, list, cmp_func, val_adr)
```

##### Parameters {-}

iterator_type
  ~ Iterator type of the list.
list
  ~ List to sort.
cmp_func
  ~ Comparison function that takes two elements.
val_adr
  ~ Set to CAG_BYADR if *cmp_func* takes its element arguments by address, else set to CAG_BYVAL.

#### CAG_STABLE_SORT_SLIST_DEFAULT {-}

Sorts a list.

```C
CAG_STABLE_SORT_SLIST(iterator_type, list, cmp_func, val_adr)
```

##### Parameters {-}

iterator_type
  ~ Iterator type of the list.
list
  ~ List to sort.
cmp_func
  ~ Comparison function that takes two elements by value.
