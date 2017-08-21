//Import
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

//global variables
bool incs = false;
pthread_mutex_t mutex;
int timestamps[20][20];
int lastprocessedevent[20]={0};
int lastreceiveevent[20][20]={0};
int allreceiveevents[20][20][100];
int noofprocs,noofevents;

int checkforsendeventtimestampexistsornot(int pid,int event)
{
	int i=lastreceiveevent[pid][event];
	lastreceiveevent[pid][event] = i+1;
	return timestamps[allreceiveevents[pid][event][(2*i)]][allreceiveevents[pid][event][2*i+1]];

}

bool checkalltimestampsfilled()
{
	 for(int i=1;i<=noofprocs;i++){
	    	for(int j=1;j<=noofevents;j++)
	    	    	{
	    	    		if(timestamps[i][j]==-1 || timestamps[i][j] == -2)
	    	    			return false;
	    	    	}

	    }
	 return true;
}

int max(int a, int b)
{
	if(a>=b)
		return a;
	else return b;
}
void calculcatetimestamps(int pid)
{
	printf("\ncalculating for pid = %d \n",pid);
	for(int i=lastprocessedevent[pid];i<=noofevents;i++)
	{
		int value = checkforsendeventtimestampexistsornot(pid,i);
		printf("value = %d\n",value);
		if(timestamps[pid][i] == -2 && value==-1)
		{
			lastprocessedevent[pid] = i;
			break;
		}
		if(timestamps[pid][i] == -2 && value!=-1)
		{
			printf("\nvalue=%d,timestamp=%d\n",value,timestamps[pid][i-1]);
			timestamps[pid][i] = max(timestamps[pid][i-1] + 1,value+1);
		}
		if(timestamps[pid][i] == -1)
		{
			timestamps[pid][i] = timestamps[pid][i-1] + 1;
		}

	}

	for(int i=1;i<=noofprocs;i++){
	                      	for(int j=1;j<=noofevents;j++)
	                      	    	{
	                      	    		printf("%d ",timestamps[i][j]);
	                      	    	}
	                      	printf("\n");
	                      }
	printf("-----------------------\n");
}

void *threadB(void *t)
{
    int n = 0, rValue;

    while(1)
    {

        /* Wait for state B */
        pthread_mutex_lock(&mutex);
        //while (incs != false);

        //incs = true;
        //pthread_mutex_unlock(&mutex);
        int a = *((int *) t);

                    printf("id=%d\n",a);
                  calculcatetimestamps(a);
        //do stuff
        /* Set state to A and wake up thread A */
        //pthread_mutex_lock(&mutex);
        //incs = false;
        //pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        sleep(5);
        if(checkalltimestampsfilled())
        	break;
    }

    return 0;
}

void printvalues()
{
	for(int i=1;i<=noofprocs;i++)
	{
		for(int j=1;j<=noofevents;j++)
			    		printf("%d ",lastreceiveevent[i][j]);
		printf("\n");
	}

}

int main(int argc, char *argv[])
{
    //create our threads
    pthread_t pt[5];
    int tid[5];

    printf("Enter the No. of processes : ");
    scanf("%d",&noofprocs);
    printf("Enter the No. of Events : ");
    scanf("%d",&noofevents);
    for(int i=1;i<=noofprocs;i++){

            	for(int j=1;j<=noofevents;j++)
            	    	{
            	    		timestamps[i][j] = -1;
            	    	}

            }
    int p1,p1send,p2,p2recv,ind,input;
    while(1)
    {
    	printf("Enter the send process number  : ");
    	scanf("%d",&p1);
    	printf("Enter the event in send process %d  : ",p1);
    	scanf("%d",&p1send);
    	printf("Enter the receive process number  : ");
    	scanf("%d",&p2);
    	printf("Enter the event in receive process %d  : ",p2);
    	scanf("%d",&p2recv);

        timestamps[p2][p2recv] = -2;
        ind = lastreceiveevent[p2][p2recv];
        lastreceiveevent[p2][p2recv] = lastreceiveevent[p2][p2recv] + 1;
        allreceiveevents[p2][p2recv][2*ind]=p1;
        allreceiveevents[p2][p2recv][2*ind+1]=p1send;

        printf("Press 1 to enter more inputs,0 to start calculating timestamps : ");
        scanf("%d",&input);
        if(input == 0)
        	break;

    }

    for(int i=1;i<=10;i++)
    	{
    		for(int j=1;j<=10;j++)
    			    		printf("%d ",lastreceiveevent[i][j]);
    		printf("\n");
    	}

    for(int i=1;i<=10;i++)
    	for(int j=1;j<=10;j++)
    		lastreceiveevent[i][j] = 0;

    for(int i=1;i<=noofprocs;i++)
    {
    	tid[i]=i;
    	pthread_create(&pt[i], NULL, threadB, (void *)&tid[i]);
    	sleep(2);
    }

    for(int i=1;i<=noofprocs;i++)
    {
    	pthread_join(pt[i], NULL);
    }

    return 0;
}
