
# Appendix: Detailed function blueprint documentation {#functions} {-}

**WORK IN PROGRESS ON THIS PART OF THE MANUAL**

This part of the manual is going to be a long work in progress. The aim is to document every CAGL function blueprint with the following: syntax, description, parameters, data races, time and space complexity and an example.

## new {#array-new -}

### Syntax {-}

```C
[container] *new_[container] ([container] * array);
```

### Description {-}

Initializes a container variable. Every CAGL container variable must be initialized with a call to a new function before any other operation is performed on the container. It is undefined to call a *new* function on a container more than once before calling a *free* function. For every call of *new* on a container variable there should be a corresponding call to *free*.

### Parameters {-}

array
  ~ The array to initialize.

Return value
  ~ On success, the initialized container, else NULL.

### Data races {-}

The array parameter is modified.

### Complexity {-}

This is an O(1) operation. It allocates CAG_QUANTUM_ARRAY * sizeof(type) bytes from the heap.

### Example {-}

```C
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
```

## new_with_capacity

### Syntax

```C
[container] *new_with_capacity_[container] ([container] * array,
	    				   const size_t capacity);
```

### Description {-}

Initializes an array and sets its capacity to a user-specified amount. The *capacity* specifies the number of elements to make space for, but the array size is initialized to zero. By default, once the capacity is reached, CAGL functions that add further elements will call realloc to attempt to get additional capacity.

### Parameters {-}

array
  ~ The array to initialize.

capacity
  ~ Enough memory is allocated to accommodate *capacity* elements in the array.

Return value
  ~ On success, the initialized container, else NULL.

### Data races {-}

The array parameter is modified.

### Complexity {-}

This is an O(1) operation. It allocates CAG_QUANTUM_ARRAY * sizeof(type) bytes from the heap.

### Example {-}

See the example for [new_[container]][#array-new].

## new_with_size {-}

### Syntax {-}

```C
[container] *new_with_size_[container] ([container] * array, const size_t size);
```

### Description {-}

Initializes an array and inserts *size* elements into it. The values of the elements are not set and no assumptions should be made about their values. The capacity of the array is set to the minimum of *size* and *CAG_QUANTUM_ARRAY*. It is useful to initialize an array using this function before calling *copy_over*.

### Parameters {-}

array
  ~ The array to initialize.

size
  ~ The array is initialized to have *size* elements.

Return value
  ~ On success, the initialized container with *size* elements, else NULL.


### Data races {-}

The array parameter is modified.

### Complexity {-}

This is an O(1) operation.

### Example {-}

See the example for [new_[container]][#array-new].
