#include <stdio.h>
#include <stdlib.h>
#define NO_OF_TRIALS 1000000

int *peoplearray,currenttrial=1;
int noofpeople,noofdays;

void assignrandomdatestopeople();
int checkfortwopeoplehavingsamebirthday();
void intializepeoplearraytozeroes();
void printarray();

void main()
{

	printf("Enter the No of people : ");
	scanf("%d",&noofpeople);
	printf("Enter the no of days :");
	scanf("%d",&noofdays);
	peoplearray = (int *)malloc((noofpeople+1)*sizeof(int));
	int count = 0;
	while(currenttrial <= NO_OF_TRIALS)
	{
		intializepeoplearraytozeroes();
		assignrandomdatestopeople();
		count += checkfortwopeoplehavingsamebirthday();
		//printarray();
		currenttrial++;
	}
	printf("Out of %d Trials, there are %d Trials where atleast 2 people have same birthday!!\n",NO_OF_TRIALS,count);
	float prob = (float)count / (float)NO_OF_TRIALS;
	printf("So Probability is %.4f\n",prob);
}

int checkfortwopeoplehavingsamebirthday()
{
	for(int i=1;i<=noofpeople;i++)
	{
		for(int j=i+1;j<=noofpeople;j++)
			if(peoplearray[i] == peoplearray[j])
				return 1;
	}
	return 0;
}

void assignrandomdatestopeople()
{
	for(int i=1;i<=noofpeople;i++)
	{
		peoplearray[i] = (rand() % noofdays + 1);
	}
}

void intializepeoplearraytozeroes()
{
	for(int i=1;i<=noofpeople;i++)
	{
		peoplearray[i] = 0;
	}
}

void printarray()
{
	printf("people array is ");
	for(int i=1;i<=noofpeople;i++)
		printf("%d | ",peoplearray[i]);
	printf("\n");
}





