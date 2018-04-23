//Zach Hopman
//Kevin Anderson
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	struct stat statBuf;
    DIR *dirPtr;
    struct dirent *entryPtr;

    dirPtr = opendir(".");

    while ((entryPtr = readdir(dirPtr))){

	 if (stat(entryPtr->d_name, &statBuf) < 0) {

		}

	printf("%-20s", entryPtr->d_name);
	printf("size is: %u\n", statBuf.st_size);
}
    closedir(dirPtr);
    return 0;
}
