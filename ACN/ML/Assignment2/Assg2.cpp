#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>
#define HIGHVALUE -100;

using namespace std;

struct AtttributeMetadata{
	int Index;
	string Name;
	string DistinctValues[200];
	double Range[200][3];
	int NoOfdistinctValues;
	int IsContinous;
	int IsDone;
	struct AtttributeMetadata *PtrToNextAttr;
};

struct DTreeValue
{
	int Index;
	string Value;
	double range[2][3];
	struct DTreeNode *ptrToNextNode;
};

struct DTreeNode
{
	string Name;
	int NoOfDistinctValues;
	int isContinous;
	int isLeaf;
	int index;
	int isPruned;
	struct DTreeValue *PtrToNextValue[200];
};


string **tData = NULL; // [No_of_instances][No_of_atributes]
string **vData = NULL; // [No_of_instances][No_of_atributes]
unsigned long long int temparray[3][35000];
unsigned long long int noOfInstances = 1;
unsigned long long int countOfVData = 1;
int noOfAttributes = 15;
struct AtttributeMetadata *headOfMetadata = NULL;
set<unsigned long long int> sa;
struct DTreeNode * headOfDTree = NULL;
int countOfValues = 0;
string **trimmedData;
int trimmedDataCount = 1;
struct DTreeNode *randomTree[10] = {NULL};
string result;
struct DTreeNode *nodeToBePruned = NULL;

/*
void ReadTDataFrmFile();
void printTData();
void handleContinousValues();
void ReadMetadata();
struct AtttributeMetadata *insertMetadata();
void makeItDiscreteValues(struct AtttributeMetadata* temp);
void printArray(unsigned long long int (&array)[3][35000],int length);
unsigned long long int findCount(int index,unsigned long long int value,string compare);
struct DTreeNode* BuildDTree(string (&oldArray)[35000][30] ,int oldCount);
*/


void printTData(string **array,int length)
{
	cout << "+++++++++++++++++++ printing data ++++++++++++++++++++++++++++++++\n";
	cout << "address is " << array << "\n";
	for(int i=1;i<length;i++)
		{
			for(int j=1;j<=noOfAttributes;j++)
				cout<<array[i][j]<<"|";
			cout<<"\n";
		}
	cout << "length " << length << "\n";
	cout << "+++++++++++++++++++ end ++++++++++++++++++++++++++++++++\n";
}

int getLessThan(struct AtttributeMetadata *temp,string value,string **array,int length)
{
	int count=0;
	if(temp!=NULL)
	{
		for(int i=1;i<length;i++)
			if(array[i][temp->Index]==value && array[i][noOfAttributes] == "<=50K")
				count++;

	}
	else
	{
		for(int i=1;i<length;i++)
			if(array[i][noOfAttributes] == "<=50K")
				count++;


	}
	return count;
}

int getGreaterThan(struct AtttributeMetadata *temp,string value,string **array,int length)
{
	int count=0;
	if(temp!=NULL)
		{
			for(int i=1;i<length;i++)
				if(array[i][temp->Index]==value && array[i][noOfAttributes] == ">50K")
					count++;

		}
		else
		{
			for(int i=1;i<length;i++)
				if(array[i][noOfAttributes] == ">50K")
					count++;


		}
	return count;

}



int getLessThanRange(struct AtttributeMetadata *temp,double lessValue,double highValue,string **array,int length)
{
	int count=0;
	if(highValue == -100)
	{
		for(int i=1;i<length;i++)
			if(atoi(array[i][temp->Index].c_str())>lessValue && array[i][noOfAttributes] == "<=50K")
				count++;
		return count;
	}
	if(temp!=NULL)
	{
		for(int i=1;i<length;i++)
			if(atoi(array[i][temp->Index].c_str())>lessValue && atoi(array[i][temp->Index].c_str()) && array[i][noOfAttributes] == "<=50K")
				count++;

	}
	else
	{
		for(int i=1;i<length;i++)
			if(array[i][noOfAttributes] == "<=50K")
				count++;


	}
	return count;
}

int getGreaterThanRange(struct AtttributeMetadata *temp,double lessValue,double highValue,string **array,int length)
{
	int count=0;
		if(highValue == -100)
		{
			for(int i=1;i<length;i++)
				if(atoi(array[i][temp->Index].c_str())>lessValue && array[i][noOfAttributes] == ">50K")
					count++;
			return count;
		}
		if(temp!=NULL)
		{
			for(int i=1;i<length;i++)
				if(atoi(array[i][temp->Index].c_str())>lessValue && atoi(array[i][temp->Index].c_str())<highValue && array[i][noOfAttributes] == ">50K")
					count++;

		}
		else
		{
			for(int i=1;i<length;i++)
				if(array[i][noOfAttributes] == ">50K")
					count++;


		}
	return count;

}

