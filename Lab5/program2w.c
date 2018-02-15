#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define SHMSZ     27

int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    int i,n;
    key = 5678;
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 1;
    }
    if ((shm = shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat");
        return 1;
    }


     while (*shm != '*')
        sleep(1);
    s = shm;

   s++;
   char m=*s;

   n=(int)m;

    int r=square(n);// called function square function 
    s=shm;
    s++;

    int a=1000;
    for (i=0;i<4;i++){
        *s = (char)(r/a+48);
        r=r%a;
        a=a/10;
        s++;
        }
    s = NULL;
    *shm='%';

    while (*shm != '$')
        sleep(1);

    return 0;
}
int square(int n){
    int a,b;
    a=n/10;
    b=n%10;
    int res=a*a*100+a*b*2*10+b*b;
    return res;
}
