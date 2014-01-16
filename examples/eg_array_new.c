/* Demonstrates different uses of *new* on an array.

   While in many environment it isn't generally necessary to free memory upon
   exit of a program this demonstration does show how to exit the program
   without leaving any memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

/* Declare and define an array of integers. */
CAG_DEC_DEF_ARRAY(int_arr, int);

int main(void)
{
	int_arr a1, a2, a3, a4, a5;
	int i;

        /* Simplest use of new. Most users should use this version
	   most of the time.
	*/
	if (!new_int_arr(&a1)) {
		fprintf(stderr, "Error initialising with simple new.\n");
		/* No need to free something that wasn't successfully
		   initialized.
		 */
		exit(1);
	}

        /* Override CAGL default capacity with space for 10 elements. */
	if (!new_with_capacity_int_arr(&a2, 10)) {
		fprintf(stderr, "Error initialising with new_with_capacity.\n");
		free_int_arr(&a1);
		exit(1);
	}

        /* Insert 10 elements into a new array */
	if (!new_with_size_int_arr(&a3, 10)) {
		fprintf(stderr, "Error initialising with new_with_size.\n");
		free_int_arr(&a1);
		free_int_arr(&a2);
		exit(1);
	}

	/* Use the new_many, free_many idiom. */
	i = new_many_int_arr(&a4, &a5, NULL); /* Initialize multiple arrays. */
	if (i <= 0) {
		fprintf(stderr, "Only %d successfully initialized\n", i);
		free_many_int_arr(i, &a4, &a5, NULL);
		exit(1);
	}

	printf("Capacity of a1 is: %lu\n", a1.capacity);
	printf("Size of a1 is: %lu\n", size_int_arr(&a1));
	printf("Capacity of a2 is: %lu\n", a2.capacity);
	printf("Size of a2 is: %lu\n", size_int_arr(&a2));
	printf("Capacity of a3 is: %lu\n", a3.capacity);
	printf("Size of a3 is: %lu\n", size_int_arr(&a3));
	printf("Capacity of a4 is: %lu\n", a4.capacity);
	printf("Size of a4 is: %lu\n", size_int_arr(&a4));
	printf("Capacity of a5 is: %lu\n", a5.capacity);
	printf("Size of a5 is: %lu\n", size_int_arr(&a5));

        /* Return arrays to heap. */
	free_many_int_arr(5, &a1, &a2, &a3, &a4, &a5, NULL);
	return 0;
}
