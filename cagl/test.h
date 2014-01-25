/*! CAGL test framework.

    \copyright Nathan Geffen.

    \license GNU Lesser General Public License Copyright.

    See COPYING for the license text.

    This test framework is deliberately simple.
*/

#ifndef CAG_TEST_H
#define CAG_TEST_H

enum cag_test_verbosity {
    CAG_V_NONE = 0,
    CAG_V_FAILURES,
    CAG_V_VERBOSE_FAILURES,
    CAG_V_SUCCESSES,
    CAG_V_VERBOSE,
    CAG_V_ALL
};

struct cag_test_series {
    unsigned cases;
    unsigned successes;
    unsigned failures;
    char description[256];
    int close_log;
    enum cag_test_verbosity verbosity;
    FILE *log;
};

int cag_test_init(struct cag_test_series *test, const char *description,
                  const enum cag_test_verbosity verbosity, FILE *log,
                  const char *logname);

unsigned cag_test_summary(const struct cag_test_series *test);

#define CAG_TEST(test_series, expression, description) \
    { \
        ++(test_series).cases; \
        if (expression) { \
            ++(test_series).successes; \
            if ((test_series).verbosity >= CAG_V_SUCCESSES) { \
                fprintf((test_series).log, \
                        "Success. Case %d: %s\n", \
                        (test_series).cases, \
                        description); \
                if ((test_series).verbosity == CAG_V_ALL) { \
                    fprintf((test_series).log, \
                            "\tExpression: %s\n", \
                            #expression); \
                } \
            } \
        } else { \
            ++(test_series).failures; \
            if ((test_series).verbosity >= CAG_V_FAILURES) { \
                fprintf((test_series).log, \
                        "Failure. Case %d (%s line %d): " \
                        "%s\n",(test_series).cases, \
                        __FILE__, __LINE__, description); \
                if ((test_series).verbosity >= CAG_V_FAILURES \
                        && ((test_series).verbosity != \
                            CAG_V_SUCCESSES)) { \
                    fprintf((test_series).log, \
                            "\tExpression: %s\n", \
                            #expression); \
                } \
            } \
        } \
    }

#endif
