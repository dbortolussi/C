//Daniel Bortolussi
//I have neither given nor received unauthorized help on this assignment

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <stdbool.h>

#include "Socket.h"
#include "shell.h"

int main(int argc, char* argv[]){
	
	int i,c,rc;
	int count = 0;
	
	char line_data[MAX_LINE];
	
	Socket connect_socket;
	
	if (argc < 3) {
		printf("No host and port\n");
		return(-1);
	}
	
	connect_socket = Socket_new(argv[1], atoi(argv[2]));
	
	if(connect_socket < 0){
		printf("Failed to connect to server\n");
		return(-1);
	}
	
	printf("%s","% ");
	
	while(fgets(line_data, sizeof(line_data), stdin) != NULL){
		
		count = strlen(line_data) + 1;
		
		//send input line to server
		for(i = 0;i < count;i++){
			c = line_data[i];
			rc = Socket_putc(c, connect_socket);
			if(rc == EOF){
				printf("Socket_putc EOF or error\n");
				Socket_close(connect_socket);
				exit(-1);
			}
		}
		
		c = Socket_getc(connect_socket);
		
		int n = 0;
		
		//get command output from server
		while(c != '\0'){
			if(n >= MAX_LINE){
				line_data[n] = '\0';
				printf("%s",line_data);
				printf("Maximum character amount reached\n");
				break;
			}
			if(c == EOF){
				printf("Socket_getc EOF or error\n");
				Socket_close(connect_socket);
				exit(-1);
			}
			else{
				line_data[n] = c;
			}
			c = Socket_getc(connect_socket);
			n++;
		}
		
		line_data[n] = '\0';
			
		printf("%s\n",line_data);
		
		n = 0;
		
		c = Socket_getc(connect_socket);
		
		//get response line from server
		while(c != '\0'){
			if(n >= MAX_LINE){
				break;
			}
			if(c == EOF){
				printf("Socket_getc EOF or error\n");
				Socket_close(connect_socket);
				exit(-1);
			}
			else{
				line_data[n] = c;
			}
			c = Socket_getc(connect_socket);
			n++;
		}
		
		line_data[n] = '\0';
			
		printf("%s\n",line_data);
		
		
		printf("%s","% ");
	}
	
	Socket_close(connect_socket);
	exit(0);	
}