#include <stdio.h>
#include <stdlib.h>

#include <cagl/tree.h>
#include <cagl/hash.h>

struct dictionary {
	char *word;
	char *definition;
};

CAG_DEC_DEF_STR_STR_TREE(tree_dict, struct dictionary);

CAG_DEC_DEF_STR_STR_HASH(hash_dict, struct dictionary);

int main(void)
{
	tree_dict t;
	hash_dict h;
	it_tree_dict itt;
	it_hash_dict ith;
	struct dictionary dict;
	struct dictionary key;

	if (!new_tree_dict(&t)) {
		fprintf(stderr, "Error allocating tree.\n");
		exit(1);
	}
	if (!new_hash_dict(&h)) {
		fprintf(stderr, "Error allocating hash.\n");
		exit(1);
	}

	dict.word = "cat";
	dict.definition = "Meeowing domestic pet";
	insertp_tree_dict(&t, &dict);

	dict.word = "dog";
	dict.definition = "Barking domestic pet";
	insertp_hash_dict(&h, &dict);

	key.word = "cat";
	itt = getp_tree_dict(&t, &key);
	if (!itt)
		printf("Cat not found.\n");
	else
		printf("%s - %s\n", itt->value.word, itt->value.definition);

	key.word = "dog";
	ith = getp_hash_dict(&h, &key);
	if (!ith)
		printf("Dog not found.\n");
	else
		printf("%s - %s\n", ith->value.word, ith->value.definition);

	free_tree_dict(&t);
	free_hash_dict(&h);
	return 0;
}
