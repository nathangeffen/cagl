/*! Tests for CAGL singly linked list.

  \copyright Copyright 2013 Nathan Geffen. All rights reserved.
  \license GNU Lesser General Public License Copyright.
  See COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAG_SAFER 1
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/slist.h"
#include "cagl/prim.h"

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

CAG_DEC_CMP_SLIST(complex_slist, struct complex);
CAG_DEC_IT(it_complex, struct complex);
CAG_DEC_IT(it_int, int);

static void populate_list(complex_slist *l1,
			  const int from, const int to, const int step)
{
	int i;
	struct complex c;

	if (from < to)
		for (i = from; i < to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			prepend_complex_slist(l1, c);
		}
	else
		for (i = from; i > to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			prepend_complex_slist(l1, c);
		}
}

static void test_it(struct cag_test_series *tests)
{
	it_complex node1, node2;
	it_int intlist, it;
	struct complex c;
	int i, failure = 0;

	c.real = 1;
	c.imag = -1;
	CAG_MAKE_SLIST(node1, c);
	CAG_TEST(*tests, node1 && node1->value.real == 1,
		 "cag_slist: make node");
	c.real = 2;
	CAG_MAKE_SLIST(node2, c);
	CAG_INSERT_AFTER_SLIST(node1, node2);
	CAG_TEST(*tests, node1 && node1->value.real == 1 &&
		 node1->next->value.real == 2 &&
		 node1->next->next == NULL,
		 "cag_slist: insert after node");
	CAG_DISTANCE_SLIST(node1, NULL, i);
	CAG_TEST(*tests, i == 2,
		 "cag_slist: distance after two make nodes");
	CAG_ERASE_ALL_SLIST(node1, CAG_NO_FREE_FUNC);
	node1 = NULL;
	for (i = 0; i < 5; ++i) {
		c.real = i;
		CAG_MAKE_SLIST(node2, c);
		CAG_PREPEND_SLIST(node1, node2);
	}
	CAG_TEST(*tests, node1 &&
		 node1->value.real == 4 &&
		 node1->next->value.real == 3 &&
		 node1->next->next->value.real == 2 &&
		 node1->next->next->next->value.real == 1 &&
		 node1->next->next->next->next->value.real == 0 &&
		 node1->next->next->next->next->next == NULL,
		 "cag_slist: insert after node");
	CAG_ERASE_ALL_SLIST(node1, CAG_NO_FREE_FUNC);
	node1 = NULL;
	for (i = 0; i < 5; ++i) {
		c.real = i;
		CAG_MAKE_SLIST(node2, c);
		CAG_PREPEND_SLIST(node1, node2);
	}
	CAG_REVERSE_SLIST(it_complex, node1);
	for (i = 0, node2 = node1; node2 != NULL; node2 = node2->next, ++i)
		if (node2->value.real != i)
			failure = 1;
	CAG_TEST(*tests, failure == 0,
		 "cag_slist: reverse node");
	CAG_STABLE_SORT_SLIST_DEFAULT(it_complex, node1, cmp_complex);
	c.real = 0;
	c.imag = 205;
	for (i = 0, node2 = node1; node2 != NULL; ++i, node2 = node2->next) {
		if (node2->value.real < c.real ||
		    (node2->value.real == c.real &&
		     node2->value.imag > c.imag)) {
			failure = 1;
		}
		c.real = node2->value.real;
		c.imag = node2->value.imag;
	}
	CAG_TEST(*tests, failure == 0 || i < 5,
		 "cag_slist: stable sort node");
	CAG_ERASE_ALL_SLIST(node1, CAG_NO_FREE_FUNC);

	intlist = NULL;
	for (i = 0; i < 5; ++i) {
		CAG_MAKE_SLIST(it, 5 - i);
		CAG_PREPEND_SLIST(intlist, it);
	}
	CAG_ERASE_ALL_SLIST(intlist, CAG_NO_FREE_FUNC);
}

static void test_new(struct cag_test_series *tests)
{
	complex_slist l;

	CAG_TEST(*tests, new_complex_slist(&l) != NULL,
		 "cag_slist: list returned from new");
	CAG_TEST(*tests, l.header == NULL,
		 "cag_slist: header is NULL");
	free_complex_slist(&l);
}


static void test_prepend(struct cag_test_series *tests)

{
	int i;
	struct complex c;
	complex_slist l1;
	it_complex_slist it;

	new_complex_slist(&l1);
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		prepend_complex_slist(&l1, c);
	}
	i = 5;
	for(it = beg_complex_slist(&l1); it != end_complex_slist(&l1);
	    it = it->next) {
		--i;
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: prepend value");
	}
	i = 0;
	free_complex_slist(&l1);

	new_complex_slist(&l1);
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		prependp_complex_slist(&l1, &c);
	}
	i = 5;
	for(it = beg_complex_slist(&l1); it != end_complex_slist(&l1);
	    it = it->next) {
		--i;
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_dlist: prependp value");
	}
	i = 0;
	free_complex_slist(&l1);
}

static void test_front(struct cag_test_series *tests)
{
	complex_slist l;
	struct complex c;

	new_complex_slist(&l);
	populate_list(&l, 0, 5, 1);
	c = *front_complex_slist(&l);
	CAG_TEST(*tests, c.real == 4.0 && c.imag == -4.0,
		 "cag_dlist: Retrieve front value after prepends");
	free_complex_slist(&l);
}

static void test_insert(struct cag_test_series *tests)
{
	struct complex c;
	complex_slist l;
	it_complex_slist it;
	int i, failure = 0;

	new_complex_slist(&l);
	c.real = 1;
	c.imag = -1;
	it = prependp_complex_slist(&l, &c);
	c.real = 2;
	c.imag = -2;
	it = insert_after_complex_slist(&l, beg_complex_slist(&l), c);
	CAG_TEST(*tests, it->value.real == 2 && it->value.imag == -2,
		 "cag_slist: insert after first element");
	c = *front_complex_slist(&l);
	CAG_TEST(*tests, c.real == 1 && c.imag == -1,
		 "cag_slist: front after insertion");
	c.real = 3; c.imag = -3;
	it = insert_after_complex_slist(&l, it, c);
	CAG_TEST(*tests, it->value.real == 3 && it->value.imag == -3,
		 "cag_slist: insert after second element");
	free_complex_slist(&l);
	new_complex_slist(&l);
	it = beg_complex_slist(&l);
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		it = insert_after_complex_slist(&l, it, c);
	}
	for (i = 0, it = beg_complex_slist(&l); i < 5; it = it->next, ++i) {
		if (it->value.real != i)
			failure = 1;
	}
	CAG_TEST(*tests, failure == 0,
		 "cag_slist: insert list of several elements");
	free_complex_slist(&l);

}

static void test_insertp(struct cag_test_series *tests)
{
	struct complex c;
	complex_slist l;
	it_complex_slist it;

	new_complex_slist(&l);
	c.real = 1;
	c.imag = -1;
	it = prependp_complex_slist(&l, &c);
	c.real = 2;
	c.imag = -2;
	it = insertp_after_complex_slist(&l, beg_complex_slist(&l), &c);
	CAG_TEST(*tests, it->value.real == 2 && it->value.imag == -2,
		 "cag_slist: insertp after first element");
	c = *front_complex_slist(&l);
	CAG_TEST(*tests, c.real == 1 && c.imag == -1,
		 "cag_slist: front after insertp");
	c.real = 3; c.imag = -3;
	it = insertp_after_complex_slist(&l, it, &c);
	CAG_TEST(*tests, it->value.real == 3 && it->value.imag == -3,
		 "cag_slist: insertp after second element");
	free_complex_slist(&l);
}

static void test_distance(struct cag_test_series *tests)
{
	complex_slist l;

	new_complex_slist(&l);
	populate_list(&l, 0, 5, 1);
	CAG_TEST(*tests, distance_complex_slist(beg_complex_slist(&l),
					        end_complex_slist(&l)) == 5,
		 "cag_slist: distance after populate");
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 5,
		 "cag_slist: distance after populate");
	free_complex_slist(&l);
}

static void test_erase(struct cag_test_series *tests)
{
	complex_slist l;
	it_complex_slist it;

	new_complex_slist(&l);
	erase_front_complex_slist(&l);
	erase_range_complex_slist(&l, beg_complex_slist(&l),
				  end_complex_slist(&l));
	erase_all_complex_slist(&l);
	populate_list(&l, 0, 6, 1);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 6,
		 "cag_slist: distance after erase then populate");
	it = last_complex_slist(&l);
	CAG_TEST(*tests, it->value.real == 0.0,
		 "cag_slist: last after erase then populate");
	erase_after_complex_slist(beg_complex_slist(&l));
	CAG_TEST(*tests, beg_complex_slist(&l)->next->value.real == 3.0,
		 "cag_slist: value after single erase after");
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 5,
		 "cag_slist: distance after single erase after");
	erase_after_range_complex_slist(beg_complex_slist(&l), it);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 2,
		 "cag_slist: distance after erase after range");
	erase_after_range_complex_slist(beg_complex_slist(&l), NULL);
	erase_front_complex_slist(&l);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 0,
		 "cag_slist: distance after erase all range and erase front");
	populate_list(&l, 0, 6, 1);
	erase_front_complex_slist(&l);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 5,
		 "cag_slist: distance after erase after range");
	erase_front_complex_slist(&l);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 4,
		 "cag_slist: distance after erase after range");
	erase_all_complex_slist(&l);
	CAG_TEST(*tests, distance_all_complex_slist(&l) == 0,
		 "cag_slist: distance after erase after range");
	free_complex_slist(&l);
}

static void test_copy(struct cag_test_series *tests)
{
	complex_slist l1, l2;
	new_complex_slist(&l1);
	new_complex_slist(&l2);
	populate_list(&l1, 0, 5, 1);
	copy_all_complex_slist(&l1, &l2);
	CAG_TEST(*tests, cmp_all_complex_slist(&l1, &l2) == 0,
		 "cag_test: assigned lists equals");
	CAG_TEST(*tests, distance_all_complex_slist(&l2) ==
		 distance_all_complex_slist(&l1),
		 "cag_test: assigned lists have same distance");
	free_complex_slist(&l2);
	free_complex_slist(&l1);
}

static void test_reverse(struct cag_test_series *tests)
{
	complex_slist l;
	it_complex_slist node1, node2;
	int failure = 0;
	int i;

	new_complex_slist(&l);
	populate_list(&l, 0, 5, 1);
	reverse_all_complex_slist(&l);
	node1 = beg_complex_slist(&l);
	for (i = 0, node2 = node1; node2 != NULL; node2 = node2->next, ++i)
		if (node2->value.real != i)
			failure = 1;
	CAG_TEST(*tests, failure == 0,
		 "cag_slist: reverse all");
	free_complex_slist(&l);
}

static void test_sort(struct cag_test_series *tests)
{
	complex_slist l;
	it_complex_slist it;
	struct complex c;
	int failure = 0;
	int i;

	new_complex_slist(&l);
	for (i = 0; i < 203; ++i) {
		c.real = rand() %  203;
		c.imag = i;
		prepend_complex_slist(&l, c);
	}
	stable_sort_all_complex_slist(&l);
	c.real = 0;
	c.imag = 205;
	for (i = 0, it = beg_complex_slist(&l); it; ++i, it = it->next) {
		if (it->value.real < c.real ||
		    (it->value.real == c.real && it->value.imag > c.imag)) {
			failure = 1;
		}
		c.real = it->value.real;
		c.imag = it->value.imag;
	}
	CAG_TEST(*tests, failure == 0 || i < 203,
		 "cag_slist: stable sort random list");
	reverse_all_complex_slist(&l);
	for (i = 0, it = beg_complex_slist(&l); it; ++i, it = it->next) {
		if (it->value.real > c.real ||
		    (it->value.real == c.real && it->value.imag < c.imag)) {
			failure = 1;
			printf("D0: %.0f %.0f %.0f %.0f\n",
			       it->value.real, it->value.imag,
			       c.real, c.imag);
		}
		c.real = it->value.real;
		c.imag = it->value.imag;
	}
	CAG_TEST(*tests, failure == 0 || i < 203,
		 "cag_slist: reverse after stable sort");
	stable_sort_all_complex_slist(&l);
	c.real = 0;
	c.imag = 0;
	for (i = 0, it = beg_complex_slist(&l); it; ++i, it = it->next) {
		if (it->value.real < c.real ||
		    (it->value.real == c.real && it->value.imag < c.imag)) {
			failure = 1;
			printf("D0: %.0f %.0f %.0f %.0f\n",
			       it->value.real, it->value.imag,
			       c.real, c.imag);
		}
		c.real = it->value.real;
		c.imag = it->value.imag;
	}
	CAG_TEST(*tests, failure == 0 || i < 203,
		 "cag_slist: stable sort reversed list");
	free_complex_slist(&l);
}

static void test_copy_over(struct cag_test_series *tests)
{
	complex_slist l1, l2;
	new_complex_slist(&l1);
	new_complex_slist(&l2);
	populate_list(&l1, 0, 5, 1);
	populate_list(&l2, 5, 0, -1);
	CAG_TEST(*tests, cmp_all_complex_slist(&l1, &l2) != 0,
		 "cag_test: differently populated lists not equal");
	copy_over_complex_slist(beg_complex_slist(&l1), end_complex_slist(&l1),
				beg_complex_slist(&l2));
	CAG_TEST(*tests, cmp_all_complex_slist(&l1, &l2) == 0,
		 "cag_test: copied over list equals copied from");
	CAG_TEST(*tests, distance_all_complex_slist(&l2) ==
		 distance_all_complex_slist(&l1),
		 "cag_test: copied over list has same distance");
	free_complex_slist(&l2);
	free_complex_slist(&l1);
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
	complex_slist l;
	it_complex_slist it;
	struct complex c;

	c.imag = -9;

	new_complex_slist(&l);
	populate_list(&l, 0, 10, 1);
	it = find_all_complex_slist(&l,c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_slist: find element");
	c.imag = -10;
	it = find_all_complex_slist(&l, c, find_element);
	CAG_TEST(*tests, it == end_complex_slist(&l),
		 "cag_slist: find element");
	c.imag = -9.0;
	it = findp_all_complex_slist(&l, &c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_slist: find element");
	c.imag = -10;
	it = findp_all_complex_slist(&l, &c, find_element);
	CAG_TEST(*tests, it == end_complex_slist(&l),
		 "cag_slist: find element");
	c.imag = -9.0;
	free_complex_slist(&l);
}


void test_slist(struct cag_test_series *tests)
{
	test_it(tests);
	test_new(tests);
	test_prepend(tests);
	test_insert(tests);
	test_insertp(tests);
	test_distance(tests);
	test_front(tests);
	test_erase(tests);
	test_copy(tests);
	test_reverse(tests);
	test_copy_over(tests);
	test_sort(tests);
	test_find(tests);
}

CAG_DEF_CMP_SLIST(complex_slist, struct complex, cmp_complex);
