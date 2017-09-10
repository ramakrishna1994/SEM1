#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
int shared = 5;
pthread_mutex_t lock;

void sendreply(int replyfrompno,int replytopno,int ts)
{
	FILE *ptr;
	if(replytopno==1)
	{
		ptr = fopen("file1.txt","a");
	}
	if(replytopno==2)
	{
		ptr = fopen("file2.txt","a");
	}
	if(replytopno==3)
	{
		ptr = fopen("file3.txt","a");
	}
	fprintf(ptr,"%d %d\n",replyfrompno,ts);
	fclose(ptr);
}

void *process1(void *val)
{
	int replyfrom[4]={0};
	int queue[3]={0};
	int clockvalue = 0;
	int replyto[4]={0};

	FILE *ptr1,*ptr2,*ptr3;
	pthread_mutex_lock(&lock);
	printf("--------------process1---------------\n");
	ptr1 = fopen("file1.txt","r");
	 char line [1000];
	 int pno=0,ts=0,timefromotherprocess=0;
	 while (!feof (ptr1))
	    {

		 	 fscanf(ptr1,"%d %d",&pno,&ts);
		 	 if(ts > 0 && (pno == 2 || pno == 3))
		 	 {
		 		if(ts > timefromotherprocess)
		 			timefromotherprocess = ts;
		 	 }


	 }
	 if(clockvalue < timefromotherprocess)
		clockvalue = timefromotherprocess + 1;
	else
		clockvalue = clockvalue + 1;

	printf("file1 %d %d\n",pno,ts);
	 fclose(ptr1);
	 pthread_mutex_unlock(&lock);
	 pthread_mutex_lock(&lock);
	 printf("--------------process1---------------\n");
	ptr2 = fopen("file2.txt","a");
	fprintf(ptr2,"%d %d\n",1,clockvalue);
	fclose(ptr2);
	ptr3 = fopen("file3.txt","a");
	fprintf(ptr3,"%d %d\n",1,clockvalue);
	fclose(ptr3);
	pthread_mutex_unlock(&lock);


	pthread_mutex_lock(&lock);
			printf("--------------process1---------------\n");
				ptr1 = fopen("file1.txt","r");
				while (!feof (ptr1))
			    {
				 	 int pno,ts;
				 	 fscanf(ptr1,"%d %d",&pno,&ts);
				 	 if(ts > 0 && (pno!=1))
				 	 {
				 		sendreply(1,pno,-ts);
				 		replyto[pno]=1;
				 	 }

				 	 printf("1file1 %d %d\n",pno,ts);
			    }
				fclose(ptr1);
				pthread_mutex_unlock(&lock);

	while(1)
	{
		pthread_mutex_lock(&lock);
		printf("--------------process1---------------\n");
			ptr1 = fopen("file1.txt","r");
			while (!feof (ptr1))
		    {
			 	 int pno,ts;
			 	 fscanf(ptr1,"%d %d",&pno,&ts);
			 	 if(ts < 0)
			 		replyfrom[pno]=1;
			 	 printf("1file1 %d %d\n",pno,ts);
		    }
			if(replyfrom[2] == 1 && replyfrom[3]==1)
				break;
			fclose(ptr1);
			pthread_mutex_unlock(&lock);
			sleep(2);

	}

	pthread_mutex_lock(&lock);
				printf("--------------process1---------------\n");
					ptr1 = fopen("file1.txt","r");
					while (!feof (ptr1))
				    {+
					 	 fscanf(ptr1,"%d %d",&pno,&ts);
					 	 if(ts > 0 && (pno!=1))
					 	 {
					 		sendreply(1,pno,-ts);
					 		replyto[pno]=1;
					 	 }

					 	 printf("1file1 %d %d\n",pno,ts);
				    }
					fclose(ptr1);
					pthread_mutex_unlock(&lock);
	 pthread_mutex_lock(&lock);
	 printf("--------------process1---------------\n");
		shared = shared + 10;

		printf("p1 did %d\n",shared);
		pthread_mutex_unlock(&lock);


}