double calculateEntropyWithRespectToAttr(double a,double b,double total)
{
		double part1 = (double)a/(double)total;
		double part2 = (double)b/(double)total;
		double entropy = 0;
		if((part1 == 0 && part2 == 0) || total == 0)
		{
			return entropy;
		}
		else if(part2 == 0)
		{
			entropy =  -(part1 * log(part1)) / log(2);
		}
		else if(part1 == 0)
		{
			entropy =  -(part2 * log(part2)) / log(2);
		}
		else
		{
			entropy =  (-(part1 * log(part1))-(part2 * log(part2))) / log(2);
		}

		//cout << "float is " << entropy << "\n";
		return entropy;

}


double calculateInformationGain(struct AtttributeMetadata *attr,string **array,int count)
{

	float globala = getLessThan(NULL,"",array,count);
	float globalb = getGreaterThan(NULL,"",array,count);
	float globalTotal = globala + globalb;
	//cout << globala << "---" << globalb << "\n";
	float locala,localb,localTotal;
	double globalEntropy = calculateEntropyWithRespectToAttr(globala,globalb,globalTotal);
	if(globalEntropy == 0)
		return 0;
	if(attr->IsContinous == 0)
	{
		for(int i=1;i<attr->NoOfdistinctValues;i++)
		{
			locala = getLessThan(attr,attr->DistinctValues[i],array,count);
			localb = getGreaterThan(attr,attr->DistinctValues[i],array,count);
			localTotal = locala + localb;
			//cout << locala << "---" << localb << "\n";
			globalEntropy -= (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
		}
		return globalEntropy;
	}
	else
	{
		for(int i=1;i<attr->NoOfdistinctValues;i++)
		{
			//TO-DO//
			locala = getLessThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localb = getGreaterThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localTotal = locala + localb;
			//cout << locala << "---" << localb << "\n";
			globalEntropy -= (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
			return globalEntropy;

		}
	}
	return 0;
}

int checkForAlgoCompletion()
{
	//cout << "done" << "\n";
	struct AtttributeMetadata *check = headOfMetadata;
	int noOfAttributesDone = 0;
	while(check)
	{
		if(check->IsDone == 1)
			noOfAttributesDone++;
		check = check->PtrToNextAttr;
	}
	if(noOfAttributesDone == noOfAttributes-1)
		return 1;
	else
		return 0;
}


void traverseTree(struct DTreeNode *temp)
{
	if(temp == NULL)
		return;
	if(temp->isLeaf == 1)
	{
		//cout << "leaf node and value is  " << temp->PtrToNextValue[1]->Value << endl;
		return;
	}
	else
	{
			cout << "name is  " << temp->Name  << " and pruning is " << temp->isPruned << endl;
			for(int i=1;i<temp->NoOfDistinctValues;i++)
			{
				//cout << temp->PtrToNextValue[i]->range[1][1] << " & " << temp->PtrToNextValue[i]->range[1][1] << endl;
				//cout << "child " << i << " is " << temp->PtrToNextValue[i] << endl;
				traverseTree(temp->PtrToNextValue[i]->ptrToNextNode);

			}

	}

}

struct DTreeNode *BuildLeafNode(string **oldArray,int oldCount)
{
	int noOfLessThan50K = getLessThan(NULL,"",oldArray,oldCount);
	int noOfGreaterThan50K = getGreaterThan(NULL,"",oldArray,oldCount);
	struct DTreeNode *newDtreeNode = new DTreeNode();
	newDtreeNode->Name = "LeafNode";
	newDtreeNode->isLeaf = 1;
	newDtreeNode->NoOfDistinctValues = 2;
	struct DTreeValue *newValueNode = new DTreeValue();
	newValueNode->ptrToNextNode = NULL;
	newDtreeNode->PtrToNextValue[1] = newValueNode;
	if(noOfLessThan50K == noOfGreaterThan50K)
	{
		newValueNode->Value = "AcceptAny";
	}
	else if(noOfLessThan50K > noOfGreaterThan50K)
	{
		newValueNode->Value = "<=50K";
	}
	else if(noOfLessThan50K < noOfGreaterThan50K)
	{
		newValueNode->Value = ">50K";
	}
	return newDtreeNode;

}

struct DTreeNode* BuildDTree(string **oldArray,int oldCount,string name,int child)
{


	//cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	//cout << "Parent is " << name << " and child id is " << child << endl;
	if(checkForAlgoCompletion())
		return NULL;
	else
	{
		struct AtttributeMetadata *temp = headOfMetadata;
		struct AtttributeMetadata *finalOne = NULL;
		float prev = 0;
		double newGain = -1;
		double max = -1;
		int index;

		//printTData(oldArray,oldCount);
		while(temp)
		{
			if(temp->IsDone != 1)
			{
				newGain = calculateInformationGain(temp,oldArray,oldCount);
				//cout << temp->Name << " information gain is : " << newGain << "\n";
			}

			if(max < newGain)
			{
				max = newGain;
				finalOne = temp;
			}

			temp = temp->PtrToNextAttr;
		}
		if(max == 0)
		{
			//cout << "No Max so Null Returned\n";
			struct DTreeNode *leafNode = BuildLeafNode(oldArray,oldCount);
			return leafNode;
		}
		else
		{
			//cout << "\nmax is " << finalOne->Name << " and information gain is : " << max << "\n";
		}

		struct DTreeNode *newDtreeNode = new DTreeNode();
		newDtreeNode->Name = finalOne->Name;
		newDtreeNode->NoOfDistinctValues = finalOne->NoOfdistinctValues;
		newDtreeNode->isContinous = finalOne->IsContinous;
		newDtreeNode->index = finalOne->Index;
		newDtreeNode->isLeaf = 0;
		newDtreeNode->isPruned = 0;
		if(finalOne->IsContinous == 1)
		{
			for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
			{
				struct DTreeValue *newValueNode = new DTreeValue();
				newValueNode->range[1][1] = finalOne->Range[i][1];
				newValueNode->range[1][2] = finalOne->Range[i][2];
				newValueNode->Index = finalOne->Index;
				newDtreeNode->PtrToNextValue[i] = newValueNode;
			}
		}
		else
		{
			for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
			{
				struct DTreeValue *newValueNode = new DTreeValue();
				newValueNode->Value = finalOne->DistinctValues[i];
				newValueNode->Index = finalOne->Index;
				newDtreeNode->PtrToNextValue[i] = newValueNode;
			}

		}
		finalOne->IsDone = 1;



		for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
		{

			// dynamic allocation
			string** newArray = new string*[oldCount];
			for(int rk = 0; rk <= oldCount; ++rk)
				newArray[rk] = new string[20];
			int newCount = 1;
			if(newDtreeNode->isContinous == 1)
			{
				//cout << "came inside if " << i << " and " << newDtreeNode->Name << endl;

				if(newDtreeNode->PtrToNextValue[i]->range[1][2] == -100)
				{
					for(int k=1;k<oldCount;k++)
					{
						if(atoi((oldArray[k][newDtreeNode->PtrToNextValue[i]->Index]).c_str()) > newDtreeNode->PtrToNextValue[i]->range[1][1])
						{
							for(int j=1;j<=noOfAttributes;j++)
							{
								newArray[newCount][j] = oldArray[k][j];

							}
							newCount++;
						}
					}
				}
				else
				{
					for(int k=1;k<oldCount;k++)
					{

						if(atoi((oldArray[k][newDtreeNode->PtrToNextValue[i]->Index]).c_str()) > newDtreeNode->PtrToNextValue[i]->range[1][1]
							   && atoi((oldArray[k][newDtreeNode->PtrToNextValue[i]->Index]).c_str()) < newDtreeNode->PtrToNextValue[i]->range[1][2])
						{
							for(int j=1;j<=noOfAttributes;j++)
							{
								newArray[newCount][j] = oldArray[k][j];

							}
							newCount++;
						}
					}


				}
				//cout <<"\n";
				//cout << "calling with count : " << newCount << " and value : " << newDtreeNode->PtrToNextValue[i]->range[1][1] << " & "<< newDtreeNode->PtrToNextValue[i]->range[1][2] << endl;


			}
			else
			{
				//cout << "came inside else " << i << " and " << newDtreeNode->Name << endl;
				for(int k=1;k<oldCount;k++)
				{
					if(oldArray[k][newDtreeNode->PtrToNextValue[i]->Index] == newDtreeNode->PtrToNextValue[i]->Value)
					{
						for(int j=1;j<=noOfAttributes;j++)
						{
							newArray[newCount][j] = oldArray[k][j];

						}
						newCount++;
					}

				}
				//cout <<"\n";
				//cout << "calling with count : " << newCount << " and value : " << newDtreeNode->PtrToNextValue[i]->Value << "\n";


			}

			newDtreeNode->PtrToNextValue[i]->ptrToNextNode = BuildDTree(newArray,newCount,newDtreeNode->Name,i);
			//delete(newArray);


		}



	return newDtreeNode;
	}

}





struct AtttributeMetadata* insertMetadata()
{
	struct AtttributeMetadata *temp = headOfMetadata;
	struct AtttributeMetadata *newnode = new AtttributeMetadata();
	newnode->PtrToNextAttr = NULL;
	if(temp == NULL)
	{
		headOfMetadata = newnode;
	}
	else
	{
		while(temp->PtrToNextAttr)
		{
			temp = temp->PtrToNextAttr;
		}
		temp->PtrToNextAttr = newnode;
	}
	return newnode;
}


void ReadMetadata()
{
	ifstream infile("attrmetadata.txt");
	string line;
	string temporary[100];
	int index = 1;
	while (std::getline(infile, line))
	{
		struct AtttributeMetadata *newnode = NULL;
		istringstream ss1(line);
		string token;
		newnode = insertMetadata();
		newnode->Index = index;
		newnode->IsDone = 0;
		int i=1;
		while(std::getline(ss1, token, ':')) {
			//cout << i << "---" << token  << "\n";
			temporary[i]=token;
			i++;
		}
		newnode->Name = temporary[1];
		if(temporary[2]!="continuous")
		{
			newnode->IsContinous = 0;
			istringstream ss2(temporary[2]);
			newnode->NoOfdistinctValues = 1;
			while(std::getline(ss2, token, ',')) {
				newnode->DistinctValues[newnode->NoOfdistinctValues] = token;
				//cout << newnode->DistinctValues[newnode->NoOfdistinctValues] << "\n";
				(newnode->NoOfdistinctValues)++;
			}
		}
		else
		{
			newnode->NoOfdistinctValues = 1;
			newnode->IsContinous = 1;
		}
		index++;

	}
}

unsigned long long int findCount(int index,unsigned long long int value,string compare)
{
	//cout << index << "--" << value << "--" << compare << "\n";
	unsigned long long int count = 0;
	for(int i=1;i<noOfInstances;i++)
		if(atoi(tData[i][index].c_str()) == value && tData[i][noOfAttributes]==compare)
			count++;
	return count;
}


void printArray(unsigned long long int (&array)[3][35000],int length)
{
	for(int i=1;i<length;i++)
	{
		cout << array[1][i] << "--" << array[2][i] << "\n";
	}
}


void makeItDiscreteValues(struct AtttributeMetadata* temp)
{


		sa.clear();

		if(temp->Index == 3)
		{
			temp->NoOfdistinctValues = 4;
			temp->Range[1][1] = -1;
			temp->Range[1][2] = 133366.5;

			temp->Range[2][1] = 133366.5;
			temp->Range[2][2] = 201111.5;

			temp->Range[3][1] = 201111.5;
			temp->Range[3][2] = -100;
			return;
		}


	    for(unsigned long long int i=1;i<noOfInstances;i++)
		{
			//cout << "copying to set " << i << "\n";
			sa.insert(atoi(tData[i][temp->Index].c_str()));
			//temparray[2][i] = tData[i][noOfAttributes];
		}
	    //cout << sa.size() << std::endl;
	    set <unsigned long long int, greater <unsigned long long int> > :: iterator itr;
	    int noOfDistinctValues = 1;
	    for (itr = sa.begin(); itr != sa.end(); ++itr)
	        {
	    		temparray[1][noOfDistinctValues] = *itr;
	            noOfDistinctValues++;
	        }

	    /*
	    if(temp->Index == 3)
		{
	    	temp->NoOfdistinctValues = 1;
			temp->Range[1][1] = 0;
			temp->Range[1][2] = temparray[1][200]+0.5;
			double prev = temparray[1][200]-0.5;
			int i=2;
			for(i=2;i<100;i++)
			{
				temp->Range[i][1] = prev;
				temp->Range[i][2] = temparray[1][200*i]+0.5;
				prev = temparray[1][200*i]+0.5;

			}

			temp->Range[temp->NoOfdistinctValues][1] = prev;
			temp->Range[temp->NoOfdistinctValues][2] = -1;
			temp->NoOfdistinctValues++;
			return;
		}
	    */

	    for(int i=1;i<noOfDistinctValues;i++)
	    {
	    	//unsigned long long int value = temparray[1][i];
	    	unsigned long long int lessThanCount = findCount(temp->Index,temparray[1][i],"<=50K");
			unsigned long long int greaterThanCount = findCount(temp->Index,temparray[1][i],">50K");
			if(lessThanCount <= greaterThanCount)
				temparray[2][i] = 1; // 1 indicates >50K
			else
				temparray[2][i] = 0; // 0 indicates <=50K
			/*cout << "less than count is " << lessThanCount << "\n";
			cout << "greater than count is " << greaterThanCount << "\n";
			cout << "-----------------------\n";*/
	    }
	    //printArray(temparray,noOfDistinctValues);

	    int count = 0;
	    for(int i=1;i<noOfDistinctValues-1;i++)
	    {
	    	if(temparray[2][i] == temparray[2][i+1])
	    		count++;
	    	else
	    		break;
	    }
	    //cout << "count is "<< count <<"\n";
	    //cout << "No of distinct values " << noOfDistinctValues << "\n";
	    temp->NoOfdistinctValues = 1;
	    if(count+2 == noOfDistinctValues)
	    {
	    	//cout << "inside 1\n";
			//cout << "average is " << temparray[1][noOfDistinctValues]+1 << "\n";
	    	temp->Range[temp->NoOfdistinctValues][1] = -1;
			temp->Range[temp->NoOfdistinctValues][2] = temparray[1][noOfDistinctValues-1]+1;
			temp->NoOfdistinctValues ++;
			return;
	    }
	    double previousRange = -1;
	    int prev = temparray[2][1];
	    //cout << "inside 2\n";
	    for(int i=2;i<noOfDistinctValues;i++)
	    {
	    	//cout << "previous one is " << prev << " and current is " << temparray[2][i] << "\n";
	    	if(prev != temparray[2][i])
	    		{
	    			double average = ((double)temparray[1][i-1] + (double)temparray[1][i])/2;
	    			//cout << "average is " << average << "\n";
	    			prev = temparray[2][i];
	    			temp->Range[temp->NoOfdistinctValues][1] = previousRange;
	    			temp->Range[temp->NoOfdistinctValues][2] = average;
	    			previousRange = average;
	    			//temp->DistinctValues[noOfDistinctValues] = average;
	    			temp->NoOfdistinctValues ++;

	    		}
	    }
	    temp->Range[temp->NoOfdistinctValues][1] = previousRange;
	    temp->Range[temp->NoOfdistinctValues][2] = -100; // To indicate last filter
	    temp->NoOfdistinctValues ++;





}

void printContinousValues(struct AtttributeMetadata *temp)
{
	cout << "+++++++++++++++++++++++++++++++++\n";
	cout << "name is " << temp->Name << "\n";
	for(int i=1;i<temp->NoOfdistinctValues;i++)
	{
		cout << "Range is :" << temp->Range[i][1] << "\t" << temp->Range[i][2] << "\n";
	}
}


void handleContinousValues()
{
	struct AtttributeMetadata* temp = headOfMetadata;
	while(temp)
	{
		if(temp->IsContinous == 1)
		{
			makeItDiscreteValues(temp);
			//printContinousValues(temp);
			//cout << "\n+++++++++++++ Done +++++++++++++++++\n";
		}
		temp = temp->PtrToNextAttr;
	}
}



void ReadVDataFrmFile()
{

	std::ifstream infile("validation.txt");
		std::string line;
		while (std::getline(infile, line))
		{
			std::istringstream ss(line);
			std::string token;
			int i=1;
			while(std::getline(ss, token, ',')) {
			    //std::cout << token << '\n';
			    vData[countOfVData][i] = token;
			    i++;
			}
			countOfVData++;
		}
}


void ReadTDataFrmFile()
{

	std::ifstream infile("training.txt");
		std::string line;
		while (std::getline(infile, line))
		{
			std::istringstream ss(line);
			std::string token;
			int i=1;
			while(std::getline(ss, token, ',')) {
			    //std::cout << token << '\n';
			    tData[noOfInstances][i] = token;
			    i++;
			}
			noOfInstances++;
		}
}

void getFinalResult(struct DTreeNode *temp,string **dataArray,int i)
{
	if(temp->isLeaf == 1)
	{
		result = temp->PtrToNextValue[1]->Value;
	}

	string value = dataArray[i][temp->index];
	if(temp->isContinous == 1)
	{

		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			if(temp->PtrToNextValue[i]->range[1][2] == -100)
			{
				if(atoi(value.c_str()) > temp->PtrToNextValue[i]->range[1][1])
				{
					getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
				}
			}
			else
			{
				if(atoi(value.c_str()) > temp->PtrToNextValue[i]->range[1][1] &&
						atoi(value.c_str()) < temp->PtrToNextValue[i]->range[1][2])
				{
					getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
				}
			}
		}
	}
	else
	{
		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			if(temp->PtrToNextValue[i]->Value == value)
			{
				getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
			}
		}
	}
}


