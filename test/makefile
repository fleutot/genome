CC = gcc
CFLAGS = -std=c99 -g -Wall -O3 -Wno-unused-function

SRC = ../genome.c ../linkedlist/linkedlist.c genome_test.c
OBJ = $(SRC:.c=.o)
TARGET = genome_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
