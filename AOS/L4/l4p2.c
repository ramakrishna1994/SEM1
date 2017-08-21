/* thread2.c*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 5
int sum = 0;
int inc = 5;
pthread_mutex_t lock;
void *fun (void *val) 
{
    pthread_mutex_lock (&lock);
    sum += inc;
	printf ("Value: %d\n", sum);
	inc += 5;
    pthread_mutex_unlock (&lock);
    pthread_exit (NULL);
}

int main (int argc, char* argv[]) 
{
    pthread_t t[N];
	int errcode, i;
    for (i = 0; i < N; i ++) 
    {
		if (pthread_create (&t[i], NULL, fun, NULL)) 
		{
		printf ("Error creating thread\n");
		return EXIT_FAILURE;
		}
	}
	for (i = 0; i < N; i ++) 
	{
		pthread_join (t[i], NULL);
	}
	return EXIT_SUCCESS;
}