void checkForAccuracy(struct DTreeNode *temp,string **data,int length,string dataName)
{
	int actualDataCount = 0,wrongData = 0;
	for(int i=1;i<length;i++)
	{
		result = '0';
		getFinalResult(temp,data,i);
		//cout << result << endl;
		if(result == "AcceptAny")
		{
			actualDataCount++;
		}
		else if(result == data[i][noOfAttributes])
		{
			actualDataCount++;
		}
		else if(result!=data[i][noOfAttributes])
		{
			wrongData++;
		}
	}

	float accuracyPercentage = ((float)actualDataCount/(float)(length-1))*100;
	//cout << "actual data count is " << actualDataCount << " and wrong data count is "<< wrongData << endl;
	cout << "Accuracy Percentage for "<< dataName << " is " << accuracyPercentage << endl;
}


string pickRandomValueFromAttribute(int index,string **data,int length)
{
	struct AtttributeMetadata *temp = headOfMetadata;
	while(temp)
	{
		if(temp->Index == index)
		{
			if(temp->IsContinous == 1)
			{
				int randNumber = rand() % (length-1) + 1;
				//cout << randNumber << endl;
				return data[randNumber][index];
			}

			else
			{
				int randNumber = rand() % (temp->NoOfdistinctValues-1) + 1;
				//cout << randNumber << endl;
				return temp->DistinctValues[randNumber];
			}

		}
		temp = temp->PtrToNextAttr;
	}
	return "hello";

}



