
# Quick start to C Almost Generic Library

The C Almost Generic Library (CAGL) provides *almost* generic containers and algorithms to manipulate them.

The implemented containers are arrays, doubly-linked lists, singly linked lists, balanced binary trees (as red-black trees) and hash tables. The library manages the memory of its containers. It can also manage the memory of the elements (values or data) of the containers. All the containers grow automatically.

## Example

This code populates an array with random integers and then sums the array.

```C
/* Simple array of integers example.

   This program populates an array with random integers and sums them.

   Possible output:
   Sum is 97.
*/

#include <stdio.h>
#include <cagl/array.h> /* CAGL array code. */

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr iarr; /* an array of integers that grows automatically. */
	it_int_arr it; /* an iterator over the array. */
	int total = 0;
	size_t i;

	new_int_arr(&iarr); /* Initialize container */

    /* populate the array with random integers. */
	for (i = 0; i < 10; ++i)
		append_int_arr(&iarr, rand() % 20);

	/* iterate over the array, summing its elements. */
    for (it = beg_int_arr(&iarr); it != end_int_arr(&iarr);
                                  it = next_int_arr(it))
    	total += it->value;
    printf("Sum is %d.\n", total);

   	free_int_arr(&iarr); /* Return array to heap. */
   	return 0;
}
```

## Installation

The CAGL source code is available on github at <https://github.com/nathangeffen/cagl>.

CAGL uses a standard autotools installation. These instructions would work on a **typical** GNU/Linux distribution:

1. Download and unpack the latest version from Github:
   <https://github.com/nathangeffen/cagl/latest.tar.gz>.

1. In the CAGL root directory run these commands:

```
    ./configure
	make
	sudo make install
```

How to compile CAGL programs differs across environments, but assuming you have a common, standard GNU/Linux distribution, the most likely scenario is that CAGL will be installed in _/usr/local/lib_ and _/usr/local/include_ and that you have *pkg-config* installed. To compile a program called *myprog.c* that uses CAGL, you would typically do this:

    cc -Wall -pedantic myprog.c -o myprog `pkg-config --libs --cflags cagl-0.1`

You might get this error when you run *myprog*:

    ./myprog: error while loading shared libraries: libcagl-0.1.so.1: cannot open shared object file: No such file or directory

This shell command will likely resolve the problem:

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

