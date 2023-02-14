//Daniel Bortolussi
//I have neither given nor received unauthorized help on this assignment

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#include "Socket.h"
#include "shell.h"

void getArgs(char* string);

ServerSocket welcome_socket;
Socket connect_socket;

char str[MAX_LINE];
char* args[50];

bool error = false;

int main(int argc, char* argv[]){
	
	pid_t spid, term_pid;
	int chld_status;
	
	char tmp_name[1024];
	FILE* fp;
	
	char line_data[MAX_LINE];
	
	if(argc < 2){
		printf("No port specified\n");
		return(-1);
	}
	
	welcome_socket = ServerSocket_new(atoi(argv[1]));
	if(welcome_socket < 0){
		printf("Failed new server socket\n");
		return(-1);
	}
	
	//accept clinet
	connect_socket = ServerSocket_accept(welcome_socket);
	if(connect_socket < 0){
		printf("Failed accept on server socket\n");
		exit(-1);
	}
	
	//dynamically redirect stdout to temp file
	int this_pid = (int) getpid();
	char pid[64];
	sprintf(pid,"%d",this_pid);
	strcat(tmp_name,"tmp");
	strcat(tmp_name,pid);
	fp = freopen(tmp_name,"w",stdout);
	
	//read from client
	
	char c = Socket_getc(connect_socket);
	
	while( c != EOF){
		
		int n = 0;
		
		while(c != '\n'){
			if(c == EOF){ break;}
			line_data[n] = c;
			c = Socket_getc(connect_socket);
			n++;
		}
		
		line_data[n] = '\0';
			
		//fork new process
		spid = fork();
		if(spid == -1){
			perror("fork");
			exit(-1);
		}
		if(spid == 0){
			//process client input
			Socket_close(connect_socket);
			getArgs(str);
			execvp(args[0],args);
			error = true;
			//printf(stderr,"%s\n","File not found.");
			exit(0);
		}
		else{
			term_pid = waitpid(-1,&chld_status,WNOHANG);
			//send temp file contents to client
			fp = fopen(tmp_name,"r");
			fgets(&c,1,fp);
			while(c != EOF){
				Socket_putc(c,connect_socket);
				fgets(&c,1,fp);
			}
			Socket_putc('\0',connect_socket);
			fp = freopen(tmp_name,"w",stdout);
			//send response line to client
			if(error){
				char* line = "Command could not be executed";
				for(int i=0;i<strlen(line);i++){
					Socket_putc(line[i],connect_socket);
				}
				Socket_putc('\0',connect_socket);
			}
			else{
				char* line = "Command executed succesfully";
				for(int i=0;i<strlen(line);i++){
					Socket_putc(line[i],connect_socket);
				}
				Socket_putc('\0',connect_socket);
			}
		}
		c = Socket_getc(connect_socket);
	}
	//close connecting socket
	Socket_close(connect_socket);
	
	//remove temp file
	remove(tmp_name);
}

void getArgs(char* string) {
	
	char* p;
	
	p = strtok(string," \t");
	
	int i = 0;
	
	while(p != NULL){
		
		if(i>MAX_LINE){
			error = true;
			break;
		}
		
		args[i] = p;
		p = strtok(NULL," \t");
		
		
		i++;
		
	}
	args[i] = '\0';
}
