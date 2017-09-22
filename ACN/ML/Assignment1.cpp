#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hypothesis
{
	bool hair;
	bool feathers;
	bool eggs;
	bool milk;
	bool airborne;
	bool aquatic;
	bool predator;
	bool toothed;
	bool backbone;
	bool breathes;
	bool venomous;
	bool fins;
	int legs;
	bool tail;
	bool domestic;
	bool catsize;
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

void ReadTDataFrmFile();
struct trainingData *insertTDataInStruct();
void printData(int choice);
void printTData();
void printHData();


int main()
{
	ReadTDataFrmFile();
	printData(1);
	return 0;
}


void ReadTDataFrmFile()
{
	FILE *fptr =fopen("TData.txt","r");
	struct trainingData *temp = NULL;
	while(!feof(fptr))
	{
		temp = insertTDataInStruct();
		fscanf(fptr,"%[^,],%d,%d,%d,%d,%d,%d,%d,%d,"
				"%d,%d,%d,%d,%d,%d,%d,%d,%d%*[\n]",
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
			printHData();
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
		printf("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				temp->name,temp->hyp.hair,temp->hyp.feathers,
				temp->hyp.eggs,temp->hyp.milk,temp->hyp.airborne,
				temp->hyp.aquatic,temp->hyp.predator,temp->hyp.toothed,
				temp->hyp.backbone,temp->hyp.breathes,temp->hyp.venomous,
				temp->hyp.fins,temp->hyp.legs,temp->hyp.tail,
				temp->hyp.tail,temp->hyp.domestic,temp->type);
		temp = temp->ptrToNxtTdata;
	}
}

void printHData()
{

}


















