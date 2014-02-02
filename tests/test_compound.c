/*! Tests for compound container structures.

  \copyright Copyright 2013 Nathan Geffen. All rights reserved.

  \license GNU Lesser General Public License Copyright.

  \sa COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#include "cagl/dlist.h"
#include "cagl/array.h"
#include "cagl/tree.h"
#include "cagl/hash.h"
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/slist.h"

#define CAG_SAFER 1

CAG_DEC_DEF_CMP_DLIST(ilist, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_SLIST(islist, int);
CAG_DEC_DEF_ARRAY(iarr, int);
CAG_DEC_DEF_CMP_TREE(itree, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_HASH(ihash, int, CAG_CMP_PRIMITIVE, CAG_INT_HASH, sizeof);


CAG_DEC_ARRAY(adj_list, ilist);
CAG_DEC_ARRAY(adj_slist, islist);

CAG_DEC_DEF_ALL_ARRAY(adj_mat, iarr, CAG_STRUCT_ALLOC_STYLE,
		      new_from_iarr, free_iarr, CAG_BYADR);

CAG_DEC_DEF_ALL_ARRAY(adj_tree, itree, CAG_STRUCT_ALLOC_STYLE,
		      new_from_itree, free_itree, CAG_BYADR);

CAG_DEC_DEF_ALL_ARRAY(adj_hash, ihash, CAG_STRUCT_ALLOC_STYLE,
		      new_from_ihash, free_ihash, CAG_BYADR);

CAG_DEC_DEF_ALL_DLIST(lat, adj_hash, CAG_STRUCT_ALLOC_STYLE,
		      new_from_adj_hash, free_adj_hash, CAG_BYADR);

static void populate_adj_list(adj_list *m, int num)
{
	int i, j;
	for (i = 0; i < num; ++i) {
		ilist l;
		if (new_ilist(&l) == NULL) {
			fprintf(stderr, "Out of memory %d", __LINE__);
			exit(1);
		}
		for (j = 0; j < num; ++j) {
			if (prepend_ilist(&l, j) == NULL) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
		}
		if (append_adj_list(m, l) == NULL) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
		}
		free_ilist(&l);
	}
}

static void populate_adj_list_efficient(adj_list *m, int num)
{
	int i, j;
	ilist l;
	ilist *pl;
	new_ilist(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_list(m, &l);
		pl = back_adj_list(m);
		for (j = 0; j < num; ++j)
			if (!prepend_ilist(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_ilist(&l);
}

static void populate_adj_slist_efficient(adj_slist *m, int num)
{
	int i, j;
	islist l;
	islist *pl;
	new_islist(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_slist(m, &l);
		pl = back_adj_slist(m);
		for (j = 0; j < num; ++j)
			if (!prepend_islist(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_islist(&l);
}


static void populate_matrix_efficient(adj_mat *m, int num)
{
	int i, j;
	iarr l;
	iarr *pl;
	new_iarr(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_mat(m, &l);
		pl = back_adj_mat(m);
		for (j = 0; j < num; ++j)
			if (!append_iarr(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_iarr(&l);
}

static void populate_adj_tree_efficient(adj_tree *m, int num)
{
	int i, j;
	itree l;
	itree *pl;
	new_itree(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_tree(m, &l);
		pl = back_adj_tree(m);
		for (j = 0; j < num; ++j)
			if (!insert_itree(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_itree(&l);
}

static void populate_adj_hash_efficient(adj_hash *m, int num)
{
	int i, j;
	ihash l;
	ihash *pl;
	new_ihash(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_hash(m, &l);
		pl = back_adj_hash(m);
		for (j = 0; j < num; ++j)
			if (!insert_ihash(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_ihash(&l);
}

static void populate_lat_efficient(lat *l, int num)
{
	int i;
	adj_hash ah;
	new_adj_hash(&ah);
	for (i = 0; i < num; ++i) {
		appendp_lat(l, &ah);
		populate_adj_hash_efficient(back_lat(l), num);
	}
	free_adj_hash(&ah);
}


#define NUM_ELEMS 5

static void test_adj_list(struct cag_test_series *tests)
{
	adj_list m;
	it_ilist it;
	ilist *l;
	int i, j, failures = 0;
	new_adj_list(&m);
	populate_adj_list(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_list(begin_adj_list(&m), i)->value;
		j = 4;
		CAG_FOR_ALL(ilist, l, it,
			    {
				    if (it->value != j)
					    ++failures;
				    --j;
			    });
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: adjacency list has expected values");
	free_adj_list(&m);
	failures = 0;
	new_adj_list(&m);
	populate_adj_list_efficient(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_list(begin_adj_list(&m), i)->value;
		j = 4;
		CAG_FOR_ALL(ilist, l, it,
			    {
				    if (it->value != j)
					    ++failures;
				    --j;
			    });
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: adjacency list has expected values");
	free_adj_list(&m);
}

static void test_adj_slist(struct cag_test_series *tests)
{
	adj_slist m;
	it_islist it;
	islist *l;
	int i, j, failures = 0;
	new_adj_slist(&m);
	populate_adj_slist_efficient(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_slist(begin_adj_slist(&m), i)->value;
		j = 4;
		CAG_FOR_ALL(islist, l, it,
			    {
				    if (it->value != j)
					    ++failures;
				    --j;
			    });
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: adjacency slist has expected values");
	free_adj_slist(&m);
}


static void test_matrix(struct cag_test_series *tests)
{
	adj_mat m;
	it_iarr it;
	iarr *l;
	int i, j, failures = 0;
	new_adj_mat(&m);
	populate_matrix_efficient(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_mat(begin_adj_mat(&m), i)->value;
		j = 0;
		CAG_FOR_ALL(iarr, l, it,
			    {
				    if (it->value != j)
					    ++failures;
				    ++j;
			    });
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: adjacency matrix has expected values");
	free_adj_mat(&m);
}


static void test_adj_tree(struct cag_test_series *tests)
{
	adj_tree m;
	it_itree it;
	itree *l;
	int i, j, failures = 0;
	new_adj_tree(&m);
	populate_adj_tree_efficient(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_tree(begin_adj_tree(&m), i)->value;
		j = 0;
		CAG_FOR_ALL(itree, l, it,
			    {
				    if (it->value != j)
					    ++failures;
				    ++j;
			    });
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: adjacency tree has expected values");
	free_adj_tree(&m);
}

static void test_adj_hash(struct cag_test_series *tests)
{
	adj_hash m;
	it_ihash it;
	ihash *l;
	int i, j, total = 0;
	new_adj_hash(&m);
	populate_adj_hash_efficient(&m, NUM_ELEMS);
	for (i = 0; i < NUM_ELEMS; ++i) {
		l = &at_adj_hash(begin_adj_hash(&m), i)->value;
		j = (NUM_ELEMS - 1) * NUM_ELEMS / 2;
		CAG_FOR_ALL(ihash, l, it, total += it->value);
	}
	CAG_TEST(*tests, j * NUM_ELEMS == total,
		 "cag_compound: adjacency hash has expected values");
	free_adj_hash(&m);
}

static void test_lat(struct cag_test_series *tests)
{
	lat l;
	it_lat it;
	it_ihash ih;
	adj_hash m;
	ihash h;
	int i, j, total;

	new_lat(&l);
	populate_lat_efficient(&l, 5);
	for (it = begin_lat(&l); it != end_lat(&l); it = next_lat(it)) {
		m = it->value;
		total = 0;
		for (i = 0; i < NUM_ELEMS; ++i) {
			h = at_adj_hash(begin_adj_hash(&m), i)->value;
			j = (NUM_ELEMS - 1) * NUM_ELEMS / 2;
			CAG_FOR_ALL(ihash, &h, ih, total += ih->value);
		}
		CAG_TEST(*tests, j * NUM_ELEMS == total,
			 "cag_compound: adjacency hash has expected values");
	}
	free_lat(&l);
}

static void test_copy_over_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	it_iarr it2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	set_exact_size_islist(&list, 10);
	CAG_COPY_OVER(iarr, &arr, islist, &list);
	CAG_TEST(*tests, distance_all_islist(&list) == distance_all_iarr(&arr),
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), it2 = beg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), it2 = next_iarr(it2))
		if (it1->value != it2->value)
			++failures;
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}

static void test_rcopy_over_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	rit_iarr rit2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	set_exact_size_islist(&list, 10);
	CAG_RCOPY_OVER(iarr, &arr, islist, &list);
	CAG_TEST(*tests, distance_all_islist(&list) == distance_all_iarr(&arr),
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), rit2 = rbeg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), rit2 = rnext_iarr(rit2))
		if (it1->value != rit2->value)
			++failures;
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}


static int even(int x)
{
	return x % 2 == 0;
}

static void test_copy_over_if_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	it_iarr it2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	set_exact_size_islist(&list, 5);
	CAG_COPY_OVER_IF(iarr, &arr, islist, &list, even);
	CAG_TEST(*tests, distance_all_islist(&list) == 5,
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), it2 = beg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), it2 = next_iarr(it2)) {
		if (it1->value != it2->value)
			++failures;
		it2 = next_iarr(it2);
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}

static void test_rcopy_over_if_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	rit_iarr rit2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	set_exact_size_islist(&list, 5);
	CAG_RCOPY_OVER_IF(iarr, &arr, islist, &list, even);
	CAG_TEST(*tests, distance_all_islist(&list) == 5,
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), rit2 = rbeg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), rit2 = rnext_iarr(rit2)) {
		rit2 = rnext_iarr(rit2);
		if (it1->value != rit2->value)
			++failures;
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}



static void test_copy_macro(struct cag_test_series *tests)
{
	islist list;
	ilist dlist;
	iarr arr;
	it_islist it1;
	it_ilist it3;
	it_iarr it2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);
	new_ilist(&dlist);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	CAG_COPY(iarr, &arr, islist, &list, i);
	CAG_TEST(*tests, i == CAG_TRUE &&
		 distance_all_islist(&list) == distance_all_iarr(&arr),
		 "cag_compound: Copy results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), it2 = beg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), it2 = next_iarr(it2)) {
		if (it1->value != it2->value)
			++failures;
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	CAG_COPY(iarr, &arr, ilist, &dlist, i);
	CAG_TEST(*tests, i == CAG_TRUE &&
		 distance_all_ilist(&dlist) == distance_all_iarr(&arr),
		 "cag_compound: Copy results in same size containers.");

	failures = 0;
	for (it3 = beg_ilist(&dlist), it2 = beg_iarr(&arr);
	     it3 != end_ilist(&dlist);
	     it3 = next_ilist(it3), it2 = next_iarr(it2)) {
		if (it3->value != it2->value)
			++failures;
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied dlist identical to copied from array.");


	free_ilist(&dlist);
	free_islist(&list);
	free_iarr(&arr);
}

static void test_rcopy_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	rit_iarr rit2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	CAG_RCOPY(iarr, &arr, islist, &list, i);
	CAG_TEST(*tests, i == CAG_TRUE &&
		 distance_all_islist(&list) == distance_all_iarr(&arr),
		 "cag_compound: Copy results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), rit2 = rbeg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), rit2 = rnext_iarr(rit2)) {
		if (it1->value != rit2->value)
			++failures;
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: Copied list identical to copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}


static void test_copy_if_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	it_iarr it2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	CAG_COPY_IF(iarr, &arr, islist, &list, even, i);
	CAG_TEST(*tests, distance_all_islist(&list) == 5,
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), it2 = beg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), it2 = next_iarr(it2)) {
		if (it1->value != it2->value)
			++failures;
		it2 = next_iarr(it2);
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: List equal array ints copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}

static void test_rcopy_if_macro(struct cag_test_series *tests)
{
	islist list;
	iarr arr;
	it_islist it1;
	rit_iarr rit2;
	int i, failures;

	new_iarr(&arr);
	new_islist(&list);

	for (i = 0; i < 10; ++i)
		append_iarr(&arr,i);

	i = 0;
	CAG_RCOPY_IF(iarr, &arr, islist, &list, even, i);
	CAG_TEST(*tests, distance_all_islist(&list) == 5,
		 "cag_compound: Copy over results in same size containers.");

	failures = 0;
	for (it1 = beg_islist(&list), rit2 = rbeg_iarr(&arr);
	     it1 != end_islist(&list);
	     it1 = next_islist(it1), rit2 = rnext_iarr(rit2)) {
		rit2 = rnext_iarr(rit2);
		if (it1->value != rit2->value)
			++failures;
	}
	CAG_TEST(*tests, failures == 0,
		 "cag_compound: List equal array ints copied from array.");

	free_islist(&list);
	free_iarr(&arr);
}


void test_compound(struct cag_test_series *tests)
{
	test_adj_list(tests);
	test_matrix(tests);
	test_adj_slist(tests);
	test_adj_tree(tests);
	test_adj_hash(tests);
	test_lat(tests);
	test_copy_over_if_macro(tests);
	test_rcopy_over_if_macro(tests);
	test_copy_over_macro(tests);
	test_rcopy_over_macro(tests);
	test_copy_if_macro(tests);
	test_copy_macro(tests);
	test_rcopy_macro(tests);
	test_rcopy_if_macro(tests);
}

CAG_DEF_ALL_ARRAY(adj_list, ilist, CAG_STRUCT_ALLOC_STYLE,
		  new_from_ilist, free_ilist, CAG_BYADR);
CAG_DEF_ALL_ARRAY(adj_slist, islist, CAG_STRUCT_ALLOC_STYLE,
		  new_from_islist, free_islist, CAG_BYADR);
