CFLAGS=-std=c11 -Wall -g
#CC=clang

all: snake

.PHONY: all clean

snake: snake.o support.o

snake.o: snake.c support.h

clean:
	rm -f snake
	rm -f snake.o support.o