void *process2()
{
	int reply[4]={0};
	int clockvalue = 0;

	FILE *ptr1,*ptr2,*ptr3;
	pthread_mutex_lock(&lock);
	printf("--------------process2---------------\n");
	ptr2 = fopen("file2.txt","r");
	 char line [1000];
	 int pno=0,ts=0,timefromotherprocess=0;
	 while (!feof (ptr2))
	    {

		 	 fscanf(ptr2,"%d %d",&pno,&ts);
		 	 if(ts > 0 && (pno == 1 || pno == 3))
		 	 {
		 		if(ts > timefromotherprocess)
		 			timefromotherprocess = ts;
		 	 }


	 }
	 if(clockvalue < timefromotherprocess)
		clockvalue = timefromotherprocess + 1;
	else
		clockvalue = clockvalue + 1;

	printf("file2 %d %d\n",pno,ts);
	 fclose(ptr2);
	 pthread_mutex_unlock(&lock);
	 pthread_mutex_lock(&lock);
	 printf("--------------process2---------------\n");
	ptr1 = fopen("file1.txt","a");
	fprintf(ptr2,"%d %d\n",2,clockvalue);
	fclose(ptr2);
	ptr3 = fopen("file3.txt","a");
	fprintf(ptr3,"%d %d\n",2,clockvalue);
	fclose(ptr3);
	pthread_mutex_unlock(&lock);
	while(1)
	{
		pthread_mutex_lock(&lock);
		printf("--------------process2---------------\n");
			ptr2 = fopen("file2.txt","r");
			while (!feof (ptr2))
		    {
			 	 int pno,ts;
			 	 fscanf(ptr2,"%d %d",&pno,&ts);
			 	 if(ts < 0)
			 		 reply[pno]=1;
			 	 printf("2file2 %d %d\n",pno,ts);
		    }
			if(reply[2] == 1 && reply[3]==1)
				break;
			fclose(ptr2);
			pthread_mutex_unlock(&lock);
			sleep(1);

	}

	 pthread_mutex_lock(&lock);
	 printf("--------------process2---------------\n");
		shared = shared + 10;

		printf("p1 did %d\n",shared);
		pthread_mutex_unlock(&lock);

}


void *process3()
{
	printf("--------------process3---------------\n");
	int reply[4]={0};
	int clockvalue = 0;

	FILE *ptr1,*ptr2,*ptr3;
	pthread_mutex_lock(&lock);
	ptr3 = fopen("file3.txt","r");
	 char line [1000];
	 int pno=0,ts=0,timefromotherprocess=0;
	 while (!feof (ptr3))
	    {

		 	 fscanf(ptr3,"%d %d",&pno,&ts);
		 	 if(ts > 0 && (pno == 2 || pno == 1))
		 	 {
		 		if(ts > timefromotherprocess)
		 			timefromotherprocess = ts;
		 	 }


	 }
	 if(clockvalue < timefromotherprocess)
		clockvalue = timefromotherprocess + 1;
	else
		clockvalue = clockvalue + 1;

	printf("file3 %d %d\n",pno,ts);
	 fclose(ptr3);
	 pthread_mutex_unlock(&lock);
	 pthread_mutex_lock(&lock);
	 printf("--------------process3---------------\n");
	ptr2 = fopen("file2.txt","a");
	fprintf(ptr2,"%d %d\n",3,clockvalue);
	fclose(ptr2);
	ptr1 = fopen("file1.txt","a");
	fprintf(ptr1,"%d %d\n",3,clockvalue);
	fclose(ptr1);
	pthread_mutex_unlock(&lock);
	while(1)
	{
		pthread_mutex_lock(&lock);
		printf("--------------process3---------------\n");
			ptr1 = fopen("file3.txt","r");
			while (!feof (ptr1))
		    {
			 	 int pno,ts;
			 	 fscanf(ptr1,"%d %d",&pno,&ts);
			 	 if(ts < 0)
			 		 reply[pno]=1;
			 	 printf("3file3 %d %d\n",pno,ts);
		    }
			if(reply[2] == 1 && reply[1]==1)
				break;
			fclose(ptr3);
			pthread_mutex_unlock(&lock);
			sleep(3);

	}

	 pthread_mutex_lock(&lock);
	 printf("--------------process3---------------\n");
		shared = shared + 10;

		printf("p1 did %d\n",shared);
		pthread_mutex_unlock(&lock);

}

void main()
{
	pthread_t pt[4];
	    int tid[4];
	pthread_create(&pt[1], NULL, process1,NULL);
	pthread_create(&pt[2], NULL, process2,NULL);
	pthread_create(&pt[3], NULL, process3,NULL);
	pthread_join(pt[1], NULL);
	pthread_join(pt[2], NULL);
	pthread_join(pt[3], NULL);
}


