#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tData[200][30][100]; // [No_of_instances][No_of_atributes][value_of_attribute]
int noOfTypes = 7;
int noOfInstances = 1;
int noOfAttributes = 18;
int countOfGeneralBoundariesForTypes[10];
int countOfSpecificBoundariesForTypes[10];

struct hypotheses
{
	char attr[20];
	struct hypotheses *ptrToNextHyp;
};

struct hypotheses generalHypothesis[10],specificHypothesis[10];

void ReadTDataFrmFile();
void printTData();
void initializespecificHypotheses();
void initializeGeneralHypotheses();
void startAlgo(int type);
void calculateSpecificBoundaries(int instance,int type);
void printSpecificHypotheses(int type);
void calculateGenericBoundaries(int instance,int type);
int  isHypothesisAcceptingInstance(int instance,struct hypotheses *temp);
void buildGeneralBoundariesFromSpecific(int type);
void insertIntoGeneralBoundary(int type,int attr);
void printGeneralHypotheses(int type);
struct hypotheses* removeGeneralBoundaryFromList(struct hypotheses* toRemove,int type);
void doRulePruning(int instance,int type);
void makeItLessGeneric(int instance,struct hypotheses* toRemove,int type);
int  isBothBoundariesSame(struct hypotheses *temp1,struct hypotheses *temp2);
void removeDuplicateGeneralBoundaries(int type);


int main()
{
	ReadTDataFrmFile();
	//printTData();
	initializeGeneralHypotheses();
	initializespecificHypotheses();
	for(int i=1;i<=noOfTypes;i++)
	{
		printf("++++++++++++++++++++++++++++++++++++++++ Type - %d +++++++++++++++++++++++++++++++++++++++++\n",i);
		startAlgo(i);
		removeDuplicateGeneralBoundaries(i);
		printSpecificHypotheses(i);
		printf("-------------------------------------------------------------------------------------------\n");
		printGeneralHypotheses(i);
		printf("\n\n");
	}

}

void startAlgo(int type)
{
	for(int i=1;i<noOfInstances;i++)
	{
		if(tData[i][18][1] == (char)(type+48))
		{
			calculateSpecificBoundaries(i,type);
		}
	}
	buildGeneralBoundariesFromSpecific(type);
	for(int i=1;i<noOfInstances;i++)
	{
		if(tData[i][18][1] != (char)(type+48))
		{
			calculateGenericBoundaries(i,type);
		}
		else
		{

			doRulePruning(i,type);
		}
	}
}

void calculateSpecificBoundaries(int instance,int type)
{
	if(specificHypothesis[type].attr[2] == '@')
	{
		for(int i=2;i<=noOfAttributes-1;i++) // Skipping name and type from attribute list
			{
				specificHypothesis[type].attr[i] = tData[instance][i][1];
			}
	}
	else
	{
		for(int i=2;i<=noOfAttributes-1;i++) // Skipping name and type from attribute list
		{
			if(tData[instance][i][1] != specificHypothesis[type].attr[i])
				specificHypothesis[type].attr[i] = '?';
		}

	}
}

void buildGeneralBoundariesFromSpecific(int type)
{
	for(int i=2;i<=noOfAttributes-1;i++)
	{
		if(specificHypothesis[type].attr[i] != '?')
			insertIntoGeneralBoundary(type,i);
	}
}

void insertIntoGeneralBoundary(int type,int attrID)
{
	//printf("type is %d and id is %d\n",type,attrID);
	struct hypotheses *newnode = (struct hypotheses *)malloc(sizeof(struct hypotheses));
	for(int i=2;i<=noOfAttributes-1;i++)
	{
		if(attrID == i)
			newnode->attr[i] = specificHypothesis[type].attr[attrID];
		else
			newnode->attr[i] = '?';
	}
	newnode->ptrToNextHyp = NULL;
	struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
	if(temp == NULL)
	{
		generalHypothesis[type].ptrToNextHyp = newnode;
	}
	else
	{
		while(temp->ptrToNextHyp)
		{
			temp = temp->ptrToNextHyp;
		}
		temp->ptrToNextHyp = newnode;
	}
}

void calculateGenericBoundaries(int instance,int type)
{
	struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
	while(temp)
	{
		if(!isHypothesisAcceptingInstance(instance,temp))
		{
			// Do Nothing as it is already not accepting Instance
		}
		else
		{
			makeItLessGeneric(instance,temp,type);
		}
		temp = temp->ptrToNextHyp;
	}
}

void doRulePruning(int instance,int type)
{
	struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
	while(temp)
	{
		if(!isHypothesisAcceptingInstance(instance,temp))
		{
			printf("divya\n");
			//printf("No\n");
			temp = removeGeneralBoundaryFromList(temp,type);

		}
		else
		{
			//printf("Yes\n");
			temp = temp->ptrToNextHyp;
		}
	}
}


