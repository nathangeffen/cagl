#include <stdio.h>
#include <cagl/dlist.h>

CAG_DEC_DEF_DLIST(int_dlist, int);

int main(void)
{
	int_dlist l;
	it_int_dlist it;
	int i;

	if(new_int_dlist(&l) == NULL) {
		fprintf(stderr, "Error creating new list\n");
		exit(1);
	}
	for (i = 0; i < 10; ++i)
		if (append_int_dlist(&l, i) == NULL) {
			fprintf(stderr, "Error creating new list\n");
			exit(1);
		}
	it = beg_int_dlist(&l);
	while(it != end_int_dlist(&l)) {
		printf("%d\n", it->value);
		it = at_int_dlist(it, 2);
	}
	free_int_dlist(&l);
	return 0;
}
