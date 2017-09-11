/*
 * Author : Saradhi Ramakrishna
 * Branch : M.E Computer Science
 * Below code is practical implementation of Vector clocks
 * using any number of processes and any number of events.
 * Input :  1.No. of Processes
 * 			2.No. of Events
 * 			3.(send process no,send process event ----> receive process no,receive process event)
 * 			4.Continues taking input until user wants and exits taking input if 0 is given.
 * 	Output : Displays Timestamps of all events using Vector clocks.
 * 	Note : Here we are using all integer values to represent events.
 * 			Like -1 is that timestamp is not generated yet.
 * 			Like -2 is that a receive event is there.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t mutex; //Used for blocking parallel access to threads.
int timestamps[20][20][20]; // 3-D array for storing final timestamp values.(each process--each event--each process clock)
int lastprocessedevent[20]={0}; // Used as a loop control for where thread needs to be suspended.
int countofreceiveevent[20][20]={0}; //stores how many receive events are there for each process.
int allreceiveevents[20][20][100]; //Actual receive events are stored in 3-D array.
int noofprocs,noofevents; // No of process and No of events input will stored here.

/*
 * Checks for timestamps of particluar event in a particular process is
 * generated or not.It returns that event timestamp array.
 */
int *checkforsendeventtimestampexistsornot(int pid,int event)
{
	int i=countofreceiveevent[pid][event];
	//printf("%d %d\n",allreceiveevents[pid][event][(2*i)],allreceiveevents[pid][event][(2*i+1)]);
	int value = timestamps[allreceiveevents[pid][event][(2*i)]][allreceiveevents[pid][event][2*i+1]][pid];
	if(value != -1)
	{
		countofreceiveevent[pid][event] = countofreceiveevent[pid][event]+1;
	}
	return timestamps[allreceiveevents[pid][event][(2*i)]][allreceiveevents[pid][event][2*i+1]];

}

/*
 * This function performs all checks of all timestamps filled or not.
 * If all filled, returns true and ends the program
 * if not all filled, returns false and program continues
 */
bool checkalltimestampsfilled()
{
	for(int i=1;i<=noofprocs;i++)
	{
		for(int j=1;j<=noofevents;j++)
		{
			for(int k=1;k<=noofprocs;k++)
			{
				if(timestamps[i][j][k]==-1 || timestamps[i][j][k] == -2)
					return false;
			}

		}
	}
	return true;
}

/*
 * Returns max of 2 values
 */
int max(int a, int b)
{
	if(a>=b)
		return a;
	else return b;
}

/*
 * Actual function to calculate timestamps
 */
void calculcatetimestamps(int pid)
{
	for(int i=lastprocessedevent[pid];i<=noofevents;i++)
	{
		int *value = checkforsendeventtimestampexistsornot(pid,i);
		//printf("value = %d\n",value);
		if(timestamps[pid][i][pid] == -2 && value[pid]==-1)
		{
			lastprocessedevent[pid] = i;
			break;
		}
		if(timestamps[pid][i][pid] == -2 && value[pid]!=-1)
		{
			timestamps[pid][i][pid] = max(timestamps[pid][i-1][pid] + 1,value[pid]+1);
			for(int j=1;j<=noofprocs;j++)
			{
				if(pid != j)
					timestamps[pid][i][j] = max(timestamps[pid][i-1][j],value[j]);
			}
			//printf("\nvalue=%d,timestamp=%d\n",value,timestamps[pid][i-1]);
		}
		if(timestamps[pid][i][pid] == -1)
		{
			timestamps[pid][i][pid] = timestamps[pid][i-1][pid] + 1;
			for(int j=1;j<=noofprocs;j++)
			{
				if(pid != j)
					timestamps[pid][i][j] = timestamps[pid][i-1][j];
			}

		}

	}
	printf("-----------------------\n");
}




void *doProcess(void *t)
{
    while(1)
    {
		pthread_mutex_lock(&mutex);
		int a = *((int *) t);
		printf("Thread id=%d in action now..\n",a);
		calculcatetimestamps(a);
		pthread_mutex_unlock(&mutex);
		sleep(2);
        if(checkalltimestampsfilled())
        	break;
    }
    return NULL;
}



int main(int argc, char *argv[])
{
    pthread_t pt[20];
    int tid[20];
    printf("\n-----------------------\n");
    printf("Enter the No. of processes : ");
    scanf("%d",&noofprocs);
    printf("Enter the No. of Events : ");
    scanf("%d",&noofevents);
    for(int i=1;i<=noofprocs;i++)
    {
    	for(int j=1;j<=noofevents;j++)
		{
    		countofreceiveevent[i][j] = 0;
    		for(int k=1;k<=noofprocs;k++)
    			timestamps[i][j][k] = -1;
		}
    }
    int p1,p1send,p2,p2recv,ind,input;
    while(1)
    {
    	printf("-----------------------\n");
    	printf("Enter the send process number  : ");
    	scanf("%d",&p1);
    	printf("Enter the event in send process %d  : ",p1);
    	scanf("%d",&p1send);
    	printf("Enter the receive process number  : ");
    	scanf("%d",&p2);
    	printf("Enter the event in receive process %d  : ",p2);
    	scanf("%d",&p2recv);
    	printf("-----------------------\n");

        timestamps[p2][p2recv][p2] = -2;
        ind = countofreceiveevent[p2][p2recv];
        allreceiveevents[p2][p2recv][2*ind]=p1;
        allreceiveevents[p2][p2recv][2*ind+1]=p1send;
        countofreceiveevent[p2][p2recv] = countofreceiveevent[p2][p2recv] + 1;
        printf("Press 1 to enter more send and receive inputs,0 to start calculating timestamps : ");
        scanf("%d",&input);

        if(input != 1)
        	break;

    }

    printf("-----------------------\n");
    /*** Setting back values to 0 ******/
    for(int i=1;i<=10;i++)
    	for(int j=1;j<=10;j++)
    		countofreceiveevent[i][j] = 0;

    /*** Creating equal number of threads as processes ******/
    for(int i=1;i<=noofprocs;i++)
    {
    	tid[i]=i;
    	pthread_create(&pt[i], NULL, doProcess, (void *)&tid[i]);
    	sleep(2);
    }

    /*** Waiting for all threads to end ******/
    for(int i=1;i<=noofprocs;i++)
    {
    	pthread_join(pt[i], NULL);
    }

    printf("\n************** Final Values ****************\n");
    for(int i=1;i<=noofprocs;i++)
	{
		printf("Process-%d : ",i);
		for(int j=1;j<=noofevents;j++)
		{
			for(int k=1;k<=noofprocs;k++)
			{
				printf("%d",timestamps[i][j][k]);
			}
			printf("|");

		}
		printf("\n");
	 }
    printf("*********************************************\n");

    return 0;
}
