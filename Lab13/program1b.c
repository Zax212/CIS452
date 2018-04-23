#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define SIZE 30

int main (int argc, char *argv[]) {
    struct flock fileLock;
    int fd;
	char buf;

    if (argc < 2) {
        printf ("usage: filename\n");
        exit (1);
    }
    if ((fd = open (argv[1], O_RDWR)) < 0) {
        perror ("there is");
        exit (1);
    }
	fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_END;
    fileLock.l_start = SEEK_END -100;
    fileLock.l_len =100;

    while (fcntl (fd, F_GETLK, &fileLock) == 0) {
    }
	
	while( read(fd, &buf,sizeof(char)) != EOF) {
	
		if(buf == '\n'){
			break;
		}		
		printf("%c",buf);
	}
    

    close (fd);

    return 0;
}
