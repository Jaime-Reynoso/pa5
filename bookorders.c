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



void readBookOrders()
{
	//reads in book orders and puts in queue determined by category (producer)
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

	//create thread for each category

	return 0;
}