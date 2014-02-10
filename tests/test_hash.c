/*! Tests for CAGL hash.

  \copyright Copyright 2013 Nathan Geffen. All rights reserved.
  \license GNU Lesser General Public License Copyright.
  See COPYING for the license text.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAG_SAFER 1
#include "cagl/error.h"
#include "cagl/test.h"
#include "cagl/hash.h"

#define ELEM 500


struct str_str {
	char *key;
	char *data;
};

CAG_DEC_DEF_CMP_HASH(str_hash, char *, strcmp, cag_oat_hash, strlen);
CAG_DEC_STR_HASH(string_hash);
CAG_DEC_DEF_STR_STR_HASH(str_str_hash, struct str_str);

struct cag_str_x {
	char *key;
	void *data;
};

static void populate_str_hash(str_hash *h)
{
	char *k0 = "k0";
	char *k1 = "k1";
	char *k2 = "k2";
	char *k3 = "k3";
	char *k4 = "k4";

	insert_str_hash(h, k0);
	insert_str_hash(h, k1);
	insert_str_hash(h, k2);
	insert_str_hash(h, k3);
	insert_str_hash(h, k4);
}

static void populate_string_hash(string_hash *h, const int num)
{
	char key[5];
	int i = 0;

	for (i = 0; i < num; ++i) {
		snprintf(key, 5, "k%d", i);
		insert_string_hash(h, key);
	}
}

static void populate_str_str_hash(str_str_hash *h, const int num)
{
	struct str_str x;
	char key[5];
	char data[5];
	int i = 0;

	for (i = 0; i < num; ++i) {
		snprintf(key, 5, "k%d", i);
		snprintf(data, 5, "d%d", i);
		x.key = key;
		x.data = data;
		insert_str_str_hash(h, x);
	}
}


static void test_new(struct cag_test_series *tests)
{
	str_hash h;
	string_hash o;

	CAG_TEST(*tests, new_str_hash(&h),
		 "cag_hash: new str K&R hash");
	CAG_TEST(*tests, beg_str_hash(&h) == end_str_hash(&h),
		 "cag_hash: begin == end after new for str");
	CAG_TEST(*tests, new_string_hash(&o),
		 "cag_hash: new str oat hash") ;
	CAG_TEST(*tests, beg_string_hash(&o) == end_string_hash(&o),
		 "cag_hash: begin == end after new for string");
	free_string_hash(&o);
	free_str_hash(&h);
}

static void test_insert(struct cag_test_series *tests)
{
	str_hash h1;
	string_hash h2;
	it_str_hash it1;
	it_string_hash it2;
	int i, arr[5];
	int failures = 0;

	for (i = 0; i < 5; ++i)
		arr[i] = 0;
	new_str_hash(&h1);
	populate_str_hash(&h1);
	for (it1 = begin_str_hash(&h1); it1 != end_str_hash(&h1);
	     it1 = next_str_hash(it1))
		arr[atoi(&it1->value[1])] = 1;
	for (i = 0; i < 5; ++i)
		if (arr[i] != 1)
			++failures;
	CAG_TEST(*tests, failures == 0,
		 "cag_hash: inserts into non-allocating hash");
	free_str_hash(&h1);

	failures = 0;
	new_string_hash(&h2);
	populate_string_hash(&h2, 5);
	for (it2 = begin_string_hash(&h2); it2 != end_string_hash(&h2);
	     it2 = next_string_hash(it2))
		arr[atoi(&it2->value[1])] = 2;
	for (i = 0; i < 5; ++i)
		if (arr[i] != 2)
			++failures;
	CAG_TEST(*tests, failures == 0,
		 "cag_hash: inserts into non-allocating hash");
	free_string_hash(&h2);

}

static int strcmp_p(const char **x, const char **y)
{
	return strcmp(*x, *y);
}

static int strcmp_data(const struct str_str *c1, const struct str_str *c2)
{
	return strcmp(c1->data, c2->data);
}

static void test_find(struct cag_test_series *tests)
{
	str_str_hash h;
	it_str_str_hash it;
	struct str_str s;

	string_hash sh;
	it_string_hash itsh;
	char *str;

	new_string_hash(&sh);
	populate_string_hash(&sh, 100);
	str = "k47";
	itsh = find_all_string_hash(&sh, str, strcmp_p);
	CAG_TEST(*tests, itsh && strcmp(itsh->value, "k47") == 0,
		 "cag_hash: find string");
	str = "k97";
	itsh = findp_all_string_hash(&sh, (const char **) &str, strcmp_p);
	CAG_TEST(*tests, itsh && strcmp(itsh->value, "k97") == 0,
		 "cag_hash: find pointer string");
	free_string_hash(&sh);

	s.key = "";
	new_str_str_hash(&h);
	populate_str_str_hash(&h, 100);
	s.data = "d47";
	it = find_all_str_str_hash(&h, s, strcmp_data);
	CAG_TEST(*tests, it && strcmp(it->value.key, "k47") == 0,
		 "cag_hash: find element");
	s.data = "d100";
	it = find_all_str_str_hash(&h, s, strcmp_data);
	CAG_TEST(*tests, it == end_str_str_hash(&h),
		 "cag_hash: find non-existent element");
	s.data = "d47";
	it = findp_all_str_str_hash(&h, &s, strcmp_data);
	CAG_TEST(*tests, it && strcmp(it->value.key, "k47") == 0,
		 "cag_hash: find by address element");
	s.data = "d100";
	it = findp_all_str_str_hash(&h, &s, strcmp_data);
	CAG_TEST(*tests, it == end_str_str_hash(&h),
		 "cag_hash: find by address non-existent element");
	free_str_str_hash(&h);
}

static void test_remove(struct cag_test_series *tests)
{
	string_hash h;
	it_string_hash it;
	char key[5];
	int i;
	size_t size = ELEM;
	int failure = 0;

	new_string_hash(&h);

	it = remove_string_hash(&h, "k1");
	CAG_TEST(*tests, it == NULL,
		 "cag_hash: remove from empty hash");

	populate_string_hash(&h, ELEM);

	for (i = 0; i < ELEM; ++i) {
		snprintf(key, 5, "k%d", i);
		it = get_string_hash(&h, key);
		if (!it) {
			failure = 1;
			break;
		}
		remove_string_hash(&h, key);
		--size;
		if (size != h.size) {
			failure = 1;
			break;
		}
		it = get_string_hash(&h, key);
		if (it != NULL) {
			failure = 1;
			break;
		}
	}
	CAG_TEST(*tests, failure == 0,
		 "cag_hash: remove keys");
	free_string_hash(&h);
}

static void test_erase(struct cag_test_series *tests)
{
	string_hash h;
	it_string_hash it;

	new_string_hash(&h);
	populate_string_hash(&h, ELEM);
	it = beg_string_hash(&h);
	while (it != end_string_hash(&h)) {
		it = erase_string_hash(&h, it);
	}
	CAG_TEST(*tests, h.size == 0,
		 "cag_hash: hash empty after erases");
	populate_string_hash(&h, ELEM);
	erase_range_string_hash(&h, beg_string_hash(&h), end_string_hash(&h));
	CAG_TEST(*tests, h.size == 0,
		 "cag_hash: hash empty after erase range");
	free_string_hash(&h);
}

static void test_rehash(struct cag_test_series *tests)
{
	string_hash h;
	it_string_hash it;
	int i, arr[70];
	int failure = 0;

	for (i = 0; i < 70; ++i) arr[i] = 0;
	new_string_hash(&h);
	populate_string_hash(&h, 70);
	CAG_TEST(*tests, h.buckets < 509 && h.size == 70,
		 "cag_hash: buckets and size before rehash");
	rehash_string_hash(&h, 509);

	CAG_FOR_ALL(string_hash, &h, it,
		    {
			    i = atoi(&it->value[1]);
			    arr[i] = 1;
		    });
	for (i = 0; i < 70; ++i)
		if (arr[i] != 1) {
			failure = 1;
			printf("%d %d\n", i, arr[i]);
		}
	CAG_TEST(*tests, failure == 0,
		 "cag_hash: rehash");
	CAG_TEST(*tests, h.size == 70 && h.buckets == 509,
		 "cag_hash: size and buckets after rehash");
	free_string_hash(&h);
}


static void test_copy(struct cag_test_series *tests)
{
	string_hash h1, h2;
	it_string_hash it1, it2;
	int failure, i;
	int vals[150];

	new_string_hash(&h1);
	new_string_hash(&h2);
	failure = 0;
	for (i = 0; i < 150; ++i) vals[i] = 0;
	populate_string_hash(&h1, 150);
	copy_all_string_hash(&h1, &h2);
	CAG_TEST(*tests, h1.size == h2.size,
		 "cag_hash: size of two hash tables after assign");
	CAG_MAP_SAME(string_hash, &h1, &h2, it1, it2,
		     {
			     ++vals[atoi(&it1->value[1])];
			     ++vals[atoi(&it2->value[1])];
			     if (atoi(&it2->value[1]) >= 150)
				     ++failure;
		     });
	for (i = 0; i < 150; ++i)
		if (vals[i] != 2)
			++failure;
	CAG_TEST(*tests, failure == 0,
		 "cag_hash: equal hash tables after assign");
	free_string_hash(&h2);
	free_string_hash(&h1);

	new_with_buckets_string_hash(&h1, 500);
	new_string_hash(&h2);
	failure = 0;
	for (i = 0; i < 150; ++i) vals[i] = 0;
	populate_string_hash(&h1, 150);
	copy_all_string_hash(&h1, &h2);
	CAG_TEST(*tests, h1.size == h2.size,
		 "cag_hash: size of two hash tables after assign");
	CAG_MAP_SAME(string_hash, &h1, &h2, it1, it2,
		     {
			     ++vals[atoi(&it1->value[1])];
			     ++vals[atoi(&it2->value[1])];
			     if (atoi(&it2->value[1]) >= 150)
				     ++failure;
		     });
	for (i = 0; i < 150; ++i)
		if (vals[i] != 2)
			++failure;
	CAG_TEST(*tests, failure == 0,
		 "cag_hash: equal hash tables after assign");
	free_string_hash(&h2);
	free_string_hash(&h1);

}

static void test_str_str(struct cag_test_series *tests)
{
	str_str_hash h;
	it_str_str_hash it;
	struct str_str str_str;
	int i, arr[5];
	int failures = 0;

	for (i = 0; i < 5; ++i)
		arr[i] = 0;
	new_str_str_hash(&h);
	populate_str_str_hash(&h, 5);
	CAG_TEST(*tests, distance_all_str_str_hash(&h) == 5,
		 "cag_hash: distance after inserts");
	for (it = begin_str_str_hash(&h); it != end_str_str_hash(&h);
	     it = next_str_str_hash(it)) {
		++arr[atoi(&it->value.key[1])];
		++arr[atoi(&it->value.data[1])];
		if (atoi(&it->value.key[1]) != atoi(&it->value.data[1])) {
			++failures;
			printf("FAIL 1: %s %s\n", it->value.key, it->value.data);
		}
	}
	for (i = 0; i < 5; ++i)
		if (arr[i] != 2) {
			printf("FAIL 2: %d\n", arr[i]);
			++failures;
		}
	CAG_TEST(*tests, failures == 0,
		 "cag_hash: inserts into complex struct hash");
	str_str.key = "supercalafraglisticespialadocious";
	str_str.data = "If you like it then you're being a little bit atrocious";
	insert_str_str_hash(&h, str_str);
	it = getp_str_str_hash(&h, &str_str);
	CAG_TEST(*tests, strcmp(it->value.key,
				"supercalafraglisticespialadocious") == 0 &&
		 strcmp(it->value.data, "If you like it then "
				"you're being a little bit atrocious") == 0 &&
		 distance_str_str_hash(beg_str_str_hash(&h),
				       end_str_str_hash(&h)) == 6,
		 "cag_hash: insert into with long strings");
	str_str.data = "Roses are red and violets are blue. Monkeys are pink.";
	insertp_str_str_hash(&h, &str_str);
	it = get_str_str_hash(&h, str_str);
	CAG_TEST(*tests, strcmp(it->value.key,
				"supercalafraglisticespialadocious") == 0 &&
		 strcmp(it->value.data, "Roses are red and violets are blue. "
			"Monkeys are pink.") == 0,
		 "cag_hash: insert into with same key, different data");
	removep_str_str_hash(&h, &str_str);
	it = get_str_str_hash(&h, str_str);
	CAG_TEST(*tests, it == NULL && distance_all_str_str_hash(&h) == 5,
		 "cag_hash: removed item by address");

	free_str_str_hash(&h);
}

void test_hash(struct cag_test_series *tests)
{
	test_new(tests);
	test_insert(tests);
	test_find(tests);
	test_remove(tests);
	test_erase(tests);
	test_rehash(tests);
	test_copy(tests);
	test_str_str(tests);
}

CAG_DEF_STR_HASH(string_hash);
