#include <stdio.h>
#include <stdlib.h>

#include <cagl/tree.h>
#include <cagl/hash.h>

CAG_DEC_DEF_STR_TREE(string_tree);

CAG_DEC_DEF_STR_HASH(string_hash);

int main(void)
{
	string_tree t;
	string_hash h;
	it_string_tree itt;
	it_string_hash ith;

	if (!new_string_tree(&t)) {
		fprintf(stderr, "Error allocating tree.\n");
		exit(1);
	}
	if (!new_string_hash(&h)) {
		fprintf(stderr, "Error allocating hash.\n");
		exit(1);
	}

	insert_string_tree(&t, "cat");
	insert_string_tree(&t, "dog");
	insert_string_tree(&t, "mouse");

	insert_string_hash(&h, "cat");
	insert_string_hash(&h, "dog");
	insert_string_hash(&h, "mouse");

	itt = get_string_tree(&t, "dog");
	if (!itt)
		printf("Dog not found.\n");
	else
		printf("%s barks woof!\n", itt->value);

	ith = get_string_hash(&h, "cat");
	if (!ith)
		printf("Cat not found.\n");
	else
		printf("%s say meeow!\n", ith->value);

	free_string_tree(&t);
	free_string_hash(&h);
	return 0;
}
