/*! \file Testing framework code used by the CAGL library.

    \copyright Copyright 2014 Nathan Geffen. All rights reserved.

    \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

    \sa COPYING for the license text.

    \sa howtodev.rst to learn how this code works and how to modify it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cagl/common.h"
#include "cagl/error.h"
#include "cagl/test.h"

int cag_test_init(struct cag_test_series *test, const char *description,
		   const enum cag_test_verbosity verbosity, FILE *log,
		   const char *logname)
{
	test->cases = test->successes = test->failures = 0;
	strncpy(test->description, description, 255);
	test->description[255] = '\0';
	test->verbosity = verbosity;
	if (log) {
		test->log = log;
		test->close_log = 0;
	} else if (logname) {
		CAG_CHECK(test->log = fopen(logname, "w"),
			  "Could not open file");
		test->close_log = 1;
	} else {
		test->log = stdout;
		test->close_log = 0;
	}
	return CAG_SUCCESS;
error:
	return CAG_ERROR;
}

unsigned cag_test_summary(const struct cag_test_series *test)
{
	fprintf(test->log, "Tests: %u\tSuccesses: %u\tFailures %u\n",
	       test->cases, test->successes, test->failures);
	if (test->close_log) fclose(test->log);
	return test->failures;
}