void handleMissingValues(string **data,int length)
{
	for(int i=1;i<length;i++)
	{
		for(int j=1;j<=noOfAttributes;j++)
		{
			if(data[i][j] == "?")
			{
				//cout << "? found " << endl;
				data[i][j] = pickRandomValueFromAttribute(j,data,length);
				//cout << "replaced with " << data[i][j]<< endl;
			}
		}
	}
}

struct DTreeNode *MakeCopyOfTree(struct DTreeNode *temp)
{
	if(temp->isLeaf == 1)
	{
		struct DTreeNode *newDtreeNode = new DTreeNode();
		newDtreeNode->Name = temp->Name;
		newDtreeNode->NoOfDistinctValues = temp->NoOfDistinctValues;
		newDtreeNode->index = temp->index;
		newDtreeNode->isContinous = temp->isContinous;
		newDtreeNode->isLeaf = temp->isLeaf;
		newDtreeNode->isPruned = temp->isPruned;
		struct DTreeValue *newValueNode = new DTreeValue();
		newValueNode->ptrToNextNode = NULL;
		newValueNode->Index = temp->PtrToNextValue[1]->Index;
		newValueNode->Value = temp->PtrToNextValue[1]->Value;
		newDtreeNode->PtrToNextValue[1] = newValueNode;
		return newDtreeNode;
	}
	else
	{
		struct DTreeNode *newDtreeNode = new DTreeNode();
		newDtreeNode->Name = temp->Name;
		newDtreeNode->NoOfDistinctValues = temp->NoOfDistinctValues;
		newDtreeNode->index = temp->index;
		newDtreeNode->isContinous = temp->isContinous;
		newDtreeNode->isLeaf = temp->isLeaf;
		newDtreeNode->isPruned = temp->isPruned;
		//cout << "name is  " << temp->Name << endl;
		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			struct DTreeValue *newValueNode = new DTreeValue();
			newValueNode->Index = temp->PtrToNextValue[i]->Index;
			newValueNode->Value = temp->PtrToNextValue[i]->Value;
			newValueNode->range[1][1] = temp->PtrToNextValue[i]->range[1][1];
			newValueNode->range[1][2] = temp->PtrToNextValue[i]->range[1][2];
			newDtreeNode->PtrToNextValue[i] = newValueNode;
			newDtreeNode->PtrToNextValue[i]->ptrToNextNode = MakeCopyOfTree(temp->PtrToNextValue[i]->ptrToNextNode);

		}
		return newDtreeNode;
	}
}


