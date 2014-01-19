#include <stdio.h>
#include <stdlib.h>

#include "cagl/array.h"
#include "cagl/dlist.h"

CAG_DEC_DEF_ARRAY(iarr, int);
CAG_DEC_DEF_DLIST(ilst, int);

void rev_array()
{
	int i;
	iarr a;
	it_iarr it, from, to;

	new_iarr(&a);
	for (i = 0; i < 10; ++i)
		append_iarr(&a, i);
	from = index_iarr(&a, 2);
	to = index_iarr(&a, 6);
	it = reverse_iarr(from, to);
	printf("it: %d\n", it->value);
	i = 0;
	CAG_FOR_ALL(iarr, &a, it, printf("a[%d]: %d\n", i++, it->value));
	it = reverse_all_iarr(&a);
	printf("it after reverse all: %d\n", it->value);
	free_iarr(&a);
}

void rev_list()
{
	int i;
	ilst a;
	it_ilst it, from, to;

	new_ilst(&a);
	for (i = 0; i < 10; ++i)
		append_ilst(&a, i);
	from = index_ilst(&a, 2);
	to = index_ilst(&a, 6);
	it = reverse_ilst(from, to);
	printf("it: %d\n", it->value);
	i = 0;
	CAG_FOR_ALL(ilst, &a, it, printf("a[%d]: %d\n", i++, it->value));
	free_ilst(&a);
}

int main(void)
{
	rev_array();
	rev_list();
	return 0;
}
