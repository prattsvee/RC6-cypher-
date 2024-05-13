# Makefile for RC6 implementation

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = rc6.c
OBJ = rc6.o
EXEC = run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ)
