/*
  This program generates a list of the all CAGL functions. It prints the list
  as a markdown table with links in each entry to the skeleten of a detailed
  description of each function.

  This is a big time-saver for generating documentation.

  Note with gcc -pedantic, the following unavoidable warnings are generated:

  eg_process_docs.c:74:2: warning: string length ‘5579’ is greater than the
  length ‘509’ ISO C90 compilers are required to support [-Woverlength-strings]

  eg_process_docs.c:89:2: warning: string length ‘3920’ is greater than the
  length ‘509’ ISO C90 compilers are required to support [-Woverlength-strings]

*/

#include <stdio.h>
#include <stdlib.h>

#include "cagl/slist.h"
#include "cagl/dlist.h"
#include "cagl/hash.h"
#include "cagl/array.h"
#include "cagl/tree.h"

const char *container_types[] = {
	"array",
	"dlist",
	"hash",
	"slist",
	"tree"
};

struct func_info {
	char function[50];
	char containers[5];
	char syntax[500];
};


int cmp_func_info(const struct func_info *a, const struct func_info *b)
{
	return strcmp(a->function, b->function);
}

CAG_DEC_DEF_CMPP_TREE(func_tree, struct func_info, cmp_func_info);
CAG_DEC_DEF_ARRAY(string, char);

#define CAG_DOC_TO_STR(dec, container, type, str)			\
	{								\
		char l[10000] = CAG_P_STRINGIFY(dec(container, type));	\
		char *c = l;						\
		while (*c != '\0') {					\
			if (strncmp(c, "CAG_P_", 6) == 0)		\
				while (*c != ';' && *c != '\0') c++;	\
			while(*c != ';' && *c != '\0') {		\
				append_string(str, *c);			\
				++c;					\
			}						\
			append_string(str,';');				\
			++c;						\
		}							\
	}


enum states {
	DEFAULT,
	IGNORE,
	NEXT
};


void append_to_string(string *str, char *s)
{
	while (*s)
		append_string(str, *s++);
}

void put_decs_into_string(string *str)
{
	append_to_string(str, "/*********/\n");
	append_to_string(str, "/* array */\n");
	append_to_string(str, "/*********/\n\n");
	CAG_DOC_TO_STR(CAG_DEC_CMP_ARRAY, C, T, str);
	append_to_string(str, "\n\n");
	append_to_string(str, "/*********/\n");
	append_to_string(str, "/* dlist */\n");
	append_to_string(str, "/*********/\n\n");
	CAG_DOC_TO_STR(CAG_DEC_CMP_DLIST, C, T, str);
	append_to_string(str, "\n\n");
	append_to_string(str, "/*********/\n");
	append_to_string(str, "/* slist */\n");
	append_to_string(str, "/*********/\n\n");
	CAG_DOC_TO_STR(CAG_DEC_CMP_SLIST, C, T, str);
	append_to_string(str, "\n\n");
	append_to_string(str, "/*********/\n");
	append_to_string(str, "/* tree  */\n");
	append_to_string(str, "/*********/\n\n");
	CAG_DOC_TO_STR(CAG_DEC_CMP_TREE, C, T, str);
	append_to_string(str, "\n\n");
	append_to_string(str, "/*********/\n");
	append_to_string(str, "/* hash */\n");
	append_to_string(str, "/*********/\n\n");
	CAG_DOC_TO_STR(CAG_DEC_CMP_HASH, C, T, str);
	append_to_string(str, "\n");
}


char *get_syntax(string *text, char *token, int *length)
{
	char *s = (char *) begin_string(text);
	char *t;
	while (*s) {
		if (strncmp(s, token, strlen(token)) == 0) {
			while (*s && *--s != ';');
			++s;
			while (*s && (*s == '\n' || *s == ' ')) ++s;
			*length = 0;
			t = s;
			while(*t && *t++ != ';')
				++*length;
			++*length;
			return s;
		}
		++s;
	}

	*length = 0;
	return NULL;
}

