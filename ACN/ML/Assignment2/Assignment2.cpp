/*
 * Name : Machine Learning Assignment 2 (Decision Tree Concept).
 * Authors : 1.Saradhi Ramakrishna	(2017H1030081H)
 * 			 2.Anmol Dayal Dhiman	(2017H1030087H)
 * 			 3.Bandi Bharathi		(2017H1030067H)
 * Branch : M.E Computer Science
 * Below code is practical implementation of following features in 3 Phases.
 * 1. Building Of Decision Tree.
 * 2. Reduced Error Pruning.
 * 3. Building Random Forest.
 * Input : Training Data Set and Validation Set.
 * Output : Error Percentage for the validation data set based on
 * the D-Tree built using Training Data set.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>
#define HIGHVALUE -100;

using namespace std;

/*
 * Structure to Store Attribute Metadata and its information.
 */
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

/*
 * Structure to Store Attribute Values as leaf.
 */
struct DTreeValue
{
	int Index;
	string Value;
	double range[2][3];
	struct DTreeNode *ptrToNextNode;
};


/*
 * Structure to Store Attribute Name as Node.
 */
struct DTreeNode
{
	string Name;
	int NoOfDistinctValues;
	int isContinous;
	int isLeaf;
	int index;
	int isPruned;
	struct DTreeValue *PtrToNextValue[50];
};


string **tData = NULL; // [No_of_instances][No_of_atributes]
string **vData = NULL; // [No_of_instances][No_of_atributes]
string **trimmedData = NULL;
unsigned long long int temparray[3][22000];
unsigned long long int noOfInstances = 1;
unsigned long long int countOfVData = 1;
int noOfAttributes = 15;
struct AtttributeMetadata *headOfMetadata = NULL;
set<unsigned long long int> sa;
struct DTreeNode * headOfDTree = NULL;
int countOfValues = 0;
int trimmedDataCount = 1;
struct DTreeNode *randomTree[10] = {NULL};
string result;
struct DTreeNode *nodeToBePruned = NULL;

void printTData(string **array,int length);
int getLessThan(struct AtttributeMetadata *temp,string value,string **array,int length);
int getGreaterThan(struct AtttributeMetadata *temp,string value,string **array,int length);
int getLessThanRange(struct AtttributeMetadata *temp,double lessValue,double highValue,string **array,int length);
int getGreaterThanRange(struct AtttributeMetadata *temp,double lessValue,double highValue,string **array,int length);
double calculateEntropyWithRespectToAttr(double a,double b,double total);
double calculateInformationGain(struct AtttributeMetadata *attr,string **array,int count);
int checkForAlgoCompletion();
void traverseTree(struct DTreeNode *temp);
struct DTreeNode *BuildLeafNode(string **oldArray,int oldCount);
struct DTreeNode *BuildDTree(string **oldArray,int oldCount,string name,int child);
struct AtttributeMetadata* insertMetadata();
void ReadMetadata();
unsigned long long int findCount(int index,unsigned long long int value,string compare);
void printArray(unsigned long long int (&array)[3][35000],int length);
void makeItDiscreteValues(struct AtttributeMetadata* temp);
void printContinousValues(struct AtttributeMetadata *temp);
void handleContinousValues();
void ReadVDataFrmFile();
void ReadTDataFrmFile();
void getFinalResult(struct DTreeNode *temp,string **dataArray,int i);
void checkForAccuracy(struct DTreeNode *temp,string **data,int length,string dataName);
string pickRandomValueFromAttribute(int index,string **data,int length);
void handleMissingValues(string **data,int length);
struct DTreeNode *MakeCopyOfTree(struct DTreeNode *temp);
void AllocateMemoryForDatas();
void SelectedNodeToBePruned(struct DTreeNode *temp);
int getCountOfFinalFromTree(struct DTreeNode* root,string value);
void BuildNewTreeWithPruning(struct DTreeNode *nodeToBePruned);
void updateAttributeParametersToOriginalAsNotDone();
void updateMainTreeAfterPruning(struct DTreeNode *root,int index);
bool areAllAttributesDoneForPruning(struct DTreeNode *temp);
void FreeTreeMemory(struct DTreeNode *temp);
void DoReducedErrorPruning();
void RandomForest();
void printNoOfChildsForAttributes();
void printRange();

