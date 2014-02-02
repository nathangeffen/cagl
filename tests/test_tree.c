/*! Tests for CAGL tree.

  \copyright Copyright 2013 Nathan Geffen. All rights reserved.

  \license GNU Lesser General Public License Copyright.

  \sa COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAG_SAFER 1
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/tree.h"
#include "cagl/array.h"

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


static int cmpp_complex(const struct complex *x, const struct complex *y)
{
	double z = x->real - y->real;
	if (z < 0) return -1;
	if (z > 0) return 1;
	return 0;
}

CAG_DEC_CMP_TREE(complex_tree, struct complex);
CAG_DEC_CMP_TREE(complexp_tree, struct complex);
CAG_DEC_CMP_TREE(string_tree, char *);
CAG_DEC_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_ARRAY(int_arr, int);


static void populate_tree(complex_tree *t,
			  const int from, const int to, const int step)
{
	int i;
	struct complex c;

	if (from < to)
		for (i = from; i < to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			insert_complex_tree(t, c);
		}
	else
		for (i = from; i > to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			insert_complex_tree(t, c);
		}
}


static void test_new(struct cag_test_series *tests)
{
	complex_tree ct1, ct2, ct3;
	complexp_tree cpt;
	string_tree st;
	int i;

	CAG_TEST(*tests, new_complex_tree(&ct1),
		 "cag_tree: new complex tree");
	free_complex_tree(&ct1);

	CAG_TEST(*tests, new_complexp_tree(&cpt),
		 "cag_tree: new complex pointer tree");
	free_complexp_tree(&cpt);

	CAG_TEST(*tests, new_string_tree(&st),
		 "cag_tree: new string tree");
	free_string_tree(&st);

	CAG_CHECK( (i = new_many_complex_tree(&ct1, &ct2, &ct3, NULL)) == 3,
		   "failure many new");
	CAG_TEST(*tests, i == 3,
		 "cag_tree: multiple new");
	populate_tree(&ct1, 0, 5, 1);
	CAG_TEST(*tests, distance_all_complex_tree(&ct1) == 5,
		 "cag_test: tree populated after batch new 1");
	populate_tree(&ct2, 0, 5, 1);
	CAG_TEST(*tests, distance_all_complex_tree(&ct2) == 5,
		 "cag_test: tree populated after batch new 2");
	populate_tree(&ct3, 0, 5, 1);
	CAG_TEST(*tests, distance_all_complex_tree(&ct3) == 5,
		 "cag_test: tree populated after batch new 3");
error:
	free_many_complex_tree(i, &ct1, &ct2, &ct3, NULL);
}

static void test_insert(struct cag_test_series *tests)
{
	struct complex c;
	complex_tree t;
	it_complex_tree it;

	new_complex_tree(&t);
	c.real = 3.14;
	c.imag = c.real * c.real;
	it = insert_complex_tree(&t, c);
	CAG_TEST(*tests, it->value.real == 3.14 && it->value.imag == 9.8596,
		 "cag_tree: insert");
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after first insert");
	c = begin_complex_tree(&t)->value;
	CAG_TEST(*tests, c.real == 3.14 && c.imag == 9.8596,
		 "cag_tree: begin after insertion");
	c.real = 100.1; c.imag = -100.1;
	it = insert_complex_tree(&t, c);
	CAG_TEST(*tests, it->value.real == 100.1 && it->value.imag == -100.1,
		 "cag_tree: insert second element");
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after second insertion");
	c.real = -16.0; c.imag = -16.0;
	c = insertp_complex_tree(&t, &c)->value;
	CAG_TEST(*tests, c.real == -16.0 && c.imag == -16.0,
		 "cag_tree: value after third insert");
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after third insertion");
	c.real = 32;
	it = insertp_complex_tree(&t, &c);
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after fourth insertion");
	free_complex_tree(&t);
}


static void sprintf_complex_tree(it_complex_tree it,  void *data)
{
	char *s = data;

	sprintf(s + strlen(s), "%.0f ", it->value.real);
}

static void sprintf_complex_tree2(it_complex_tree it, size_t level,
				  int level_changed, int parent_changed,
				  void *data)
{
	char *s = data;
	assert(level < 10000); /* Avoid unused parm compiler warning. */
	if (level_changed)
		sprintf(s + strlen(s), "\n");
	else if (parent_changed)
		sprintf(s + strlen(s), "\t");

	sprintf(s + strlen(s), "%.0f-%c(%.0f) ", it->value.real,
		it->red ? 'R' : 'B',
		it->parent ? it->parent->value.real : 0);
}


