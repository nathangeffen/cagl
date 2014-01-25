/*! Tests for CAGL doubly linked list.

  \copyright Copyright 2013 Nathan Geffen. All rights reserved.
  \license GNU Lesser General Public License Copyright.
  See COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAG_SAFER 1
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/dlist.h"

struct complex {
	double real;
	double imag;
};

/*
   The comparison function for complex numbers is purposefully wrong
   so that stability of sorting and insertions can be tested.
 */

static int cmp_complex(const struct complex x, const struct complex y)
{
	double z = x.real - y.real;
	if (z < 0) return -1;
	if (z > 0) return 1;
	return 0;
}

CAG_DEC_CMP_DLIST(complex_list, struct complex);
CAG_DEC_CMP_DLIST(ilist, int);

static void populate_list(complex_list *l1,
			  const int from, const int to, const int step)
{
	int i;
	struct complex c;

	if (from < to)
		for (i = from; i < to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			append_complex_list(l1, c);
		}
	else
		for (i = from; i > to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			append_complex_list(l1, c);
		}
}

static void test_new(struct cag_test_series *tests)
{
	complex_list l;

	CAG_TEST(*tests, new_complex_list(&l) != NULL,
			 "cag_test: list returned from new");
	CAG_TEST(*tests, l.begin_->next == l.end_,
		 "cag_test: begin_ next address of end_");
	CAG_TEST(*tests, l.end_->prev == l.begin_,
		 "cag_test: end_ prev equals address of begin_");
	free_complex_list(&l);
}


static void test_append_prepend(struct cag_test_series *tests)

{
	int i;
	struct complex c;
	complex_list l1, l2;
	it_complex_list it;

	new_complex_list(&l1);
	new_complex_list(&l2);
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		append_complex_list(&l1, c);
		prepend_complex_list(&l2, c);
	}
	i = 0;
	for(it = beg_complex_list(&l1); it != end_complex_list(&l1);
	    it = it->next) {
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: append value");
		++i;
	}
	i = 0;
	for(it = CAG_BEGIN_DLIST(&l1); it != CAG_END_DLIST(&l1); it = it->next) {
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: append correct value");
		++i;
	}
	CAG_TEST(*tests, i == 5,
		 "cag_dlist: test appends with macros boundaries");
	free_complex_list(&l1);
	free_complex_list(&l2);
}

static void test_appendp_prependp(struct cag_test_series *tests)

{
	int i;
	struct complex c;
	complex_list l1, l2;
	it_complex_list it;

	new_complex_list(&l1);
	new_complex_list(&l2);
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		appendp_complex_list(&l1, &c);
		prependp_complex_list(&l2, &c);
	}
	i = 0;
	for(it = beg_complex_list(&l1); it != end_complex_list(&l1);
	    it = it->next) {
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: appendp value");
		++i;
	}
	i = 0;
	for(it = CAG_BEGIN_DLIST(&l1); it != CAG_END_DLIST(&l1); it = it->next) {
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: appendp correct value");
		++i;
	}
	CAG_TEST(*tests, i == 5,
		 "cag_dlist: test appendsp with macros boundaries");
	free_complex_list(&l1);
	free_complex_list(&l2);
}


static void test_front_back(struct cag_test_series *tests)
{
	complex_list l;
	struct complex c;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	c = *front_complex_list(&l);
	CAG_TEST(*tests, c.real == 0.0 && c.imag == 0.0,
		 "cag_dlist: Retrieve front value after appends");
	c = *back_complex_list(&l);
	CAG_TEST(*tests, c.real == 4.0 && c.imag == -4.0,
		"cag_dlist: Retrieve back value after appends");
	free_complex_list(&l);
}


static void test_insert(struct cag_test_series *tests)
{
	struct complex c;
	complex_list l;
	it_complex_list it;

	new_complex_list(&l);
	c.real = 3.14;
	c.imag = c.real * c.real;
	it = insert_complex_list(beg_complex_list(&l), c);
	CAG_TEST(*tests, it->value.real == 3.14 && it->value.imag == 9.8596,
		 "cag_dlist: insert at beginning of list");
	c = *front_complex_list(&l);
	CAG_TEST(*tests, c.real == 3.14 && c.imag == 9.8596,
		 "cag_dlist: front after insertion");
	c.real = 100.1; c.imag = -100.1;
	it = insert_complex_list(it->next, c);
	CAG_TEST(*tests, it->value.real == 100.1 && it->value.imag == -100.1,
		 "cag_dlist: insert before second element");
	CAG_TEST(*tests, it->prev->value.real == 3.14 &&
		 it->prev->value.imag == 9.8596,
		 "cag_dlist: prev value after insert");
	c.real = 16.0; c.imag = -16.0;
	it = insert_complex_list(end_complex_list(&l), c);
	c = *back_complex_list(&l);
	CAG_TEST(*tests, c.real == 16.0 && c.imag == -16.0,
		 "cag_dlist: back value after insert");
	free_complex_list(&l);
}

