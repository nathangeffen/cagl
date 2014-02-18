# Appendix: Modifying, enhancing and improving CAGL {-}

You are encouraged to help improve the C Almost Generic Library (CAGL).
This document describes some of the technical "under-the-hood" details
of how the library works.

These notes currently assume you use *gcc* and *autotools* in a GNU/Linux environment. However, in time it must be possible to develop CAGL in other environments.

## CAGL structure {-}

Specific code for each container is placed in its own header (.h) file. So arrays are handled in array.h. Let's say you wanted to implement a container called *stack*. You would create a file called stack.h. Examine the code in array.h to get an idea of what should go in stack.h. Pay special attention to the implementations of CAG_DEC_ARRAY, CAG_DEF_ALL_ARRAY, CAG_DEC_CMP_ARRAY and CAG_DEF_ALL_CMP_ARRAY.

All containers must implement macros to generate *new_C*, *begin_C*, *end_C* and *free_C* functions.

The common.h file implements several algorithms as macros. These are generic in that they are used across containers.

When implementing a container such as a stack, code specific to the container will go in stack.h, but many of the algorithms, declarations and definitions in common.h should be reusable as is.

The concepts.h file reduces coding and helps CAGL containers be consistent. It helps CAGL approximate generic concepts used in the C++ STL. Containers automatically get a bunch of useful functions declared and defined by calling the macros defined in concepts.h. Here are brief explanations of them:

CAG_DEC_FORWARD and CAG_DEF_FORWARD
  ~ These declare and define functions for containers whose iterators support the *next* operation (i.e. almost any useful container). Iterators that implement *next* are *forward* iterators.

CAG_DEC_CMP_FORWARD and CAG_DEF_CMP_FORWARD
  ~ These declare and define functions for containers that have comparison functions and support forward iterators.

CAG_DEC_BIDIRECTIONAL and CAG_DEF_BIDIRECTIONAL
  ~ Iterators that implement both *prev* and *next* functions are bidirectional. *Forward* iterators are a subset of bidirectional iterators. These macros implement functions for containers whose iterators are *bidirectional*.

CAG_DEC_CMP_BIDIRECTIONAL and CAG_DEF_CMP_BIDIRECTIONAL
  ~ These declare and define functions for containers that have comparison functions and support bidirectional iterators.

CAG_DEC_REORDERABLE and CAG_DEF_REORDERABLE
  ~ Functions for containers that can be reordered are implemented by these macros. For example arrays and lists can be reordered but the order for the elements in a binary tree cannot be changed, so arrays and lists are reorderable and binary trees are not. Hash tables have no ordering so it isn't useful to reorder their elements.

CAG_DEC_CMP_REORDERABLE and CAG_DEF_CMP_REORDERABLE
  ~ These declare and define functions for containers that have comparison functions and are reorderable.

CAG_DEC_RANDOMACCESS and CAG_DEF_RANDOMACCESS
  ~ These declare and define the functions of CAG_DEC_BIDIRECTIONAL and CAG_DEF_BIDIRECTIONAL. In addition they declare and define additional functions for containers whose iterators support an *at* function. Containers that implement an *at* operation are *random access* iterators.

CAG_DEC_CMP_RANDOMACCESS and CAG_DEF_CMP_RANDOMACCESS
  ~ These declare and define functions for containers that have comparison functions and support random access iterators.

## Functions and algorithms that containers usually provide {-}