static void test_preorder(struct cag_test_series *tests)
{
	char c[1000];
	complex_tree t;

	new_complex_tree(&t);
	strcpy(c, "\0");
	populate_tree(&t, 0, 15, 1);
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after populate tree");
	CAG_TEST(*tests, distance_complex_tree(begin_complex_tree(&t),
					       end_complex_tree(&t)) == 15,
		 "cag_tree: distance after populate tree");
	preorder_complex_tree(t.root, c, sprintf_complex_tree);
	CAG_TEST(*tests, strcmp(c, "3 1 0 2 7 5 4 6 9 8 11 10 13 12 14 ") == 0,
		 "cag_tree: inorder");
	free_complex_tree(&t);
}


static void test_inorder(struct cag_test_series *tests)
{
	char c[100];
	complex_tree t;

	new_complex_tree(&t);
	strcpy(c, "\0");
	populate_tree(&t, 0, 15, 1);
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after populate tree");
	CAG_TEST(*tests, distance_complex_tree(begin_complex_tree(&t),
					       end_complex_tree(&t)) == 15,
		 "cag_tree: distance after populate tree");
	inorder_complex_tree(t.root, c, sprintf_complex_tree);
	CAG_TEST(*tests, strcmp(c, "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 ") == 0,
		 "cag_tree: inorder");
	free_complex_tree(&t);
}

static void test_postorder(struct cag_test_series *tests)
{
	char c[100];
	complex_tree t;

	new_complex_tree(&t);
	strcpy(c, "\0");
	populate_tree(&t, 0, 15, 1);
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after populate tree");
	CAG_TEST(*tests, distance_complex_tree(begin_complex_tree(&t),
					       end_complex_tree(&t)) == 15,
		 "cag_tree: distance after populate tree");
	postorder_complex_tree(t.root, c, sprintf_complex_tree);
	CAG_TEST(*tests, strcmp(c, "0 2 1 4 6 5 8 10 12 14 13 11 9 7 3 ") == 0,
		 "cag_tree: inorder");
	free_complex_tree(&t);
}


static void test_levelorder(struct cag_test_series *tests)
{
	char c[1000];
	complex_tree t;
	const char *exp = "3-B(0) \n"
		"1-B(3) 7-R(3) \n"
		"0-B(1) 2-B(1) \t5-B(7) 9-B(7) \n"
		"4-B(5) 6-B(5) \t8-B(9) 11-R(9) \n"
		"10-B(11) 13-B(11) \n"
		"12-R(13) 14-R(13) ";

	new_complex_tree(&t);
	strcpy(c, "\0");
	populate_tree(&t, 0, 15, 1);
	CAG_TEST(*tests, check_integrity_complex_tree(&t, t.root),
		 "cag_tree: integrity after populate tree");
	CAG_TEST(*tests, distance_complex_tree(begin_complex_tree(&t),
					       end_complex_tree(&t)) == 15,
		 "cag_tree: distance after populate tree");
	levelorder_complex_tree(&t, c, sprintf_complex_tree2);


	CAG_TEST(*tests, strcmp(c, exp) == 0,
		 "cag_tree: inorder");
	free_complex_tree(&t);
}

static void test_copy(struct cag_test_series *tests)
{
	complex_tree a, b, c;
	it_complex_tree it1, it2;
	new_complex_tree(&a);
	new_complex_tree(&b);
	new_complex_tree(&c);
	populate_tree(&a, 0, 5, 1);
	CAG_CHECK(copy_all_complex_tree(&a, &b), "Assignment failure");
	for (it1 = beg_complex_tree(&a), it2 = begin_complex_tree(&b);
	     it1 != end_complex_tree(&a);
	     it1 = next_complex_tree(it1), it2 = next_complex_tree(it2))
		CAG_TEST(*tests, it1->value.real == it2->value.real &&
			 it1->value.imag == it2->value.imag,
			 "cag_tree: assigned values are equal");
	CAG_TEST(*tests, distance_all_complex_tree(&a) ==
		 distance_all_complex_tree(&b),
		"cag_array: assigned array same size");
	CAG_CHECK(rcopy_all_complex_tree(&a, &c), "Assignment failure");
	for (it1 = beg_complex_tree(&a), it2 = begin_complex_tree(&c);
	     it1 != end_complex_tree(&a);
	     it1 = next_complex_tree(it1), it2 = next_complex_tree(it2))
		CAG_TEST(*tests, it1->value.real == it2->value.real &&
			 it1->value.imag == it2->value.imag,
			 "cag_tree: assigned values are equal");
	CAG_TEST(*tests, distance_all_complex_tree(&a) ==
		 distance_all_complex_tree(&c),
		"cag_array: reverse assigned array same size");
error:
	free_complex_tree(&a);
	free_complex_tree(&b);
	free_complex_tree(&c);
}

