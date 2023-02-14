//I have neither given nor received help on this assignment.
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 4097

bool run = true;
bool redo = false;

void readCommand(char* string);
void getArgs(char* string);

char str[MAX_LENGTH] = "                                                  ";

char* args[50];

int main() {
	
	char str[MAX_LENGTH] = "";
	int childPID;
	
	while(run) {
	
		printf("%s","% ");
		
		readCommand(str);
				
		if(!run) {
			return 0;
		}
	
		getArgs(str);
		
		if(redo) {
			str[0] = '\0';
			redo = false;
			continue;
		}
		
		childPID = fork();
				
		if(childPID == 0){
			execvp(args[0],args);
			fprintf(stderr,"%s\n","File not found.");
			return 0;
		} else {
			wait(&childPID);
		}
		
		str[0] = '\0';
	}
	return 0;
}

void readCommand(char* string){
	int strlength = 0;
	char c = getchar();  
	while(c != '\n'){
		if(c==EOF){
			run = false; 
			break;
		}
		if(strlength>=MAX_LENGTH-1){
			fprintf(stderr,"%s\n","Please limit commands to 4096 characters");
			redo = true;
			break;
		}
		string[strlength] = c;
		strlength++;
		c=getchar();
	}
	string[strlength] = '\0';
}

void getArgs(char* string) {
	
	char* p;
	
	p = strtok(string," \t");
	
	int i = 0;
	
	while(p != NULL){
		
		if(i>50){
			fprintf(stderr,"%s\n","Please limit number of arguments to 50");
			redo = true;
			break;
		}
		
		args[i] = p;
		p = strtok(NULL," \t");
		
		
		i++;
		
	}
	args[i] = '\0';
}