/* Demonstrates how to use the CAG_SORT_ARRAY macro. */

#include <stdio.h>
#include <stdlib.h>
#include <cagl/array.h>

CAG_DEC_DEF_CMP_ARRAY(int_array, int, CAG_CMP_PRIMITIVE);

void demo_sort_C_array()
{
	int *it;
	int iarr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	CAG_SORT_ARRAY(int, iarr, iarr + 10, CAG_CMP_PRIMITIVE);

	for (it = iarr; it != iarr + 10; ++it)
		printf("%d ", *it);
	printf("\n");
}

void demo_sort_CAGL_container()
{
	int_array arr;
	it_int_array it;
	int i;

	new_int_array(&arr);
	for (i = 9; i >= 0; --i)
		append_int_array(&arr, i);
	CAG_SORT_ARRAY(int, beg_int_array(&arr), end_int_array(&arr),
		       CAG_CMP_PRIMITIVE);
	for (it = beg_int_array(&arr); it != end_int_array(&arr);
	     it = next_int_array(it))
		printf("%d ", it->value);
	printf("\n");
	free_int_array(&arr);
}

int main(void)
{
	printf("Sorting C array of integers\n");
	demo_sort_C_array();
	printf("Sorting CAGL array of integers\n");
	demo_sort_CAGL_container();
	return 0;
}
