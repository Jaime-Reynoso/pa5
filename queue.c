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
	//reads line and assigns new node values from each token
	
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

	while (token != NULL){ //create orderNodes from the string

		node *newNode = (node*)malloc(sizeof(node));
		newNode->title = token;
		token = strtok_r(NULL, sep, &hold);
		newNode->price = token;
		token = strtok_r(NULL, sep, &hold);
		newNode->custID = token;
		token = strtok_r(NULL, sep, &hold);
		newNode->category = token;
		token = strtok_r(NULL, sep, &hold);
		newNode->remainingBalance = 0;
		newNode->next = NULL;

		//newQueue = insertItem(newQueue, newNode);

		insertItem(newQueue, newNode);
		
		/*if (newQueue->front == NULL){ //first node
			newQueue->front = newNode;
			newQueue->end = newNode;
			newQueue->size++;
		}
		else { // not the first node
			newQueue->end->next = newNode;
			newQueue->end = newNode;
			newQueue->size++;
		}*/
	}
	return newQueue;

}

void insertItem(queue* queue, node* item)
{
	node* newNode;

	if (item == NULL)
	{
		printf("insert failed");
		//return NULL;
	}

	/*if (newQueue->front == NULL){ //first node
	newQueue->front = newNode;
	newQueue->end = newNode;
	newQueue->size++;
	}
	else { // not the first node
	newQueue->end->next = newNode;
	newQueue->end = newNode;
	newQueue->size++;
	}*/

	newNode = item;
	newNode->next = NULL;

	if (queue == NULL)
	{
		printf("queue not initialized");
		free(newNode);
		//return queue;
	}
	else if (queue->front == NULL && queue->end == NULL) //fist node added 
	{
		queue->front = queue->end = newNode;
		queue->size++;
	}
		
	else //adding to existing list
	{
		queue->end->next = newNode;
		queue->end = newNode;
		queue->size++;
	}
		
	//return queue;
}

queue* insertOrder(customer* customer, node* newNode, bool acceptedOrder)
{

	//queue* acceptedQueue;
	//queue* rejectedQueue;

	if (acceptedOrder) //add to purchased list
	{
		insertItem(customer->list_purchased, newNode);
		return customer->list_purchased;
	}
	else //add to rejected list
	{
		insertItem(customer->list_rejected, newNode);
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
		node* temp = queue->front;
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
	node* ptr;
	node* prev;
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