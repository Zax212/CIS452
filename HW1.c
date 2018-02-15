﻿//Zach Hopman Program 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define STDIN_FDES 0

struct termios save;

int main(int argc, char *argv[])
{
	int cc = 0;
	struct termios work;
	char input[50];

	cc = tcgetattr(STDIN_FDES,&save);	//save default
	work = save;

	printf("Disable echo \n");
	work.c_lflag &= ~(ECHO);		//disable echo
	cc = tcsetattr(STDIN_FDES,TCSANOW,&work);

	printf("\nEnter passphrase: \n");		//ask for and collect input
	fgets(input,50,stdin);
	printf("You entered: %s\n", input);

	printf("Default behavior restored \n");
	cc = tcsetattr(STDIN_FDES,TCSANOW,&save);	//restore default value

	printf("\nEnter passphrase: \n");		//ask for and collect input
	fgets(input,50,stdin);
	printf("You entered: %s\n", input);
	return(cc);
}