void AllocateMemoryForDatas()
{
	tData = new string*[35000];
	vData = new string*[35000];
	trimmedData = new string*[100001];
	for(int i = 0; i < 35000; ++i)
	{
		 tData[i] = new string[20];
		 vData[i] = new string[20];
		 trimmedData[i] = new string[20];
	}
}


void SelectedNodeToBePruned(struct DTreeNode *temp,struct DTreeNode *root)
{

	if(temp == NULL)
			return;
	//cout << temp->Name << "and" << temp->isLeaf << endl;
	if(temp->isLeaf != 1 && temp->isPruned != 1)
	{
		if(nodeToBePruned == NULL)
		{
			temp->isPruned = 1;
			cout << "Attribute selected for pruning is  " << temp->Name << endl;
			nodeToBePruned = temp;
		}

		//cout << "leaf node and value is  " << temp->PtrToNextValue[1]->Value << endl;
		return;
	}
	else
	{
			//cout << "name is  " << temp->Name  << "and pruning is " << temp->isPruned << endl;
			for(int i=1;i<temp->NoOfDistinctValues;i++)
			{
				//cout << temp->PtrToNextValue[i]->range[1][1] << " & " << temp->PtrToNextValue[i]->range[1][1] << endl;
				//cout << "child " << i << " is " << temp->PtrToNextValue[i] << endl;
				SelectedNodeToBePruned(temp->PtrToNextValue[i]->ptrToNextNode,root);

			}

	}


}


