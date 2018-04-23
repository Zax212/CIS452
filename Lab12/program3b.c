//Zach Hopman
//Kevin Anderson
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[])
{
  struct stat statBuf;
  DIR *dirPtr;
  struct dirent *entryPtr;

  if (stat(argv[1], &statBuf) < 0) {
    perror("No input, default to current directory");
  }
  int isDir =  S_ISDIR(statBuf.st_mode);
  
  if (isDir == 0){
  	dirPtr = opendir(".");
  }
  else {
    dirPtr = opendir(argv[1]);
  }
  while ((entryPtr = readdir(dirPtr))){

    if (stat(entryPtr->d_name, &statBuf) < 0) {

    }
    printf("%-20s", entryPtr->d_name);
	if (argc > 2){
		if(strcmp(argv[2],"-i")){
			printf(" %u", statBuf.st_uid);//user id
			printf(" %u", statBuf.st_gid);//group id
			printf(" %u", statBuf.st_size);//Filesize
			printf(" %u", statBuf.st_atime);//Date
		}
		else if(strcmp(argv[2],"-n")){
			printf(" %u", statBuf.st_ino);//inode
		}
	}
    printf("\n");
  }
  closedir(dirPtr);
  return 0;
}
