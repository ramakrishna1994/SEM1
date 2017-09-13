#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NO_OF_TRIALS 100

int *binarray;
int noofballs,randomnumber,count,noofbins;
int sumofnmax = 0;
int *nmaxarray;

void allocateballstobins();
int findindexofbinwhichgotmaxballs();
int findnoofmaxballscametoabin();
int calculateaverageofnmax();
float variance();
float standarddeviation();

void main()
{
	printf("Enter No. of bins : ");
	scanf("%d",&noofbins);
	printf("Enter No. of balls : ");
	scanf("%d",&noofballs);
	binarray = (int *)malloc((noofbins+1)*sizeof(int));
	nmaxarray = (int *)malloc((NO_OF_TRIALS+1)*sizeof(int));
	int currenttrial = 1;
	while(currenttrial <= NO_OF_TRIALS)
	{
		printf("+++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++++++++\n",currenttrial);
		count = 1;
		// initializing all to zeroes
		for(int i=1;i<=noofbins;i++)
			binarray[i] = 0;
		allocateballstobins();
		printf("Maximum No. of balls in a bin are (N-Max) : %d\n",findnoofmaxballscametoabin());
		printf("They are present in Bin No. : %d\n",findindexofbinwhichgotmaxballs());
		nmaxarray[currenttrial] = findnoofmaxballscametoabin();
		currenttrial++;
	}
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("Average of N-Max across %d trials is %d\n",NO_OF_TRIALS,calculateaverageofnmax());
	printf("Variance of N-Max across %d trials is %f\n",NO_OF_TRIALS,variance());
	printf("S.D of N-Max across %d trials is %f\n",NO_OF_TRIALS,standarddeviation());
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

float variance()
{
	int deviation = 0;
	int averageofnmaax = calculateaverageofnmax();
	for(int i=1;i<=NO_OF_TRIALS;i++)
	{
		deviation += ((nmaxarray[i]-averageofnmaax)*(nmaxarray[i]-averageofnmaax));
	}
	float variance = deviation / NO_OF_TRIALS;
	return variance;
}

float standarddeviation()
{
	return sqrt(variance());
}

int calculateaverageofnmax()
{
	int sumofnmax = 0;
	for(int i=1;i<=NO_OF_TRIALS;i++)
	{
		sumofnmax += nmaxarray[i];
	}
	int averageofnmax = sumofnmax / NO_OF_TRIALS;
	return averageofnmax;
}

void allocateballstobins()
{

	for(int i=1;i<=noofballs;i++)
	{
			randomnumber = rand() % noofbins + 1;
			binarray[randomnumber]++;
	}
}

int findindexofbinwhichgotmaxballs()
{
	int max = 0;
	int binno = 0;
	for(int i=0;i<=noofbins;i++)
	{
		if(max < binarray[i])
		{
			max = binarray[i];
			binno = i;
		}

	}
	return binno;
}

int findnoofmaxballscametoabin()
{
	return binarray[findindexofbinwhichgotmaxballs()];
}






