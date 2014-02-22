# Understanding CAGL macro, struct, type and function names {#names}

CAGL is easier to use if you understand the naming conventions:

- CAGL macros are prefixed *CAG_*.

- CAGL functions (not generated ones) are prefixed *cag_*.

- If you look at the CAGL source code, you'll notice some macros are prefixed *CAG_P_*. These are macros for internal use (the *P* is for private). Don't call them and don't depend on them to be unchanged or even continuing to exist between CAGL versions. The same goes for functions prefixed *cag_p_*.

- This is obvious but for the sake of completion:

	- Macros with ARRAY in their names deal with array container types.

	- Macros with DLIST in their names deal with doubly-linked list container types.

	- Macros with HASH in their names deal with hash table container types.

	- Macros with SLIST in their names deal with singly-linked list container types.

	- Macros with TREE in their names deal with balanced binary tree container types.


- The container declaration and definition macros are the foundation of CAGL. Their naming conventions are used consistently across the different types of containers. See also the [reference for these macros](#decdef).

    - 	These declare the container and iterator struct, and function prototypes. Any module that uses a CAGL container must declare its structs and prototypes. \
	\
	CAG_DEC_ARRAY \
	CAG_DEC_DLIST  \
	CAG_DEC_SLIST

	- 	These declare the container and iterator struct, and function prototypes for containers that require a comparison function. \
	\
	CAG_DEC_CMP_ARRAY \
	CAG_DEC_CMP_DLIST \
	CAG_DEC_CMP_SLIST \
	CAG_DEC_CMP_TREE \
	CAG_DEC_CMP_HASH

	- These are identical to the corresponding CAG_DEC_CMP_ macros but for legibility purposes it is recommended that these be paired with the corresponding CAG_DEF_CMPP_ macros described below. \
	\
	CAG_DEC_CMPP_ARRAY \
	CAG_DEC_CMPP_DLIST \
	CAG_DEC_CMPP_SLIST \
	CAG_DEC_CMPP_TREE \
	CAG_DEC_CMPP_HASH

	- These define the functions for container types. These macros should only be used once per container type. They call corresponding CAG_DEF_ALL_  prefixed macros using default parameters and are intended to cover the most common cases. \
	\
	CAG_DEF_ARRAY \
	CAG_DEF_DLIST \
	CAG_DEF_SLIST

	- These define the functions for container types that require a comparison function. They call corresponding CAG_DEF_ALL_CMP_ prefixed macros using default parameters and are intended to cover the most common cases. The comparison function takes two elements by value as parameters. \
	\
	CAG_DEF_CMP_ARRAY \
	CAG_DEF_CMP_DLIST \
	CAG_DEF_CMP_SLIST \
	CAG_DEF_CMP_TREE \
	CAG_DEF_CMP_HASH

	- These are identical to the corresponding CAG_DEF_CMP_ macros except the comparison function takes its parameters by address. \
	\
	CAG_DEF_CMPP_ARRAY \
	CAG_DEF_CMPP_DLIST \
	CAG_DEF_CMPP_SLIST \
	CAG_DEF_CMPP_TREE \
	CAG_DEF_CMPP_HASH

	- These combine declarations and definitions in one call. \
	\
	CAG_DEC_DEF_ARRAY \
	CAG_DEC_DEF_DLIST \
	CAG_DEC_DEF_SLIST

	- These combine declarations and definitions in one call for containers that require comparison functions that take two elements by value. \
	\
	CAG_DEC_DEF_CMP_ARRAY \
	CAG_DEC_DEF_CMP_DLIST \
	CAG_DEC_DEF_CMP_SLIST \
	CAG_DEC_DEF_CMP_HASH \
	CAG_DEC_DEF_CMP_TREE

	- These are identical to the corresponding CAG_DEC_DEF_CMP_ macros except that the comparison functions takes its two elements by address.\
	\
	CAG_DEC_DEF_CMPP_ARRAY \
	CAG_DEC_DEF_CMPP_DLIST \
	CAG_DEC_DEF_CMPP_SLIST \
	CAG_DEC_DEF_CMPP_HASH \
	CAG_DEC_DEF_CMPP_TREE \

	- These define the functions for container types. These macros should only be used once per container type. These expect more parameters than their like-named counterparts without *ALL* in them. \
	\
	CAG_DEF_ALL_ARRAY \
	CAG_DEF_ALL_DLIST \
	CAG_DEF_ALL_SLIST

	- These definition macros are the most flexible and have the most number of parameters, including a comparison function. \
	\
	CAG_DEF_ALL_CMP_ARRAY \
	CAG_DEF_ALL_CMP_DLIST \
	CAG_DEF_ALL_CMP_SLIST \
	CAG_DEF_ALL_CMP_TREE \
	CAG_DEF_ALL_CMP_HASH

- The generated functions for a container type are suffixed with the name of the container type. E.g. if you declare an array type of integers called *int_array* then all the generated structs, types and functions are suffixed *int_array*.

- A *function blueprint* is represented by the name of a function followed by *_C*. It corresponds to a function that will be generated for a container type when a CAG_DEC or CAG_DEF macro is called. For example, consider these declaration and definition macros: *CAG_DEC_DEF_ARRAY(int_array, int)* *CAG_DEC_DEF_DLIST(char_dlist, char)*. Then *new_C* is the function blueprint for *new_int_array*, *new_char_dlist* and any new_[container type] function. Function blueprints are a fiction. They are merely a convenient way to talk about CAGL functions in the documentation. The same goes for iterator blueprints such as it_C and rit_C

- Functions that take an element as a parameter often come in two forms: by value or by address. The verb of the by address version will be suffixed with a *p*. E.g. *append_C(&container variable, element)* and *appendp_C(&container variable, &element)* are by value and by address functions respectively, where *C* is the name of the container type.

- Every container type has an iterator type defined for it: *it_C* or _struct iterator\_C *_ where *C* is the name of the container type.

- Bidirectional containers (arrays, doubly-linked lists and trees) also have a reverse iterator type defined: *rit_C* or _struct iterator\_[container] *_.

- Bidirectional containers have reverse iterator versions defined for several functions. Reverse functions are prefixed with an *r*, e.g. *rsort_all_[container]*.

- Some functions have variations that can operate on a single element and a range of elements. The version that operates on a range will have its verb suffixed with *range\_*. E.g. *erase_C*, *erase_range_C*. However, if a function only sensibly can operate on a range, then the *range\_* suffix is omitted. E.g. there is a *sort\_C* function but not a *sort_range_C* one.

- Functions that operate on a range typically can also operate on the entire container and an *_all* version will be defined. E.g. *sort_all_C* and *random_shuffle_all_C*.