int getCountOfFinalFromTree(struct DTreeNode* root,string value)
{
	if(root == NULL)
		return 0;
	if(root->isLeaf == 1)
	{
		if(root->PtrToNextValue[1]->Value == value)
		{
			//cout << "came" << endl;
			countOfValues++;
		}
	}
	for(int i=1;i<root->NoOfDistinctValues;i++)
	{
		//cout << root->Name << endl;
		getCountOfFinalFromTree(root->PtrToNextValue[i]->ptrToNextNode,value);
	}
	return 0;
}

void BuildNewTreeWithPruning(struct DTreeNode *nodeToBePruned)
{
	countOfValues = 0;
	getCountOfFinalFromTree(nodeToBePruned,"<=50K");
	int lessThanCount = countOfValues;
	countOfValues = 0;
	getCountOfFinalFromTree(nodeToBePruned,">50K");
	int greaterThanCount = countOfValues;
	//cout << "less than count is " << lessThanCount <<" and greater than count "<< greaterThanCount << endl;
	nodeToBePruned->Name = "LeafNode";
	nodeToBePruned->isLeaf = 1;
	nodeToBePruned->NoOfDistinctValues = 2;
	struct DTreeValue *newValueNode = new DTreeValue();
	newValueNode->ptrToNextNode = NULL;
	nodeToBePruned->PtrToNextValue[1] = newValueNode;
	if(lessThanCount == greaterThanCount)
	{
		newValueNode->Value = "AcceptAny";
	}
	else if(lessThanCount > greaterThanCount)
	{
		newValueNode->Value = "<=50K";
	}
	else if(lessThanCount < greaterThanCount)
	{
		newValueNode->Value = ">50K";
	}
}