static void test_insertp(struct cag_test_series *tests)
{
	struct complex c;
	complex_list l;
	it_complex_list it;

	new_complex_list(&l);
	c.real = 3.14;
	c.imag = c.real * c.real;
	it = insertp_complex_list(beg_complex_list(&l), &c);
	CAG_TEST(*tests, it->value.real == 3.14 && it->value.imag == 9.8596,
		 "cag_dlist: insert at beginning of list");
	c = *front_complex_list(&l);
	CAG_TEST(*tests, c.real == 3.14 && c.imag == 9.8596,
		 "cag_dlist: front after insertion");
	c.real = 100.1; c.imag = -100.1;
	it = insertp_complex_list(it->next, &c);
	CAG_TEST(*tests, it->value.real == 100.1 && it->value.imag == -100.1,
		 "cag_dlist: insert before second element");
	CAG_TEST(*tests, it->prev->value.real == 3.14 &&
		 it->prev->value.imag == 9.8596,
		 "cag_dlist: prev value after insert");
	c.real = 16.0; c.imag = -16.0;
	it = insertp_complex_list(end_complex_list(&l), &c);
	c = *back_complex_list(&l);
	CAG_TEST(*tests, c.real == 16.0 && c.imag == -16.0,
		 "cag_dlist: back value after insert");
	free_complex_list(&l);
}


static void test_insert_order(struct cag_test_series *tests)
{
	struct complex c = {3, -3};
	complex_list l;
	it_complex_list it;
	size_t d;
	double t = -20.0;
	int inorder = CAG_TRUE;

	new_complex_list(&l);
	c.real = 6;
	insert_gt_complex_list(&l, beg_complex_list(&l), c);
	c.real = 0;
	insert_gt_complex_list(&l, beg_complex_list(&l), c);
	c.real = 4;
	insert_gt_complex_list(&l, beg_complex_list(&l), c);
	c.real = 8;
	insert_gt_complex_list(&l, beg_complex_list(&l), c);
	c.real = 2;
	insert_gt_complex_list(&l, beg_complex_list(&l), c);
	for (it = beg_complex_list(&l); it != end_complex_list(&l);
	     it = it->next)
		if (it->value.real < t) {
			inorder = CAG_FALSE;
			break;
		} else
			t = it->value.real;
	d = distance_complex_list(beg_complex_list(&l), end_complex_list(&l));
	CAG_TEST(*tests,  d == 5 && inorder == CAG_TRUE,
		 "cag_dlist: insert ordered values");
	c.real = 3.0;
	it = insert_gt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == 2.0 &&
		 it->value.real == 3.0 &&
		 it->next->value.real == 4.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+1,
		 "cag_dlist: insert gt middle");
	c.real = 20.0;
	it = insert_gt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == 8.0 &&
		 it->value.real == 20.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+2,
		 "cag_dlist: insert gt end");
	c.real = -1.0;
	it = insert_gt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it == beg_complex_list(&l) &&
		 it->next->value.real == 0.0 &&
		 it->value.real == -1.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+3,
		 "cag_dlist: insert gt beginning");
	c.real = -0.5;
	c.imag = 10.0;
	it = insert_gt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == -1.0 &&
		 it->value.real == -0.5 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+4,
		 "cag_dlist: insert gt one beyond beginning");
	c.imag = 5.0;
	it = insert_gt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == -1.0 &&
		 it->value.real == -0.5 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+5,
		 "cag_dlist: insert gt duplicate stable");
	it = insert_gteq_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == -0.5 &&
		 it->value.real == -0.5 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+6,
		 "cag_dlist: insert gt duplicate unstable");
	free_complex_list(&l);

	new_complex_list(&l);
	populate_list(&l, 10, 0, -2);
	d = distance_complex_list(beg_complex_list(&l), end_complex_list(&l));
	c.real = 3.0;
	c.imag = 1.0;
	it = insert_lt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->next->value.real == 2.0 &&
		 it->value.real == 3.0 &&
		 it->prev->value.real == 4.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+1,
		 "cag_dlist: insert lt middle");
	c.imag = 2.0;
	it = insert_lt_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->next->value.real == 3.0 &&
		 it->value.real == 3.0 &&
		 it->prev->value.real == 4.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+2,
		 "cag_dlist: insert lt duplicate stable");
	c.imag = 3.0;
	it = insert_lteq_complex_list(&l, beg_complex_list(&l), c);
	CAG_TEST(*tests, it->prev->value.real == 3.0 &&
		 it->value.real == 3.0 &&
		 it->next->value.real == 2.0 &&
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == d+3,
		 "cag_dlist: insert lt duplicate unstable");
	free_complex_list(&l);
}

static void test_insertp_order(struct cag_test_series *tests)
{
	struct complex c = {3, -3};
	complex_list l;
	it_complex_list it;
	size_t d;
	double t = -20.0;
	int inorder = CAG_TRUE;

	new_complex_list(&l);
	c.real = 6;
	insertp_gt_complex_list(&l, beg_complex_list(&l), &c);
	c.real = 0;
	insertp_gt_complex_list(&l, beg_complex_list(&l), &c);
	c.real = 4;
	insertp_gt_complex_list(&l, beg_complex_list(&l), &c);
	c.real = 8;
	insertp_gt_complex_list(&l, beg_complex_list(&l), &c);
	c.real = 2;
	insertp_gt_complex_list(&l, beg_complex_list(&l), &c);
	for (it = beg_complex_list(&l); it != end_complex_list(&l);
	     it = it->next)
		if (it->value.real < t) {
			inorder = CAG_FALSE;
			break;
		} else
			t = it->value.real;
	d = distance_complex_list(beg_complex_list(&l), end_complex_list(&l));
	CAG_TEST(*tests,  d == 5 && inorder == CAG_TRUE,
		 "cag_dlist: insert ordered values");
	free_complex_list(&l);
}




