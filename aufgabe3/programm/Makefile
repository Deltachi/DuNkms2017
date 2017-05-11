CFLAGS=-std=c11 -Wall -pedantic -g -fopenmp
LDFLAGS=-fopenmp
CC=gcc

all: library

.PHONY: all clean

library: md5.o library.o
	$(CC) $(LDFLAGS) -o library md5.o library.o

library.o: library.c md5.h

clean:
	rm -f library
	rm -f library.o library.o md5.o

