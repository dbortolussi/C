#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
//define struct and list whats in it

struct buffer{
	sem_t empty;
	sem_t full;
	
	int nextOut;
	int nextIn;
	
	FILE* file;
	
	char* f;
};

//these methods take the buffer as an argument
//method to initialize
void initialize(struct buffer* b);
//method to destroy
void destroy(struct buffer* b);
//put method
void deposit(struct buffer* b,char c);
//get method
char remoove(struct buffer* b);