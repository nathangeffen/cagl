CC 		= gcc

C_INCLUDE_PATH 	:= $(C_INCLUDE_PATH):..

TEST_EXE	= cagtest
CFLAGS		=  -g3 -Wall -pedantic -Wstrict-prototypes -Wextra -Werror
CXXFLAGS	= $(CFLAGS)
LDFLAGS		= -g3

TEST_SOURCES	= test_suite.c test_dlist.c test_array.c test_hash.c \
test_tree.c test_slist.c test_compound.c

TEST_OBJS	= $(TEST_SOURCES:.c=.o)

SOURCES 	= error.c common.c test.c

OBJS		= $(SOURCES:.c=.o)

INCLUDES 	= test.h common.h concepts.h error.h \
array.h hash.h dlist.h tree.h slist.h

vpath %.c ../cagl
vpath %.h ../cagl

test-run: test-compile
	./cagtest

test-compile: $(TEST_EXE)

$(TEST_EXE): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(TEST_OBJS) -o $(TEST_EXE)

test_suite.o: test_suite.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $< -o $@

test.o: test.h common.h error.h

test_dlist.o: common.h concepts.h error.h test.h dlist.h

test_hash.o: common.h concepts.h error.h test.h hash.h

test_array.o: common.h concepts.h error.h test.h array.h

test_tree.o: common.h concepts.h error.h test.h tree.h

test_slist.o: common.h concepts.h error.h test.h slist.h

test_compound.o: $(INCLUDES)

error.o: error.h

common.o: common.c common.h

indent:
	bash slash79 $(INCLUDES)

test-release-run: test-release
	./cagtest

test-release: $(SOURCES) $(TEST_SOURCES)
	$(CC) -Wall -pedantic -Werror -flto -O3 -DNDEBUG $^ -o $(TEST_EXE)

test-clean:
	rm -f $(TEST_OBJS) $(OBJS) $(TEST_EXE)

clean:
	rm -f $(OBJS)

.PHONY: test-compile test-run
