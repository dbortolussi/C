#include "stdio.h"

int main(){
	
	int strlength = 0;
	
	char string[81] = "                                                                                ";
		
	char c1 ='a';
	char c2;
	
	while(c2!=EOF){
		
		while(strlength<80){
			
			c2 = getchar();
			
			if(c2==EOF){
				return 0;
			}
			
			else if(c1=='*'&&c2== '*'){
				string[strlength-1] = '^';
				c1='^';
			}
			
			else if(strlength==79&&c2=='*'){
				char c3 = getchar();
				if(c3=='*'){
					string[strlength] = '^';
					c1 = '^';
					strlength++;
					continue;
				}
				string[strlength] = c2;
				printf("%s\n",string);
				strlength = 0;
				string[strlength] = c3;
				strlength++;
				c1=c3;
				continue;
			}
			
			else if(c2=='\n'){
				string[strlength] = ' ';
				strlength++;
				c1 = c2;
			}
						
			else{
				string[strlength] = c2;
				strlength++;
				c1=c2;
			}
		}
		
		printf("%s\n",string);
		
		strlength = 0;
	}
}
