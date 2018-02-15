//Zach Hopman, Kevin Anderson
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler (int);

int main () {

	//set up signals
	signal (SIGUSR1, sigHandler);
	signal (SIGUSR2, sigHandler);
	signal (SIGINT, sigHandler);

	pid_t parent = getpid();
	//fork process
	pid_t child = fork();

	if (!child)//parent		
	{		
		printf ("Parent waiting...\n");
   	 	pause ();
   	 	return 0;
	}
	else //child
	{
		
		while(1)
		{	
			//calculate a random number between 1 and 5
			int randtime;
			randtime = (rand() % 6) + 1;
			printf (" Child waiting %d  ",randtime);
			fflush(stdout);
			//wait a the random amount of time
			sleep(randtime);

			//determine which signal to send based on randtime
			if (randtime % 2 == 0) {
				kill(parent,SIGUSR1);
			}
			else{
				kill(parent,SIGUSR2);
			}
		}
		return 0;


	}
    

}
//handles all signals
void sigHandler (int signum) {
    
 	if(signum == SIGUSR1)
	{
		printf (" Recieved a SIGUSR1 signal.\n");
	}
	else if(signum == SIGUSR2)
	{
		printf (" Recieved a SIGUSR2 signal.\n");
	}
	else
	{	
		printf (" Recieved a keyboard interupt, exitting...\n");
		exit(0);
	}
		signal (SIGUSR1, sigHandler);
		signal (SIGUSR2, sigHandler);
}
