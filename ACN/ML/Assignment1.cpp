#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hypothesis
{
	char hair;
	char feathers;
	char eggs;
	char milk;
	char airborne;
	char aquatic;
	char predator;
	char toothed;
	char backbone;
	char breathes;
	char venomous;
	char fins;
	char legs;
	char tail;
	char domestic;
	char catsize;
	struct hypothesis *ptrToNxtHyp;
};

struct trainingData
{
	char name[100];
	struct  hypothesis hyp;
	int type;
	struct trainingData * ptrToNxtTdata;
};

struct trainingData *headOfTD = NULL;
struct hypothesis *headOfHp = NULL;
struct hypothesis genericHypothesis[10] =  {'?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?',NULL};
struct hypothesis specificHypothesis[10];

void ReadTDataFrmFile();
struct trainingData *insertTDataInStruct();
void printData(int choice);
void printTData();
void printHData(struct hypothesis *temp);
void startProcessing(int type);
void calculateGenericHypothesis(int type,struct trainingData *temp);
void calculateSpecificHypothesis(int type,struct trainingData *temp);
void intializeGenericHypotheses();
void intializeSpecificHypotheses();
void copyValuesOfTDToHyp(struct trainingData *from,struct hypothesis *to);
void compareValuesOfTDWithSpecific(struct trainingData *from,struct hypothesis *to);
void printSpecificHypothesisOfAllTypes();
void buildGeneralBoundariesFromSpecificBoundaries(int type);
void checkIfGeneralAcceptingNegativeHyp(int type,struct trainingData *temp);


int main()
{
	ReadTDataFrmFile();
	intializeGenericHypotheses();
	intializeSpecificHypotheses();
	startProcessing(1);
	printSpecificHypothesisOfAllTypes();
	return 0;
}

void printSpecificHypothesisOfAllTypes()
{
	for(int i=1;i<=10;i++)
		printHData(&specificHypothesis[i]);
}

void startProcessing(char type)
{
	struct trainingData *temp = headOfTD;

	while(temp)
	{
		calculateGenericHypothesis(type,temp);
		calculateSpecificHypothesis(type,temp);
		temp=temp->ptrToNxtTdata;
	}
}

void calculateGenericHypothesis(int type,struct trainingData *temp)
{
		if(temp->type == type)
		{

		}
		else
		{
			if(!checkIfGeneralAcceptingNegativeHyp(type,temp))
			{

			}
			else
			{

			}
		}
}

void checkIfGeneralAcceptingNegativeHyp(int type,struct trainingData *temp)
{

}

void buildGeneralBoundariesFromSpecificBoundaries(int type)
{
	struct hypothesis *to = &specificHypothesis[type];
	while(to)
	{
		if(to->hair != '?')
			//generateNewGeneralBoundary();
		if(to->feathers != '?')
			to->feathers = '?';
		if(to->eggs != '?')
			to->eggs = '?';
		if(to->milk != '?')
			to->milk = '?';
		if(to->airborne != '?')
			to->airborne = '?';
		if(to->aquatic != '?')
			to->aquatic = '?';
		if(to->predator != '?')
			to->predator = '?';
		if(to->toothed != '?')
			to->toothed = '?';
		if(to->backbone != '?')
			to->backbone = '?';
		if(to->breathes != '?')
			to->breathes = '?';
		if(to->venomous != '?')
			to->venomous = '?';
		if(to->fins != '?')
			to->fins = '?';
		if(to->legs != '?')
			to->legs = '?';
		if(to->tail != '?')
			to->tail = '?';
		if(to->domestic != '?')
			to->domestic = '?';
		if(to->catsize != '?')
			to->catsize = '?';
		to = to->ptrToNxtHyp;
	}
}

void calculateSpecificHypothesis(int type,struct trainingData *temp)
{

		if(temp->type == type)
		{
			if(specificHypothesis[type].hair == '@')
			{
				//printHData(&specificHypothesis[type]);
				copyValuesOfTDToHyp(temp,&specificHypothesis[type]);
				//printHData(&specificHypothesis[type]);
			}
			else
			{
				//printHData(&specificHypothesis[type]);
				compareValuesOfTDWithSpecific(temp,&specificHypothesis[type]);
			}
		}

}

void compareValuesOfTDWithSpecific(struct trainingData *from,struct hypothesis *to)
{
	if(to->hair != from->hyp.hair)
		to->hair = '?';
	if(to->feathers != from->hyp.feathers)
		to->feathers = '?';
	if(to->eggs != from->hyp.eggs)
		to->eggs = '?';
	if(to->milk != from->hyp.milk)
		to->milk = '?';
	if(to->airborne != from->hyp.airborne)
		to->airborne = '?';
	if(to->aquatic != from->hyp.aquatic)
		to->aquatic = '?';
	if(to->predator != from->hyp.predator)
		to->predator = '?';
	if(to->toothed != from->hyp.toothed)
		to->toothed = '?';
	if(to->backbone != from->hyp.backbone)
		to->backbone = '?';
	if(to->breathes != from->hyp.breathes)
		to->breathes = '?';
	if(to->venomous != from->hyp.breathes)
		to->venomous = '?';
	if(to->fins != from->hyp.fins)
		to->fins = '?';
	if(to->legs != from->hyp.legs)
		to->legs = '?';
	if(to->tail != from->hyp.tail)
		to->tail = '?';
	if(to->domestic != from->hyp.domestic)
		to->domestic = '?';
	if(to->catsize != from->hyp.catsize)
		to->catsize = '?';
}


