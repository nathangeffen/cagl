#include <stdio.h>
#include <stdlib.h>

#include "cagl/slist.h"

CAG_DEC_DEF_SLIST(file_list, FILE *);

void print_file(FILE *f)
{
	char line[200];
	while(fgets(line, 80, f) != NULL) {
		printf("%s\n", line);
	}
}

int main(void)
{
	file_list l;
	struct iterator_file_list *it;
	FILE *f;

	new_file_list(&l);
	f = fopen("eg_slist_files.c", "r");
	prepend_file_list(&l, f);
	f = fopen("eg_array1.c", "r");
	prepend_file_list(&l, f);
	f = fopen("eg_array2.c", "r");
	prepend_file_list(&l, f);
	CAG_FOR_ALL(file_list, &l, it,
		    {
			    printf("*** START OF FILE ***\n");
			    print_file(it->value);
			    printf("*** END OF FILE ***\n");
			    fclose(it->value);
		    });
	free_file_list(&l);
	return 0;
}
