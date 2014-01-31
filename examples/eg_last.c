/* Populate a tree from 10 down to 0 then retrieve back element which should be
   10.
*/

#include <stdio.h>
#include <cagl/tree.h>

CAG_DEC_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);

int main(void)
{
	int_tree t;
	int i;

	if(new_int_tree(&t) == NULL) {
		fprintf(stderr, "Error creating new tree\n");
		exit(1);
	}
	for (i = 10; i > 0; --i)
		if (insert_int_tree(&t, i) == NULL) {
			fprintf(stderr, "Error creating new tree\n");
			exit(1);
		}
        /* Last element is pointer to highest value. */
	i = *back_int_tree(&t);
	printf("Last element in tree is: %d\n", i);
	free_int_tree(&t);
	return 0;
}