static void test_distance(struct cag_test_series *tests)
{
	complex_list l;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	CAG_TEST(*tests, distance_complex_list(beg_complex_list(&l),
					       end_complex_list(&l)) == 5,
		 "cag_dlist: number elements appended list");
	CAG_TEST(*tests, rdistance_complex_list(rbeg_complex_list(&l),
						rend_complex_list(&l)) == 5,
		 "cag_dlist: number elements appended list reverse");
	free_complex_list(&l);
}

static void test_iterator_comparison(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it1, it2;
	rit_complex_list rit1, rit2;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	it1 = beg_complex_list(&l);
	it2 = it1;
	CAG_TEST(*tests, lteq_it_complex_list(it1, it2) == 1,
		 "cag_dlist: equal iterators are equal");
	CAG_TEST(*tests, lt_it_complex_list(it1, it2) == 0,
		 "cag_dlist: equal iterators are not less than");
	it2 = it2->next;
	CAG_TEST(*tests, lteq_it_complex_list(it1, it2) == 1,
		 "cag_dlist: less than equal iterators");
	CAG_TEST(*tests, lt_it_complex_list(it1, it2) == 1,
		 "cag_dlist: less than iterators");
	it2 = end_complex_list(&l)->prev;
	CAG_TEST(*tests, lteq_it_complex_list(it1, it2) == 1,
		 "cag_dlist: less than equal last item");
	CAG_TEST(*tests, lt_it_complex_list(it1, it2) == 1,
		 "cag_dlist: less than last item");
	it2 = end_complex_list(&l)->prev;
	CAG_TEST(*tests, lteq_it_complex_list(it2, it1) == 0,
		 "cag_dlist: more than not equal last item");
	CAG_TEST(*tests, lt_it_complex_list(it2, it1) == 0,
		 "cag_dlist: more than last item");

	rit1 = rbeg_complex_list(&l);
	rit2 = rit1;
	CAG_TEST(*tests, rlteq_it_complex_list(rit1, rit2) == 1,
		 "cag_dlist: equal rev iterators are equal");
	CAG_TEST(*tests, rlt_it_complex_list(rit1, rit2) == 0,
		 "cag_dlist: equal rev iterators are not less than");
	rit2 = rit2->next;
	CAG_TEST(*tests, rlteq_it_complex_list(rit1, rit2) == 1,
		 "cag_dlist: less than equal rev iterators");
	CAG_TEST(*tests, rlt_it_complex_list(rit1, rit2) == 1,
		 "cag_dlist: less than rev iterators");
	rit2 = rend_complex_list(&l)->prev;
	CAG_TEST(*tests, rlteq_it_complex_list(rit1, rit2) == 1,
		 "cag_dlist: less than equal first item reverse");
	CAG_TEST(*tests, rlt_it_complex_list(rit1, rit2) == 1,
		 "cag_dlist: less than first item reverse");
	rit2 = rend_complex_list(&l)->prev;
	CAG_TEST(*tests, rlteq_it_complex_list(rit2, rit1) == 0,
		 "cag_dlist: more than not equal first item reverse");
	CAG_TEST(*tests, rlt_it_complex_list(rit2, rit1) == 0,
		 "cag_dlist: more than first item reverse");
	free_complex_list(&l);
}



static void test_reverse_iterator(struct cag_test_series *tests)
{
	int i = 4;
	complex_list l;
	rit_complex_list rt;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	for(rt = rbeg_complex_list(&l); rt != rend_complex_list(&l);
	    rt = rt->next) {
		CAG_TEST(*tests, rt->value.real == i,
			 "cag_dlist: append correct value");
		CAG_TEST(*tests, rt->value.imag == -i,
			 "cag_dlist: append value");
		--i;
	}
	free_complex_list(&l);
}

static void test_erase(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it;
	int i;
	size_t s;
	struct complex c;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == 5,
		 "cag_dlist: distance before erase");
	for(i = 0, it = beg_complex_list(&l), s = 4;
	    it != end_complex_list(&l);  ++i, --s) {
		c = *front_complex_list(&l);
		CAG_TEST(*tests, (i == c.real && c.imag == -i),
			 "cag_dlist: remove object");
		it = erase_complex_list(&l, it);
		CAG_TEST(*tests,
			 distance_complex_list(beg_complex_list(&l),
					       end_complex_list(&l)) == s,
			 "cag_dlist: number elements after erase");
	}
	populate_list(&l, 0, 5, 1);
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == 5,
		 "cag_dlist: distance before erase");
	erase_range_complex_list(&l, beg_complex_list(&l),
				 end_complex_list(&l));
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l),
				       end_complex_list(&l)) == 0,
		 "cag_dlist: distance after erase all");
	CAG_TEST(*tests,
		 rdistance_complex_list(rbeg_complex_list(&l),
				       rend_complex_list(&l)) == 0,
		 "cag_dlist: reverse distance after erase all");
	free_complex_list(&l);
	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	CAG_TEST(*tests, distance_all_complex_list(&l) == 5,
			 "cag_dlist: distance all container");
	erase_all_complex_list(&l);
	CAG_TEST(*tests, distance_all_complex_list(&l) == 0,
			 "cag_dlist: erase all container");
	free_complex_list(&l);
}


static void test_copy_over(struct cag_test_series *tests)
{
	complex_list l1, l2;
	it_complex_list it1, it2;

	new_complex_list(&l1);
	new_complex_list(&l2);

	populate_list(&l1, 0, 5, 1);

	set_exact_size_complex_list(&l2, 5);
	copy_over_complex_list(beg_complex_list(&l1), end_complex_list(&l1),
			       beg_complex_list(&l2));
	for (it1 = beg_complex_list(&l1), it2 = beg_complex_list(&l2);
	     it1 != end_complex_list(&l1); it1 = it1->next, it2 = it2->next)
		CAG_TEST(*tests, it1->value.real == it2->value.real &&
			 it1->value.imag == it2->value.imag,
			 "cag_test: element values after copy");
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l1),
				       end_complex_list(&l1)) ==
		 distance_complex_list(beg_complex_list(&l2),
				       end_complex_list(&l2)),
		 "cag_test: number elements after copy");
	free_complex_list(&l1);
	free_complex_list(&l2);
}


