CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
LDLIBS = -lm

TARGET = replay

OBJECTS = main.o sensor.o panel.o scenario.o iom361_r4.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c iom361_r4.h sensor.h panel.h scenario.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

test: $(TARGET) test_hw06
	./test_hw06
	./regress.sh

test_hw06: tests/test_hw06.c sensor.c panel.c scenario.c iom361_r4.c
	$(CC) $(CFLAGS) -I. -o test_hw06 tests/test_hw06.c sensor.c panel.c scenario.c iom361_r4.c $(LDLIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) test_hw06

.PHONY: all test clean
