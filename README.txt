Creators: Jaime Reynoso, Kaitlyn Sussman
Class: System's Programming - CS214
Project name: PA5 - Multi-Threaded Book Order System

Project Specification:

	Our design was based off the consumer-producer problem in Chapter 12 of the textbook. 
	The producer thread takes in an order file, containing each order on a line and parses the file to extract the 
	book name, the price, the customer id and the category that the book pertains to. Since we can't have multiple threads
	accessing the same queue, we created a semaphore to lock the data object whenever someone is extracting or inserting a book order. In order to know when the queue is full, we also included a semaphore to keep track of the number of items in the queue, as well as the number of slots. If the number of slots is at 0, the semaphore will halt the threads, allowing the consumer thread to extract the book orders. 
	The consumer thread is also simple, it takes in a queue, extracts the order, then it uses the findCustomer function to find the customer. Once the customer is found, we check their balance. If the balance is below the amount necessary then the book order gets added to the failed order linked list inside the customer object. If the customer has enough, then the price gets debitted, the book order keeps track of the credit balance after the book price was debitted(this is for the final report), and the book goes into the customer's successful order linked list. 
	Due to the fact that the customer is going to have information changed during the consumer thread function, we added a semaphore to the customer. This semaphore gets locked within the consumer thread whenever the consumer is using the customer, this way you avoid errors from multiple consumer threads attempting to edit one customer. 
	Most of the functions, other than the producer and consumerThread functions are helper methods that just initialize or destroy a certain data object, but the populateCustomer() function serves the purpose of retrieving the customer information from the file, creating a customer data struct with the correct information for each customer and then inserting the customer into hash_cell. 

	Data Structs

		One of our data struct was the bookOrder. The bookOrder struct contains the title, category, price, customer ID, a link to another book order, and a remaining balance. Due to the fact that we needed to maintain a list of successful and failed book orders, we have a pointer to another book order inside the book order. This way, we can maintain a linked list of successful and failed bookorder for each customer.

		The Queue struct contains an array of bookOrders, a category, a size, 2 ints to keep track of the first and last item, and 3 semaphores. The category helps the producer thread insert the book order into the right queue, the size helps me make sure there's less than 5 bookOrders in a queue at all times. The 3 semaphores help me make sure the threads don't access the same queue at once or that the producer thread doesn't insert more than 5 bookOrders into a queue at the same time. 

		The Customer Struct contains a mutex, so that multiple consumer threads don't access the same customer at the same time, as well as the customer information and a link to the successful and failed bookOrders. 

		The Hash_cell struct is going to be used to retrieve a customer by their customer ID. The Hash cell contains an id, a customer and a UT_hash_handle. The UT_hash_handle is crucial in having the UTHASH data struct retrieve the right customer, and everything else in this struct is self-explanatory. 