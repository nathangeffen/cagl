/* Demonstrates how to use some of the SLIST macros. */

#include <stdio.h>
#include <cagl/slist.h>

CAG_DEC_SLIST(int_slist, int);

int main(void)
{
	it_int_slist head = NULL, node;
	int i;

	for (i = 0; i < 10; ++i) {
		node = head;
		CAG_MAKE_SLIST(head, i);
		if (!head) {
			fprintf(stderr, "Memory allocation error.\n");
			exit(1);
		}
		/* Place head before node. */
		CAG_PREPEND_SLIST(node, head);
	}

	printf("Reverse printing\n");
	for (node = head; node != NULL; node = node->next)
		printf("%d ", node->value);
	printf("\n");

	CAG_STABLE_SORT_SLIST_DEFAULT(it_int_slist, head, CAG_CMP_PRIMITIVE);

	printf("In order printing\n");
	for (node = head; node != NULL; node = node->next)
		printf("%d ", node->value);
	printf("\n");

	CAG_REVERSE_SLIST(it_int_slist, head);

	printf("Reverse printing again\n");
	for (node = head; node != NULL; node = node->next)
		printf("%d ", node->value);
	printf("\n");

	CAG_ERASE_ALL_SLIST(head, CAG_NO_FREE_FUNC);
	return 0;
}
