#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>
#define HIGHVALUE -100;

using namespace std;

int global = 0;

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
	string searchIndexes[100];
	int lengthOfSearchIndexes;
	struct DTreeNode *ptrToNextNode;
};

struct DTreeNode
{
	string Name;
	int NoOfDistinctValues;
	int isContinous;
	int isLeaf;
	int index;
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
		cout << "name is  " << temp->Name << endl;
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
			temp->Range[3][2] = -1;
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
	    			prev = temparray[2][i]; //TODO
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

string getFinalResult(struct DTreeNode *temp,string **dataArray,int i)
{
	if(temp->isLeaf == 1)
	{
		return temp->PtrToNextValue[1]->Value;
	}

	string value = dataArray[i][temp->index];
	if(temp->isContinous == 1)
	{

		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			if(temp->PtrToNextValue[i]->range[1][2] == -1)
			{
				if(atoi(value.c_str()) > temp->PtrToNextValue[i]->range[1][1])
				{
					return getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
				}
			}
			else
			{
				if(atoi(value.c_str()) > temp->PtrToNextValue[i]->range[1][1] &&
						atoi(value.c_str()) < temp->PtrToNextValue[i]->range[1][2])
				{
					return getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
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
				return getFinalResult(temp->PtrToNextValue[i]->ptrToNextNode,dataArray,i);
			}
		}
	}
	return "hello";
}


void checkForAccuracy(struct DTreeNode *temp,string **data,int length)
{
	int actualDataCount = 0,wrongData = 0;
	for(int i=1;i<length;i++)
	{
		string result = getFinalResult(temp,data,i);
		if(result == "AcceptAny")
		{
			actualDataCount++;
		}
		if(result == data[i][noOfAttributes])
		{
			actualDataCount++;
		}
		else if(result!=data[i][noOfAttributes])
		{
			wrongData++;
		}
	}

	float accuracyPercentage = ((float)actualDataCount/(float)(length-1))*100;
	cout << "actual data count is " << actualDataCount << " and wrong data count is "<< wrongData << endl;
	cout << "accuracy Percentage is " << accuracyPercentage << endl;
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





int main()
{
	tData = new string*[35000];
	vData = new string*[35000];
	for(int i = 0; i < 35000; ++i)
	{
		 tData[i] = new string[20];
		 vData[i] = new string[20];
	}


	ReadMetadata();
	ReadTDataFrmFile();
	//printTData(tData,noOfInstances);
	handleContinousValues();
	handleMissingValues(tData,noOfInstances);
	//printTData(tData,noOfInstances);
	headOfDTree = BuildDTree(tData,noOfInstances,"Root",1);
	//traverseTree(headOfDTree);
	ReadVDataFrmFile();
	cout << "Vdata count is " << countOfVData << endl;
	handleMissingValues(vData,countOfVData);
	//printTData(vData,countOfVData);
	checkForAccuracy(headOfDTree,tData,noOfInstances);
	checkForAccuracy(headOfDTree,vData,countOfVData);
	return 1;

}
