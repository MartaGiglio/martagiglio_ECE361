CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

.PHONY: all test clean

all: test_ptr

test_ptr: ptr.c tests/test_ptr.c ptr.h
	$(CC) $(CFLAGS) ptr.c tests/test_ptr.c -o test_ptr

test: test_ptr
	./test_ptr

clean:
	rm -f test_ptr ptr.o