static void test_at(struct cag_test_series *tests)
{
	complex_tree t;
	it_complex_tree it;

	new_complex_tree(&t);
	populate_tree(&t, 0, 10, 1);
	it = at_complex_tree(beg_complex_tree(&t), 0);
	CAG_TEST(*tests, it == beg_complex_tree(&t),
		"cag_tree: at 0 index");
	it = at_complex_tree(it, 2);
	CAG_TEST(*tests, it->value.real == 2.0,
		 "cag_tree: at 2 index");
	it = at_complex_tree(it, 7);
	CAG_TEST(*tests, it->value.real == 9.0,
		 "cag_tree: at 7 index");
	it = at_complex_tree(it, 1);
	CAG_TEST(*tests, it == end_complex_tree(&t),
		 "cag_tree: at end");
	free_complex_tree(&t);
}

static void test_front_back(struct cag_test_series *tests)
{
	complex_tree t;
	struct complex c;

	new_complex_tree(&t);
	populate_tree(&t, 1, 5, 1);
	c = *front_complex_tree(&t);
	CAG_TEST(*tests, c.real == 1.0 && c.imag == -1.0,
		 "cag_tree: Retrieve front value");
	c = *back_complex_tree(&t);
	CAG_TEST(*tests, c.real == 4.0 && c.imag == -4.0,
		 "cag_tree: Retrieve back value");
	free_complex_tree(&t);
}

