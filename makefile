CC = gcc
CCFLAGS = -Wall -pthread -g
ARKEYS = rv
TARGET = books

all: books
books: bookorders.o
	$(CC) $(CCFLAGS) -o books bookorders.o
bookorders.o: bookorders.c
	$(CC) $(CCFLAGS) -c bookorders.c
clean:
	rm -f sl *.o books