static void test_search(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it, it_all;
	rit_complex_list rit;
	struct complex c;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	populate_list(&l, 0, 5, 1);
	c.real = 3.0;
	c.imag = -3.0;
	it = search_complex_list(beg_complex_list(&l), end_complex_list(&l), c);
	CAG_TEST(*tests, it != end_complex_list(&l) && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_test: Value found");
	it_all = search_all_complex_list(&l, c);
	CAG_TEST(*tests, it == it_all,
		 "cag_test: Search all finds same as search iterator");
	it = search_complex_list(it->next, end_complex_list(&l), c);
	CAG_TEST(*tests, it != end_complex_list(&l) && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_test: Value found second time");
	rit = (rit_complex_list) it;
	rit = rsearch_complex_list(rit, rend_complex_list(&l), c);
	CAG_TEST(*tests, rit != rend_complex_list(&l) && rit->value.real == 3.0
		 && rit->value.imag == -3.0,
		 "cag_test: Value found reverse");
	it = search_complex_list(it->next, end_complex_list(&l), c);
	CAG_TEST(*tests, it == end_complex_list(&l),
		 "cag_test: Value not found");
	c.real = 100;
	rit = rsearch_complex_list(rit->next, rend_complex_list(&l), c);
	CAG_TEST(*tests, rit == rend_complex_list(&l),
		 "cag_test: Value not found");
	free_complex_list(&l);
}

static void test_searchp(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it, it_all;
	rit_complex_list rit;
	struct complex c;

	new_complex_list(&l);
	populate_list(&l, 0, 5, 1);
	populate_list(&l, 0, 5, 1);
	c.real = 3.0;
	c.imag = -3.0;
	it = searchp_complex_list(beg_complex_list(&l),
				end_complex_list(&l), &c);
	it_all = searchp_all_complex_list(&l, &c);
	CAG_TEST(*tests, it == it_all,
		 "cag_test: Searchp all finds same as searchp iterator");
	CAG_TEST(*tests, it != end_complex_list(&l) && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_test: Value found");
	it = searchp_complex_list(it->next, end_complex_list(&l), &c);
	CAG_TEST(*tests, it != end_complex_list(&l) && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_test: Value found second time");
	it = searchp_complex_list(it->next, end_complex_list(&l), &c);
	CAG_TEST(*tests, it == end_complex_list(&l),
		 "cag_test: Value not found");
	rit = rbeg_complex_list(&l);
	rit = rsearchp_complex_list(rit->next, rend_complex_list(&l), &c);
	CAG_TEST(*tests, rit != rend_complex_list(&l) && rit->value.real == 3.0
		 && rit->value.imag == -3.0,
		 "cag_test: reverse value found");
	c.real = 100.0;
	rit = rsearchp_complex_list(rit->next, rend_complex_list(&l), &c);
	CAG_TEST(*tests, rit == rend_complex_list(&l),
		 "cag_test: value reverse not found");
	free_complex_list(&l);
}


static int find_element(const struct complex *c1, const struct complex *c2)
{
	if (c1->imag == c2->imag)
		return 0;
	else
		return -1;
}

static void test_find(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it;
	struct complex c;

	c.imag = -9;

	new_complex_list(&l);
	populate_list(&l, 0, 10, 1);
	it = find_all_complex_list(&l,c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_list: find element");
	c.imag = -10;
	it = find_all_complex_list(&l, c, find_element);
	CAG_TEST(*tests, it == end_complex_list(&l),
		 "cag_list: find element");
	c.imag = -9.0;
	it = findp_all_complex_list(&l, &c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_list: find element");
	c.imag = -10;
	it = findp_all_complex_list(&l, &c, find_element);
	CAG_TEST(*tests, it == end_complex_list(&l),
		 "cag_list: find element");
	c.imag = -9.0;
	free_complex_list(&l);
}


static void test_cmp(struct cag_test_series *tests)
{
	complex_list l1, l2, l3;
	struct complex c;
	new_complex_list(&l1);
	new_complex_list(&l2);
	new_complex_list(&l3);
	CAG_TEST(*tests, cmp_range_complex_list(beg_complex_list(&l1),
						end_complex_list(&l1),
						beg_complex_list(&l2),
						end_complex_list(&l2)) == 0,
		 "cag_test: compare empty lists");
	populate_list(&l1, 0, 5, 1);
	populate_list(&l2, 0, 5, 1);
	CAG_TEST(*tests, cmp_all_complex_list(&l1, &l2) == 0,
		 "cag_test: compare all equal lists");
	CAG_TEST(*tests, cmp_range_complex_list(beg_complex_list(&l1),
						end_complex_list(&l1),
						beg_complex_list(&l2),
						end_complex_list(&l2)) == 0,
		 "cag_test: compare equal lists");
	c.real = 10;
	c.imag = -10;
	append_complex_list(&l1, c);
	CAG_TEST(*tests, cmp_all_complex_list(&l1, &l2) > 0,
		 "cag_test: compare all 1st list bigger");
	CAG_TEST(*tests, cmp_range_complex_list(beg_complex_list(&l1),
						end_complex_list(&l1),
						beg_complex_list(&l2),
						end_complex_list(&l2)) > 0,
		 "cag_test: compare 1st list bigger");
	c.real = 20;
	c.imag = -10;
	append_complex_list(&l2, c);
	CAG_TEST(*tests, cmp_all_complex_list(&l1, &l2) < 0,
		 "cag_test: compare all 2nd list bigger");
	CAG_TEST(*tests, cmp_range_complex_list(beg_complex_list(&l1),
						end_complex_list(&l1),
						beg_complex_list(&l2),
						end_complex_list(&l2)) < 0,
		 "cag_test: compare 2nd list bigger");
	rcopy_all_complex_list(&l2, &l3);
	CAG_TEST(*tests, rcmp_all_complex_list(&l2, &l3) == 0,
		 "cag_test: compare all list and its reverse");
	free_complex_list(&l1);
	free_complex_list(&l2);
	free_complex_list(&l3);
}

static void test_equal(struct cag_test_series *tests)
{
	complex_list l1, l2, l3;
	new_complex_list(&l1);
	new_complex_list(&l2);
	new_complex_list(&l3);
	CAG_TEST(*tests,
		 equal_range_complex_list(beg_complex_list(&l1),
					  end_complex_list(&l1),
					  beg_complex_list(&l2)) == CAG_TRUE,
		 "cag_test: equal empty lists");
	populate_list(&l1, 0, 5, 1);
	populate_list(&l2, 0, 5, 1);
	rcopy_all_complex_list(&l2, &l3);
	CAG_TEST(*tests,
		 equal_range_complex_list(beg_complex_list(&l1),
					  end_complex_list(&l1),
					  beg_complex_list(&l2)) == CAG_TRUE,
		 "cag_test: equal lists");
	CAG_TEST(*tests, equal_all_complex_list(&l1, &l2) == CAG_TRUE,
		 "cag_test: equal all lists");
	at_complex_list(beg_complex_list(&l1), 3)->value.real = 10;
	at_complex_list(beg_complex_list(&l1), 3)->value.imag = 20;
	CAG_TEST(*tests,
		 equal_range_complex_list(beg_complex_list(&l1),
					  end_complex_list(&l1),
					  beg_complex_list(&l2)) == CAG_FALSE,
		 "cag_test: unequal lists");
	CAG_TEST(*tests, equal_all_complex_list(&l1, &l2) == CAG_FALSE,
		 "cag_test: unequal all lists");
	CAG_TEST(*tests, requal_all_complex_list(&l2, &l3) == CAG_TRUE,
		 "cag_test: requal all list and its reverse");
	free_complex_list(&l1);
	free_complex_list(&l2);
	free_complex_list(&l3);
}


static void test_copy(struct cag_test_series *tests)
{
	complex_list l1, l2;
	new_complex_list(&l1);
	new_complex_list(&l2);
	populate_list(&l1, 0, 5, 1);
	copy_all_complex_list(&l1, &l2);
	CAG_TEST(*tests, cmp_all_complex_list(&l1, &l2) == 0,
		 "cag_test: assigned lists equals");
	free_complex_list(&l2);
	new_complex_list(&l2);
	rcopy_all_complex_list(&l1, &l2);
	free_complex_list(&l1);
	free_complex_list(&l2);
}

static void test_at(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it;
	rit_complex_list rit;
	new_complex_list(&l);
	populate_list(&l, 0, 10, 1);
	it = at_complex_list(beg_complex_list(&l), 0);
	CAG_TEST(*tests, it == beg_complex_list(&l),
		"cag_test: 0 index");
	it = at_complex_list(it, 2);
	CAG_TEST(*tests, it->value.real == 2.0,
		 "cag_test: 2 index");
	it = at_complex_list(it, 7);
	CAG_TEST(*tests, it->value.real == 9.0,
		 "cag_test: 7 index");
	it = at_complex_list(it, 1);
	CAG_TEST(*tests, it == end_complex_list(&l),
		 "cag_test: at end");

	rit = rat_complex_list(rbeg_complex_list(&l), 0);
	CAG_TEST(*tests, rit == rbeg_complex_list(&l),
		"cag_test: 0 index reverse");
	rit = rat_complex_list(rit, 2);
	CAG_TEST(*tests, rit->value.real == 7.0,
		 "cag_test: 2 index reverse");
	rit = rat_complex_list(rit, 7);
	CAG_TEST(*tests, rit->value.real == 0.0,
		 "cag_test: 7 index reverse");
	rit = rat_complex_list(rit, 1);
	CAG_TEST(*tests, rit == rend_complex_list(&l),
		 "cag_test: rat end");

	free_complex_list(&l);
}

static void test_reverse(struct cag_test_series *tests)
{
	complex_list l;
	it_complex_list it;
	int i;

	new_complex_list(&l);
	populate_list(&l, 0, 10, 1);
	it = reverse_complex_list(beg_complex_list(&l), end_complex_list(&l));
	CAG_TEST(*tests, it == beg_complex_list(&l),
			 "cag_test: reversed list returns iterator = begin");
	for (it = beg_complex_list(&l), i = 9; it != end_complex_list(&l);
	     it = it->next, --i) {
		CAG_TEST(*tests, it->value.real == i,
			"cag_test: reversed list");
	}
	free_complex_list(&l);
	new_complex_list(&l);
	populate_list(&l, 0, 9, 1);
	reverse_complex_list(beg_complex_list(&l), end_complex_list(&l));
	for (it = beg_complex_list(&l), i = 8; it != end_complex_list(&l);
	     it = it->next, --i) {
		CAG_TEST(*tests, it->value.real == i,
			"cag_test: reversed list");
	}
	reverse_all_complex_list(&l);
	for (it = beg_complex_list(&l), i = 0; it != end_complex_list(&l);
	     it = it->next, ++i) {
		CAG_TEST(*tests, it->value.real == i,
			"cag_test: reverse all container list");
	}
	free_complex_list(&l);
}

static void test_batch(struct cag_test_series *tests)
{
	complex_list l1, l2, l3;
	int i;

	CAG_CHECK( (i = new_many_complex_list(&l1, NULL) > 0),
		   "failure many new");
	populate_list(&l1, 0, 5, 1);
	CAG_TEST(*tests, distance_complex_list(beg_complex_list(&l1),
					       end_complex_list(&l1)) == 5,
		 "cag_test: lists populated after batch new 1");
error:
	free_many_complex_list(i, &l1, NULL);

	CAG_CHECKL( (i = new_many_complex_list(&l1, &l2, NULL) > 0),
		    "failure many new", error_1);
	populate_list(&l1, 0, 5, 1);
	CAG_CHECKL(copy_many_complex_list(&l1, &l2, NULL) > 0,
		   "assign many failed", error_1);
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l1),
				       end_complex_list(&l1)) == 5 &&
		 cmp_all_complex_list(&l1, &l2) == 0,
		 "cag_test: lists populated after batch new 2");
error_1:
	free_many_complex_list(i, &l1, &l2, NULL);

	CAG_CHECKL( (i = new_many_complex_list(&l1, &l2, &l3, NULL)) > 0,
		   "failure many new", error_2);
	populate_list(&l1, 0, 5, 1);
	CAG_CHECKL(copy_many_complex_list(&l1, &l2, &l3, NULL) > 0,
		   "assign many failed", error_2);
	CAG_TEST(*tests,
		 distance_complex_list(beg_complex_list(&l1),
				       end_complex_list(&l1)) == 5 &&
		 cmp_all_complex_list(&l1, &l2) == 0 &&
		 cmp_all_complex_list(&l2, &l3) == 0,
		 "cag_test: lists populated after batch new 3");
error_2:
	free_many_complex_list(i, &l1, &l2, &l3, NULL);

}

static void test_int_list(struct cag_test_series *tests)
{
	int i, total = 0;
	ilist l;
	iterator_ilist *it;
	new_ilist(&l);
	append_ilist(&l, 2);
	for (i = 1; i < 5; ++i)
		append_ilist(&l, i);
	for (i = 0, it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next, ++i)
		total += it->value;
	CAG_TEST(*tests, i == 5 && total == 12,
		 "cag_test: ilist append");
	free_ilist(&l);
	new_ilist(&l);
	total = 0;
	for (i = 1; i < 5; ++i)
		appendp_ilist(&l, &i);
	for (i = 0, it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next, ++i)
		total += it->value;
	CAG_TEST(*tests, i == 4 && total == 10,
		 "cag_test: ilist pappend");
	free_ilist(&l);
}

static void test_shuffle(struct cag_test_series *tests)
{
	complex_list a;
	it_complex_list it1, it2;
	int inorder = 1;

	new_complex_list(&a);
	populate_list(&a, 0, 52, 1);
	random_shuffle_complex_list(beg_complex_list(&a),
				     end_complex_list(&a));
	it1 = beg_complex_list(&a);
	CAG_TEST(*tests,
		 at_complex_list(it1, 0)->value.real != 0.0 &&
		 at_complex_list(it1, 51)->value.real != 51.0 &&
		 at_complex_list(it1, 25)->value.real != 25.0,
		"cag_dlist: random shuffle puts values out of order");
	sort_all_complex_list(&a);
	CAG_FOLD_ALL(complex_list, &a, it1, it2,
		     {
			     if (it2->value.real <= it1->value.real)
				     inorder = 0;
		     },
		     inorder);
	CAG_TEST(*tests, inorder == 1,
		 "cag_dlist: random shuffle doesn't create dups");
	inorder = 1;
	CAG_FOLD(beg_complex_list(&a), end_complex_list(&a),
		 next_complex_list, it1, it2,
		     {
			     if (it2->value.real <= it1->value.real)
				     inorder = 0;
		     },
		     inorder);
	CAG_TEST(*tests, inorder == 1,
		 "cag_dlist: fold random shuffle doesn't create dups");
	it1 = beg_complex_list(&a);
	CAG_TEST(*tests,
		 at_complex_list(it1, 0)->value.real == 0.0 &&
		 at_complex_list(it1, 51)->value.real == 51.0 &&
		 at_complex_list(it1, 25)->value.real == 25.0,
		"cag_dlist: random shuffle doesn't lose data");
	free_complex_list(&a);
}


void test_sort(struct cag_test_series *tests)
{
	int i, j, inorder = CAG_TRUE;
	ilist l;
	iterator_ilist *it;

	new_ilist(&l);
	j = 11;
	for (i = 0; i < 11; ++i)
		append_ilist(&l, j - i);
	it = sort_ilist(beg_ilist(&l),
				end_ilist(&l));
	CAG_TEST(*tests, beg_ilist(&l) == it,
			"cag_test: sort returns iterator to beginning of sorted list");
	i = -1;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next) {
		if (it->value < i) {
			inorder = CAG_FALSE;
			break;
		}
		i = it->value;
	}
	CAG_TEST(*tests, inorder == CAG_TRUE &&
		 distance_ilist(beg_ilist(&l),
					end_ilist(&l)) == 11,
		 "cag_test: list in order after sort backward list");
	rsort_ilist(rbeg_ilist(&l),
			    rend_ilist(&l));
	i = 402;
	inorder = CAG_TRUE;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next) {
		if (it->value > i) {
			inorder = CAG_FALSE;
			break;
		}
		i = it->value;
	}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after reverse sort backward list");

	free_ilist(&l);

	new_ilist(&l);
	for (i = 0; i < 400; ++i)
		append_ilist(&l, rand() % 400);
	sort_ilist(beg_ilist(&l),
			   end_ilist(&l));
	i = -1;
	inorder = CAG_TRUE;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next) {
		if (it->value < i) {
			inorder = CAG_FALSE;
			break;
		}
		i = it->value;
	}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after sort random list");
	free_ilist(&l);

	new_ilist(&l);
	for (i = 0; i < 100; ++i)
		append_ilist(&l, rand() % 200);
	rsort_ilist(rbeg_ilist(&l),
			    rend_ilist(&l));
	i = 201;
	inorder = CAG_TRUE;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next) {
		if (it->value > i) {
			inorder = CAG_FALSE;
			break;
		}
		i = it->value;
	}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after reverse sort random list");
	free_ilist(&l);
}