static void test_search(struct cag_test_series *tests)
{
	complex_tree t;
	it_complex_tree it;
	struct complex c, d, e, f;

	c.real = 3.0;
	c.imag = -3.0;
	d.real = -1;
	d.imag = 1.0;
	e.real = 2.5;
	e.imag = 2.5;
	f.real = 8.0;
	f.imag = -8.0;
	new_complex_tree(&t);
	populate_tree(&t, 0, 5, 1);
	populate_tree(&t, 0, 5, 1);

	it = get_complex_tree(&t, c);
	CAG_TEST(*tests, it && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_tree: Value found");
	it = get_complex_tree(&t, d);
	CAG_TEST(*tests, !it, "cag_tree: Value not found < all");
	it = get_complex_tree(&t, e);
	CAG_TEST(*tests, !it, "cag_tree: Value not found in between");
	it = get_complex_tree(&t, f);
	CAG_TEST(*tests, !it, "cag_tree: Value not found > all");

	it = getp_complex_tree(&t, &c);
	CAG_TEST(*tests, it && it->value.real == 3.0
		 && it->value.imag == -3.0,
		 "cag_tree: Value found");
	it = getp_complex_tree(&t, &d);
	CAG_TEST(*tests, !it, "cag_tree: Value not found < all");
	it = getp_complex_tree(&t, &e);
	CAG_TEST(*tests, !it, "cag_tree: Value not found in between");
	it = getp_complex_tree(&t, &f);
	CAG_TEST(*tests, !it, "cag_tree: Value not found > all");
	free_complex_tree(&t);
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
	complex_tree l;
	it_complex_tree it;
	struct complex c;

	c.imag = -9;

	new_complex_tree(&l);
	populate_tree(&l, 0, 10, 1);
	it = find_all_complex_tree(&l,c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_tree: find element");
	c.imag = -10;
	it = find_all_complex_tree(&l, c, find_element);
	CAG_TEST(*tests, it == end_complex_tree(&l),
		 "cag_tree: find element");
	c.imag = -9.0;
	it = findp_all_complex_tree(&l, &c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_tree: find element");
	c.imag = -10;
	it = findp_all_complex_tree(&l, &c, find_element);
	CAG_TEST(*tests, it == end_complex_tree(&l),
		 "cag_tree: find element");
	c.imag = -9.0;
	free_complex_tree(&l);
}


#define NUM_ELEMS 200

static void test_rb_insert_erase(struct cag_test_series *tests)
{
	int_tree tree;
	int_arr arr;
	int i;
	it_int_arr intit;
	int failure = 0;

	new_int_tree(&tree);
	insert_int_tree(&tree, 10);
	remove_int_tree(&tree, 10);
	free_int_tree(&tree);

	new_int_tree(&tree);
	insert_int_tree(&tree, 10);
	remove_int_tree(&tree, 5);
	remove_int_tree(&tree, 10);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 0,
		 "cag_tree: distance in rb insert/erase tests");
	insert_int_tree(&tree, 10);
	insert_int_tree(&tree, 5);
	remove_int_tree(&tree, 10);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 1,
		 "cag_tree: distance in rb insert/erase tests");
	free_int_tree(&tree);

	new_int_tree(&tree);
	insert_int_tree(&tree, 10);
	insert_int_tree(&tree, 5);
	remove_int_tree(&tree, 5);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 1,
		 "cag_tree: distance in rb insert/erase tests");
	remove_int_tree(&tree, 10);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 0,
		 "cag_tree: distance in rb insert/erase tests");
	insert_int_tree(&tree, 10);
	insert_int_tree(&tree, 15);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 2,
		 "cag_tree: distance in rb insert/erase tests");
	remove_int_tree(&tree, 10);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 1,
		 "cag_tree: distance in rb insert/erase tests");
	insert_int_tree(&tree, 20);
	remove_int_tree(&tree, 20);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 1,
		 "cag_tree: distance in rb insert/erase tests");
	insert_int_tree(&tree, 10);
	insert_int_tree(&tree, 20);
	remove_int_tree(&tree, 20);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	insert_int_tree(&tree, 20);
	remove_int_tree(&tree, 15);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	insert_int_tree(&tree, 15);
	insert_int_tree(&tree, 17);
	insert_int_tree(&tree, 13);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	remove_int_tree(&tree, 17);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	remove_int_tree(&tree, 10);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	insert_int_tree(&tree, 6);
	insert_int_tree(&tree, 15);
	insert_int_tree(&tree, 17);
	remove_int_tree(&tree, 15);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	free_int_tree(&tree);

	new_int_tree(&tree);
	insert_int_tree(&tree, 3);
	insert_int_tree(&tree, 1);
	insert_int_tree(&tree, 8);
	insert_int_tree(&tree, 0);
	insert_int_tree(&tree, 2);
	insert_int_tree(&tree, 6);
	insert_int_tree(&tree, 9);
	insert_int_tree(&tree, 4);
	insert_int_tree(&tree, 7);
	insert_int_tree(&tree, 5);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 10,
		 "cag_tree: distance in rb insert/erase tests");
	remove_int_tree(&tree, 3);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 9,
		 "cag_tree: distance in rb insert/erase tests");
	free_int_tree(&tree);

	new_int_tree(&tree);
	insert_int_tree(&tree, 3);
	insert_int_tree(&tree, 1);
	insert_int_tree(&tree, 8);
	insert_int_tree(&tree, 0);
	insert_int_tree(&tree, 2);
	insert_int_tree(&tree, 6);
	insert_int_tree(&tree, 9);
	insert_int_tree(&tree, 4);
	insert_int_tree(&tree, 7);
	insert_int_tree(&tree, 5);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 10,
		 "cag_tree: distance in rb insert/erase tests");
	remove_int_tree(&tree, 9);
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 9,
		 "cag_tree: distance in rb insert/erase tests");
	free_int_tree(&tree);

	new_int_tree(&tree);
	new_int_arr(&arr);

	for (i = 0; i < NUM_ELEMS; ++i)
		append_int_arr(&arr, i);
	random_shuffle_all_int_arr(&arr);
	CAG_FOR_ALL(int_arr, &arr, intit,
		    {
			    insert_int_tree(&tree, intit->value);
		    });
	CAG_TEST(*tests, check_integrity_int_tree(&tree, tree.root),
		 "cag_tree: integrity in rb insert/erase tests");
	CAG_TEST(*tests, distance_all_int_tree(&tree) == NUM_ELEMS,
		 "cag_tree: distance in rb insert/erase tests");
	random_shuffle_all_int_arr(&arr);
	CAG_FOR_ALL(int_arr, &arr, intit,
		    {
			    if (remove_int_tree(&tree, intit->value) == NULL)
				    ++failure;
			    if (check_integrity_int_tree(&tree, tree.root) == 0)
				    ++failure;
			    if (failure) break;
		    });
	CAG_TEST(*tests, failure == 0,
		 "cag_tree: integrity in rb insert/erase tests");
	free_int_arr(&arr);
	free_int_tree(&tree);

	new_int_tree(&tree);
	for (i = 0; i < NUM_ELEMS; ++i)
		insert_int_tree(&tree, i);
	CAG_TEST(*tests, distance_all_int_tree(&tree) == NUM_ELEMS,
		 "cag_tree: distance in rb insert/erase tests");
	erase_all_int_tree(&tree);
	CAG_TEST(*tests, distance_all_int_tree(&tree) == 0,
		 "cag_tree: distance in rb insert/erase tests");
		 free_int_tree(&tree);
}

void test_tree(struct cag_test_series *tests)
{
	test_new(tests);
	test_insert(tests);
	test_preorder(tests);
	test_inorder(tests);
	test_postorder(tests);
	test_levelorder(tests);
	test_copy(tests);
	test_at(tests);
	test_search(tests);
	test_find(tests);
	test_front_back(tests);
	test_rb_insert_erase(tests);
}

CAG_DEF_CMP_TREE(complex_tree, struct complex, cmp_complex);

CAG_DEF_ALL_CMP_TREE(complexp_tree, struct complex, cmpp_complex, CAG_BYADR,
		     CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,  CAG_NO_FREE_FUNC);

CAG_DEF_ALL_CMP_TREE(string_tree, char *, strcmp, CAG_BYVAL,
		     CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free);
