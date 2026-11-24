CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SANFLAGS = -fsanitize=address,undefined

TARGET = booth

SOURCES = main.c sensor.c fsm.c store.c aux.c report.c status.c bits.c iom361_r4.c
OBJECTS = $(SOURCES:.c=.o)

TESTS = tests/test_status \
        tests/test_fsm \
        tests/test_store \
        tests/test_aux \
        tests/test_sensor \
        tests/test_report

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/test_status: tests/test_status.c status.c bits.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

tests/test_fsm: tests/test_fsm.c fsm.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

tests/test_store: tests/test_store.c store.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

tests/test_aux: tests/test_aux.c aux.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

tests/test_sensor: tests/test_sensor.c sensor.c iom361_r4.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^ -lm

tests/test_report: tests/test_report.c report.c store.c
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^ -lm

test: all $(TESTS)
	@./tests/run_all_tests.sh

clean:
	rm -f $(OBJECTS) $(TARGET) $(TESTS)