void test_stable_sort(struct cag_test_series *tests)
{
	int i, inorder = CAG_TRUE;
	ilist l;
	iterator_ilist *it;
	complex_list cl;
	it_complex_list cit;
	struct complex c;

	new_ilist(&l);

	for (i = 0; i < 501; ++i)
		append_ilist(&l, 501 - i);
	it = stable_sort_ilist(beg_ilist(&l),
								   end_ilist(&l));
	CAG_TEST(*tests, beg_ilist(&l) == it,
			 "cag_test: stable sort returns iterator to begin of sorted list");
	i = -1;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next)
		if (it->value < i) inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder == CAG_TRUE &&
		 distance_ilist(beg_ilist(&l),
					end_ilist(&l)) == 501,
		 "cag_test: list in order after stable sort backward list");
	free_ilist(&l);
	new_ilist(&l);
	for (i = 0; i < 51; ++i)
		append_ilist(&l, rand() % 200);
	stable_sort_ilist(beg_ilist(&l),
				  end_ilist(&l));
	i = -1;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next)
		if (it->value <= i) inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after stable sort backward list");
	free_ilist(&l);

	new_ilist(&l);
	for (i = 0; i < 51; ++i)
		append_ilist(&l, 51 - i);
	rstable_sort_ilist(rbeg_ilist(&l),
				   rend_ilist(&l));
	i = 52;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next)
		if (it->value > i) inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after stable sort backward list");
	free_ilist(&l);
	new_ilist(&l);
	for (i = 0; i < 51; ++i)
		append_ilist(&l, rand() % 200);
	rstable_sort_ilist(rbeg_ilist(&l),
				   rend_ilist(&l));
	i = 201;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next)
		if (it->value >= i) inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after stable sort backward list");
	free_ilist(&l);
	new_complex_list(&cl);
	c.real = 4.0;
	c.imag = 0.0;
	appendp_complex_list(&cl, &c);
	c.real = 3.0;
	c.imag = 1.0;
	appendp_complex_list(&cl, &c);
	c.real = 3.0;
	c.imag = 2.0;
	appendp_complex_list(&cl, &c);
	c.real = 2.0;
	c.imag = 1.0;
	appendp_complex_list(&cl, &c);
	c.real = 2.0;
	c.imag = 2.0;
	appendp_complex_list(&cl, &c);
	c.real = 1.0;
	c.imag = 0.0;
	appendp_complex_list(&cl, &c);
	stable_sort_complex_list(beg_complex_list(&cl),
							 end_complex_list(&cl));
	cit = beg_complex_list(&cl);
	CAG_TEST(*tests, cit->value.real == 1.0 &&
		 cit->next->value.real == 2.0 && cit->next->value.imag == 1.0 &&
		 cit->next->next->value.real == 2.0 &&
		 cit->next->next->value.imag == 2.0 &&
		 cit->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->value.imag == 1.0 &&
		 cit->next->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->next->value.imag == 2.0 &&
		 cit->next->next->next->next->next->value.real == 4.0,
		 "stability of stable sort");
    stable_sort_all_complex_list(&cl);
	CAG_TEST(*tests, cit->value.real == 1.0 &&
		 cit->next->value.real == 2.0 && cit->next->value.imag == 1.0 &&
		 cit->next->next->value.real == 2.0 &&
		 cit->next->next->value.imag == 2.0 &&
		 cit->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->value.imag == 1.0 &&
		 cit->next->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->next->value.imag == 2.0 &&
		 cit->next->next->next->next->next->value.real == 4.0,
		 "stability of stable sort container");
	free_complex_list(&cl);
}

