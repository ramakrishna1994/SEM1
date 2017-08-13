#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
void handler(int sig)
{
pid_t pid;
pid = wait(NULL);
printf("\t\tChild %d exited.\n", pid);
signal(SIGCHLD, handler);
}
int main()
{
int i;
signal(SIGCHLD, handler);
for(i=0;i<3;i++)
switch(fork())
{
case 0:
printf("\tChild created %d\n", getpid());
exit(0);
}
sleep(2);
return 0;
}
