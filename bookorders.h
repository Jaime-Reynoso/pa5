#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include "uthash.h"
#include <sys/stat.h>
//M: -> mauricio comments

#DEFINE MAX 5

/*
*	Customer Struct contains name, customer ID. balance, address, state. zip and a list of both purchased
*	and rejected books
*/
struct customer{
	char* name;
	float balance;
	char* address;
	char* city;
	char* state;
	char* zip;
	bookOrder* success_order;
	bookOrder* fail_order;
	UT_Hash_Handle hh;
};

typedef struct customer customer;

typedef struct hash_cell{
	int customer_ID;
	customer cust;
}hash_cell;


/*
*	A queue contains the size of the queue, a link to the beginning and a link to the end Queue
*/
struct queue{
	bookOrder* cat_orders;
	char *category;
	int size;
	int position_of_last_item;
	int position_of_first_item;
	sem_t mutex;
	sem_t slots;
	sem_t items;
};

typedef struct queue queue;

/*
*	 This is a book order struct that contains what a book order would have.
*/
struct bookOrder{
	char* title;
	float price;
	int *customer_ID;
	char *category;
	struct bookOrder *next;
}
typedef struct bookOrder bookOrder;
/*
*	This method initializes  the Queue by giving it a size of 0 and sets both the nodes equal to NULL
*/
queue* initializeQueue(char * category);

/*
*	This is a method to initialize a bookOrder Struct
*/
void initializeBookStruct(bookOrder* pointer);

/*
*	This method adds the books into the Queue
*/
queue* createQueue(FILE* order);

/*
*	This is the processor Thread method
*/
void readBookOrders(File *orders);

/*
*	This is going to be used to pop items from a queue
*/
void* pop(queue *queue);

/*
*	 This method inserts a new item into an already initialized Queue
*/
queue* insertItem(queue* queue, bookOrder* item);

/*
*	Determines whether the person has enough credit to purchase the book, and then decides what list to add the purchase
*	to. The accepted purchase or the Rejected list.
*/
queue* insertOrder(customer* customer, bookOrder *newNode, int list);

/*
*	The Queue is on the heap, so in order to not have memory leaks freeQueue frees each individual node, then the Queue itself.
*/ 	
void freeQueue(queue* queue);