#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bookorders.h"
#include <semaphore.h>

queue **queue_array;
hash_cell *customer_database;

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

	char buffer[100];
	
	while ((fgets(buffer, 100, database)) != NULL)
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

void producerThread(File *orders)
{

	printf("Processor Thread has begun processing\n");
	char order_temp[256];
	bookOrder *temp = NULL;
	initializeBookStruct(temp);
	char token_delim[2] = "\"|";
	char* token;
	int counter;

	if(queue_array == NULL){
		perror("There was an error initializing the queues");
	}
	else{

		while(!feof(orders)){
			fgets(order_temp, 256, orders);
			token = strtok(order_temp, token_delim);
			temp = (bookOrder*) malloc(sizeof(bookOrder));

			counter = 0;
			while(token != NULL)
			{
				counter++;
				switch(counter)
				{
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
				token = strtok(order_temp, token_delim);
			}
			temp->next = NULL;
			int count_cat;
			for(count_cat = 0; count_cat < sizeof(queue_array)/sizeof(queue*); count_cat++ )
			{
				if(strcmp(queue_array[count_cat]->category, temp->category) == 0)
				{
					insertBookOrder(queue_array[count_cat], temp);
				}
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

	queue* queue;
	customer* temp_customer;
	bookOrder* tempOrder = queue->cat_orders;
	int q_size = queue->size;

	while(q_size != 0)
	{
		if(strcmp(category, tempOrder->category)) //order matches thread's category
		{
			//find customer in customer array
			temp_customer = customerArray[tempOrder->customer_ID % 100]
			if(temp_customer->balance > tempOrder->price) //atof()??
			{
				bookOrder* purchased_order = malloc(sizeof(bookOrder));
				purchased_order->title = tempOrder->title;
				purchased_order->price = tempOrder->price;
				purchased_order->customer_ID = tempOrder->customer_ID;
				purchased_order->category = tempOrder->category;
				purchesed_order->remainingBalance = tempOrder->remainingBalance;
				temp_customer->list_purchased = insertOrder(temp_customer, purchased_order, true);
				totalRevenueProduced += tempOrder->price; //atof()??
				printf("Order Successful:\nTitle: %s \nPrice: %s \nName: %s\n Address: %s %s %s", purchased_order->title, purchased_order->price, temp_customer->name, temp_customer->address, temp_customer->state, temp_customer->zip);
			} else
			{
				bookOrder* rejected_order = malloc(sizeof(bookOrder));
				purchased_order->title = tempOrder->title;
				purchased_order->price = tempOrder->price;
				purchased_order->customer_ID = tempOrder->customer_ID;
				purchased_order->category = tempOrder->category;
				purchesed_order->remainingBalance = tempOrder->remainingBalance;
				temp_customer->list_rejected = insertOrder(temp_customer, rejected, false);
				printf("Order Rejected:\nTitle: %s \nPrice: %s \nName: %s\n Address: %s %s %s", purchased_order->title, purchased_order->price, temp_customer->name, temp_customer->address, temp_customer->state, temp_customer->zip);
			}
		}
		tempOrder = tempOrder->next;
		q_size--;
	}
	
	pthread_mutex_unlock(&lock);
}

void printFinalReport(customer** customerArray)
;
				while(customerArray[i]->list_purchased->size >0)
				{
					printf("%s, %s, %f \n", bookOrderPtr->title, bookOrderPtr->price, bookOrderPtr->remainingBalance);
					bookOrderPtr = bookOrderPtr->next;
					customerArray[i]->list_rejected->size--;
				}
			}
		}

	}

	printf("Total Revenue Produced: %f\n, totalRevenueProduced");
}


/*
*	This will initialize a queue 
*/
void initializeQueue(queue* temp_queue, char* category)
{
	temp_queue->cat_orders = calloc(MAX, sizeof(bookOrder));
	temp_queue->category = calloc(strlen(category), sizeof(char));
	strncpy(temp_queue->category ,category,strlen(category)); 
	temp->size = MAX;
	temp->position_of_last_item = temp->position_of_first_item = 0;
	Sem_init(&temp_queue->mutex, 0, 1);
	Sem_init(&temp_queue->slots, 0, MAX);
	Sem_init(&temp_queue->items, 0, 0);
}

