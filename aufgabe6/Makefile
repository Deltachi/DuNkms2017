CFLAGS=-std=c11 -Wall -g
LDFLAGS= -pthread
CC=gcc

all: philo

.PHONY: all clean

philo: philo.o

philo.o: philo.c

clean:
	rm -f philo
	rm -f philo.o
