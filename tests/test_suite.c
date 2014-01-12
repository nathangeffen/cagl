/*! Test suite for GDA code.

    \copyright Copyright 2013 University of Cape Town. All rights reserved.
    \license This code is licensed under the FreeBSD Copyright.
    See COPYING for the license text.

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cagl/error.h"
#include "cagl/common.h"
#include "cagl/test.h"

void test_dlist(struct cag_test_series *tests);
void test_slist(struct cag_test_series *tests);
void test_array(struct cag_test_series *tests);
void test_hash(struct cag_test_series *tests);
void test_tree(struct cag_test_series *tests);
void test_compound(struct cag_test_series *tests);

int test_test(struct cag_test_series *tests)
{
	struct cag_test_series test_of_test;
	char filename[L_tmpnam];

	CAG_CHECK(tmpnam(filename), "Could not generate temporary file name");
	CAG_TEST(*tests,
		 cag_test_init(&test_of_test,
			       "Test of GDA test framework",
			       CAG_V_ALL, NULL, filename) ==  CAG_SUCCESS,
		 "cag_test: initialisation");
	/* Check testing code. */
	CAG_TEST(test_of_test, 1 == 1, "Test that 1 == 1");
	CAG_TEST(*tests,
		 test_of_test.successes == 1 &&
		 test_of_test.failures == 0 &&
		 test_of_test.cases == 1, "cag_test: successful expression");
	CAG_TEST(test_of_test, 1 == 0, "Test that 1 == 0");
	CAG_TEST(*tests,
		 test_of_test.successes == 1 &&
		 test_of_test.failures == 1 &&
		 test_of_test.cases == 2, "cag_test: failed expression");
	CAG_TEST(test_of_test, 1 <= 2 && 1 >= 0,
		 "Test that 1 between 0 and 2");
	CAG_TEST(*tests,
		 test_of_test.successes == 2 &&
		 test_of_test.failures == 1 &&
		 test_of_test.cases == 3, "cag_test: range expression");

	#if __STDC_VERSION__ >= 199901L
	CAG_DEBUG("Testing debug output macros C99: %d %d", 19, 99);
	#else
	CAG_DEBUG_C89( ("Testing debug output macros C89: %d %d", 19, 89) );
	#endif
	CAG_TEST(*tests, cag_test_summary(&test_of_test) == 1,
		 "gda-test: test summary returns number failures");
	return CAG_SUCCESS;
error:
	return CAG_ERROR;
}

int main(void)
{
	struct cag_test_series test;
	CAG_CHECK(cag_test_init(&test, "Test of GDA",
				CAG_V_VERBOSE_FAILURES, NULL, NULL)
		  == CAG_SUCCESS,
		  "Error initializing test framework");
	CAG_CHECK(test_test(&test) == CAG_SUCCESS,
		  "Error in testing framework");
	test_dlist(&test);
	test_slist(&test);
	test_array(&test);
	test_hash(&test);
	test_tree(&test);
	test_compound(&test);
	cag_test_summary(&test);
	return CAG_SUCCESS;
error:
	return CAG_ERROR;
}