void updateAttributeParametersToOriginalAsNotDone()
{
	struct AtttributeMetadata *check = headOfMetadata;
	while(check)
	{
		check->IsDone = 0;
		check = check->PtrToNextAttr;
	}
}

void updateMainTreeAfterPruning(struct DTreeNode *root,int index)
{
	if(root == NULL)
		return;
	if(root->index == index)
	{
		//cout << "came" << endl;;
		root->isPruned = 1;
	}
	for(int i=1;i<root->NoOfDistinctValues;i++)
	{
		updateMainTreeAfterPruning(root->PtrToNextValue[i]->ptrToNextNode,index);
	}
}

bool areAllAttributesDoneForPruning(struct DTreeNode *temp)
{
	if(temp == NULL)
	{
		return true;
	}
	if(temp->isPruned == 0)
		return false;
	for(int i=1;i<temp->NoOfDistinctValues;i++)
	{
		areAllAttributesDoneForPruning(temp->PtrToNextValue[i]->ptrToNextNode);
	}
	return true;
}

void DoReducedErrorPruning()
{
	int i=1;
	while(1)
	{
		cout << "-----------------------------------------------" << endl;
		nodeToBePruned = NULL;
		//cout << "+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		struct DTreeNode *copyOfTree = MakeCopyOfTree(headOfDTree);
		SelectedNodeToBePruned(copyOfTree,copyOfTree);
		struct DTreeNode *nodeSelectedForPruning = nodeToBePruned;
		//cout << nodeToBePruned << endl;
		//cout << "index is " << nodeSelectedForPruning->index << endl;
		updateMainTreeAfterPruning(headOfDTree,nodeToBePruned->index);
		cout << "Building new tree After pruning selected Attribute.." << endl;
		BuildNewTreeWithPruning(nodeToBePruned);
		//traverseTree(copyOfTree);
		cout << "Done!!" << endl;
		//cout << "----" << endl;
		cout << "Calculating Accuracy.." << endl;
		checkForAccuracy(copyOfTree,tData,noOfInstances,"Training Data");
		checkForAccuracy(copyOfTree,vData,countOfVData,"Validation Data");
		//if(areAllAttributesDoneForPruning(headOfDTree))*/
		copyOfTree = NULL;
		if(i==12)
		//if(areAllAttributesDoneForPruning(headOfDTree))
			break;
		i++;
	}
}

