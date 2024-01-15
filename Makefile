CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: tests

tests: tests.o polynomial.o
	$(CC) $(CFLAGS) -o tests tests.o polynomial.o

tests.o: tests.c polynomial.h
	$(CC) $(CFLAGS) -c tests.c

polynomial.o: polynomial.c polynomial.h
	$(CC) $(CFLAGS) -c polynomial.c

clean:
	rm -f tests tests.o polynomial.o
