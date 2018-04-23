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

    dirPtr = opendir(".");

    while ((entryPtr = readdir(dirPtr))){

	 if (stat(entryPtr->d_name, &statBuf) < 0) {

		}

	printf("%-20s", entryPtr->d_name);
	printf(" %u", statBuf.st_uid);//user id
	printf(" %u", statBuf.st_gid);//group id
	printf(" %u", statBuf.st_size);//Filesize
	printf(" %u", statBuf.st_atime);//Date
	printf(" %u", statBuf.st_ino);//inode
	printf("\n");
}
    closedir(dirPtr);
    return 0;
}
