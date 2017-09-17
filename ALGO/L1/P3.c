#include <stdio.h>
#include <stdlib.h>
#define NO_OF_TRIALS 1

int *binarray,noofbins,randombinnumber,phase,noofbinsfull,currentnoofbinsfull;
int currentnoofballsthrown,noofballs;

void printbinarray();
int getnoofballstomakeallbinsfull();
void initializeallbinstozeros();
int checkforatleastoneballineachbin();
int checkforhowmanybinsfull();


void main()
{
	printf("Enter No of bins : ");
	scanf("%d",&noofbins);
	binarray = (int *)malloc((noofbins+1)*sizeof(int));
	int currenttrial = 1;
	while(currenttrial <= NO_OF_TRIALS)
	{
		printf("++++++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++++\n",currenttrial);
		initializeallbinstozeros();
		phase = 0;
		noofbinsfull = 0;
		currentnoofbinsfull = 0;
		currentnoofballsthrown = 0;
		while(1)
		{
			randombinnumber = rand() % noofbins + 1;
			binarray[randombinnumber]++;
			noofbinsfull = checkforhowmanybinsfull();
			if(noofbinsfull != currentnoofbinsfull)
			{
				currentnoofbinsfull = noofbinsfull;
				noofballs = getnoofballstomakeallbinsfull() - currentnoofballsthrown;
				printf("To Transistion from Phase %d to %d, no of balls thrown : %d\n",phase,(phase+1),noofballs);
				currentnoofballsthrown = getnoofballstomakeallbinsfull();
				phase++;
			}
			if(checkforatleastoneballineachbin() == 1)
				break;
		}
		printf("No of balls thrown to make each bin has atleast one ball : %d\n",getnoofballstomakeallbinsfull());
		//printbinarray();
		currenttrial++;
	}
}

int checkforhowmanybinsfull()
{
	int fullbins = 0;
	for(int i=1;i<=noofbins;i++)
		if(binarray[i] > 0)
			fullbins++;

	return fullbins;
}

void printbinarray()
{
	printf("Binarray is ");
	for(int i=1;i<=noofbins;i++)
		printf("%d | ",binarray[i]);
	printf("\n");
}

int getnoofballstomakeallbinsfull()
{
	int noofballs = 0;
	for(int i=1;i<=noofbins;i++)
		noofballs += binarray[i];

	return noofballs;
}

void initializeallbinstozeros()
{
	for(int i=1;i<=noofbins;i++)
		binarray[i] = 0;
}

int checkforatleastoneballineachbin()
{
	for(int i=1;i<=noofbins;i++)
		if(binarray[i] == 0)
			return 0; //Not full

	return 1; // Full
}




