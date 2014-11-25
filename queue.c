#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include "bookorders.h"

queue* initializeQueue()
{
	queue* newQueue = (queue*)malloc(sizeof(queue));
	newQueue->front = NULL;
	newQueue->end = NULL;
	newQueue->size = 0;

	return newQueue;
}

queue* createQueue(FILE* order)
{
	//builds queue from order file
	//reads line and assigns new bookOrder values from each token
	
	queue* newQueue;
	newQueue = initializeQueue();
	
	char c;
	char *string = (char*)malloc(sizeof(char));
	string = "";
	c = getc(order);

	while (c != EOF)
	{
		string = stradd(string, c);
		c = getc(order);
	}

	if (strlen(string) == 0)
		return NULL;

	char *token, *hold, *sep;
	hold = NULL;
	sep = "|\n";
	token = strtok_r(string, sep, &hold);

	while (token != NULL){ //create orderbookOrders from the string

		bookOrder *newbookOrder = (bookOrder*)malloc(sizeof(bookOrder));
		newbookOrder->title = token;
		token = strtok_r(NULL, sep, &hold);
		newbookOrder->price = token;
		token = strtok_r(NULL, sep, &hold);
		newbookOrder->custID = token;
		token = strtok_r(NULL, sep, &hold);
		newbookOrder->category = token;
		token = strtok_r(NULL, sep, &hold);
		newbookOrder->remainingBalance = 0;
		newbookOrder->next = NULL;

		//newQueue = insertItem(newQueue, newbookOrder);

		insertItem(newQueue, newbookOrder);
		
		/*if (newQueue->front == NULL){ //first bookOrder
			newQueue->front = newbookOrder;
			newQueue->end = newbookOrder;
			newQueue->size++;
		}
		else { // not the first bookOrder
			newQueue->end->next = newbookOrder;
			newQueue->end = newbookOrder;
			newQueue->size++;
		}*/
	}
	return newQueue;

}

void insertItem(queue* queue, bookOrder* item)
{
	bookOrder* newbookOrder;

	if (item == NULL)
	{
		printf("insert failed");
		//return NULL;
	}

	/*if (newQueue->front == NULL){ //first bookOrder
	newQueue->front = newbookOrder;
	newQueue->end = newbookOrder;
	newQueue->size++;
	}
	else { // not the first bookOrder
	newQueue->end->next = newbookOrder;
	newQueue->end = newbookOrder;
	newQueue->size++;
	}*/

	newbookOrder = item;
	newbookOrder->next = NULL;

	if (queue == NULL)
	{
		printf("queue not initialized");
		free(newbookOrder);
		//return queue;
	}
	else if (queue->front == NULL && queue->end == NULL) //fist bookOrder added 
	{
		queue->front = queue->end = newbookOrder;
		queue->size++;
	}
		
	else //adding to existing list
	{
		queue->end->next = newbookOrder;
		queue->end = newbookOrder;
		queue->size++;
	}
		
	//return queue;
}

queue* insertOrder(customer* customer, bookOrder* newbookOrder, bool acceptedOrder)
{

	//queue* acceptedQueue;
	//queue* rejectedQueue;

	if (acceptedOrder) //add to purchased list
	{
		insertItem(customer->list_purchased, newbookOrder);
		return customer->list_purchased;
	}
	else //add to rejected list
	{
		insertItem(customer->list_rejected, newbookOrder);
		return customer->list_rejected;
	}

	
}

/*queue* deleteItem(queue* queue)
{
	if (queue == NULL)
	{
		printf("list is empty\n");
		return queue;
	}
	else if (queue->front == NULL && queue->end == NULL)
	{
		printf("list is empty\n");
		return queue;
	}
	else
	{
		//value = queue->first->data
		bookOrder* temp = queue->front;
		queue->front = queue->front->next;
		free(temp);

		//return value
	}

}

bool isEmpty(queue* queue)
{
	if (queue->size = 0)
		return true;

	return false;
}*/

void freeQueue(queue* queue)
{
	int count = queue->size;
	bookOrder* ptr;
	bookOrder* prev;
	ptr = queue->front;
	prev = NULL;
	int i;
	for (i = 0; i < count; i++)
	{
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
}