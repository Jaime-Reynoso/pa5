#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bookorders.h"
#include <semaphore.h>
#include <pthread.h>

queue **queue_array;
hash_cell *customer_database;
void producerThread(File* something);
void consumerThread(queue* queue);

//adds character to end of string
char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a), b);
}

/*
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
*/

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
	pointer->remaining_Balance = 0;
	pointer->next = NULL;
}

//function that each thread calls, processing the orders for their individual category

void consumerThread(queue* queue)
{

	customer *temp_customer = NULL;
	bookOrder *tempOrder = removeBookOrder(queue);
	bookOrder *traversal_order = NULL;

	while(tempOrder != NULL)
	{
		temp_customer = findCustomer(tempOrder->customer_ID);
		sem_wait(&temp_customer->mutex);
		if(temp_customer->balance >= tempOrder->price){
			temp_customer->balance = temp_customer->balance - tempOrder->price;
			tempOrder->remaining_Balance = temp_customer->balance;
			traversal_order = temp_customer->success_order;
			while(traversal_order != NULL){
				traversal_order = traversal_order->next;
			}
			traversal_order = tempOrder;
		}
		else{
			traversal_order = temp_customer->fail_order;
			while(traversal_order != NULL){
				traversal_order = traversal_order->next;
			}
			traversal_order = tempOrder;
		}
		sem_post(&temp_customer->mutex);
		tempOrder = removeBookOrder(queue);
	
	}
	
}

void printFinalReport(File* finalDatabase)
{
	int i;
	customer* traversal_customer;
	bookOrder* traversal_order;
	float totalRevenueProduced = 0;

	for(i = 1; findCustomer(i) != NULL; i++){
		traversal_customer = findCustomer(i);
		fputs("== BEGIN CUSTOMER INFO ==\n", finalDatabase);
		fputs("### BALANCE ###\n", finalDatabase);
		fprintf(finalDatabase, "Customer Name: %s \n", traversal_customer->name);
		fprintf(finalDatabase, "Customer ID Number %d \n", traversal_customer->success_order->customer_ID);
		fprintf(finalDatabase, "Remaining Credit Balance after all purchases( a dollar amount) %4.2f", traversal_customer->balance);
		fprintf(finalDatabase, "### SUCCESSFUL ORDERS ### \n");
		traversal_order = traversal_customer->success_order;
		while(traversal_order != NULL){
			fprintf(finalDatabase, "\"%s\"|%4.2f|%4.2f \n", traversal_order->title, traversal_order->price, traversal_order->remaining_Balance);
			totalRevenueProduced += (traversal_order->price);
			traversal_order = traversal_order->next;
		}
		fprintf(finalDatabase, "### REJECTED ORDERS ### \n");
		traversal_order = traversal_customer->fail_order;
		while(traversal_order != NULL){
			fprintf(finalDatabase, "\"%s\"|%4.2f\n", traversal_order->title, traversal_order->price);
		}
		fprintf(finalDatabase, "== END CUSTOMER INFO == \n \n \n");

	}	

	fprintf(finalDatabase, "Total Revenue from all purchases: %4.2f", totalRevenueProduced);		

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

			sem_init(&individual_customer->mutex, 0, 1);
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
	if((File *finalReport = fopen(finalReport.txt, "+w")) == NULL) perror("Couldn't create the final report");

	queue_array = malloc(sizeof(queue*));

	char category[64];
	int i;
	for(i = 1 ; !feof(categories) ; i++){

		queue_array = (queue **) realloc(sizeof(queue*)*i);
		fgets(category, 64, categories);
		initializeQueue(queue_array[i-1], category);
	}

	if(queue_array == NULL) perror("The Queue Array seems to be null");

	populateCustomerDatabase(customer_database);
	int number_of_threads = sizeof(queue_array)/sizeof(queue*);

	pthread_t thread[number_of_threads+1];

	for(i = 0; i <= number_of_threads;i++){
		if(i = 0){
			pthread_create(&thread[i], NULL, producerThread, orders);
		}
		else{
			pthread_create(&thread[i], NULL, consumerThread, queue_array[i-1]);
		}
	}
	for(i = 0;i <= number_of_threads; i++){
		pthread_join(thread[i], NULL);
	}

	return 0;
}