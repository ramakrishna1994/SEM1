#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void handler(int sig)
{
	pid_t pid;
	pid = wait(NULL);
	printf("\t\tChild %d exited.\n", pid);
	signal(SIGCHLD, handler);
}
int main()
{
	int i;pid_t pid[3];
	signal(SIGCHLD, handler);
	for(i=0;i<3;i++)
	{
		pid[i]= fork();
		switch(pid[i])
		{
			case 0:
				printf("\tChild created %d\n", getpid());
				exit(0);
		}
		wait(NULL);
	}



	sleep(2);
	return 0;
}
