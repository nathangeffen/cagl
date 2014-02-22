# CAGL's main macros and functions

Every CAGL macro and function is prefixed *CAG_* (for macros) or *cag_* (for functions). Please consider this the namespace for CAGL and do not prefix your own variables and types with *CAG_*.

The CAGL container header files are:

cagl/array.h
  ~ For declaring and defining automatically sized arrays similar to a C++ STL *vector*.

cagl/dlist.h
  ~ For declaring and defining doubly-linked lists similar to a C++ STL *list*.

cagl/slist.h
  ~ For declaring and defining singly-linked lists similar to a C++11 STL *forward_list*.

cagl/tree.h
  ~ For declaring and defining balanced binary trees (implemented as red-black trees) similar to a C++ STL *map*.

cagl/hash.h
  ~ For declaring and defining hash tables similar to a C++11 *unordered_map*.

The simplest way to declare and define a container whose elements are a particular type is to do this:

```
CAG_DEC_DEF_[ARRAY/DLIST/SLIST/HASH/TREE](name of container type, element type, ... other parameters ...);
```

This declares a doubly-linked list of doubles container type. It also declares forward and reverse iterators for the list. It defines a bunch of functions to manipulate the list:

```C
CAG_DEC_DEF_DLIST(dbl_list, double);
```

The first parameter is the name of the container type. You can give this any legal C type name. The second parameter is an already existing C pointer, primitive type or struct (or *typedef*ed version thereof).

The *DEC* stands for *declare* and the *DEF* stands for *define*.

In C you should declare functions you use in each module that uses it, but you may only define the functions once. Therefore if you need a list of doubles in two C files, say file1.c and file2.c, you would declare the type in, say, file1.h as follows:

```C
CAG_DEC_DLIST(dbl_list, double);
```

You would need to include file1.h in both file1.c and file2. Then in one C source code file only, say file1.c, you would define the functions like this:

```C
CAG_DEF_DLIST(dbl_list, double);
```

Arrays and lists may be ordered. Balanced binary trees have to be ordered. To declare an ordered CAGL container, you need to pass the definition macro a comparison function or macro. For example, this declares a binary tree of integers:

```C
CAG_DEC_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);
```

The comparison mechanism we've provided is a CAGL macro called *CAG_CMP_PRIMITIVE*. It compares primitive types. Instead of using *CAG_CMP_PRIMITIVE* You could define your own comparison macro or function that takes two integer parameters by value. It should returns zero if the two parameters are equal, a negative integer if the first parameter is less than the second and a positive integer if it is bigger.

You could also declare and define *int_tree* this way:

```C
CAG_DEC_CMP_TREE(int_tree, int);
CAG_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);
```

For C++ STL users, the *int_tree* type is similar to a std::map\<int\>.

The *CMP* part of the macro indicates that this is a container that requires a comparison function (or comparison macro such as CAG_CMP_PRIMITIVE), usually to maintain the container in a particular order. However, not all all containers that require a comparison function are ordered.


Hash tables require a comparison function but are not ordered. Hash table container types require a slightly more complicated macro to define them. This declares and defines a hash table of C strings type called *str_hash*.

```C
CAG_DEC_DEF_CMP_HASH(str_hash, char *, strcmp, CAG_STRING_HASH, strlen);
```

- The first parameter, *str_hash*, is the name of the container type.

- The second parameter is the C string type, _char *_, which tells CAGL that the elements of this container are strings.

- The third parameter is the comparison function, which in this example is the C standard library function *strcmp*.

- The fourth parameter is the default CAGL string hash function, *CAG_STRING_HASH*.

- The final parameter is a function for calculating the length of each element, the C standard library function *strcmp*. This is needed by *CAG_STRING_HASH*.

The following code snippet is an example of how to populate the hash table:

```C
static void populate_my_hash_table(str_hash *h)
{
	char *k0 = "k0";
	char *k1 = "k1";
	char *k2 = "k2";
	char *k3 = "k3";
	char *k4 = "k4";

	insert_str_hash(h, k0);
	insert_str_hash(h, k1);
	insert_str_hash(h, k2);
	insert_str_hash(h, k3);
	insert_str_hash(h, k4);
}
```

For primitive types and many types of structs, the default *CAG_DEC_DEF* or *CAG_DEF* macros are often sufficient. However, the string hash table type we declared has a limitation. The CAGL container manages the memory of the container itself, but it does no memory management of the elements of the container.

In the *str_hash* container k0 through k5 are assigned constants. If instead k0 to k5 were declared as string arrays of length 3 whose values were copied into them using strcpy, their values would be undefined as soon soon as *populate_str_hash* finished executing, even if the hash table remained alive. Attempting to access the elements from the function that called them would likely result in getting garbage values or a crash.

CAGL offers a way for its containers to manage the memory of its elements. The C++ STL containers manage the memory of their elements through the constructors and destructors of those elements. C does not have constructors and destructors, so CAGL needs to be told how to allocate and de-allocate its elements. CAGL macros that begin *CAG_DEF_ALL_* allow you to define the allocation and deallocation functions or macros for your container's elements.

Here is an example of a tree that manages the memory for C strings:

```C
CAG_DEC_TREE(string_tree, char *)
CAG_DEF_ALL_CMP_TREE(string_tree, char *, strcmp, CAG_BYVAL,
                     CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free);
```

The parameters of the call to CAG_DEF_ALL_CMP_TREE are as follows:

1. *string_tree* is the name of the container type.

1. _char *_ is the C string type. This is the element type of the container.

1. *strcmp* is the C standard function for comparing strings.

1. *CAG_BYVAL* is a CAGL macro which tells CAGL that the comparison function for this container, *strcmp*, takes its parameters by value and there is therefore no need to prefix the parameters with the & sign. Inside CAGL, all calls to *strcmp* with C strings (e.g. _char *s1_ and _char *s2_) will therefore be of the correct form *strcmp(s1, s2);* and not the incorrect *strcmp(&s1, &s2);*.  The alternative to *CAG_BYVAL* is *CAG_BYADR*. See the [Deck of cards](#deck-of-cards) example to understand when you'd use *CAG_BYADR*.

1. *CAG_SIMPLE_ALLOC_STYLE* is a CAGL macro which specifies how the allocation function should be invoked. For pointer elements, like _char *_, *CAG_SIMPLE_ALLOC_STYLE* is almost always sufficient. See [Allocation style macros](#allocation-style) for other options.

1. *cag_strdup* is a CAGL function that allocates a C string from the heap. Most C libraries provide a *strdup* function which you could also use, or you could write your own.

1. *free* is the standard C free function that will deallocate a string once it is no longer needed.

Now you can insert C strings into a *string_tree* container without worrying about managing their memory in the container. CAGL will handle this for you. When the container is freed using the *free_string_tree* that has been generated, all the elements will be freed too.
