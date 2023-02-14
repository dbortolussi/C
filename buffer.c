#include<semaphore.h>
#include "buffer.h"
#include<sys/mman.h>
#include<stdio.h>
//actually write the methods that are in the .h file

//these methods take the buffer as an argument
//method to initialize

void initialize(struct buffer* b){
	b->nextOut = 0;
	b->nextIn = 0;
	sem_init(&b->empty,0,4096);
	sem_init(&b->full,0,0);
	
	int fd = fileno(b->file);
	
	b->file = fdopen(fd,"r+");
	
	b->f = mmap(0,sizeof(b->file),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
}

//method to destroy
void destroy(struct buffer* b){
	sem_destroy(&b->empty);
	sem_destroy(&b->full);
	munmap(0,sizeof(b->file));
}

//put method
void deposit(struct buffer* b, char c){
	sem_wait(&b->empty);
	(b->f)[b->nextIn] = c;
	b->nextIn = (b->nextIn+1) % 4096;
	sem_post(&b->full);
}

//get method
char remoove(struct buffer* b){
	sem_wait(&b->full);
	char c = b->f[b->nextOut];
	(b->f)[b->nextOut] = '\0';
	b->nextOut = (b->nextOut+1) % 4096;
	sem_post(&b->empty);
	return c;
}