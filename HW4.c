#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

//create pipes
int pipefd1[2];
int pipefd2[2];
int pipefd3[2];


void* readInput(){ 
	//printf("reading\n");
	close(pipefd1[0]);
	close(pipefd2[0]);
	close(pipefd2[1]);
	close(pipefd3[0]);
	close(pipefd3[1]);
	char c = getchar();
	while(c != EOF){
		write(pipefd1[1],&c,1);
		c = getchar();
	}
	char e = EOF;
	write(pipefd1[1],&e,1);
	close(pipefd1[1]);
	}
	
void* checkNewline(){
	//printf("checking newline\n");
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd3[0]);
	close(pipefd3[1]);
	char c;
	read(pipefd1[0],&c,1);
	while(c != EOF) {
		if(c == '\n'){
			char b = ' ';
			write(pipefd2[1],&b,1);
		} else {
			write(pipefd2[1],&c,1);
		}
		read(pipefd1[0],&c,1);
	}
	write(pipefd2[1],&c,1);
	close(pipefd1[0]);
	close(pipefd2[1]);
}

void* checkStar(){
	//printf("checking star\n");
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[1]);
	close(pipefd3[0]);
	char c;
	read(pipefd2[0],&c,1);
	while(c != EOF) {
		if(c == '*'){
			char d;
			read(pipefd2[0],&d,1);
			if(d==EOF){
				write(pipefd3[1],&c,1);
				break;
			}
			else if(d == '*'){
				char b = '^';
				write(pipefd3[1],&b,1);
			}
			else{
				write(pipefd3[1],&c,1);
				write(pipefd3[1],&d,1);
			}
		}
		else{
			write(pipefd3[1],&c,1);
		}
		read(pipefd2[0],&c,1);
	}
	char e = EOF;
	write(pipefd3[1],&e,1);
	close(pipefd2[0]);
	close(pipefd3[1]);
}

void* output(){
	//printf("outputting\n");
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd2[1]);
	close(pipefd3[1]);
	char c;
	read(pipefd3[0],&c,1);
	int strlength = 0;
	char string[81] = "                                                                                ";
	while(c != EOF){
		//printf("%c",c);
		if(strlength<80){
			string[strlength] = c;
			strlength++;
		}
		if(strlength>=80){
			printf("%s\n",string);
			strlength = 0;
		}
		read(pipefd3[0],&c,1);
	}
	close(pipefd3[0]);
}

int main(){
	
	//create pipes
	pipe(pipefd1);
	pipe(pipefd2);
	pipe(pipefd3);
	
	//fork processes
	int child1 = fork();
	
	if(child1 == 0){
		int child2 = fork();
		if(child2 == 0){
			int child3 = fork();
			if(child3 == 0){
				int child4 = fork();
				if(child4 == 0){
					readInput();
				}
				else{
					checkNewline();
			}
		}
		else{
			checkNewline();
		}
	} else {
		output();
	}
	}
	else {
		wait(&child1);
	}
	return 0;
}