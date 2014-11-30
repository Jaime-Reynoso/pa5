CC = gcc
CCFLAGS = -Wall -pthread -g
ARKEYS = rv
TARGET = books

all: books
books: bookorders.o queue.o
	$(CC) $(CCFLAGS) -o books bookorders.o queue.o
bookorders.o: bookorders.c
	$(CC) $(CCFLAGS) -c bookorders.c
queue.o: queue.c
	$(CC) $(CCFLAGS) -c queue.c
clean:
	rm -f sl *.o books