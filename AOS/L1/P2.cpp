#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int global=1;
int main()
{
	int pid,var;
	if((pid=fork())==0) // Child process
	{
		global++;
		var++;
		printf("This is the Child Process.\n");
		printf("PID: %d, global: %d, var: %d\n",getpid(),global,var);
	}
	else // Parent process
	{
		sleep(2);
		printf("This is the Parent Process.\n");
		printf("PID: %d, global: %d, var: %d\n",getpid(),global,var);
	}
return 0;
}
