#include <stdio.h>
#include <cagl/hash.h>

CAG_DEC_DEF_STR_HASH(string_hash);

int main(void)
{
	string_hash h1, h2;
	it_string_hash it1, it2;

	if(new_string_hash(&h1) == NULL) {
		fprintf(stderr, "Error creating new first hash table.\n");
		exit(1);
	}
	if (new_string_hash(&h2) == NULL) {
		fprintf(stderr, "Error creating second hash table.\n");
		exit(1);
	}
	insert_string_hash(&h1, "cow");
	insert_string_hash(&h1, "mouse");
	insert_string_hash(&h1, "dog");
	insert_string_hash(&h2, "cat");
	insert_string_hash(&h2, "giraffe");
	insert_string_hash(&h2, "dog");

	it1 = get_string_hash(&h1, "dog");
	it2 = get_string_hash(&h2, "dog");

	if (cmp_string_hash(it1, it2) == 0)
		printf("A dog is a dog\n");
	else
		printf("Whoops. That shouldn't happen. %s != %s.\n",
		       it1->value, it2->value);

	it2 = get_string_hash(&h2, "giraffe");

	if (cmp_string_hash(it1, it2) != 0)
		printf("A dog is indeed not a giraffe.\n.");
	else
		printf("Whoops. That shouldn't happen. %s == %s.\n",
		       it1->value, it2->value);

	free_string_hash(&h1);
	free_string_hash(&h2);
	return 0;
}
