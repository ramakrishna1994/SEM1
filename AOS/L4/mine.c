#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
struct arguments
{
	int a;
	int b;
};
int sum;
void * addtwonumbers(void *args)
{
	struct arguments *actual=(struct arguments *)args;
	int c;
	sum=actual->a + actual->b;
	return NULL;
}
int main()
{
	pthread_t handle;
	struct arguments args;
	printf("enter two elements to add : ");
	scanf("%d",&args.a);
	scanf("%d",&args.b);
	if((pthread_create(&handle,NULL,addtwonumbers,(void *)&args))!=0)
	{
		perror("Error in creating thread");
		exit(1);
	}
	pthread_join(handle,NULL);
	printf("sum is %d",sum);
	return 0;
	
}