void test_stable_sort_macro(struct cag_test_series *tests)
{
	int i, inorder = CAG_TRUE;
	ilist l;
	iterator_ilist *it;
	complex_list cl;
	it_complex_list cit;
	struct complex c;

	new_ilist(&l);
	for (i = 0; i < 101; ++i)
		append_ilist(&l, 101 - i);

	CAG_STABLE_SORT(beg_ilist(&l), end_ilist(&l),
			ilist, iterator_ilist,
			new_ilist, free_ilist,
			beg_ilist, end_ilist,
			next_ilist, at_ilist,
			distance_ilist, appendp_ilist,
			CAG_CMP_DEFAULT, CAG_BYADR);

	i = -1;
	for (it = beg_ilist(&l); it != end_ilist(&l);
	     it = it->next)
		if (it->value < i) inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "cag_test: list in order after macro stable sort");
	free_ilist(&l);

	new_complex_list(&cl);
	c.real = 4.0;
	c.imag = 0.0;
	appendp_complex_list(&cl, &c);
	c.real = 3.0;
	c.imag = 1.0;
	appendp_complex_list(&cl, &c);
	c.real = 3.0;
	c.imag = 2.0;
	appendp_complex_list(&cl, &c);
	c.real = 2.0;
	c.imag = 1.0;
	appendp_complex_list(&cl, &c);
	c.real = 2.0;
	c.imag = 2.0;
	appendp_complex_list(&cl, &c);
	c.real = 1.0;
	c.imag = 0.0;
	appendp_complex_list(&cl, &c);
	CAG_STABLE_SORT(beg_complex_list(&cl), end_complex_list(&cl),
			complex_list, iterator_complex_list,
			new_complex_list, free_complex_list,
			beg_complex_list, end_complex_list,
			next_complex_list, at_complex_list,
			distance_complex_list, appendp_complex_list,
			cmp_complex, CAG_BYVAL);
	cit = beg_complex_list(&cl);
	CAG_TEST(*tests, cit->value.real == 1.0 &&
		 cit->next->value.real == 2.0 && cit->next->value.imag == 1.0 &&
		 cit->next->next->value.real == 2.0 &&
		 cit->next->next->value.imag == 2.0 &&
		 cit->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->value.imag == 1.0 &&
		 cit->next->next->next->next->value.real == 3.0 &&
		 cit->next->next->next->next->value.imag == 2.0 &&
		 cit->next->next->next->next->next->value.real == 4.0,
		 "stability of macro stable sort");

	free_complex_list(&cl);
}

