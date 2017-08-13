#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sig(int s)
{
	printf("I receive a signal %d\n", s);
	(void) signal(SIGINT, SIG_DFL);
}

int main()
{
	(void) signal(SIGINT, sig);
	while(1)
	{
		printf("Hello to BITS!\n");
		sleep(1);
	}
	return 0;
}
