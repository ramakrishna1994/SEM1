#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_OF_TRIALS 100

int *hatarray;
int  noofpeople,randomnumber,count;

void printpeoplewhogottheirownhat();
void randomizepeopleandhats();
int getcountofpeoplewhogottheirhat();
int checkrandomnumberalreadypresent(int rand);


void main()
{
	printf("Enter No. of people : ");
	scanf("%d",&noofpeople);
	hatarray = (int *)malloc((noofpeople+1)*sizeof(int));
	int currenttrial = 1;
	while(currenttrial <= NO_OF_TRIALS)
	{
		printf("+++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++++++++\n",currenttrial);
		count = 1;
		// initializing all to zeroes
		for(int i=1;i<=noofpeople;i++)
			hatarray[i] = 0;
		randomizepeopleandhats();
		printf("No of people who got their hats are : %d\n",getcountofpeoplewhogottheirhat());
		printpeoplewhogottheirownhat();
		currenttrial++;
	}


}

void printpeoplewhogottheirownhat()
{
	printf("people are ");
	for(int i=1;i<=noofpeople;i++)
	{
		if(i == hatarray[i])
			printf("%d | ",i);
	}
	if(getcountofpeoplewhogottheirhat() == 0)
		printf("none");
	printf("\n");
}

void randomizepeopleandhats()
{
	srand(time(NULL));
	for(int i=1;i<=noofpeople;i++)
	{
		while(1)
		{
			randomnumber = rand() % noofpeople + 1;
			if(!checkrandomnumberalreadypresent(randomnumber))
			{
				hatarray[count] = randomnumber;
				count++;
				break;
			}
		}
	}
}

int getcountofpeoplewhogottheirhat()
{
	int count = 0;
	for(int i=1;i<=noofpeople;i++)
	{
		if(i == hatarray[i])
			count++;
	}
	return count;
}

int checkrandomnumberalreadypresent(int rand)
{
	for(int i=1;i<=count;i++)
	{
		if(rand == hatarray[i])
			return 1;
	}
	return 0;
}
