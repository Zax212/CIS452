//Zach Hopman
//Kevin Anderson
//Lab 6

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
//#include <bits/sem.h>

#define SIZE 16

int main (int argc, char *argv[]) {
    int status;
    long int i, loop, *shmPtr;
    int shmId;
    pid_t pid;
	int semId;

	/* create a new semaphore set for use by this (and other) processes..*/
	semId = semget (IPC_PRIVATE, 1, 00600);

	/* initialize the semaphore set referenced by the previously obtained 		semIdhandle. */
	semctl (semId, 0, SETVAL, 1);


    /*
     * TODO: get value of loop variable(from command - line
     * argument
     */

		loop = atoi(argv[1]);

    if ((shmId = shmget (IPC_PRIVATE, SIZE,
                         IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    shmPtr[0] = 0;
    shmPtr[1] = 1;
		
	struct sembuf sb_unlock;
	sb_unlock.sem_num = 0;
	sb_unlock.sem_op = 1;
 	sb_unlock.sem_flg = 0;
	
	struct sembuf sb_lock;
	sb_lock.sem_num = 0;
	sb_lock.sem_op = -1;
 	sb_lock.sem_flg = 0;

    if (!(pid = fork ())) {		
        for (i = 0; i < loop; i++) {

            /*
             * TODO: swap the contents of shmPtr[0] and  shmPtr[1]
             */
				semop (semId, &sb_lock, 1);
				long int temp; 
				temp = shmPtr[0];
				shmPtr[0] = shmPtr[1];
				shmPtr[1] = temp;
				semop (semId, &sb_unlock, 1);
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can 't let go\n");
            exit (1);
        }
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {
			
            /*
             * TODO: swap the contents of shmPtr[1] and shmPtr[0]
             */
				semop (semId, &sb_lock, 1);
				long int temp; 
				temp = shmPtr[0];
				shmPtr[0] = shmPtr[1];
				shmPtr[1] = temp;
				semop (semId, &sb_unlock, 1);
        }
    }

    wait (&status);


	// remove the semaphore referenced by semId
	semctl (semId, 0, IPC_RMID);

    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    return 0;
}
