#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
void *writer_function (void *threadnumber)
{
	pthread_mutex_lock (&lock);
	printf("Thread id : %d\n",*(int *)threadnumber);
	pthread_mutex_unlock (&lock);
	pthread_exit (NULL);
}

int main ()
{
	pthread_t pt[3];
	for(int i=1;i<=2;i++)
	{
		pthread_create(&pt[i],NULL,writer_function,(void *)&i);
	}

	for(int i=1;i<=2;i++)
	{
		pthread_join(pt[i],NULL);
	}
	exit(0);
}