static void test_abstract(struct cag_test_series *tests)
{
	complex_list a, b;
	it_complex_list it1, it2;
	struct complex c;
	int set = 1;
	int correct = 1;

	new_complex_list(&a);
	new_complex_list(&b);

	populate_list(&a, 0, 10, 1);
	c.real = 2;
	c.imag = 2;
	append_complex_list(&a, c);
	c.real = 5;
	c.imag = 5;
	append_complex_list(&a, c);

	/* Remove duplicates from a into b */
	sort_all_complex_list(&a);
	appendp_complex_list(&b, front_complex_list(&a));
	CAG_FOLD_ALL(complex_list, &a, it1, it2,
		     {
			     if (it1->value.real != it2->value.real)
				     appendp_complex_list(&b, &it2->value);
			     else
				     set = 0;
		     }, set);
	CAG_TEST(*tests, set == 0,
		 "cag_list: fold identifies duplicates");
	set = 1;
	CAG_FOLD_ALL(complex_list, &b, it1, it2,
		     {
			     if (it1->value.real >= it2->value.real) {
				     set = 0;
				     break;
			     }
		     }, set);
	CAG_TEST(*tests, set == 1,
		 "cag_list: duplicates removed");
	CAG_TEST(*tests, distance_all_complex_list(&b) ==
		 distance_all_complex_list(&a) - 2,
		 "cag_list: size of list with dups removed");
	set_exact_size_complex_list(&b, distance_all_complex_list(&a));
	CAG_MAP_SAME(complex_list, &a, &b, it1, it2,
		     {
			     it2->value.real = it1->value.real * 2;
		     });
	CAG_TEST(*tests, distance_all_complex_list(&b) ==
		 distance_all_complex_list(&a),
		 "cag_list: size of lists equal after map");
	it2 = beg_complex_list(&a);
	CAG_FOR_ALL(complex_list, &b, it1,
		{
			if(it1->value.real != it2->value.real * 2)
				correct = 0;
			it2 = next_complex_list(it2);
		});
	CAG_TEST(*tests, correct == 1,
		 "cag_list: expected values after map");
	CAG_FILTER_SAME(complex_list, &a, &b, it1, it2,
			(int) it1->value.real % 2 == 0);
	erase_range_complex_list(&b, it2, end_complex_list(&b));
	CAG_TEST(*tests, distance_all_complex_list(&b) == 6,
		"cag_list: size reduced after filter and erase");
	CAG_TEST(*tests,
		 at_complex_list(beg_complex_list(&b), 0)->value.real == 0 &&
		 at_complex_list(beg_complex_list(&b), 5)->value.real == 8,
		 "cag_list: expected values after filter");
	free_complex_list(&b);
	free_complex_list(&a);
}




void test_dlist(struct cag_test_series *tests)
{
	test_new(tests);
	test_append_prepend(tests);
	test_appendp_prependp(tests);
	test_front_back(tests);
	test_insert(tests);
	test_insertp(tests);
	test_insert_order(tests);
	test_insertp_order(tests);
	test_distance(tests);
	test_iterator_comparison(tests);
	test_reverse_iterator(tests);
	test_erase(tests);
	test_copy_over(tests);
	test_search(tests);
	test_searchp(tests);
	test_find(tests);
	test_cmp(tests);
	test_equal(tests);
	test_copy(tests);
	test_at(tests);
	test_reverse(tests);
	test_batch(tests);
	test_int_list(tests);
	test_shuffle(tests);
	test_sort(tests);
	test_stable_sort(tests);
	test_stable_sort_macro(tests);
	test_abstract(tests);

	return;
}

CAG_DEF_CMP_DLIST(complex_list, struct complex, cmp_complex);