Almost any useful container will support the functions of *CAG_DEC_FORWARD* and *CAG_DEF_FORWARD*. These macros need the container to provide the following functions: [new_C](#new_C-adhst), [new_from_C](#new_from_C-adhst), [free_C](#free_C-adhst), [begin_C](#begin_C-adhst), [end_C](#end_C-adhst), [distance_C](#distance_C-adhst), [distance_all_C](#distance_all_C-adhst), [next_C](#next_C-adhst), [put_C](#put_C-adhst) and [putp_C](#putp_C).


## Macro parameters {-}

The parameters used by the CAGL macros are described here.

*apply\_func*
:   User supplied function in an apply macro. Apply macros are used to
    generate a full container version of a function that works on an
    iterator range. For example, the apply macro for
    distance\_[container](from, to) is
    distance\_all\_[container](container variable).

*at*
:   Function to find iterator that is *n* places from the input
    parameter, i.e. at\_[container].

*alloc\_style*
:   A macro to tell a container how to allocate memory for its elements.
    Three are provided by CAGL and should be sufficient for nearly all
    cases: CAG\_SIMPLE\_ALLOC\_STYLE (used for assigning allocated
    memory to pointers), CAG\_STRUCT\_ALLOC\_STYLE (used for assigning
    allocated memory to structures) and CAG\_NO\_ALLOC\_STYLE (used when
    the container does not have to assign memory for its elements).

*alloc\_func*
:   A function or macro to do memory allocation. For pointers this is
    often malloc. For containers that do not need to do memory
    allocation, use CAG\_NO\_ALLOC\_FUNC. C++ programmers can think of
    this as a constructor for the container element.

*apply\_func*
:   Function to apply to every element in a container.

*begin*
:   Function to find beginning of container, i.e. begin\_[container] or
    rbegin\_[container].

*cmp\_func*
:   Function that takes two iterators and compares their values.

*container*
:   Container type.

*container\_var*
:   Container variable.

*dir*
:   Function to step an iterator forward or backward, i.e.
    next\_[container], prev\_[container], rnext\_[container] or
    rprev\_[container].

*distance*
:   Function to calculate distance between two iterators. i.e.
    distance\_[container] or rdistance\_[container].

*end*
:   Function to find end of container, i.e. end\_[container] or
    rend\_[container].

*first*
:   Iterator pointing to the first element in a sequence. Synonymous
    with *from*.

*free\_func*
:   User provided function to free an element of a container. Use
    CAG\_NO\_FREE\_FUNC if the container does not allocate memory for
    its elements. For elements that are pointers, *free* is often
    sufficient. C++ programmers could think of this as a destructor.

*free\_code*
:   This is a parameter to an *alloc\_style* macro. It is code that
    frees any already allocated memory in the case of a failed
    allocation that succeeded partially. The partially allocated memory
    should be returned to the heap with this code. It is seldom that
    users should have to implement this.

*from*
:   Iterator pointing to the first element in a sequence. Synonymous
    with *first*.

*from\_1*
:   Iterator pointing to the first element in a sequence. Used when
    multiple sequences are being operated upon.

*from\_2*
:   Iterator pointing to the first element in a sequence. Used when
    multiple sequences are being operated upon.

*func*
:   Function supplied to generic algorithm.

*function*
:   Name of function in macro declaration and definition.

*hash\_func*
:   User provided function for hash tables, but some reasonable hashing
    functions are provided by the CAGL.

*insert\_out*
:   Function that inserts a value into an output iterator.

*it*
:   An *iterator\_type* variable mostly used in the functional
    programming macros.

*iterator\_type*
:   Iterator type, generally of the form it\_[container] or
    rit\_[container].

*iterator\_type\_1*
:   Same as *iterator\_type* but used when two or more sequences are
    operated upon.

*iterator\_type\_2*
:   Same as *iterator\_type* but used when two or more sequences are
    operated upon.

*iterator\_type\_in*
:   Same as *iterator\_type* but specifically for input sequence.

*key*
:   Usually compared against an iterator value in a search.

*last*
:   Iterator pointing to the last element in a sequence.

*length\_func*
:   User provided function that calculates the length of a type
    variable. Used for hashing.

*next*
:   Function to step an iterator forward, i.e. next\_[container] or
    rnext\_[container].

*next\_1*
:   Same as *next* but used when two are more sequences are operated
    upon.

*next\_2*
:   Same as *next* but used when two are more sequences are operated
    upon.

*next\_in*
:   Same as *next* but for input iterator.

*next\_out*
:   Same as *next* but for output iterator.

*prev*
:   Function to step an iterator backward, i.e. prev\_[container] or
    rprev\_[container].

*result*
:   Iterator pointing to first element in output sequence.

*swap*
:   Function to swap two iterators, i.e. swap\_[container].

*type*
:   Data type of the elements operated on in a container. Every CAGL
    container has an iterator type. Every iterator type has a field
    called value, which has a type indicated by this parameter.

*val\_adr*
:   Either equal to blank or &. Used by *cmp\_func* to compare by value
    or by address.

*value*
:   Function or macro to get the value of an iterator. For all CAGL
    containers, this should always be *it-\>next* where *it* is an
    iterator variable.

## Coding conventions {-}

CAGL is mostly made up of macros. There's a small amount of non-macro
code in a a few small .c libraries. This makes CAGL quite unusual. It is
hard to read complex C macros and CAGL is almost entirely composed of
them. So it's important to stick to conventions that makes reading the
code easier.

-   Use Kernighan & Ritchie coding conventions in the .h files.

-   After editing a .h file (say file.h), run: *slash79 file.h*.

-   Use the Linux Kernel coding conventions in the .c files.

-   Place the end-of-line slash \**\** in column 79.

-   All macros in CAGL start CAG\_.

-   All C functions (there aren't many of them) start cag\_.

-   Macros that are strictly for internal use should be prefixed
    CAG\_P\_ (the "P" stands for private).

## Compiling and running in development

To compile in development the *cagl* sub-directory has to be in the *C_INCLUDE_PATH* environment variable. If you use any CAGL provided functions, as opposed to macros, you'll also need to compile and link cagl/common.c when compiling your own source code.

## Tools to help work with CAGL {-}

Compiling and debugging macro code can be tricky. Here are tools to help.

#### cpp_gcc {-}

In the *bin* directory of the distribution is a script called *cpp_gcc* which compiles a single source file to a file called *tmp.c*. You can use this to find compiler errors.

#### Test scripts {-}

The makefile runs several test scripts. Before checking in code, you should pass all the tests. These include compiling the code with different compilers, doing a release build, running the test suite and running the test suite through Valgrind.

To run the test scripts, run *make check* in the root folder of the CAGL distribution. It can take a couple of minutes.

To run just the test suite, change to the *tests* sub-folder and run make. The Makefile in the *tests* folder is hand-coded and straightforward. Take a look at it to see several useful options available.

The test suite programs are in the *tests* sub-folder.
