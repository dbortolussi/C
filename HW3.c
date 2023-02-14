#include<stdio.h>
#include"buffer.h"
#include<pthread.h>

//create buffers buffers
struct buffer b12;
struct buffer b23;
struct buffer b34;


void* readInput(void *argv){ 
	//printf("reading\n");
	char c = getchar();
	while(c != EOF){
		deposit(&b12,c);
		c = getchar();
	}
	deposit(&b12,EOF);
	}
	
void* checkNewline(void *argv){
	//printf("checking newline\n");
	char c = remoove(&b12);
	while(c != EOF) {
		if(c == '\n'){
			deposit(&b23,' ');
		} else {
			deposit(&b23,c);
		}
		c = remoove(&b12);
	}
	deposit(&b23,EOF);
}

void* checkStar(void *argv){
	//printf("checking star\n");
	char c = remoove(&b23);
	while(c != EOF) {
		if(c == '*'){
			char d = remoove(&b23);
			if(d==EOF){
				deposit(&b34,c);
				break;
			}
			else if(d == '*'){
				deposit(&b34,'^');
			}
			else{
				deposit(&b34,c);
				deposit(&b34,d);
			}
		}
		else{
			deposit(&b34,c);
		}
		c = remoove(&b23);
	}
	deposit(&b34,EOF);
}

void* output(void *argv){
	//printf("outputting\n");
	char c = remoove(&b34);
	int strlength = 0;
	char string[81] = "                                                                                ";
	while(c != EOF){
		printf("%c",c);
		if(strlength<80){
			string[strlength] = c;
			strlength++;
		}
		else{
			printf("%s\n",string);
			strlength = 0;
		}
		c = remoove(&b34);
	}
}

int main(){
	
	//create threads
	pthread_t thread1, thread2, thread3, thread4;
	
	//initialize buffers
	initialize(&b12);
	initialize(&b23);
	initialize(&b34);
	
	//initialize threads
	pthread_create(&thread1,NULL,readInput,NULL);
	pthread_create(&thread2,NULL,checkNewline,NULL); 
	pthread_create(&thread3,NULL,checkStar,NULL);
	pthread_create(&thread4,NULL,output,NULL);
	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	
	//destroy buffers
	destroySemaphore(&b12);
	destroySemaphore(&b23);
	destroySemaphore(&b34);
	
	return 0;
}