/*
*	This will insert a book node into the designated queue
*/

void insertBookOrder(queue *order_cont, bookOrder book){
	sem_wait(&order_cont->slots);
	sem_wait(&order_cont->mutex);
	order_cont->buffer[(++order_cont->position_of_last_item)%(order_cont->size)] = book;
	sem_post(&order_cont->mutex);
	sem_post(&order_cont->items);
}

/*
*	This will retrieve a book from a designated queue
*/

bookOrder removeBookOrder(queue *temp_order){
	bookOrder item;
	sem_wait(&temp_order->items);
	sem_wait(&temp_order->mutex);
	item = temp_order->cat_orders[(++temp_order->position_of_first_item)%(temp_order->size)];
	sem_post(&temp_order->mutex);
	sem_post(&temp_order->slots);
	return item;
}


void freeCustomers(customer** customerArray)
{
	int i;
	for (i = 0; i < 200; i++)
		if (customerArray[i] != NULL)
			free(customerArray[i]);
}

void populateCustomerDatabase(File *customer_database){

	char customer_temp[256];
	customer *individual_customer;
	char *customer_delim = "\",|";
	char *token;
	int i, customer_id;

	while(!feof(customer_database)){
		fgets(customer_temp, 256, customer_database);
		token = strtok(customer_temp, customer_delim);
		i = 0;

		while(token != NULL){
			i++;
			individual_customer = malloc(sizeof(customer));

			switch(i){
				case 1:
					strcpy(individual_customer->name, token);
				case 2:
					customer_id = atoll(token);
				case 3:
					individual_customer->balance = atof(token);
				case 4:
					strcpy(individual_customer->address, token);
				case 5:
					strcpy(individual_customer->city, token);
				case 6:
					strcpy(individual_customer->state, token);
				case 7:
					strcpy(individual_customer->zip, token);
				default:
					perror("Error populating the individual customer");
			}
			token = strtok(customer_temp, customer_delim);
		}
		individual_customer->success_order = NULL;
		individual_customer->fail_order = NULL;

		addCustomer(individual_customer, customer_id);
	}
}

void addCustomer(customer* customerI, int customerID){
	hash_cell *temp;

	HASH_FIND_INT(customer_database, &customerID, temp);
	if(temp == NULL){

		temp = malloc(sizeof(hash_cell));
		temp->customer_ID = customerID;
		HASH_ADD_INT(customer_database, id, temp);
	}
	temp->cust = customerI;
}

void delete_all() {
  hash_cell *current_user, *tmp;

  HASH_ITER(hh, customer_database, current_user, tmp) {
    HASH_DEL(customer_database,current_user);  /* delete it (users advances to next) */
    free(current_user->cust); 
    free(current_user);           /* free it */
  }
}

customer *findCustomer(int customerID)
{
	hash_cell *tmp;

	HASH_FIND_INT(customer_database, &customerID, tmp);
	return tmp->cust;
}

int main(int argc, char* argv[])
{
	if(argc!= 4){
		printf("Error: Incorrect Input \n");
		printf("Correct Arguments: ./bookorders [arg1] [arg2] [arg3]\n");
		printf("Arg1- The name of the database input file \nArg2 - The name of the book order input file\n");
		printf("Arg3 - The name of the category input file\n");
		exit();
	}

	if((File *categories = fopen(argv[3], "r")) == NULL) perror("Couldn't open the category file");
	if((File *orders = fopen(argv[2], "r")) == NULL) perror("Couldn't open the order");
	if((File *customer_database = fopen(argv[1], "r"))==NULL) perror("Couldn't open database");

	queue_array = malloc(sizeof(queue*));

	char category[64];
	int i;
	for(i = 1 ; !feof(categories) ; i++){

		queue_array = (queue **) realloc(sizeof(queue*)*i);
		fgets(category, 64, categories);
		initializeQueue(queue_array[i-1], category);
	}

	if(queue_array == NULL) perror("The Queue Array seems to be null");

	int totalRevenueProduced, numCategories = 0;
	totalRevenueProduced = 0;
	
	numCategories = i;

	for(i=0; i<numCategories; i++)
	{
		pthread_create(&tid, 0, processBookOrders, 0);
	}
	pthread_exit(0);

	return 0;
}