/*
 * Processing Starts From here.
 */
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
	cout << "Handling Continous Values for Training Data" << endl;
	handleContinousValues();
	cout << "Done!!" << endl;
	cout << "Handling Missing Values for Training Data" << endl;
	handleMissingValues(tData,noOfInstances);
	cout << "Done" << endl;
	cout << "Building Decision Tree for Training Data.." << endl;
	headOfDTree = BuildDTree(tData,noOfInstances,"Root",1);
	cout << "Decision Tree Successfully built" << endl;
	cout << "Reading Validation data From file.." << endl;
	ReadVDataFrmFile();
	cout << "Validation data successfully read!!" << endl;
	cout << "Handling Missing Values for Validation Data" << endl;
	handleMissingValues(vData,countOfVData);
	cout << "Done" << endl;
	cout << "Checking Accuracy of Validation Data on Built Decision Tree.." << endl;
	checkForAccuracy(headOfDTree,vData,countOfVData,"Validation Data");
	cout << "Checking Accuracy of Training Data on Built Decision Tree.." << endl;
	checkForAccuracy(headOfDTree,tData,noOfInstances,"Training Data");
	headOfDTree->isPruned = 1; // Marking root as already pruned to avoid actual pruning
	cout << "++++++++++++++++ End Of Phase - 1 ++++++++++++++++++++++" << endl;
	cout << "++++++++++++++++ Start Of Phase - 2 ++++++++++++++++++++++" << endl;
	cout << "Using Method : Reduced Error Pruning" << endl;
	DoReducedErrorPruning();
	headOfDTree = NULL;
	cout << "++++++++++++++++ End Of Phase - 2 ++++++++++++++++++++++" << endl;
	cout << "++++++++++++++++ Start Of Phase - 3 ++++++++++++++++++++++" << endl;
	cout << "Building Random Forests.."<< endl;
	RandomForest();
	cout << "++++++++++++++++ End Of Phase - 3 ++++++++++++++++++++++" << endl;
	return 1;
}

/*
 * Function to print Data which can be training or validation.
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

/*
 * Function to get count of rows which are discrete and which are having "<=50K".
 */
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


/*
 * Function to get count of rows which are discrete and which are having ">50K".
 */
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


/*
 * Function to get count of rows which are Continous and which are having "<=50K".
 */
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


/*
 * Function to get count of rows which are continous and which are having ">50K".
 */
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

/*
 * Function to calculate entropy.
 */
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
		return entropy;
}

/*
 * Function to calculate Information gain based on entropy returned by above function.
 */
double calculateInformationGain(struct AtttributeMetadata *attr,string **array,int count)
{

	float globala = getLessThan(NULL,"",array,count);
	float globalb = getGreaterThan(NULL,"",array,count);
	float globalTotal = globala + globalb;
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
			globalEntropy -= (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
		}
		return globalEntropy;
	}
	else
	{
		for(int i=1;i<attr->NoOfdistinctValues;i++)
		{
			locala = getLessThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localb = getGreaterThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localTotal = locala + localb;
			globalEntropy -= (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
			return globalEntropy;
		}
	}
	return 0;
}

/*
 * Function to check whether algorithm has ended or not.
 */
int checkForAlgoCompletion()
{
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

/*
 * Traverse Tree in Depth First Search Fashion.
 */
void traverseTree(struct DTreeNode *temp)
{
	if(temp == NULL)
		return;

	if(temp->isLeaf == 1)
	{
		return;
	}
	else
	{
		//cout << "name is "<< temp->Name <<" and pruned is " << temp->isPruned << endl;
		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			traverseTree(temp->PtrToNextValue[i]->ptrToNextNode);
		}
	}
}

/*
 * Function to Build leaf nodes after entire tree is built based on <=50K or <50K.
 */
