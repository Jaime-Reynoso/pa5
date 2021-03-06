#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "uthash.h"
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX 5

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
	struct bookOrder* success_order;
	struct bookOrder* fail_order;
};

typedef struct customer customer;

/*
*	I created a separate struct that contains the customer id and the customer itself, this way i can use UTHASH
*	to find the customer 
*/

typedef struct hash_cell{
	int customer_ID;
	customer *cust;
	UT_hash_handle hh;
	sem_t mutex;
}hash_cell;


/*
*	This queue can't be accessed by multiple threads, and it can't have more than a max amount of book orders
*	or the thread will have too many book orders to handle, so I have 3 semaphores
*	one to keep track of how many available slots there are, one to keep track of the items and one 
*	to lock the queue when it's in use. The size is important because it's an int and the position of the 
*	first and last item along with the book order that it contains. 
*/
struct queue{
	struct bookOrder** cat_orders;
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
	int customer_ID;
	char *category;
	float remaining_Balance;
	struct bookOrder *next;
};
typedef struct bookOrder bookOrder;



void *producerThread(FILE* something);
void *consumerThread(queue* queue);
void initializeBookStruct(bookOrder *pointer);
void printFinalReport(FILE* finalDatabase);
void initializeQueue(queue* temp_queue, char* category);
void insertBookOrder(queue *order_cont, bookOrder **book);
bookOrder* removeBookOrder(queue *temp_order);
void populateCustomerDatabase(FILE* customer_database);
void addCustomer(customer* customerI, int customerID);
hash_cell *findCustomer(int customerID);
customer *findCustomerFinal(int customerID);
void delete_all();
void freeCustomerBookOrder(bookOrder* order);