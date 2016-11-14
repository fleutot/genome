CC = gcc
CFLAGS = -std=c99 -g -Wall -O3 -Wno-unused-function

# ../machine.c included in the test file, needed for testing module internals
SRC = machine_test.c
OBJ = $(SRC:.c=.o)
TARGET = machine_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
