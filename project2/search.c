//Zach Hopman

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MAX 1024

void sigHandler (int);
bool DoneFlag = false;

void sigHandler (int sigNum) {
  if(sigNum == SIGUSR1) {
  // shutdown flag changed
    DoneFlag = true;
  }
}
//check input is alpha
int checkalpha(char *string)
{
  int i;
  for(i=0;string[i];i++)
    if(!isalpha(string[i]))
      return 0;
  return 1;
}
//Child process
void childProcess(const int rpipe, const int wpipe, const int id)
{                    
  char in_c[50];        //read in
  bool fileRec = false; //indicates first read in (filename)
  FILE *fp;
  char fname[50];

  printf("The parent of child: %d is: %d\n", getpid(),getppid());
  while (!DoneFlag ) {

    //read input from parent
    int num_read;
    if ( (num_read = read(rpipe, &in_c, 50)) == -1 ) {
      printf("error reading from pipe in child");
    }
    //pipe closed so exit
    else if ( num_read == 0 ) {
      printf("Pipe from parent closed to child  %d.\n", id);
      DoneFlag = true;
    }
    else {
      //first message recieved will be file to open
      if(!fileRec) {
	strcpy(fname,in_c);
	strtok(fname,"\n");//remove \n from filename
	printf ("Child %d will search %s\n",id,fname);
	fileRec = true;
      }
      else //search file for string
	{
	  char temp[512];
	  int strCount = 0;
	  int i , j, found, stringLen, inputLen;
	  
	  fp = fopen(fname,"r");
	  inputLen = strlen(in_c);

	  while(fgets(temp, 512, fp) != NULL) {
	     stringLen = strlen(temp);
	    for(i=0; i <= stringLen-inputLen; i++)
	      {
		found = 1;//start found as 1
		for(j=0; j<inputLen; j++)
		  {
		    if(temp[i+j] != in_c[j])
		      {
			found = 0;//change to 0 if current window is not in_c
			break;
		      }
		  }
		//word is found but may be apart of another word
		if(found == 1)
		  {
		    if(!isalpha(temp[j+i])){
		      if(!isalpha(temp[i-1])){
			strCount++;
		      }
		    }
		  }
	      } 
	  }
	    //send count to parent
	    if (write(wpipe,&strCount,5) == -1) {
	      printf("error writing to pipe");
	    }
	}
    }
  }
  /*  Close file descriptors and exit  */
  printf ("Child %d Terminating\n",id);
  fclose(fp);
  close(rpipe);
  close(wpipe);
  exit(0);
}
//main process
int main () {
    signal (SIGUSR1, sigHandler);
    FILE *fileList;
    int numchild=0;
    char line[50];
    char files[20][50];
    char input[MAX];
    int readIn = 0;
    int totalFound = 0;
    
    //hard code list of filenames here, can easily be changed to user input
    fileList = fopen("fnames.txt", "r");
    //print parent PID
    printf("Parent PID#: %d\n", getpid());
    //loop through list of filenames and add to an array
    while(fgets(line,sizeof(line), fileList)!=NULL) {
      strcpy(files[numchild],line);
      numchild++;
    }	 
 
    int ptoc_fd[numchild][2];   //Parent to child pipe
    int ctop_fd[numchild][2];   //Child to parent pipe
    pid_t children[numchild];   //PIDs of children
    
    int i = 0;
    //open pipes and fork children
    for (i=0; i < numchild; ++i ) {
      pipe(ptoc_fd[i]);
      pipe(ctop_fd[i]);

      if ( (children[i] = fork()) == -1 ) {
	perror("error calling fork()");
	return -1;
      }
      else if ( children[i] == 0 ) {
	printf("Child %d created.\n", i + 1);
	close(ctop_fd[i][0]);
	close(ptoc_fd[i][1]);
	childProcess(ptoc_fd[i][0], ctop_fd[i][1], i + 1);
	printf("Child %d terminating.\n", i + 1);
	return 0;
      }
      else {
	close(ptoc_fd[i][0]);
	close(ctop_fd[i][1]);
      }
    }
    //send filenames to children
    for(i=0;i < numchild; i++) {
      if (write(ptoc_fd[i][1],&files[i][0],50) == -1) {
	printf("error writing to pipe");
      }
    }
    usleep(2000);//makes sure children are down writing to console
    //loop until DoneFlag is changed (SIGUSR1)
    while(!DoneFlag) {
      totalFound = 0;
      printf ("Enter a string to search for, anything non-alpha to exit\n");
      scanf("%s",input);
      //exit command
      if (!checkalpha(input)) {
	kill(getpid(),SIGUSR1);
      }
      else {
	//send input to children
	for(i=0;i < numchild; ++i) {
	  if (write(ptoc_fd[i][1],&input,20) == -1) {
	    perror("error writing to pipe");
	  }
	}
	//recieve output from children
	int num_read;
	for(i=0;i < numchild; i++) {
	  if ( (num_read = read(ctop_fd[i][0], &readIn, 5)) == -1 ) {
	    printf("error reading from pipe");
	    return EXIT_FAILURE;
	  }
	  else if ( num_read == 0 ) {
	    printf("Pipe from child %d closed.\n", i+1);
	  }
	  else {
	    totalFound = totalFound + readIn;
	    printf("Child %d Found %d\n",i+1,readIn);
	  }
	}
	printf("Total number of occurences: %d\n",totalFound);
      }
    }
     //cleanup
     printf("Waiting for Children to terminate\n");
     for (i=0; i < numchild; ++i ) {
       close(ctop_fd[i][0]);
       close(ptoc_fd[i][1]);
      }
     for (i=0; i < numchild; ++i ) {
       wait(NULL);
     }
    printf ("Program Exitting\n");
    return 0;
}
