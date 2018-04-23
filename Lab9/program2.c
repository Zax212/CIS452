//Zach Hopman Kevin Anderson

#include <stdio.h>
#include <stdbool.h>

int global;
int global_init = 0;

double big_one[500];
double small_one[50];

int main () {
		
	int local_init = 10;
	int local;
	int local2;
	int local3;
	int *ptr;
	bool test;
	char *data1;
 	data1 = malloc (local_init);
	char *data2;
 	data2 = malloc (local_init);
	char *data3;
 	data3 = malloc (local_init);
	
	printf ("Malloc:\n");
	printf ("%X\n\n", data1);
	printf ("Malloc2:\n");
	printf ("%X\n\n", data2);
	printf ("Malloc3:\n");
	printf ("%X\n\n", data3);

    printf ("This main function begins at %X\n", main);
    printf ("The two arrays are allocated at %X and %X\n", big_one,
            small_one);
	printf ("Test is at %X\n\n", &test);

	printf ("Local uninitilized:\n");
	printf ("%X\n\n", &local);
	printf ("Local uninitilized:\n");
	printf ("%X\n\n", &local2);
	printf ("Local uninitilized:\n");
	printf ("%X\n\n", &local3);

	printf ("Global unintilized:\n");	
	printf ("%X\n\n", &global);

	printf ("Local initilized:\n");
	printf ("%X\n\n", &local_init);

	printf ("Global intilized:\n");	
	printf ("%X\n\n", &global_init);

	printf ("Pointer:\n");	
	printf ("%X\n\n", &ptr);	

	 free (data1);
	free (data2);
	free (data3);
    return 0;
}