int isHypothesisAcceptingInstance(int instance,struct hypotheses *temp)
{
	//printf("instance is %d and hyp is %c\n",instance,temp->attr[2]);
	int count = 0;
	for(int i=2;i<=noOfAttributes-1;i++)
	{
		if(temp->attr[i]=='?' || tData[instance][i][1] == temp->attr[i])
			count++;
	}
	if(count == noOfAttributes-2)
		return 1;
	else
		return 0;
}

void ReadTDataFrmFile()
{
	FILE *fptr =fopen("TData.txt","r");
	struct trainingData *temp = NULL;
	while(!feof(fptr))
	{
		fscanf(fptr,"%[^,],%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c%*[\n]",
				tData[noOfInstances][1],&tData[noOfInstances][2][1],&tData[noOfInstances][3][1],
				&tData[noOfInstances][4][1],&tData[noOfInstances][5][1],&tData[noOfInstances][6][1],
				&tData[noOfInstances][7][1],&tData[noOfInstances][8][1],&tData[noOfInstances][9][1],
				&tData[noOfInstances][10][1],&tData[noOfInstances][11][1],&tData[noOfInstances][12][1],
				&tData[noOfInstances][13][1],&tData[noOfInstances][14][1],&tData[noOfInstances][15][1],
				&tData[noOfInstances][16][1],&tData[noOfInstances][17][1],&tData[noOfInstances][18][1]);
		noOfInstances++;
	}
}

void printTData()
{
	for(int i=1;i<noOfInstances;i++)
		{
			printf("%s | ",tData[i][1]);
			for(int j=2;j<=noOfAttributes;j++)
				printf("%c | ",tData[i][j][1]);
			printf("\n");
		}
}

void printSpecificHypotheses(int type)
{
	for(int j=2;j<=noOfAttributes-1;j++)
		printf("%c | ",specificHypothesis[type].attr[j]);
	printf("--------->Specific Boundary\n");
}

void printGeneralHypotheses(int type)
{
	struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
	if(temp == NULL)
	{
		printf("\t\t\t\t\t\t\tNo Generic Boundary exists!!\n");
		return;
	}
	while(temp)
	{
		for(int j=2;j<=noOfAttributes-1;j++)
			printf("%c | ",temp->attr[j]);
		printf("--------->Generic Boundary\n");
		temp = temp->ptrToNextHyp;
	}
}

void initializeGeneralHypotheses()
{
	for(int i=1;i<=noOfTypes;i++)
	{
		for(int j=1;j<=noOfAttributes;j++)
			generalHypothesis[i].attr[j] = '?';
		generalHypothesis[i].ptrToNextHyp = NULL;
	}

}

void initializespecificHypotheses()
{
	for(int i=1;i<=noOfTypes;i++)
	{
		for(int j=1;j<=noOfAttributes;j++)
			specificHypothesis[i].attr[j] = '@';
		specificHypothesis[i].ptrToNextHyp = NULL;
	}
}

struct hypotheses* removeGeneralBoundaryFromList(struct hypotheses* toRemove,int type)
{
	struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
	if(toRemove == temp)
	{
		generalHypothesis[type].ptrToNextHyp = toRemove->ptrToNextHyp;
		free(toRemove);
		return generalHypothesis[type].ptrToNextHyp;
	}
	else
	{
		while(temp)
		{
			if(temp->ptrToNextHyp == toRemove)
			{
				temp->ptrToNextHyp = toRemove->ptrToNextHyp;
				free(toRemove);
				return temp->ptrToNextHyp;
			}
			temp= temp->ptrToNextHyp;
		}
	}
	return NULL;
}


void makeItLessGeneric(int instance,struct hypotheses* toChange,int type)
{
	//printf("None\n");
	//struct hypotheses *tempToGeneral = generalHypothesis[type].ptrToNextHyp;
	struct hypotheses *tempToSpecific = &specificHypothesis[type];
	for(int i=2;i<=noOfAttributes-1;i++)
	{
		if(tData[instance][i][1] != tempToSpecific->attr[i])
		{
			if(tempToSpecific->attr[i]!='?')
				{
					toChange->attr[i] = tempToSpecific->attr[i];
					break;
				}
		}
	}

}

int isBothBoundariesSame(struct hypotheses *temp1,struct hypotheses *temp2)
{
	int count = 0;
	for(int i=2;i<=noOfAttributes-1;i++)
		if(temp1->attr[i] == temp2->attr[i])
			count++;
	if(count == noOfAttributes-2)
		return 1;
	else
		return 0;
}


void removeDuplicateGeneralBoundaries(int type)
{
		struct hypotheses *temp = generalHypothesis[type].ptrToNextHyp;
		while(temp->ptrToNextHyp->ptrToNextHyp)
		{
			struct hypotheses *temp1 = temp->ptrToNextHyp;
			while(temp1)
			{
				if(isBothBoundariesSame(temp,temp1))
					temp1 = removeGeneralBoundaryFromList(temp1,type);
				else
					temp1 = temp1->ptrToNextHyp;
			}
			temp = temp->ptrToNextHyp;
		}



}













