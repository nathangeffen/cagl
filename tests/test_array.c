/*! Tests for CAGL arrays.

  \copyright Copyright 2013 University of Cape Town. All rights reserved.
  \license GNU Lesser General Public License Copyright.
  See COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAG_SAFER 1
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/array.h"
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


static int cmpp_complex(const struct complex *x, const struct complex *y)
{
	double z = x->real - y->real;
	if (z < 0) return -1;
	if (z > 0) return 1;
	return 0;
}



CAG_DEC_CMP_ARRAY(complex_array, struct complex);
CAG_DEC_CMP_ARRAY(complexp_array, struct complex);
CAG_DEC_CMP_ARRAY(string_array, char *);

static void test_default_sort(struct cag_test_series *tests)
{
	const int size = 100;
	long double *arr = malloc(size * sizeof(*arr));
	int i, inorder = CAG_TRUE;

	for (i = 0; i < size; ++i) arr[i] = size - i;
	CAG_SORT_ARRAY_DEFAULT(long double, arr, arr+size, -CAG_CMP_DEFAULT);
	for (i = 1; i < size; ++i)
		if (arr[i] != arr[i-1] - 1) {
			inorder = CAG_FALSE;
			break;
		}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "Macro sort backward array in descending order.");
	CAG_SORT_ARRAY_DEFAULT(long double, arr, arr+size, CAG_CMP_DEFAULT);
	inorder = CAG_TRUE;
	for (i = 1; i < size; ++i)
		if (arr[i] != arr[i-1] + 1) {
			inorder = CAG_FALSE;
			break;
		}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "Macro sort backward array in ascending order.");
	for (i = 0; i < size; ++i) arr[i] = rand() % size;
	CAG_SORT_ARRAY_DEFAULT(long double, arr, arr+size, CAG_CMP_DEFAULT);
	inorder = CAG_TRUE;
	for (i = 1; i < size; ++i)
		if (arr[i] < arr[i-1]) {
			inorder = CAG_FALSE;
			break;
		}
	CAG_TEST(*tests, inorder == CAG_TRUE,
		 "Macro sort random array in ascending order.");
	free(arr);
}

static void test_new(struct cag_test_series *tests)
{
	complex_array a;

	CAG_TEST(*tests, new_with_capacity_complex_array(&a, 0),
		 "cag_array: new array") ;
	free_complex_array(&a);
}

static void test_append(struct cag_test_series *tests)
{
	complex_array a;
	it_complex_array it;
	rit_complex_array rit;
	struct complex c;
	int i;
	size_t s;

	CAG_CHECK(new_with_capacity_complex_array(&a, 2),
		  "No memory for array");
	for (i = 0; i < 5; ++i) {
		c.real = i;
		c.imag = -i;
		it = append_complex_array(&a, c);
		CAG_TEST(*tests, it == a.objects + i,
			 "cag_array: iterator points to new object after append");
		CAG_TEST(*tests, it->value.real  == i && it->value.imag == -i,
			 "cag_array: iterator has value after append");
	}
	i = 0;
	for(it = beg_complex_array(&a); it != end_complex_array(&a);
	    it = next_complex_array(it)) {
		CAG_TEST(*tests, it->value.real  == i && it->value.imag == -i,
			 "cag_array: iterate through appends");
		++i;
	}
	CAG_TEST(*tests, CAG_DISTANCE_ARRAY(beg_complex_array(&a),
					    end_complex_array(&a)) == 5,
		 "cag_array: distance after appends");
	CAG_TEST(*tests, CAG_SIZE_ARRAY(a) == 5,
                 "cag_array: size macro");
	CAG_TEST(*tests, size_complex_array(&a) == 5,
                 "cag_array: size function");
	i = 4;
	for(rit = rbeg_complex_array(&a); rit != rend_complex_array(&a);
	    rit = rnext_complex_array(rit)) {
		CAG_TEST(*tests, rit->value.real  == i && rit->value.imag == -i,
			 "cag_array: reverse iterate through appends");
		--i;
	}
	c.real = 6.0;
	c.imag = -6.0;
	rit = rprepend_complex_array(&a, c);
	CAG_TEST(*tests,
		 rdistance_complex_array(rit, rend_complex_array(&a)) == 6,
		 "cag_array: distance after reverse prepend");
	CAG_TEST(*tests, rit->value.real  == 6 && rit->value.imag == -6,
		 "cag_array: reverse iterator has value after prepend");
	c.real = 7.0;
	c.imag = -7.0;
	it = appendp_complex_array(&a, &c);
	CAG_TEST(*tests,
		 distance_complex_array(beg_complex_array(&a),
					end_complex_array(&a)) == 7,
		 "cag_array: distance after pointer append");
	CAG_TEST(*tests,
		 distance_all_complex_array(&a) == 7,
		 "cag_array: distance all after pointer append");

	CAG_TEST(*tests, it->value.real  == 7 && it->value.imag == -7,
		 "cag_array: iterator has value after pointer append");
	c.real = 8.0;
	c.imag = -8.0;
	rit = rprependp_complex_array(&a, &c);
	CAG_TEST(*tests,
		 distance_complex_array(beg_complex_array(&a),
					end_complex_array(&a)) == 8,
		 "cag_array: distance after pointer prepend");
	CAG_TEST(*tests, rit->value.real  == 8 && rit->value.imag == -8,
		 "cag_array: rev iterator has value after rev pointer prepend");
	s = a.capacity;
	for (i = 0; i < (signed) s; ++i) {
		c.real = i;
		c.imag = -i;
		it = append_complex_array(&a, c);
	}
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a))
		 > s && a.capacity == 2 * s,
		 "cag_array: realloc of space after multiple appends");
	free_complex_array(&a);
error:
	return;
}

static void populate_array(complex_array *a,
			   const int from, const int to, const int step)
{
	int i;
	struct complex c;
	if (from < to) {
		for (i = from; i < to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			append_complex_array(a, c);
		}
	} else {
		for (i = from; i > to; i += step) {
			c.real = (double) i;
			c.imag = (double) -i;
			append_complex_array(a, c);
		}
	}
}

static void test_front_back(struct cag_test_series *tests)
{
	complex_array a;
	struct complex c;

	new_complex_array(&a);
	populate_array(&a, 1, 6, 1);
	c = *front_complex_array(&a);
	CAG_TEST(*tests, c.real == 1 && c.imag == -1,
		 "cag_array: retrieve front");
	c = *back_complex_array(&a);
	CAG_TEST(*tests, c.real == 5 && c.imag == -5,
		 "cag_array: retrieve back");
	c = *rfront_complex_array(&a);
	CAG_TEST(*tests, c.real == 5 && c.imag == -5,
		 "cag_array: retrieve front");
	c = *rback_complex_array(&a);
	CAG_TEST(*tests, c.real == 1 && c.imag == -1,
		 "cag_array: retrieve back");
	free_complex_array(&a);
}

static void test_insert(struct cag_test_series *tests)
{
	complex_array a;
	struct complex c;
	it_complex_array it;
	int i;

	new_with_capacity_complex_array(&a, 2);
	it = beg_complex_array(&a);
	for (i = 0; i < 8; ++i) {
		c.real = i;
		c.imag = -i;
		it = insert_complex_array(&a, it, c);
	}
	i = 7;
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it, --i)
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_array: insert values");
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 8,
		"cag_array: distance after inserts");
	it = end_complex_array(&a);
	c.real = 9;
	c.imag = -9;
	it = insert_complex_array(&a, it, c);
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 9,
		"cag_array: distance after end insert");
	c.real = 8;
	c.imag = -8;
	it = insert_complex_array(&a, it, c);
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 10,
		"cag_array: distance after insert before last element");
	i = 8;
	for (; it != end_complex_array(&a); ++it, ++i)
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_array: insert values");
	free_complex_array(&a);

	new_complex_array(&a);
	it = beg_complex_array(&a);
	for (i = 0; i < 8; ++i) {
		c.real = i;
		c.imag = -i;
		it = insertp_complex_array(&a, it, &c);
	}
	i = 7;
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it, --i)
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_array: insert pointer values");
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 8,
		"cag_array: distance after pointer inserts");
	free_complex_array(&a);
}


static void test_insert_order(struct cag_test_series *tests)
{
	struct complex c = {3, -3};
	complex_array l;
	it_complex_array it;
	size_t d;
	double t = -20.0;
	int inorder = CAG_TRUE;

	new_complex_array(&l);
	c.real = 6;
	insert_gt_complex_array(&l, beg_complex_array(&l), c);
	c.real = 0;
	insert_gt_complex_array(&l, beg_complex_array(&l), c);
	c.real = 4;
	insert_gt_complex_array(&l, beg_complex_array(&l), c);
	c.real = 8;
	insert_gt_complex_array(&l, beg_complex_array(&l), c);
	c.real = 2;
	insert_gt_complex_array(&l, beg_complex_array(&l), c);
	for (it = beg_complex_array(&l); it != end_complex_array(&l);
	     ++it)
		if (it->value.real < t) {
			inorder = CAG_FALSE;
			break;
		} else
			t = it->value.real;
	d = distance_complex_array(beg_complex_array(&l), end_complex_array(&l));
	CAG_TEST(*tests,  d == 5 && inorder == CAG_TRUE,
		 "cag_array: insert ordered values");
	c.real = 3.0;
	it = insert_gt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == 2.0 &&
		 it->value.real == 3.0 &&
		 (it+1)->value.real == 4.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+1,
		 "cag_array: insert gt middle");
	c.real = 20.0;
	it = insert_gt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == 8.0 &&
		 it->value.real == 20.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+2,
		 "cag_array: insert gt end");
	c.real = -1.0;
	it = insert_gt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, it == beg_complex_array(&l) &&
		 (it+1)->value.real == 0.0 &&
		 it->value.real == -1.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+3,
		 "cag_array: insert gt beginning");
	c.real = -0.5;
	c.imag = 10.0;
	it = insert_gt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == -1.0 &&
		 it->value.real == -0.5 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+4,
		 "cag_array: insert gt one beyond beginning");
	c.imag = 5.0;
	it = insert_gt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == -1.0 &&
		 it->value.real == -0.5 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+5,
		 "cag_array: insert gt duplicate stable");
	it = insert_gteq_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == -0.5 &&
		 it->value.real == -0.5 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+6,
		 "cag_array: insert gt duplicate unstable");
	free_complex_array(&l);

	new_complex_array(&l);
	populate_array(&l, 10, 0, -2);
	d = distance_complex_array(beg_complex_array(&l), end_complex_array(&l));
	c.real = 3.0;
	c.imag = 1.0;
	it = insert_lt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it+1)->value.real == 2.0 &&
		 it->value.real == 3.0 &&
		 (it-1)->value.real == 4.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+1,
		 "cag_array: insert lt middle");
	c.imag = 2.0;
	it = insert_lt_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it+1)->value.real == 3.0 &&
		 it->value.real == 3.0 &&
		 (it-1)->value.real == 4.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+2,
		 "cag_array: insert lt duplicate stable");
	c.imag = 3.0;
	it = insert_lteq_complex_array(&l, beg_complex_array(&l), c);
	CAG_TEST(*tests, (it-1)->value.real == 3.0 &&
		 it->value.real == 3.0 &&
		 (it+1)->value.real == 2.0 &&
		 distance_complex_array(beg_complex_array(&l),
				       end_complex_array(&l)) == d+3,
		 "cag_array: insert lt duplicate unstable");
	free_complex_array(&l);
}


static void test_prepend(struct cag_test_series *tests)
{
	complex_array a;
	struct complex c;
	it_complex_array it;
	int i;

	new_complex_array(&a);
	for (i = 0; i < 8; ++i) {
		c.real = i;
		c.imag = -i;
		it = prepend_complex_array(&a, c);
	}
	i = 7;
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it, --i)
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_array: prepend values");
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 8,
		"cag_array: distance after prepends");
	free_complex_array(&a);

	new_complex_array(&a);
	it = beg_complex_array(&a);
	for (i = 0; i < 8; ++i) {
		c.real = i;
		c.imag = -i;
		it = prependp_complex_array(&a, &c);
	}
	i = 7;
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it, --i)
		CAG_TEST(*tests, it->value.real == i && it->value.imag == -i,
			 "cag_array: prepend pointer values");
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a),
						end_complex_array(&a)) == 8,
		"cag_array: distance after pointer prepends");
	free_complex_array(&a);
}

static void test_copy_over(struct cag_test_series *tests)
{
	complex_array a, b;
	it_complex_array it1, it2;

	CAG_CHECKL(new_with_capacity_complex_array(&a, 2),
		   "new failure",  error_a);
	populate_array(&a, 0, 5, 1);
	CAG_CHECKL(new_with_size_complex_array(&b, size_complex_array(&a)),
		   "new failure", error_b);
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&b),
		"cag_array: copied array same size");
	CAG_CHECK(copy_over_complex_array(beg_complex_array(&a),
					  end_complex_array(&a),
					  beg_complex_array(&b)),
		  "copy failure");
	for (it1 = beg_complex_array(&a), it2 = begin_complex_array(&b);
	     it1 != end_complex_array(&a); ++it1, ++it2)
		CAG_TEST(*tests, it1->value.real == it2->value.real &&
			 it1->value.imag == it2->value.imag,
			 "cag_array: copied values are equal");
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&b),
		"cag_array: copied array same size");
	free_complex_array(&a);
	free_complex_array(&b);

	CAG_CHECK(new_complex_array(&a), "new failure");
	populate_array(&a, 0, 5, 1);
	CAG_CHECK(new_with_capacity_complex_array(&b, 2),
		  "new with capacity");
	CAG_CHECK(set_exact_size_complex_array(&b, 5), "set exact failure");
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&b),
		"cag_array: copied array same size after set exact");
	copy_over_complex_array(beg_complex_array(&a),
				end_complex_array(&a),
				beg_complex_array(&b));
	for (it1 = beg_complex_array(&a), it2 = begin_complex_array(&b);
	     it1 != end_complex_array(&a); ++it1, ++it2)
		CAG_TEST(*tests, it1->value.real == it2->value.real &&
			 it1->value.imag == it2->value.imag,
			 "cag_array: copied values are equal after set exact");
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&b),
		"cag_array: copied array same size after set exact and copy");
error:
	free_complex_array(&b);
error_b:
	free_complex_array(&a);
error_a:
	return;
}



static void test_bsearch(struct cag_test_series *tests)
{
	complex_array l;
	struct complex c;
	it_complex_array it;
	int b;

	new_complex_array(&l);
	populate_array(&l, 0, 10, 1);
	c.real = 5.0;
	it = lower_bound_all_complex_array(&l, c);
	CAG_TEST(*tests, it->value.real == 5.0,
		 "cag_array: lower bound found");
	c.real = 5.5;
	it = lower_bound_all_complex_array(&l, c);
	CAG_TEST(*tests, it->value.real == 6.0,
		 "cag_array: lower bound found");
	c.real = -1.0;
	it = lower_boundp_all_complex_array(&l, &c);
	CAG_TEST(*tests, it->value.real == 0.0,
		 "cag_array: lower bound found");
	c.real = 10.0;
	it = lower_boundp_all_complex_array(&l, &c);
	CAG_TEST(*tests, it == end_complex_array(&l),
		 "cag_array: lower bound found");
	c.real = 9.0;
	it = lower_boundp_all_complex_array(&l, &c);
	CAG_TEST(*tests, it->value.real == 9.0,
		 "cag_array: lower bound found");

	b = binary_search_all_complex_array(&l, c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 2.0;
	b = binary_search_all_complex_array(&l, c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 0.0;
	b = binary_search_all_complex_array(&l, c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 11.0;
	b = binary_search_all_complex_array(&l, c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: binary search not found");
	b = binary_rsearch_complex_array(rbeg_complex_array(&l),
					 rend_complex_array(&l), c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: reverse binary search not found");
	c.real = 5.0;
	b = binary_searchp_all_complex_array(&l, &c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 9.0;
	b = binary_searchp_all_complex_array(&l, &c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 2.0;
	b = binary_searchp_all_complex_array(&l, &c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 0.0;
	b = binary_searchp_all_complex_array(&l, &c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 11.0;
	b = binary_searchp_all_complex_array(&l, &c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: binary search not found");
	b = binary_rsearchp_complex_array(rbeg_complex_array(&l),
					  rend_complex_array(&l), &c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: reverse binary search not found");
	free_complex_array(&l);
	new_complex_array(&l);
	c.real = 5.0;
	b = binary_rsearch_complex_array(rbeg_complex_array(&l),
					 rend_complex_array(&l), c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: reverse binary search on empty array");
	populate_array(&l, 10, 0, -1);
	c.real = 2.0;
	b = binary_rsearchp_complex_array(rbeg_complex_array(&l),
					  rend_complex_array(&l), &c);
	CAG_TEST(*tests, b == CAG_TRUE,
		 "cag_array: binary search found");
	c.real = 11.0;
	b = binary_rsearchp_complex_array(rbeg_complex_array(&l),
					  rend_complex_array(&l), &c);
	CAG_TEST(*tests, b == CAG_FALSE,
		 "cag_array: binary search found");
	free_complex_array(&l);
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
	complex_array l;
	it_complex_array it;
	struct complex c;

	c.imag = -9;

	new_complex_array(&l);
	populate_array(&l, 0, 10, 1);
	it = find_all_complex_array(&l,c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_array: find element");
	c.imag = -10;
	it = find_all_complex_array(&l, c, find_element);
	CAG_TEST(*tests, it == end_complex_array(&l),
		 "cag_array: find element");
	c.imag = -9.0;
	it = findp_all_complex_array(&l, &c, find_element);
	CAG_TEST(*tests, it && it->value.real == 9.0,
		 "cag_array: find element");
	c.imag = -10;
	it = findp_all_complex_array(&l, &c, find_element);
	CAG_TEST(*tests, it == end_complex_array(&l),
		 "cag_array: find element");
	c.imag = -9.0;
	free_complex_array(&l);
}


static void test_cmp(struct cag_test_series *tests)
{
	complex_array l1, l2;
	struct complex c;
	new_complex_array(&l1);
	new_complex_array(&l2);
	CAG_TEST(*tests, cmp_range_complex_array(beg_complex_array(&l1),
						 end_complex_array(&l1),
						 beg_complex_array(&l2),
						 end_complex_array(&l2)) == 0,
		 "cag_array: compare empty arrays");
	populate_array(&l1, 0, 5, 1);
	populate_array(&l2, 0, 5, 1);
	CAG_TEST(*tests, cmp_all_complex_array(&l1, &l2) == 0,
		 "cag_array: compare all equal arrays");
	CAG_TEST(*tests, cmp_range_complex_array(beg_complex_array(&l1),
						 end_complex_array(&l1),
						 beg_complex_array(&l2),
						 end_complex_array(&l2)) == 0,
		 "cag_array: compare equal arrays");
	c.real = 10;
	c.imag = -10;
	append_complex_array(&l1, c);
	CAG_TEST(*tests, cmp_all_complex_array(&l1, &l2) > 0,
		 "cag_array: compare all 1st array bigger");
	CAG_TEST(*tests, cmp_range_complex_array(beg_complex_array(&l1),
						 end_complex_array(&l1),
						 beg_complex_array(&l2),
						 end_complex_array(&l2)) > 0,
		 "cag_array: compare 1st array bigger");
	c.real = 20;
	c.imag = -10;
	append_complex_array(&l2, c);
	CAG_TEST(*tests, cmp_all_complex_array(&l1, &l2) < 0,
		 "cag_array: compare all 2nd array bigger");
	CAG_TEST(*tests, cmp_range_complex_array(beg_complex_array(&l1),
						 end_complex_array(&l1),
						 beg_complex_array(&l2),
						 end_complex_array(&l2)) < 0,
		 "cag_array: compare 2nd array bigger");
	free_complex_array(&l1);
	free_complex_array(&l2);
}

static void test_equal(struct cag_test_series *tests)
{
	complex_array l1, l2;
	new_complex_array(&l1);
	new_complex_array(&l2);
	CAG_TEST(*tests,
		 equal_range_complex_array(beg_complex_array(&l1),
					   end_complex_array(&l1),
					   beg_complex_array(&l2)) == CAG_TRUE,
		 "cag_array: equal empty arrays");
	populate_array(&l1, 0, 5, 1);
	populate_array(&l2, 0, 5, 1);
	CAG_TEST(*tests,
		 equal_range_complex_array(beg_complex_array(&l1),
					   end_complex_array(&l1),
					   beg_complex_array(&l2)) == CAG_TRUE,
		 "cag_array: equal arrays");
	CAG_TEST(*tests, equal_all_complex_array(&l1, &l2) == CAG_TRUE,
		 "cag_array: equal all arrays");
	at_complex_array(beg_complex_array(&l1), 3)->value.real = 10;
	at_complex_array(beg_complex_array(&l1), 3)->value.imag = 20;
	CAG_TEST(*tests,
		 equal_range_complex_array(beg_complex_array(&l1),
					   end_complex_array(&l1),
					   beg_complex_array(&l2))
		 == CAG_FALSE,
		 "cag_array: unequal arrays");
	CAG_TEST(*tests, equal_all_complex_array(&l1, &l2) == CAG_FALSE,
		 "cag_array: unequal all arrays");
	free_complex_array(&l1);
	free_complex_array(&l2);
}


static int is_even_complex(struct complex *c, void *data)
{
	(void) data;
	if ((int) c->real % 2)
		return 0;
	else
		return 1;
}

static void test_copy(struct cag_test_series *tests)
{
	complex_array a, b, c, d;
	it_complex_array it1;
	rit_complex_array rit;
	new_complex_array(&a);
	new_complex_array(&b);
	new_complex_array(&c);
	new_complex_array(&d);
	populate_array(&a, 0, 5, 1);
	CAG_CHECK(copy_all_complex_array(&a, &b), "Assignment failure");
	CAG_TEST(*tests, cmp_all_complex_array(&a, &b) == 0,
		 "cag_test: assigned array equals");
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&b),
		"cag_array: assigned array same size");
	CAG_CHECK(rcopy_all_complex_array(&a, &c), "Assignment failure");
	for (it1 = beg_complex_array(&a), rit = rbegin_complex_array(&c);
	     it1 != end_complex_array(&a); ++it1, --rit)
		CAG_TEST(*tests, it1->value.real == rit->value.real &&
			 it1->value.imag == rit->value.imag,
			 "cag_array: assigned values are equal");
	CAG_TEST(*tests, size_complex_array(&a) == size_complex_array(&c),
		"cag_array: assigned array same size");
	CAG_CHECK(copy_if_all_complex_array(&a, &d, is_even_complex, NULL),
		  "Assignment failure");
	it1 = beg_complex_array(&d);
	CAG_TEST(*tests,
		 at_complex_array(it1, 0)->value.real == 0.0 &&
		 at_complex_array(it1, 1)->value.real == 2.0 &&
		 at_complex_array(it1, 2)->value.real == 4.0,
		 "cag_array: copy_if copies even numbers");
	CAG_TEST(*tests, size_complex_array(&d) == 3,
		 "cag_array: copy_if copies expected number");
error:
	free_complex_array(&a);
	free_complex_array(&b);
	free_complex_array(&c);
	free_complex_array(&d);
}

static void test_reverse(struct cag_test_series *tests)
{
	complex_array a;
	it_complex_array it;
	int i = 4;
	new_complex_array(&a);
	populate_array(&a, 0, 5, 1);
	reverse_complex_array(beg_complex_array(&a), end_complex_array(&a));
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it,--i)
		CAG_TEST(*tests, it->value.real == i &&
			 it->value.imag == -i,
			 "cag_array: values are reversed");
	CAG_TEST(*tests, size_complex_array(&a) == 5,
		"cag_array: reversed array same size");
	reverse_all_complex_array(&a);
	i = 0;
	for (it = beg_complex_array(&a); it != end_complex_array(&a); ++it, ++i)
		CAG_TEST(*tests, it->value.real == i &&
			 it->value.imag == -i,
			 "cag_array: values are reversed all");
	CAG_TEST(*tests, distance_all_complex_array(&a) == 5,
		"cag_array: reversed array same size");
	free_complex_array(&a);
}

static void test_erase(struct cag_test_series *tests)
{
	complex_array a;
	it_complex_array it;
	new_with_capacity_complex_array(&a, 2);
	populate_array(&a, 0, 5, 1);
	it = next_complex_array(beg_complex_array(&a));
	it = erase_complex_array(&a, it);
	CAG_TEST(*tests, it->value.real == 2 && it->value.imag == -2,
		 "cag_array: iterator value after one erase");
	CAG_TEST(*tests, size_complex_array(&a) == 4,
		"cag_array: size after one erase");
	it = erase_complex_array(&a, it);
	CAG_TEST(*tests, it->value.real == 3 && it->value.imag == -3,
		 "cag_array: iterator value after two erases");
	CAG_TEST(*tests, size_complex_array(&a) == 3,
		"cag_array: size after two erases");
	it = erase_range_complex_array(&a, beg_complex_array(&a),
				       end_complex_array(&a));
	CAG_TEST(*tests, it == end_complex_array(&a),
		 "cag_array: iterator after range erase");
	CAG_TEST(*tests, size_complex_array(&a) == 0,
		"cag_array: size after range erase");
	free_complex_array(&a);
}

static void test_shuffle(struct cag_test_series *tests)
{
	complex_array a;
	it_complex_array it1, it2;
	int inorder = 1;

	new_complex_array(&a);
	populate_array(&a, 0, 52, 1);
	random_shuffle_complex_array(beg_complex_array(&a),
				     end_complex_array(&a));
	it1 = beg_complex_array(&a);
	CAG_TEST(*tests,
		 at_complex_array(it1, 0)->value.real != 0.0 &&
		 at_complex_array(it1, 51)->value.real != 51.0 &&
		 at_complex_array(it1, 25)->value.real != 25.0,
		"cag_array: random shuffle puts values out of order");
	sort_all_complex_array(&a);
	CAG_FOLD_ALL(complex_array, &a, it1, it2,
		     {
			     if (it2->value.real <= it1->value.real)
				     inorder = 0;
		     },
		     inorder);
	CAG_TEST(*tests, inorder == 1,
		 "cag_array: fold_all random shuffle doesn't create dups");
	inorder = 1;
	CAG_FOLD(beg_complex_array(&a), end_complex_array(&a),
		 next_complex_array, it1, it2,
		     {
			     if (it2->value.real <= it1->value.real)
				     inorder = 0;
		     },
		     inorder);
	CAG_TEST(*tests, inorder == 1,
		 "cag_array: fold random shuffle doesn't create dups");
	it1 = beg_complex_array(&a);
	CAG_TEST(*tests,
		 at_complex_array(it1, 0)->value.real == 0.0 &&
		 at_complex_array(it1, 51)->value.real == 51.0 &&
		 at_complex_array(it1, 25)->value.real == 25.0,
		"cag_array: random shuffle doesn't lose data");
	free_complex_array(&a);
}

static void test_sort(struct cag_test_series *tests)
{
	complex_array a;
	it_complex_array it;
	int inorder;

	new_complex_array(&a);
	populate_array(&a, 50, 0, -1);
	CAG_SORT_ARRAY_DEFAULT(struct complex, beg_complex_array(&a),
			       end_complex_array(&a), cmp_complex);
	inorder = CAG_TRUE;
	for (it = beg_complex_array(&a) + 1; it != end_complex_array(&a); ++it)
		if (it->value.real < (it - 1)->value.real)
			inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder, "cag_aray: macro sort in order");
	CAG_TEST(*tests, size_complex_array(&a) == 50,
		"cag_array: size after macro sort");
	free_complex_array(&a);

	new_complex_array(&a);
	populate_array(&a, 0, 50, 1);
	CAG_SORT_ARRAY_DEFAULT(struct complex, beg_complex_array(&a),
			       end_complex_array(&a), -cmp_complex);
	inorder = CAG_TRUE;
	for (it = beg_complex_array(&a) + 1; it != end_complex_array(&a); ++it)
		if (it->value.real > (it - 1)->value.real)
			inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder, "cag_aray: macro reverse sort in order");
	CAG_TEST(*tests, size_complex_array(&a) == 50,
		"cag_array: size after macro reverse sort");
	free_complex_array(&a);


	new_complex_array(&a);
	populate_array(&a, 50, 0, -1);
	sort_complex_array(beg_complex_array(&a), end_complex_array(&a));
	inorder = CAG_TRUE;
	for (it = beg_complex_array(&a) + 1; it != end_complex_array(&a); ++it)
		if (it->value.real < (it - 1)->value.real)
			inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder, "cag_aray: sort in order");
	CAG_TEST(*tests, size_complex_array(&a) == 50,
		"cag_array: size after sort");
	free_complex_array(&a);

	new_complex_array(&a);
	populate_array(&a, 0, 50, 1);
	rsort_complex_array(rbeg_complex_array(&a), rend_complex_array(&a));
	inorder = CAG_TRUE;
	for (it = beg_complex_array(&a) + 1; it != end_complex_array(&a); ++it)
		if (it->value.real > (it - 1)->value.real)
			inorder = CAG_FALSE;
	CAG_TEST(*tests, inorder, "cag_aray: reverse sort in order");
	CAG_TEST(*tests, size_complex_array(&a) == 50,
		"cag_array: size after reverse sort");
	free_complex_array(&a);
}

static void test_batch(struct cag_test_series *tests)
{
	complex_array a1, a2, a3;
	int i;
	CAG_CHECK( (i = new_many_complex_array(&a1, NULL)) > 0,
		   "failure many new");
	populate_array(&a1, 0, 5, 1);
	copy_many_complex_array(&a1, NULL);
	CAG_TEST(*tests, distance_complex_array(beg_complex_array(&a1),
						end_complex_array(&a1)) == 5,
		 "cag_array: array populated after batch new 1");
error:
	free_many_complex_array(i, &a1, NULL);

	CAG_CHECKL( (i = new_many_complex_array(&a1, &a2, &a3, NULL)) > 0,
		    "failure many new", error_1);
	populate_array(&a1, 0, 5, 1);
	copy_many_complex_array(&a1, &a2, &a3, NULL);
	CAG_TEST(*tests, cmp_all_complex_array(&a1, &a2) == 0,
		 "cag_array: compare all equal array");
	CAG_TEST(*tests, cmp_all_complex_array(&a2, &a3) == 0,
		 "cag_array: compare all equal array");
error_1:
	free_many_complex_array(i, &a1, &a2, &a3, NULL);
}

static void test_stable_sort(struct cag_test_series *tests)
{
	complex_array ca;
	it_complex_array cit;
	struct complex c;

	new_complex_array(&ca);
	c.real = 4.0;
	c.imag = 0.0;
	appendp_complex_array(&ca, &c);
	c.real = 3.0;
	c.imag = 1.0;
	appendp_complex_array(&ca, &c);
	c.real = 3.0;
	c.imag = 2.0;
	appendp_complex_array(&ca, &c);
	c.real = 2.0;
	c.imag = 1.0;
	appendp_complex_array(&ca, &c);
	c.real = 2.0;
	c.imag = 2.0;
	appendp_complex_array(&ca, &c);
	c.real = 1.0;
	c.imag = 0.0;
	appendp_complex_array(&ca, &c);
	stable_sort_complex_array(beg_complex_array(&ca),
				  end_complex_array(&ca));
	cit = beg_complex_array(&ca);
	CAG_TEST(*tests, cit->value.real == 1.0 && ++cit &&
		 cit->value.real == 2.0 && cit->value.imag == 1.0 && ++cit &&
		 cit->value.real == 2.0 && cit->value.imag == 2.0 && ++cit &&
		 cit->value.real == 3.0 && cit->value.imag == 1.0 && ++cit &&
		 cit->value.real == 3.0 && cit->value.imag == 2.0 && ++cit &&
		 cit->value.real == 4.0,
		 "stability of stable sort");
	stable_sort_all_complex_array(&ca);
	cit = beg_complex_array(&ca);
	CAG_TEST(*tests, cit->value.real == 1.0 && ++cit &&
		 cit->value.real == 2.0 && cit->value.imag == 1.0 && ++cit &&
		 cit->value.real == 2.0 && cit->value.imag == 2.0 && ++cit &&
		 cit->value.real == 3.0 && cit->value.imag == 1.0 && ++cit &&
		 cit->value.real == 3.0 && cit->value.imag == 2.0 && ++cit &&
		 cit->value.real == 4.0,
		 "stability of stable sort container");
	free_complex_array(&ca);
}

static void test_int_array(struct cag_test_series *tests)
{
	int i, total = 0;
	cag_int_array l;
	iterator_cag_int_array *it;
	new_cag_int_array(&l);
	append_cag_int_array(&l, 2);
	for (i = 1; i < 5; ++i)
		append_cag_int_array(&l, i);
	for (i = 0, it = beg_cag_int_array(&l); it != end_cag_int_array(&l);
	     it = next_cag_int_array(it), ++i)
		total += it->value;
	CAG_TEST(*tests, i == 5 && total == 12,
		 "cag_array: cag_int_array append");
	free_cag_int_array(&l);
	new_cag_int_array(&l);
	total = 0;
	for (i = 1; i < 5; ++i)
		appendp_cag_int_array(&l, &i);
	for (i = 0, it = beg_cag_int_array(&l); it != end_cag_int_array(&l);
	     ++it, ++i)
		total += it->value;
	CAG_TEST(*tests, i == 4 && total == 10,
		 "cag_array: cag_int_array pappend");
	free_cag_int_array(&l);
}

static void test_abstract(struct cag_test_series *tests)
{
	complex_array a, b;
	it_complex_array it1, it2;
	struct complex c;
	int set = 1;
	int correct = 1;

	new_complex_array(&a);
	new_complex_array(&b);

	populate_array(&a, 0, 10, 1);
	c.real = 2;
	c.imag = 2;
	append_complex_array(&a, c);
	c.real = 5;
	c.imag = 5;
	append_complex_array(&a, c);

	/* Remove duplicates from a into b */
	sort_all_complex_array(&a);
	appendp_complex_array(&b, front_complex_array(&a));
	CAG_FOLD_ALL(complex_array, &a, it1, it2,
		     {
			     if (it1->value.real != it2->value.real)
				     appendp_complex_array(&b, &it2->value);
			     else
				     set = 0;
		     }, set);
	CAG_TEST(*tests, set == 0,
		 "cag_array: fold identifies duplicates");
	set = 1;
	CAG_FOLD_ALL(complex_array, &b, it1, it2,
		     {
			     if (it1->value.real >= it2->value.real) {
				     set = 0;
				     break;
			     }
		     }, set);
	CAG_TEST(*tests, set == 1,
		 "cag_array: duplicates removed");
	CAG_TEST(*tests, size_complex_array(&b) == size_complex_array(&a) - 2,
		 "cag_array: size of array with dups removed");
	set_exact_size_complex_array(&b, size_complex_array(&a));
	CAG_MAP_SAME(complex_array, &a, &b, it1, it2,
		     {
			     it2->value.real = it1->value.real * 2;
		     });
	CAG_TEST(*tests, size_complex_array(&b) == size_complex_array(&a),
		 "cag_array: size of arrays equal after map");
	it2 = beg_complex_array(&a);
	CAG_FOR_ALL(complex_array, &b, it1,
		{
			if(it1->value.real != it2->value.real * 2)
				correct = 0;
			it2 = next_complex_array(it2);
		});
	CAG_TEST(*tests, correct == 1,
		 "cag_array: expected values after map");
	CAG_FILTER_SAME(complex_array, &a, &b, it1, it2,
			(int) it1->value.real % 2 == 0);
	erase_range_complex_array(&b, it2, end_complex_array(&b));
	CAG_TEST(*tests, size_complex_array(&b) == 6,
		"cag_array: size reduced after filter and erase");
	CAG_TEST(*tests, b.objects[0].value.real == 0 &&
		 b.objects[5].value.real == 8,
		 "cag_array: expected values after filter");
	free_complex_array(&b);
	free_complex_array(&a);
}

