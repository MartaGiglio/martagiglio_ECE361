CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

all: events

events: events.o tally.o
	$(CC) $(CFLAGS) -o events events.o tally.o

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c

test_tally: tests/test_tally.c tally.o
	$(CC) $(CFLAGS) -o test_tally tests/test_tally.c tally.o

test: test_tally
	./test_tally

clean:
	rm -f *.o events test_tally

.PHONY: all test clean
