CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g

TEST_TARGET = test_bst
STATS_TARGET = treestats
TEST_SOURCES = bst.c tests/test_bst.c
STATS_SOURCES = bst.c treestats.c

.PHONY: all test memcheck clean

all: $(TEST_TARGET) $(STATS_TARGET)

$(TEST_TARGET): $(TEST_SOURCES) bst.h
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

$(STATS_TARGET): $(STATS_SOURCES) bst.h
	$(CC) $(CFLAGS) $(STATS_SOURCES) -o $(STATS_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

memcheck:
	$(CC) $(CFLAGS) -fsanitize=address,undefined $(TEST_SOURCES) -o test_bst_memcheck
	./test_bst_memcheck

clean:
	rm -f $(TEST_TARGET) $(STATS_TARGET) test_bst_memcheck