static void test_complexp_array(struct cag_test_series *tests)
{
	complexp_array a;
	it_complexp_array it;
	struct complex c;
	int i, failures = 0;

	new_complexp_array(&a);
	for (i = 50; i > 0; --i) {
		c.real = (double) i;
		c.imag = (double) -i;
		append_complexp_array(&a, c);
	}
	sort_all_complexp_array(&a);
	i = 1;
	CAG_FOR_ALL(complexp_array, &a, it,
		    {
			    if (it->value.real != i) ++failures;
			    ++i;
		    });
	CAG_TEST(*tests, failures == 0,
		 "cag_array: sorting using 'by address' compare function");
	free_complexp_array(&a);
}


static void populate_string_array(string_array *s, int num)
{
	char c[6];
	int i;

	for (i = 0; i < num; ++i) {
		sprintf(c, "s%d", i);
		append_string_array(s, c);
	}
}

static void test_string_array(struct cag_test_series *tests)
{
	string_array s;
	char c[6];
	it_string_array it;
	int i = 0, failures = 0;

	new_string_array(&s);
	populate_string_array(&s, 5);
	CAG_FOR_ALL(string_array, &s, it,
		    {
			    sprintf(c, "s%d", i++);
			    if (strcmp(it->value, c)) ++failures;
		    });
	CAG_TEST(*tests, failures == 0,
		 "cag_array: values after string appends");
	CAG_TEST(*tests, size_string_array(&s) == 5,
		 "cag_array: size after string appends");
	it = at_string_array(beg_string_array(&s), 3);
	it = erase_string_array(&s, it);
	CAG_TEST(*tests, size_string_array(&s) == 4,
		 "cag_array: size after string appends");
	free_string_array(&s);
}

void test_array(struct cag_test_series *tests)
{
	test_default_sort(tests);
	test_new(tests);
	test_append(tests);
	test_front_back(tests);
	test_insert(tests);
	test_insert_order(tests);
	test_prepend(tests);
	test_copy_over(tests);
	test_bsearch(tests);
	test_find(tests);
	test_cmp(tests);
	test_equal(tests);
	test_copy(tests);
	test_reverse(tests);
	test_erase(tests);
	test_shuffle(tests);
	test_sort(tests);
	test_stable_sort(tests);
	test_batch(tests);
	test_abstract(tests);
	test_int_array(tests);
	test_complexp_array(tests);
	test_string_array(tests);
}

CAG_DEF_CMP_ARRAY(complex_array, struct complex, cmp_complex);
CAG_DEF_ALL_CMP_ARRAY(complexp_array, struct complex, cmpp_complex, &,
		   CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,  CAG_NO_FREE_FUNC);

CAG_DEF_ALL_CMP_ARRAY(string_array, char *, strcmp, CAG_BYVAL,
		   CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free);