To test that the library is working, type this on the command line in the unpacked cagl directory. You need [gcc](http://gcc.gnu.org/), [clang](http://clang.llvm.org/) and [valgrind](http://valgrind.org/) installed to pass all the tests.

    make check

It will likely take a couple of minutes to run all the tests.

To uninstall CAGL, type this on the command line in the CAGL root directory:

    make uninstall

For more information read the INSTALL file in the CAGL root directory.

## Compiling CAGL programs

When compiling a program that uses CAGL with *gcc* or *clang* using *-Wall -pedantic* any compiler warnings that arise in connection with CAGL should be considered errors. Conversely, any C compiler compliant with [ISO C 1990](http://en.wikipedia.org/wiki/ANSI_C#C89_and_C90) or later should compile well-written CAGL programs without warnings. For example, in development on GNU/Linux systems with pkg-config installed, you can compile the examples in this user guide like this:

    cc -g -Wall -pedantic -Werror nameofprog.c -o nameofprog `pkg-config --libs --cflags cagl-0.1`


Because CAGL generates many functions, only some of which your program might use, you might want to reduce the size of your executable in production. With *gcc* you can do this using the *-flto* option. For example:

	gcc -Wall -pedantic -O3 -flto nameofprog.c -o nameofprog  `pkg-config --libs --cflags cagl-0.1`

Compiling with the *-flto* option can be slow, so you will likely only use it when compiling release builds.

## Documentation

The CAGL manual is available at <http://www.cagl.nathangeffen.webfactional.com>.

If you wish to generate the documentation from sources, you need to have *pandoc* installed. CAGL documentation is written in [Pandoc Markdown](http://johnmacfarlane.net/pandoc/README.html).

To generate the manual as a single HTML page on a GNU/Linux system go the directory to which CAGL was unpacked. Type:

```
./bin/makedocs
```

A file called *manual.html* will be created in the docs directory.

To generate a PDF file, type:

```
./bin/makedocs pdf
```

A file called *manual.pdf* will be created in the *docs* directory.

## Contributing to CAGL

There's lots to do to improve CAGL. See TODO.md in the docs directory.

## Purpose

CAGL provides C programmers with an easy-to-use, fast library of commonly used containers for holding data. It also provides algorithms, implemented as functions and, to a lesser extent, macros for manipulating the containers and their data.

Despite its age, C remains one of the most popular programming languages. It is fast and simple. Well-written C code is easy for most programmers to understand. It is widely used in embedded devices and vies with Java for top spot on the Tiobe Programming Index.

However, C comes with no standard libraries to handle the drudgery of the most needed data structures, or containers, such as arrays, trees, hash tables and lists. C++ provides the STL, but requires programmers to use an extremely complex language.

There are some excellent C generic libraries for managing containers such as [glib](https://developer.gnome.org/glib/) and [SGLIB](http://sglib.sourceforge.net/).

CAGL is inspired by *glib* and *SGLIB* but differs from both of them.

In contrast to glib which uses void pointers for generic types and requires primitive types to be passed by address, CAGL uses macro calls to define containers and generate functions that operate upon them. CAGL is type-safer than glib in that a decent C compiler will generate warnings for *type mismatch* programming errors when you use CAGL. CAGL is not nearly as comprehensive or as well-tested or documented as glib.

SGLIB also uses macros except that CAGL, in contrast to SGLIB's expressed philosophy, manages memory for the container and, if the programmer chooses, CAGL manages the memory of the container elements too. Also SGLIB provides macros to users. While a CAGL container is declared and defined using a macro call, subsequent operations on the container are typically done using functions generated by the macro that defined the container. See the example above.

Also, CAGL implements iterators, although these are not as flexible as the ones in the C++ STL.

If you use C, but would like some of the convenience of the C++ STL, CAGL may meet your needs. Of course C doesn't have many C++ features used to develop the STL, so CAGL is much less sophisticated and comprehensive than the STL -- and always will be -- but it has been designed to cover common use cases.

## Pros and cons of CAGL

CAGL has the following benefits:

- It is type safer than alternative C generic libraries.
- Its interface is reasonably consistent. It is therefore easy to use.
- It handles the most common data structures.
- Most of the algorithms are efficiently implemented. CAGL is fast.

It has these limitations:

- It's immature and needs testing. Nevertheless, there are several hundred test cases and code is not checked in without passing all test cases. The test suite is also run through *valgrind* and not checked in unless there are no errors.
- The CAGL macros generate a bunch of functions, only some of which users may need. But with *gcc* these can be removed from object files passing the *-flto* to the linker.
- Without reading the documentation (or using the CAG_DOC macro which lists all the functions for a container), users can struggle to know the names of the generated functions.

## How CAGL has been tested

Several hundred test cases have been written and most common use cases have been tested. These are primarily in C files in the *tests* sub-directory. More test cases are being written. But this is new and immature software so expect serious bugs. Please feel welcome to help improve it.

## Why *almost* generic?

The library is called *almost* generic for several reasons, the main one being that the generic functions that are generated for different container types must have different names because C doesn't support function overloading. The library is also *almost* type safe, but not quite. C allows a function with pointer parameters to be called with pointers of a different type. This is usually a programming mistake. However mature C compilers will give warnings when this happens. An important principle of CAGL is that when you use it you should get no compiler warnings (at least as far as your use of CAGL goes).

CAGL works as follows:

- You use a CAGL macro to declare a container, e.g. *CAG_DEC_ARRAY(int_arr, int);*  and another container to define the container's functions, e.g. *CAG_DEF_ARRRAY(int_arr, int)*. You can combine this into one macro. E.g. *CAG_DEC_DEF_ARRAY(int_arr, int)* declares a container called *int_arr* whose elements are integers.

- The macro generates functions, e.g. *begin_int_arr*, *end_int_arr*, *append_int_arr*, *copy_int_arr* and many others.

- Since C doesn't support function overloading, each container type that you declare and define using CAGL macros is uniquely named by being suffixed with the type name you've used in the declaration and definition macros (*int_arr* in the above example). So it's not truly generic.

- Also, since C doesn't support templates, you can't extend a CAGL container with generic code unless you're prepared to write complex C macros.

## Why type saf*er*?

C is not type safe. You can, for example, call a function that expects a parameter that is a pointer of type A with a pointer to any type. However, good compilers will pick this up and warn you. You should not get any compiler warnings when using CAGL containers, else you're likely doing something undefined or dangerous.

The test suite of CAGL is compiled with these options:

    -Wall -pedantic -Wstrict-prototypes -Wextra -Werror

If any warnings are generated, the tests are considered to have failed. You should be able to compile your code that uses CAGL with these options too and get no warnings.

## Portability

CAGL has been tested using gcc and clang. It is C89 compatible, which means code that uses it should compile on a C preprocesser and compiler compatible with the 1989 ANSI C specification without warnings.

It needs to be tested with other compilers, including Intel's and Microsoft's C compilers. Please help with this.


## Bugs

CAGL bugs are managed using the Github's issue tracker. Please report bugs there or by emailing nathangeffen at gmail.com. please start the subject line with *CAGL:*.

## License

CAGL is free software licensed under the GNU Lesser General Public License version 3. See *COPYING*.

## Contributors

Contributors will be listed here:

- Nathan Geffen (email: nathangeffen at gmail.com - please start the subject line with *CAGL:*)
