#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int main(int argc,char **argv)
{
	CLIENT *cl;
	sort_in in;
	sort_out *out;
	if(argc!=2){
		printf("client localhost");
		exit(1);
	}
	cl = clnt_create(argv[1],SORTARRAY_PROG,SORTARRAY_VERS,"tcp");
	printf("Enter 10 values to sort");
	for(int i=0;i<10;i++)
	{
		scanf("%d",&in.unsortedarray[i]);
	}
	for(int i=0;i<10;i++)
        {
                printf("%d",in.unsortedarray[i]);
        }

	if((out = sortarray_proc_1(&in,cl)) == NULL)
	{
		printf("Error");
		exit(1);
	}
	printf("Succesfully sorted : ");
	for(int i=0;i<10;i++)
	{
		printf("%d,",out->sortedarray[i]);
	}
	return 0;

}