void put_funcs_into_tree(func_tree *t, string *text)
{
	string text_copy;
	char *delims = "/ *\n{}(),";
	char *token, *s;
	it_func_tree it;
	int i, index, length;
	struct func_info f;
	enum states state = DEFAULT;

	new_from_string(&text_copy, text);
	token = strtok( (char *) begin_string(text), delims);
	if (token == NULL)
		return;
	do {
		for (i = 0; i < 5; ++i) {
			if (strcmp(token, container_types[i]) == 0) {
				index = i;
				break;
			}
		}

		if (token[strlen(token) - 1] == ';') {
			state = DEFAULT;
		} else if (strcmp(token, "typedef") == 0 ||
			   strcmp(token, "struct") == 0) {
			state = IGNORE;
		} else if (state == DEFAULT) {
			state = NEXT;
		} else if (state == NEXT && token[strlen(token)-1] == 'C') {
			strncpy(f.function, token, 49);
			it = get_func_tree(t, f);
			if (it) {
				it->value.containers[index] = 1;
			}
			else {
				memset(f.containers, 0, 5);
				f.containers[index] = 1;
				s = get_syntax(&text_copy, token, &length);
				strncpy(f.syntax, s, length);
				f.syntax[length] = '\0';
				insert_func_tree(t, f);
			}
			state = IGNORE;

		}
	} while( (token = strtok(NULL, delims)) );
	free_string(&text_copy);
}

void print_index_by_container_type(func_tree *t)
{
	it_func_tree it;
	int i, j;

	for (i = 0; i < 5; ++i) {
		printf("# %s\n\n", container_types[i]);
		for (it = beg_func_tree(t); it != end_func_tree(t);
		     it = next_func_tree(it)) {
			if (it->value.containers[i]) {
				printf("- [%s](#%s-",
				       it->value.function, it->value.function);
				for (j = 0; j < 5; ++j) {
					if (it->value.containers[j])
						printf("%c",
						       container_types[j][0]);
				}
				printf(")\n");
			}
		}
		printf("\n\n");
	}
}

void print_blueprints(func_tree *t)
{
	it_func_tree it;
	int i;

	for (it = beg_func_tree(t); it != end_func_tree(t);
	     it = next_func_tree(it)) {
		printf("## %s {#%s-", it->value.function,
		       it->value.function);
		for (i = 0; i < 5; ++i) {
			if (it->value.containers[i])
				printf("%c", container_types[i][0]);
		}
		puts(" - }\n\n");
		puts("```C");
		printf("%s\n", it->value.syntax);
		puts("```\n\n");
		puts("Containers: ");
		for (i = 0; i < 5; ++i) {
			if (it->value.containers[i])
				printf("%s\t", container_types[i]);
		}
		puts("\n\n");
		puts("#### Parameters {-}\n\n");
		puts("#### Return value {-}\n\n");
		puts("#### Example {-}\n\n");
		puts("#### Complexity {-}\n\n");
		puts("#### Data races {-}\n\n");
		puts("#### See also {-}\n\n");
		puts("------\n\n");
	}
}

void print_full_index(func_tree *t)
{
	it_func_tree it;
	int i, j;

	printf("# List of functions {#function-list -}\n\n");
	printf("FUNCTION | ARRAY | DLIST | HASH | TREE | SLIST |\n");
	printf(":--------|:-----:|:-----:|:----:|:----:|:-----:|\n");

	for (it = beg_func_tree(t); it != end_func_tree(t);
	     it = next_func_tree(it)) {
		j = 0;
		char tag[] = "\0\0\0\0\0";
		printf("| %-30s |", it->value.function);
		for (i = 0; i < 5; ++i)
			if (it->value.containers[i])
				tag[j++] = container_types[i][0];
		for (i = 0; i < 5; ++i) {
			if (it->value.containers[i])
				printf(" [%c](#%s-%s) |",
				       container_types[i][0],
				       it->value.function,
				       tag);
			else
				printf("  |");
		}
		printf("\n");
	}

}

void print_funcs(func_tree *t)
{
	print_index_by_container_type(t);
	print_blueprints(t);
	print_full_index(t);
}


int main(void)
{
	func_tree tree;
	string str;

	new_string(&str);
	new_func_tree(&tree);
	put_decs_into_string(&str);
	put_funcs_into_tree(&tree, &str);
	print_funcs(&tree);
	free_func_tree(&tree);
	free_string(&str);
	return 0;
}
