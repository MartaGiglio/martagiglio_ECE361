CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

TARGET = booth

SOURCES = main.c sensor.c fsm.c store.c aux.c report.c status.c bits.c iom361_r4.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TESTS)

TESTS = tests/test_status

.PHONY: test
test: $(TESTS)
	@for t in $(TESTS); do ./$$t || exit 1; done

tests/test_status: tests/test_status.c status.c bits.c status.h bits.h
	$(CC) $(CFLAGS) -fsanitize=address,undefined -o $@ tests/test_status.c status.c bits.c
