CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

TARGET = booth

SOURCES = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
