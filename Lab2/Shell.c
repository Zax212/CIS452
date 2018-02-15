#include <stdio.h>
#include <unistd.h>

#define MAXLINE 80
#define MAXARG 20

char cmd[MAXLINE];
void main(){
	int status;
	while (1)
	{
		printf("mysh$ ");
		fgets(cmd, MAXLINE, stdin); 	// read a command from the user
		 
		//if the command in cmd is “exit\n”, then terminate this program;
		if (strcmp(cmd,"exit\n")==0){
			printf("Exiting");
			exit(0);
		}
		//otherwise, create a child process to handle that command.
		else
		{
			if (fork()==0){
			background(cmd);
			}
		wait(&status);
		}

	}
}
void background (char * cmd)
{

	int i = 0;
	char *argv[MAXARG];

	// Here you may use strtok(cmd, "\t \n") to get each token 
	// from cmd and then storeit in array argv sequentially.
	// strtok() returns NULL when the end of cmd is reached.

	char *temp = strtok(cmd,"\t \n");
	while (temp != NULL){
		argv[i] = temp;
		i++;
		temp = strtok(NULL,"\t \n");
	}	
	putchar ('\n');
	execvp (argv[0], argv);
	exit(0);
		
}
