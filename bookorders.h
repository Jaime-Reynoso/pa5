#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
*	Customer Struct contains name, customer ID. balance, address, state. zip and a list of both purchased
*	and rejected books
*/
struct customer{
	char* name;
	char* custID;
	float balance;
	char* address;
	char* state;
	char* zip;
	queue* list_purchased;
	queue* list_rejected;
};

typedef struct customer customer;


/*
*	Node struct goes into the queue and contains the book information
*/
struct node{
	char* title;
	char* price;
	char* custID;
	char* category;
	float remainingBalance;
	struct node* next;
};

typedef struct node node;

/*
*	A queue contains the size of the queue, a link to the beginning and a link to the end Queue
*/
struct queue{
	node* front;
	node* end;
	int size;
};

typedef struct queue queue;

/*
*	This method initializes  the Queue by giving it a size of 0 and sets both the nodes equal to NULL
*/
queue* initializeQueue();

/*
*	This method adds the books into the Queue
*/
queue* createQueue(FILE* order);

/*
*	 This method inserts a new item into an already initialized Queue
*/
queue* insertItem(queue* queue, node* item);

/*
*	Determines whether the person has enough credit to purchase the book, and then decides what list to add the purchase
*	to. The accepted purchase or the Rejected list.
*/
queue* insertOrder(customer* customer, node *newNode, int list);

/*
*	The Queue is on the heap, so in order to not have memory leaks freeQueue frees each individual node, then the Queue itself.
*/ 	
void freeQueue(queue* queue);