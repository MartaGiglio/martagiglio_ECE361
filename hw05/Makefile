CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

TARGET = lock
TEST = test_lock

all: $(TARGET)

$(TARGET): main.o lock.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o lock.o

main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

$(TEST): tests/test_lock.c lock.o lock.h
	$(CC) $(CFLAGS) -I. -o $(TEST) tests/test_lock.c lock.o

test: $(TEST)
	./$(TEST)

clean:
	rm -f *.o $(TARGET) $(TEST)

.PHONY: all test clean
