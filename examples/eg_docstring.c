#include <stdio.h>
#include <stdlib.h>

#include "cagl/array.h"
#include "cagl/hash.h"
#include "cagl/dlist.h"
#include "cagl/slist.h"
#include "cagl/tree.h"

int main(void)
{
	printf("/*********/\n");
	printf("/* ARRAY */\n");
	printf("/*********/\n\n");
	CAG_DOC(CAG_DEC_CMP_ARRAY, C, T);
	printf("\n\n");
	printf("/*********/\n");
	printf("/* DLIST */\n");
	printf("/*********/\n\n");
	CAG_DOC(CAG_DEC_CMP_DLIST, C, T);
	printf("\n\n");
	printf("/*********/\n");
	printf("/* SLIST */\n");
	printf("/*********/\n\n");
	CAG_DOC(CAG_DEC_CMP_SLIST, C, T);
	printf("\n\n");
	printf("/*********/\n");
	printf("/* TREE  */\n");
	printf("/*********/\n\n");
	CAG_DOC(CAG_DEC_CMP_TREE, C, T);
	printf("\n\n");
	printf("/*********/\n");
	printf("/* HASH */\n");
	printf("/*********/\n\n");
	CAG_DOC(CAG_DEC_CMP_HASH, C, T);
	printf("\n");
	return 0;
}
