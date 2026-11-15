CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g

all: balance tests/test_istack

balance: balance.c istack.c istack.h
	$(CC) $(CFLAGS) balance.c istack.c -o balance

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CFLAGS) istack.c tests/test_istack.c -o tests/test_istack

test: all
	./tests/test_istack
	./regress.sh

memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="-std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer" test

clean:
	rm -f balance tests/test_istack actual.txt

.PHONY: all test memcheck clean
