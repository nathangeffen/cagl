/* Simple array of integers example.

   This program populates an array of integers with random values, then sums
   them and then sorts them.

   Possible output:

   Sum is: 97
   Sum is: 97
   3 6 17 15 13 15 6 12 9 1
   1 3 6 6 9 12 13 15 15 17
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

	/* Every container must be initiated before use with a
	   call to new_[container] or similar function.
	 */
	new_int_arr(&iarr);

	/* populate the array with random integers. */
	for (i = 0; i < 10; ++i)
		append_int_arr(&iarr, rand() % 20);

	/* iterate over the array, summing its elements. */
	for (it = beg_int_arr(&iarr); it != end_int_arr(&iarr);
	     it = next_int_arr(it))
        /* Every iterator has an element called value which is the element
           pointed to.
        */
		total += it->value;
	printf("Sum is: %d\n", total);

	/* You could also do it this way. */
	total = 0;
	it = beg_int_arr(&iarr);
	for (i = 0; i < distance_all_int_arr(&iarr); ++i)
		total += at_int_arr(it, i)->value;
	printf("Sum is: %d\n", total);

	/* Print all the elements of the array. This demonstrates a third way of
	   applying an operation to every element of a container, using a macro.
	*/
	CAG_FOR_ALL(int_arr, &iarr, it,
		    {
			    printf("%d ", it->value);
		    });
	printf("\n");

	/* Let's sort the array. This example uses a macro, but a safer sorting
	   function can be generated instead.
	 */
	CAG_SORT_ARRAY_DEFAULT(int, beg_int_arr(&iarr),
			       end_int_arr(&iarr), CAG_CMP_PRIMITIVE);


	/* A fourth way to iterate over the elements, also using a macro. In
	   fact CAG_FOR_EACH is called by CAG_FOR_ALL.
	*/
	CAG_FOR_EACH(beg_int_arr(&iarr), end_int_arr(&iarr),
		     next_int_arr, it,
		     {
			    printf("%d ", it->value);
		     });
	printf("\n");

	/* Every container must be destroyed after use with a
	   call to free_[container] or similar function.
	 */
	free_int_arr(&iarr);
	return 0;
}
