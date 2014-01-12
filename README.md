# C Almost Generic Library

Last updated: 12 January 2014.

Version: 0.1

The C Almost Generic Library (CAGL) provides *almost* generic containers and algorithms to manipulate them.

Currently these containers are implemented: arrays, doubly-linked lists, singly linked lists, balanced binary trees -- implemented as red-black trees -- and hash tables.

## Example

```C
    /* Simple array of integers example.

       This program populates an array with random integers and sums them.

       Possible output:
       Sum is: 97
     */

    #include <stdio.h>
    #include <stdlib.h>
    #include "cagl/array.h"

    /* Declare and define an array of integers. */
    CAG_DEC_DEF_ARRAY(int_arr, int);

    int main(void)
    {
    	int_arr iarr; /* an array of integers. */
    	it_int_arr it; /* an iterator over the array. */
    	int total = 0;
    	size_t i;

    	new_int_arr(&iarr); /* Initiate container */

    	/* populate the array with random integers. */
    	for (i = 0; i < 10; ++i)
    		append_int_arr(&iarr, rand() % 20);

    	/* iterate over the array, summing its elements. */
    	for (it = beg_int_arr(&iarr); it != end_int_arr(&iarr);
    	     it = next_int_arr(it))
    		total += it->value;
    	printf("Sum is: %d\n", total);

    	free_int_arr(&iarr); /* Return array to heap. */
    	return 0;
    }
```

Other examples will be included in the documentation.

## Installation

CAGL's installation is handled by autotools.

1. Download and unpack the latest version from Github.

1. Run:

    ./configure
	make
	sudo make install

How you compile CAGL programs might differ slightly across environments, but assuming you have a common, standard GNU/Linux distribution, the most likely scenario is that CAGL will be installed in /usr/local/lib and /usr/local/include and that you have *pkg-config* installed. To compile your program called *myprog.c* in development that use CAGL, you would typically do this:

    cc -g -Wall -pedantic myprog.c -o myprog `pkg-config --libs --cflags cagl-0.1`

You might get this error when you run *myprog*:

    ./myprog: error while loading shared libraries: libcagl-0.1.so.1: cannot open shared object file: No such file or directory

This will likely resolve the problem:

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

## Documentation

The documentation is a work in progress, but there is already enough written to help get you going. The user guide is written in [Markdown](http://daringfireball.net/projects/markdown/), which is an easy plain text format to read.  See userguide.md in the docs directory. HTML and PDF versions can be generated with [pandoc](http://johnmacfarlane.net/pandoc/). If you install pandoc, then *makedocs* in the *bin* directory is a script that generates an HTML version.

## Contribute

There's lots to do to improve CAGL. See TODO.md in the docs directory. Email nathangeffen at gmail.com if you want to contribute.

## Reason for CAGL

If you use C, but would like something analogously convenient to the C++ STL, this may meet your needs. Of course C doesn't have many C++ features used to develop the STL, so CAGL is much less sophisticated and comprehensive than the STL -- and always will be -- but it has been designed to cover common use cases.

## If you need something like the STL, why not just use C++?

For many reasons. Perhaps the main one is that C remains ubiquitous, especially on GNU/Linux systems and embedded software.

Many people find C simple and elegant and prefer using it. However, they would prefer to avoid the error-prone drudgery of continuously re-implementing containers and commonly used algorithms.

## How does CAGL differ from other C generic libraries?

There are some excellent C generic libraries such as [glib](https://developer.gnome.org/glib/) and [SGLIB](http://sglib.sourceforge.net/).

CAGL is inspired by *glib* and *SGLIB* but differs from both of them.

In contrast to glib which uses void pointers for generic types and requires primitive types to be passed by address, CAGL uses macro calls to define containers and generate functions that operate upon them. CAGL is type-safer than glib in that a decent C compiler will generate warnings for type mismatch programming errors when you use CAGL. CAGL is not nearly as comprehensive or as well-tested or documented as glib.

SGLIB also uses macros except that CAGL, in contrast to SGLIB's expressed philosophy, manages memory for the container and, if the programmer chooses, CAGL manages the memory of the container elements too. Also SGLIB provides macros to users. While a CAGL container is declared and defined using a macro call, subsequent operations on the container are typically done using functions generated by the macro that defined the container. See the example above.

Also, CAGL implements iterators, although these are not as flexible as the ones in the C++ STL.

## Testing

Several hundred test cases have been written and most common use cases have been tested. More test cases are being written. But this is new and immature software so expect serious bugs. Please feel welcome to help improve it.

## Portability

CAGL has been tested using gcc and clang. It is C89 compatible, which means code that uses it should compile on a C preprocesser and compiler compatible with the 1989 ANSI C specification without warnings.

It needs to be tested with other compilers, including Intel's and Microsoft's C compilers. Please help with this.

## Pros and cons of CAGL

CAGL has the following benefits:

- It is type safer than alternative C generic libraries.
- Its interface is reasonably consistent. It is therefore easy to use.
- It handles the most common data structures.
- Most of the algorithms are efficiently implemented. CAGL is fast.

It has these limitations:

- It's immature and needs testing. Nevertheless, there are several hundred test cases and code is not checked in without passing all test cases. The test suite is also run through *valgrind* and not checked in unless there are no errors.
- The CAGL macros generate a bunch of functions, only some of which users may need. But with *gcc* these can be removed from object files passing the *-flto* to the linker.
- Without reading the documentation (or using the CAG_DOC macro which lists all the functions for a container), users might struggle to know the names of the generated functions.

## Why *almost* generic?

CAGL works as follows:

- You use a CAGL macro to declare a container and another container to define the container's functions. You can combine this into one macro, e.g. *CAG_DEC_DEF_ARRAY(int_arr, int)* declares a container called *int_arr* whose elements are integers.

- The macro generates functions, e.g. *begin_int_arr*, *end_int_arr*, *append_int_arr*, *copy_int_arr* and many others.

- Since C doesn't support function overloading, each container type that you declare and define using CAGL macros is uniquely named. So it's not truly generic.

- Also, since C doesn't support templates, you can't extend a CAGL container with generic code unless you're prepared to write C macros.

## Why type *safer*?

C is not type safe. You can, for example, call a function that expects a parameter that is a pointer of type A with a pointer to any type. However, good compilers will pick this up and warn you. You should not get any compiler warnings when using CAGL containers, else you're likely doing something undefined or dangerous.

The test suite of CAGL is compiled with these options:

    -Wall -pedantic -Wstrict-prototypes -Wextra

If any warnings are generated, the tests are considered to have failed. You should be able to compile your code that uses CAGL with these options too and get no warnings.

## CAGL is implemented using complicated and difficult to read macros. Isn't this bad style?

The author of [SGLIB](http://sglib.sourceforge.net/) says it best:

"Everyone knows that the C preprocessor can be used to imitate genericity of other languages and everyone consider[s] this idea dangerous and ugly. I don't."

The CAGL macros are necessarily complex, even necessarily ugly. But that's something the developers of CAGL have to worry about not the users of the library. CAGL is easy to use.

## License

CAGL is free software licensed under GNU LGLPL version 3. See *COPYING*.

## Contributors

Contributors will be listed here:

- Nathan Geffen (email: nathangeffen at gmail.com - please start the subject line with *CAGL:*.)
