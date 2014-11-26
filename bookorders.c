#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bookorders.h"
//M: -> mauricio comments

//M: missing mutex definitions, semaphores/conditional mutexes
//M: mutex --> locks a global variable when a thread is working on it
//M: semaphores/conditional mutexes --> keeps track of the state of the buffer
//M: if buffer is full call consumer threads, if buffer is empty call producer thread

queue **queue_array;
FILE *temp_order;


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
	//M: mutex definition should be global so that functions could share the same mutex, not local
	//M: This function needs a mutex as well since it modifies the buffer

	printf("Processor Thread has begun processing\n");
	char order_temp[256];
	//bookOrder *head = NULL;
	bookOrder *temp = NULL;
	//initializeBookStruct(head);
	initializeBookStruct(temp);
	char token_delim[2] = "\"|";
	char* token;
	int counter;

	if(orders == NULL){
		printf("Orders have been completely processed\n");
	}
	else{

		temp_order = fopen("temporaryOrder.txt", "w+");
		//temp = head;
		while(!feof(orders)){
			fgets(order_temp, 256, orders);
			token = strtok(order_temp, token_delim);
			temp = (bookOrder*) malloc(sizeof(bookOrder));

			counter = 0;
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
				int count_cat;
				for(count_cat = 0; count_cat < sizeof(queue_array)/sizeof(queue*); count_cat++ ){
					if(strcmp(queue_array[count_cat]->category, temp->category) == 0){
						if(queue_array[count_cat]->front == NULL){
						 queue_array[count_cat]->front = temp;
						}
						else{
							if(queue_array[count_cat]->size <= 5){
								temp->next = queue_array[count_cat]->front;
								queue_array[count_cat]->front = temp;
								queue_array[count_cat]->size++;
							}
							else{
								fputs(order_temp, temp_order);
							}

						}
					}
				}


			}

		}

	/*
	*	Here I'm deleting the old file name with all the orders and setting the File Pointer equal to the new file
	*	with the orders that haven't been completed yet
	*/
	char *fileName;
	fileName = recover_filename(orders);
	unlink(fileName);
	rename(temp_order, filename);
	orders = temp_order;
	temp_order = NULL;

	}

}
/*
*	This is a method to recover the filename from the pointer
*/
char * recover_filename(FILE * f) { 
  int fd; 
  char fd_path[255]; 
  char * filename = malloc(255); 
  ssize_t r;

  fd = fileno(f); 
  sprintf(fd_path, "/proc/self/fd/%d", fd); 
  r = readlink(fd_path, filename, 255); 
  filename[r] = '\0';
  return filename; 
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
void processBookOrders(char* category)
{
	//initialize mutex
	//M: mutex definition should be global so that functions could share the same mutex, not local
	//M: mutex should not wrap the whole function since it hogs resources, it should only wrap the part
	//M: where the buffer is altered
	pthread_mutex_t lock;
	pthread_mutex_lock(&lock);

	queue* queue;
	customer* temp_customer;
	bookOrder* tempOrder = queue->front;
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
{
	//print node by node each element in queue until empty

	bookOrder* bookOrderPtr;
	int i;
	for(i=0; i<100; i++)
	{
		if(customerArray[i] != NULL)
		{
			printf("Customer Name: %s \nCustomer ID: %s \nRemaining Balance: %f \n", customerArray[i]->name, customerArray[i]->custID, customerArray[i]->balance);
			printf("Successful Orders: \n");
			if(customerArray[i]->list_purchased->size >0)
			{
				bookOrderPtr = customerArray->list_purchased->front;
				while(customerArray[i]->list_purchased->size >0)
				{
					printf("%s, %s, %f \n", bookOrderPtr->title, bookOrderPtr->price, bookOrderPtr->remainingBalance);
					bookOrderPtr = bookOrderPtr->next;
					customerArray[i]->list_purchased->size--;
				}
			}

			printf("Rejected Orders: \n");
			if(customerArray[i]->list_rejected->size >0)
			{
				bookOrderPtr = customerArray->list_rejected->front;
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

void addToQueue(char* category)
{
	//adds book order to queue
}

queue* initializeQueue(char* category){
	queue* temp_queue;
	temp_queue->front = NULL;
	temp_queue->category = category;
	temp_queue->size = 0;
	return temp_queue;
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
	if(argc!= 4){
		printf("Error: Incorrect Input \n");
		printf("Correct Arguments: ./bookorders [arg1] [arg2] [arg3]\n");
		printf("Arg1- The name of the database input file \nArg2 - The name of the book order input file\n");
		printf("Arg3 - The name of the category input file\n");
		exit();
	}

	File *categories = fopen(argv[3], "r");

	queue_array = malloc(sizeof(queue*));

	char category[64];
	int
	for(i!feof(categories)){

		queue_array = realloc(sizeof(queue*)*i);
		queue_array[i] = (queue*) malloc(sizeof(queue));
		fgets(category, 64, categories);
		queue_array[i] = initalizeQueue(category);
	}



	//M: from my understanding you need multiple consumer threads and 1 producer thread
	//M: I dont see them initialized anywhere in main

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