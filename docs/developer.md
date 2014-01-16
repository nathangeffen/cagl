Improving and modifying the C Almost Generic Library
====================================================

You are encouraged to help improve the C Almost Generic Library (CAGL).
This document describes some of the technical "under-the-hood" details
of how the library works.

Coding conventions
==================

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

Functions and algorithms that containers usually provide
========================================================

begin\_[container]
------------------

-   Returns iterator to first element in container.

-   Parameters:

    -   Container variable

end\_[container]
----------------

-   Returns iterator to last element in container.

-   Parameters:

    -   Container variable

put\_[container]
----------------

Every container should have a put function that takes three parameters,
a container variable, an iterator and an element variable. The function
should insert the element variable into the container.
*put\_[container]* is used by generic algorithms like
*copy\_[container]*.

-   Returns iterator to the position in the container, such that if the
    iterator is moved forward one position, the next call to put with a
    second element would insert it immediately after the first one.

-   Parameters:

    -   Container variable to insert into

    -   Iterator to element insert after or before.

    -   Element to insert

Macro function declaration and definition names
===============================================

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