void copyValuesOfTDToHyp(struct trainingData *from,struct hypothesis *to)
{
	to->hair = from->hyp.hair;
	to->feathers = from->hyp.feathers;
	to->eggs = from->hyp.eggs;
	to->milk = from->hyp.milk;
	to->airborne = from->hyp.airborne;
	to->aquatic = from->hyp.aquatic;
	to->predator = from->hyp.predator;
	to->toothed = from->hyp.toothed;
	to->backbone = from->hyp.backbone;
	to->breathes = from->hyp.breathes;
	to->venomous = from->hyp.breathes;
	to->fins = from->hyp.fins;
	to->legs = from->hyp.legs;
	to->tail = from->hyp.tail;
	to->domestic = from->hyp.domestic;
	to->catsize = from->hyp.catsize;
	to->ptrToNxtHyp = NULL;
}


void ReadTDataFrmFile()
{
	FILE *fptr =fopen("TData.txt","r");
	struct trainingData *temp = NULL;
	while(!feof(fptr))
	{
		temp = insertTDataInStruct();
		fscanf(fptr,"%[^,],%c,%c,%c,%c,%c,%c,%c,%c,"
				"%c,%c,%c,%c,%c,%c,%c,%c,%d%*[\n]",
				temp->name,&temp->hyp.hair,&temp->hyp.feathers,
				&temp->hyp.eggs,&temp->hyp.milk,&temp->hyp.airborne,
				&temp->hyp.aquatic,&temp->hyp.predator,&temp->hyp.toothed,
				&temp->hyp.backbone,&temp->hyp.breathes,&temp->hyp.venomous,
				&temp->hyp.fins,&temp->hyp.legs,&temp->hyp.tail,
				&temp->hyp.tail,&temp->hyp.domestic,&temp->type);
	}
}

struct trainingData *insertTDataInStruct()
{
	struct trainingData *newnode = (struct trainingData *)malloc(sizeof(struct trainingData));
	if(headOfTD == NULL)
		headOfTD = newnode;
	else
	{
		struct trainingData *temp = headOfTD;
		while(temp->ptrToNxtTdata)
		{
			temp=temp->ptrToNxtTdata;
		}
		temp->ptrToNxtTdata = newnode;
	}
	return newnode;
}

void printData(int choice)
{
	switch(choice)
	{
		case 1:
			printTData();
			break;
		case 2:
			//printHData();
			break;
		default:
			printf("Enter Correct Choice\n");
			break;
	}
}

void printTData()
{
	struct trainingData *temp = headOfTD;
	while(temp)
	{
		printf("%s,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%d\n",
				temp->name,temp->hyp.hair,temp->hyp.feathers,
				temp->hyp.eggs,temp->hyp.milk,temp->hyp.airborne,
				temp->hyp.aquatic,temp->hyp.predator,temp->hyp.toothed,
				temp->hyp.backbone,temp->hyp.breathes,temp->hyp.venomous,
				temp->hyp.fins,temp->hyp.legs,temp->hyp.tail,
				temp->hyp.tail,temp->hyp.domestic,temp->type);
		temp = temp->ptrToNxtTdata;
	}
}

void printHData(struct hypothesis *temp)
{
	while(temp)
		{
		printf("%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c\n",
				temp->hair,temp->feathers,
				temp->eggs,temp->milk,temp->airborne,
				temp->aquatic,temp->predator,temp->toothed,
				temp->backbone,temp->breathes,temp->venomous,
				temp->fins,temp->legs,temp->tail,
				temp->tail,temp->domestic);
		temp = temp->ptrToNxtHyp;
		}

}

void intializeGenericHypotheses()
{
	for(int i=0;i<10;i++)
	{
		genericHypothesis[i].hair = '?';
		genericHypothesis[i].feathers = '?';
		genericHypothesis[i].eggs = '?';
		genericHypothesis[i].milk = '?';
		genericHypothesis[i].airborne = '?';
		genericHypothesis[i].aquatic = '?';
		genericHypothesis[i].predator = '?';
		genericHypothesis[i].toothed = '?';
		genericHypothesis[i].backbone = '?';
		genericHypothesis[i].breathes = '?';
		genericHypothesis[i].venomous = '?';
		genericHypothesis[i].fins = '?';
		genericHypothesis[i].legs = '?';
		genericHypothesis[i].tail = '?';
		genericHypothesis[i].domestic = '?';
		genericHypothesis[i].catsize = '?';
		genericHypothesis[i].ptrToNxtHyp = NULL;
	}
}

void intializeSpecificHypotheses()
{
	for(int i=0;i<10;i++)
	{
		specificHypothesis[i].hair = '@';
		specificHypothesis[i].feathers = '@';
		specificHypothesis[i].eggs = '@';
		specificHypothesis[i].milk = '@';
		specificHypothesis[i].airborne = '@';
		specificHypothesis[i].aquatic = '@';
		specificHypothesis[i].predator = '@';
		specificHypothesis[i].toothed = '@';
		specificHypothesis[i].backbone = '@';
		specificHypothesis[i].breathes = '@';
		specificHypothesis[i].venomous = '@';
		specificHypothesis[i].fins = '@';
		specificHypothesis[i].legs = '@';
		specificHypothesis[i].tail = '@';
		specificHypothesis[i].domestic = '@';
		specificHypothesis[i].catsize = '@';
		specificHypothesis[i].ptrToNxtHyp = NULL;
	}
}


















