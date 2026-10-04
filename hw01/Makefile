CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = test_bits

SRC = bits.c status.c tests/test_bits.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o