void RandomForest()
{
	//srand(time(NULL));
	for(int noOfTrees = 1;noOfTrees<=10;noOfTrees++)
	{
		trimmedDataCount = 1;
		unsigned long long int randomNumber = 0;

		for(int i=1;i<=15000;i++)
		{
			randomNumber = rand() % (32000-1) + 1;
			//cout << randomNumber << endl;
			for(int j=1;j<=noOfAttributes;j++)
				trimmedData[trimmedDataCount][j] = tData[randomNumber][j];
			trimmedDataCount++;
		}
		//printTData(trimmedData,trimmedDataCount);
		cout << "For Random Tree - " << noOfTrees << " : "<< endl;
		randomTree[noOfTrees] = BuildDTree(trimmedData,trimmedDataCount,"Root",1);
		checkForAccuracy(randomTree[noOfTrees],vData,countOfVData,"Validation Data");
		updateAttributeParametersToOriginalAsNotDone();
		//traverseTree(randomTree[noOfTrees]);
		//cout << "+++++++++++++++++" << endl;
	}
}


int main()
{
	cout << "\n++++++++++++++++ Start Of Phase - 1 ++++++++++++++++++++++" << endl;
	AllocateMemoryForDatas();
	cout << "Reading Metadata From file.." << endl;
	ReadMetadata();
	cout << "Metadata File successfully read!!" << endl;
	cout << "Reading Training data From file.." << endl;
	ReadTDataFrmFile();
	cout << "Training data successfully read!!" << endl;
	//printTData(tData,noOfInstances);
	cout << "Handling Continous Values for Training Data" << endl;
	handleContinousValues();
	cout << "Done!!" << endl;
	cout << "Handling Missing Values for Training Data" << endl;
	handleMissingValues(tData,noOfInstances);
	cout << "Done" << endl;
	//printTData(tData,noOfInstances);
	cout << "Building Decision Tree for Training Data.." << endl;
	headOfDTree = BuildDTree(tData,noOfInstances,"Root",1);
	//traverseTree(headOfDTree);
	cout << "Decision Tree Successfully built" << endl;
	//traverseTree(headOfDTree);
	cout << "Reading Validation data From file.." << endl;
	ReadVDataFrmFile();
	cout << "Validation data successfully read!!" << endl;
	//printTData(tData,noOfInstances);
	cout << "Handling Missing Values for Validation Data" << endl;
	handleMissingValues(vData,countOfVData);
	cout << "Done" << endl;
	//printTData(vData,countOfVData);
	cout << "Checking Accuracy of Validation Data on Built Decision Tree.." << endl;
	checkForAccuracy(headOfDTree,vData,countOfVData,"Validation Data");
	cout << "Checking Accuracy of Training Data on Built Decision Tree.." << endl;
	checkForAccuracy(headOfDTree,tData,noOfInstances,"Training Data");
	headOfDTree->isPruned = 1; // Marking root as already pruned to avoid actual pruning
	cout << "++++++++++++++++ End Of Phase - 1 ++++++++++++++++++++++" << endl;
	cout << "++++++++++++++++ Start Of Phase - 2 ++++++++++++++++++++++" << endl;
	cout << "Using Method : Reduced Error Pruning" << endl;
	DoReducedErrorPruning();
	cout << "++++++++++++++++ End Of Phase - 2 ++++++++++++++++++++++" << endl;
	cout << "++++++++++++++++ Start Of Phase - 3 ++++++++++++++++++++++" << endl;
	cout << "Building Random Forests.."<< endl;
	RandomForest();
	cout << "++++++++++++++++ End Of Phase - 3 ++++++++++++++++++++++" << endl;
	return 1;

}