struct DTreeNode *BuildLeafNode(string **oldArray,int oldCount)
{
	int noOfLessThan50K = getLessThan(NULL,"",oldArray,oldCount);
	int noOfGreaterThan50K = getGreaterThan(NULL,"",oldArray,oldCount);
	//cout << "less than count is " << noOfLessThan50K << endl;
	//cout << "greate than count is " << noOfGreaterThan50K << endl;
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


/*
 * Function to build Decision Tree based on Information Gain and Entropy.
 */
struct DTreeNode* BuildDTree(string **oldArray,int oldCount,string name,int child)
{
		struct AtttributeMetadata *temp = headOfMetadata;
		struct AtttributeMetadata *finalOne = NULL;
		float prev = 0;
		double newGain = -1;
		double max = -1;
		int index;
		while(temp)
		{
			if(temp->IsDone != 1)
			{
				newGain = calculateInformationGain(temp,oldArray,oldCount);
				//cout << "new gain is " << newGain << " and name is " << temp->Name << endl;
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
			struct DTreeNode *leafNode = BuildLeafNode(oldArray,oldCount);
			return leafNode;
		}
		finalOne->IsDone = 1;
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
		for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
		{
			string** newArray = new string*[oldCount];
			for(int rk = 0; rk <= oldCount; ++rk)
				newArray[rk] = new string[20];
			int newCount = 1;
			if(newDtreeNode->isContinous == 1)
			{
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
			}
			else
			{
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
			}
			newDtreeNode->PtrToNextValue[i]->ptrToNextNode = BuildDTree(newArray,newCount,newDtreeNode->Name,i);
		}
	return newDtreeNode;
}


void printNoOfChildsForAttributes()
{
	struct AtttributeMetadata *temp = headOfMetadata;
	while(temp)
	{
		cout << "name is " << temp->Name << "and distinct values are " << temp->NoOfdistinctValues << endl;
		temp = temp->PtrToNextAttr;
	}
}

void printRange()
{
	struct AtttributeMetadata *temp = headOfMetadata;
	while(temp)
	{
		if(temp->Index == 3)
		{
			for(int i=1;i<temp->NoOfdistinctValues;i++)
				cout << temp->Range[i][1] << "\t" << temp->Range[i][2] << endl;
		}
		temp = temp->PtrToNextAttr;
	}
}

/*
 * Function to Insert Metadata into Linked List Structure.
 */
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

/*
 * Function to Read Metadata from file and create a structure for it.
 */
void ReadMetadata()
{
	ifstream infile("AttributeMetaData.txt");
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
		while(std::getline(ss1, token, ':'))
		{
			temporary[i]=token;
			i++;
		}
		newnode->Name = temporary[1];
		if(temporary[2]!="continuous")
		{
			newnode->IsContinous = 0;
			istringstream ss2(temporary[2]);
			newnode->NoOfdistinctValues = 1;
			while(std::getline(ss2, token, ','))
			{
				newnode->DistinctValues[newnode->NoOfdistinctValues] = token;
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



/*
 * Function to get count of rows which are having same value as value(compare) passed to function.
 */
unsigned long long int findCount(int index,unsigned long long int value,string compare)
{
	unsigned long long int count = 0;
	for(int i=1;i<noOfInstances;i++)
		if(atoi(tData[i][index].c_str()) == value && tData[i][noOfAttributes]==compare)
			count++;
	return count;
}

/*
 * Function to Print Array passed to it.
 */
void printArray(unsigned long long int (&array)[3][35000],int length)
{
	for(int i=1;i<length;i++)
	{
		cout << array[1][i] << "--" << array[2][i] << "\n";
	}
}

/*
 * Function to calculate discrete values for continous range.
 * Comes as a handy function in handling continous values.
 */
void makeItDiscreteValues(struct AtttributeMetadata* temp)
{
	sa.clear();

	for(unsigned long long int i=1;i<noOfInstances;i++)
	{
		sa.insert(atoi(tData[i][temp->Index].c_str()));
	}
	set <unsigned long long int, greater <unsigned long long int> > :: iterator itr;
	int noOfDistinctValues = 1;
	for (itr = sa.begin(); itr != sa.end(); ++itr)
	{
		temparray[1][noOfDistinctValues] = *itr;
		noOfDistinctValues++;
	}
	//cout << "No of distinct " << noOfDistinctValues << endl;
	if(temp->Index == 3)
	{
		long long int prev = -1;
		int ml = 1;
		for(ml = 1;ml<30;ml++)
		{
			temp->Range[ml][1] = prev;
			temp->Range[ml][2] = temparray[1][600*ml]+0.5;
			prev = temp->Range[ml][2];

		}

		temp->Range[ml][1] = prev;
		temp->Range[ml][2] = -100;
		ml++;
		temp->NoOfdistinctValues = ml;
		//cout << "ML is " << ml << endl;
		return;
	}
	for(int i=1;i<noOfDistinctValues;i++)
	{
		unsigned long long int lessThanCount = findCount(temp->Index,temparray[1][i],"<=50K");
		unsigned long long int greaterThanCount = findCount(temp->Index,temparray[1][i],">50K");
		if(lessThanCount <= greaterThanCount)
			temparray[2][i] = 1; // 1 indicates >50K
		else
			temparray[2][i] = 0; // 0 indicates <=50K
	}
	int count = 0;
	for(int i=1;i<noOfDistinctValues-1;i++)
	{
		if(temparray[2][i] == temparray[2][i+1])
			count++;
		else
			break;
	}
	temp->NoOfdistinctValues = 1;
	if(count+2 == noOfDistinctValues)
	{
		temp->Range[temp->NoOfdistinctValues][1] = -1;
		temp->Range[temp->NoOfdistinctValues][2] = temparray[1][noOfDistinctValues-1]+1;
		temp->NoOfdistinctValues ++;
		return;
	}
	double previousRange = -1;
	int prev = temparray[2][1];
	for(int i=2;i<noOfDistinctValues;i++)
	{
		if(prev != temparray[2][i])
			{
				double average = ((double)temparray[1][i-1] + (double)temparray[1][i])/2;
				prev = temparray[2][i];
				temp->Range[temp->NoOfdistinctValues][1] = previousRange;
				temp->Range[temp->NoOfdistinctValues][2] = average;
				previousRange = average;
				temp->NoOfdistinctValues ++;
			}
	}
	temp->Range[temp->NoOfdistinctValues][1] = previousRange;
	temp->Range[temp->NoOfdistinctValues][2] = -100; // To indicate last filter
	temp->NoOfdistinctValues ++;
	sa.clear();
}

/*
 * Function to Print continous values after the have been converted.
 */
void printContinousValues(struct AtttributeMetadata *temp)
{
	cout << "+++++++++++++++++++++++++++++++++\n";
	cout << "name is " << temp->Name << "\n";
	for(int i=1;i<temp->NoOfdistinctValues;i++)
	{
		cout << "Range is :" << temp->Range[i][1] << "\t" << temp->Range[i][2] << "\n";
	}
}

/*
 * Function to handle continous values.
 */
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


/*
 * Function to read Validation Data from File.
 */
void ReadVDataFrmFile()
{
	std::ifstream infile("ValidationData.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream ss(line);
		std::string token;
		int i=1;
		while(std::getline(ss, token, ','))
		{
			vData[countOfVData][i] = token;
			i++;
		}
		countOfVData++;
	}
}

/*
 * Function to read Training Data from File.
 */
void ReadTDataFrmFile()
{
	std::ifstream infile("TrainingData.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream ss(line);
		std::string token;
		int i=1;
		while(std::getline(ss, token, ','))
		{
			//std::cout << token << '\n';
			tData[noOfInstances][i] = token;
			i++;
		}
		noOfInstances++;
	}
}


/*
 * Function to get the final value of a row based on the tree built.
 */
void getFinalResult(struct DTreeNode *temp,string **dataArray,int i)
{
	if(temp == NULL)
		return;
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

/*
 * Function to calculate Accuracy of the data from the tree built.
 */
void checkForAccuracy(struct DTreeNode *temp,string **data,int length,string dataName)
{
	int actualDataCount = 0,wrongData = 0;
	for(int i=1;i<length;i++)
	{
		result = '0';
		getFinalResult(temp,data,i);
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
	cout << "Accuracy Percentage for "<< dataName << " is " << accuracyPercentage << endl;
}


/*
 * Function to pick Random Value for attribute.
 * Comes handy when handling missing attributes.
 */
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


/*
 * Function to handle missing values.
 */
void handleMissingValues(string **data,int length)
{
	for(int i=1;i<length;i++)
	{
		for(int j=1;j<=noOfAttributes;j++)
		{
			if(data[i][j] == "?")
			{
				data[i][j] = pickRandomValueFromAttribute(j,data,length);
			}
		}
	}
}

/*
 * Function to make copy of Decision tree.
 * Comes handy for pruning trees.
 */
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

/*
 * Function to Allocate Dynamic Memory from heap to all data arrays.
 */
void AllocateMemoryForDatas()
{
	tData = new string*[33000];
	vData = new string*[33000];
	for(int i = 0; i < 33000; ++i)
	{
		 tData[i] = new string[20];
		 vData[i] = new string[20];
	}
	trimmedData = new string*[15002];
	for(int i=0;i<15002;i++)
		trimmedData[i] = new string[20];
}

/*
 * Function to select node to be pruned.
 */
void SelectedNodeToBePruned(struct DTreeNode *temp)
{
	if(temp == NULL)
			return;
	if(temp->isLeaf != 1 && temp->isPruned != 1)
	{
		if(nodeToBePruned == NULL)
		{
			temp->isPruned = 1;
			cout << "Attribute selected for pruning is  " << temp->Name << endl;
			nodeToBePruned = temp;
			return;
		}

	}
	else
	{
		for(int i=1;i<temp->NoOfDistinctValues;i++)
		{
			SelectedNodeToBePruned(temp->PtrToNextValue[i]->ptrToNextNode);
		}
	}
}


/*
 * Function to get count of final result for the data row from D-Tree.
 */
int getCountOfFinalFromTree(struct DTreeNode* root,string value)
{
	if(root == NULL)
		return 0;
	if(root->isLeaf == 1)
	{
		if(root->PtrToNextValue[1]->Value == value)
		{
			countOfValues++;
		}
	}
	for(int i=1;i<root->NoOfDistinctValues;i++)
	{
		getCountOfFinalFromTree(root->PtrToNextValue[i]->ptrToNextNode,value);
	}
	return 0;
}


/*
 * Function to Build a tree after pruning selected attribute.
 */
void BuildNewTreeWithPruning(struct DTreeNode *nodeToBePruned)
{
	countOfValues = 0;
	getCountOfFinalFromTree(nodeToBePruned,"<=50K");
	int lessThanCount = countOfValues;
	countOfValues = 0;
	getCountOfFinalFromTree(nodeToBePruned,">50K");
	int greaterThanCount = countOfValues;
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


/*
 * Updating attribute parameter "isDone" to 0 for building new trees.
 */
void updateAttributeParametersToOriginalAsNotDone()
{
	struct AtttributeMetadata *check = headOfMetadata;
	while(check)
	{
		check->IsDone = 0;
		check = check->PtrToNextAttr;
	}
}

/*
 * Function to update main tree parameter "isPruned" to 1 to avoid
 * selecting same attribute again.
 */
void updateMainTreeAfterPruning(struct DTreeNode *root,int index)
{
	if(root == NULL)
		return;
	if(root->index == index)
	{
		root->isPruned = 1;
	}
	for(int i=1;i<root->NoOfDistinctValues;i++)
	{
		updateMainTreeAfterPruning(root->PtrToNextValue[i]->ptrToNextNode,index);
	}
}


/*
 * Function to check whether all attributes are selected for pruning.
 */
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


/*
 * Function to free D-Tree memory.
 */
void FreeTreeMemory(struct DTreeNode *temp)
{
	if(temp == NULL)
		return;

	for(int i=1;i<temp->NoOfDistinctValues;i++)
	{
		FreeTreeMemory(temp->PtrToNextValue[i]->ptrToNextNode);
		delete temp->PtrToNextValue[i];
	}
	delete temp;
}


/*
 * Function to Perform Reduced Error Pruning.
 */
void DoReducedErrorPruning()
{
	int i=1;
	while(1)
	{
		cout << "-----------------------------------------------" << endl;
		nodeToBePruned = NULL;
		struct DTreeNode *copyOfTree = MakeCopyOfTree(headOfDTree);
		SelectedNodeToBePruned(copyOfTree);
		struct DTreeNode *nodeSelectedForPruning = nodeToBePruned;
		updateMainTreeAfterPruning(headOfDTree,nodeToBePruned->index);
		cout << "Building new tree After pruning selected Attribute.." << endl;
		BuildNewTreeWithPruning(nodeToBePruned);
		traverseTree(copyOfTree);
		cout << "Done!!" << endl;
		cout << "Calculating Accuracy.." << endl;
		checkForAccuracy(copyOfTree,tData,noOfInstances,"Training Data");
		checkForAccuracy(copyOfTree,vData,countOfVData,"Validation Data");
		cout << "Deleting Tree and freeing up memory..." << endl;
		FreeTreeMemory(copyOfTree);
		cout << "Done!!" <<endl;
		copyOfTree = NULL;
		if(i==12)
			break;
		i++;
	}
}



/*
 * Function in Building Random Forests.
 */
void RandomForest()
{
	for(int noOfTrees = 1;noOfTrees<=4;noOfTrees++)
	{
		trimmedDataCount = 1;
		unsigned long long int randomNumber = 0;
		for(int i=1;i<=15000;i++)
		{
			randomNumber = rand() % (32000-1) + 1;
			for(int j=1;j<=noOfAttributes;j++)
				trimmedData[trimmedDataCount][j] = tData[randomNumber][j];
			trimmedDataCount++;
		}
		cout << "For Random Tree - " << noOfTrees << " : "<< endl;
		randomTree[noOfTrees] = BuildDTree(trimmedData,trimmedDataCount,"Root",1);
		checkForAccuracy(randomTree[noOfTrees],vData,countOfVData,"Validation Data");
		updateAttributeParametersToOriginalAsNotDone();
		cout << "Deleting Tree and freeing up memory..." << endl;
		//FreeTreeMemory(randomTree[noOfTrees]);
		cout << "Done!!" << endl;
	}
}
