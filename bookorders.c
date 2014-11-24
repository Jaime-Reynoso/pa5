#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bookorders.h"

//adds character to end of string
char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a), b);
}

//assigns values to customer
customer **readDatabase(FILE *database)
{
	char c;
	char *string = (char*)malloc(sizeof(char));
	string = "";
	c = getc(database);
	
	while (c != EOF)
	{ 
		string = stradd(string, c);
		c = getc(database);
	}

	if (strlen(string) == 0)
		return NULL;

	customer **customerArr = (customer**)malloc(200 * sizeof(customer*));

	char *token, *hold, *sep;
	hold = NULL;
	sep = "|\n";
	token = strtok_r(string, sep, &hold);
	
	while (token != NULL)
	{
		customer *newCust = (customer*)malloc(sizeof(customer));
		newCust->name = token;
		token = strtok_r(NULL, sep, &hold);
		newCust->custID = token;
		token = strtok_r(NULL, sep, &hold);
		newCust->balance = atoi(token);
		token = strtok_r(NULL, sep, &hold);
		newCust->address = token;
		token = strtok_r(NULL, sep, &hold);
		newCust->state = token;
		token = strtok_r(NULL, sep, &hold);
		newCust->zip = token;
		
		newCust->list_purchased = initQueue();
		newCust->list_rejected = initQueue();
		
		customerArr[getIndex(newCust->custID)] = newCust;
		token = strtok_r(NULL, sep, &hold);
	}
	return customerArr;
}

/*
*	This is the function for the producer thread
*/

void readBookOrders(File *orders)
{
	char order_temp[256];
	bookOrder *head = NULL;
	bookOrder *temp = NULL;
	initializeBookStruct(head);
	initializeBookStruct(temp);
	char token_delim[2] = "\"|";
	char* token;
	int counter = 0;

	if(orders == NULL) perror("Sorry, but the file seems to be null");
	else{

		temp = head;
		while(!feof(orders)){
			fgets(order_temp, 256, orders);
			token = strtok(order_temp, token_delim);
			temp = (bookOrder*) malloc(sizeof(bookOrder));

			while(token != NULL){
				counter++;
				switch(counter){
					case 1:
						temp -> title = token;
					case 2:
						temp -> price = atof(token);
					case 3:
						temp -> customer_ID = atoll(token);
					case 4:
						temp -> category = token;
					default:
						perror("There seems to have been a problem extracting the order");
				}
				
			}
			counter = 0;
			temp = head -> next;

		}
	}


}
/*
*	This is just a method to initialize a book order 
*/

void initializeBookStruct(bookOrder *pointer){
	pointer->title = NULL;
	pointer->price = 0;
	pointer->customer_ID = 0;
	pointer->category = NULL;
	pointer->next = NULL;
}

//function that each thread calls, processing the orders for their individual category
void processBookOrders()
{
	//initialize mutex
	//extracts book order threads for its category (consumer)
	//one consumer per category
	//prints individual orders when processed
	//uses customer database
	//gets customer from customer array
	//adds info to either purchased list or rejected list
}


//gets the index of the customer id from the customer array
int getCustID()
{

}

void printFinalReport(customer** customerArray)
{

}

void addToQueue(char* category)
{
	//adds book order to queue
}

void freeCustomers(customer** customerArray)
{
	int i;
	for (i = 0; i < 200; i++)
		if (customerArray[i] != NULL)
			free(customerArray[i]);
}

int main(int argc, char* argv[])
{
	pthread_t tid; //the thread identifier 
	pthread_mutex_init(&mutex, NULL);
	//pthread_attr_t attr; //the set of attributes for the thread

	//pthread_attr_init(&attr); //get the default attributes
	//pthread_create(&tid, &attr, thread, )

	int totalRevenueProduced, numCategories = 0;
	totalRevenueProduced = 0;
	char* categories;
	customer **customerArray;
	queue* queue;

	FILE* database = fopen(argv[1], "r");
	FILE* orders = fopen(argv[2], "r");

	if (database == NULL || orders == NULL) 
	{
		printf("Error opening files");
		return 1;
	}

	FILE* f_categories = fopen(argv[3], "r");
	
	customerArray = readDatabase(database);

	if (customerArray == NULL)
		printf("error\n");

	queue = createQueue(orders);

	categories = f_categories;

	//reads categories into categories string
	char buffer[100];
	int i;
	while (fgets(buffer, sizeof(buffer), f_categories)) 
	{
		categories[i] = buffer;
		numCategories++;
	}
	fclose(f_categories);

	for(i=0; i<numCategories; i++)
	{
		pthread_create(&tid, 0, processBookOrders, 0);
	}
	pthread_exit(0);

	return 0;
}