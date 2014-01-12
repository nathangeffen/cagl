Improving and modifying the C Almost Generic Library
####################################################

You are encouraged to help improve the C Almost Generic Library (CAGL). This document describes some of the technical "under-the-hood" details of how the library works.


Coding conventions
##################

CAGL is mostly made up of macros. There's a small amount of non-macro code in a a few small .c libraries. This makes CAGL quite unusual. It is hard to read complex C macros and CAGL is almost entirely composed of them. So it's important to stick to conventions that makes reading the code easier.

- Use Kernighan & Ritchie coding conventions in the .h files.

- After editing a .h file (say file.h), run: *slash79 file.h*.

- Use the Linux Kernel coding conventions in the .c files.

- Place the end-of-line slash **\** in column 79.

- All macros in CAGL start CAG_.

- All C functions (there aren't many of them) start cag_.

- Macros that are strictly for internal use should be prefixed CAG_P_ (the "P" stands for private).


Functions and algorithms that containers usually provide
########################################################

begin_[container]
~~~~~~~~~~~~~~~~~

- Returns iterator to first element in container.

- Parameters:

  - Container variable



end_[container]
~~~~~~~~~~~~~~~

- Returns iterator to last element in container.

- Parameters:

  - Container variable


put_[container]
~~~~~~~~~~~~~~~

Every container should have a put function that takes three  parameters, a container variable, an iterator and an element variable. The  function should insert the element variable into the container. *put_[container]* is used by generic algorithms like *copy_[container]*.

- Returns iterator to the position in the container, such that if the iterator  is moved forward one position, the next call to put with a second element  would insert it immediately after the first one.

- Parameters:

  - Container variable to insert into

  - Iterator to element insert after or before.

  - Element to insert



Macro function declaration and definition names
###############################################

*apply_func*
  User supplied function in an apply macro. Apply macros are used to generate a full container version of a function that works on an iterator range. For example, the apply macro for distance_[container](from, to)  is distance_all_[container](container variable).

*at*
  Function to find iterator that is *n* places from the input parameter, i.e. at_[container].

*alloc_style*
  A macro to tell a container how to allocate memory for its elements. Three are provided by CAGL and should be sufficient for nearly all cases: CAG_SIMPLE_ALLOC_STYLE (used for assigning allocated memory to pointers), CAG_STRUCT_ALLOC_STYLE (used for assigning allocated memory to structures) and CAG_NO_ALLOC_STYLE (used when the container does not have to assign memory for its elements).

*alloc_func*
  A function or macro to do memory allocation. For pointers this is often  malloc. For containers that do not need to do memory allocation, use CAG_NO_ALLOC_FUNC. C++ programmers can think of this as a constructor for the container element.

*apply_func*
  Function to apply to every element in a container.

*begin*
  Function to find beginning of container, i.e. begin_[container] or rbegin_[container].

*cmp_func*
  Function that takes two iterators and compares their values.

*container*
  Container type.

*container_var*
  Container variable.

*dir*
  Function to step an iterator forward or backward, i.e. next_[container], prev_[container], rnext_[container] or rprev_[container].

*distance*
  Function to calculate distance between two iterators. i.e. distance_[container] or rdistance_[container].

*end*
  Function to find end of container, i.e. end_[container] or rend_[container].

*first*
  Iterator pointing to the first element in a sequence. Synonymous with *from*.

*free_func*
  User provided function to free an element of a container. Use CAG_NO_FREE_FUNC if the container does not allocate memory for its elements. For elements that are pointers, *free* is often sufficient. C++ programmers could think of this as a destructor.

*free_code*
  This is a parameter to an *alloc_style* macro. It is code that frees any already allocated memory in the case of a failed allocation that succeeded partially. The partially allocated memory should be returned to the heap with this code. It is seldom that users should have to implement this.

*from*
  Iterator pointing to the first element in a sequence. Synonymous with *first*.

*from_1*
  Iterator pointing to the first element in a sequence. Used when multiple sequences are being operated upon.

*from_2*
  Iterator pointing to the first element in a sequence. Used when multiple sequences are being operated upon.

*func*
  Function supplied to generic algorithm.

*function*
  Name of function in macro declaration and definition.

*hash_func*
  User provided function for hash tables, but some reasonable hashing functions are provided by the CAGL.

*insert_out*
  Function that inserts a value into an output iterator.

*it*
  An *iterator_type* variable mostly used in the functional programming macros.

*iterator_type*
  Iterator type, generally of the form it_[container] or rit_[container].

*iterator_type_1*
  Same as *iterator_type* but used when two or more sequences are operated upon.

*iterator_type_2*
  Same as *iterator_type* but used when two or more sequences are operated upon.

*iterator_type_in*
  Same as *iterator_type* but specifically for input sequence.

*key*
  Usually compared against an iterator value in a search.

*last*
  Iterator pointing to the last element in a sequence.

*length_func*
  User provided function that calculates the length of a type variable. Used for hashing.

*next*
  Function to step an iterator forward, i.e. next_[container] or rnext_[container].

*next_1*
  Same as *next* but used when two are more sequences are operated upon.

*next_2*
  Same as *next* but used when two are more sequences are operated upon.

*next_in*
  Same as *next* but for input iterator.

*next_out*
  Same as *next* but for output iterator.

*prev*
  Function to step an iterator backward, i.e. prev_[container] or rprev_[container].

*result*
  Iterator pointing to first element in output sequence.

*swap*
  Function to swap two iterators, i.e. swap_[container].

*type*
  Data type of the elements operated on in a container. Every CAGL container has an iterator type. Every iterator type has a field called value, which has a type indicated by this parameter.

*val_adr*
  Either equal to blank or &. Used by *cmp_func* to compare by value or by address.

*value*
  Function or macro to get the value of an iterator. For all CAGL containers, this should always be *it->next* where *it* is an iterator variable.
