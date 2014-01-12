#include <stdio.h>
#include <stdlib.h>

#include "cagl/tree.h"

struct entry {
	char *word;
	char *definition;
};

int cmp_entry(const struct entry *e1, const struct entry *e2)
{
	return strcmp(e1->word, e2->word);
}

int alloc_entry(struct entry *to, const struct entry *from)
{
	to->word = cag_strdup(from->word);
	if (!to->word)
		return CAG_FALSE;
	to->definition = cag_strdup(from->definition);
	if (!to->definition) {
		free(to->word);
		return CAG_FALSE;
	}
	return CAG_TRUE;
}

void free_entry(struct entry *e)
{
	free(e->word);
	free(e->definition);
}

CAG_DEC_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
			 CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, free_entry);

int populate_dictionary(dictionary *d)
{
	char *word;
	char *definition;
	struct entry e;

	word = malloc(sizeof(char) * 20);
	if (!word) return CAG_FALSE;
	while(!feof(stdin) && strcmp(fgets(word, 20, stdin), "\n")) {
		word[strlen(word) - 1] = '\0'; /* remove newline */
		definition = malloc(sizeof(char) * 200);
		if (!definition) return CAG_FALSE;
		fgets(definition, 200, stdin);
		definition[strlen(definition) - 1] = '\0'; /* remove newline */
		e.word = word;
		e.definition = definition;
		if ((!insertp_dictionary(d, &e))) return CAG_FALSE;
		word = malloc(sizeof(char) * 20);
		if (!word) return CAG_FALSE;
	}
	free(word); /* free the last unused word */
	return CAG_TRUE;
}

int main(void)
{
	dictionary d;
	it_dictionary it;

	if (!new_dictionary(&d)) {
		fprintf(stderr, "Error initializing dictionary.\n");
		exit(1);
	};
	if (!populate_dictionary(&d)) {
		fprintf(stderr, "Error populating dictionary.\n");
		exit(1);
	}
	CAG_FOR_ALL(dictionary, &d, it,
		    printf("%s: %s\n", it->value.word, it->value.definition));
	free_dictionary(&d);
	return 0;
}
