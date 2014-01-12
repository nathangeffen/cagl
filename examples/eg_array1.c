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
