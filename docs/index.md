The C Almost Generic Library (CAGL) provides programmers with arrays, lists (singly or doubly-linked), hash tables and balanced binary trees, as well as many useful functions to manipulate them. The containers are typed so your code is safer. CAGL is free software published under the [GNU LGPL Version 3](http://www.gnu.org/copyleft/lesser.html).

- [Download latest stable version (0.1)](cagl-latest.tar.gz)
- [Get started (includes installation instructions)](manual/readme.html)
- Examples
    - [Doubly-linked list of Euclidean points](manual/eg_euclid.html)
    - [Array of cards](manual/eg_cards.html)
    - [Balanced binary tree dictionary](manual/eg_dictionary.html)
    - [Adjacency list](manual/eg_adjacency.html)
- [User guide and reference manual](manual.html)

CAGL is still in alpha phase. Please help test it.  Report bugs on [Github](https://github.com/nathangeffen/cagl/issues) or by [email](mailto:bugs@almostgeneric.org).

## Simple example

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
