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
	string DistinctValues[21000];
	double Range[25000][3];
	int NoOfdistinctValues;
	int IsContinous;
	int IsDone;
	struct AtttributeMetadata *PtrToNextAttr;
};

struct DTreeValue
{
	int Index;
	string Value;
	string searchIndexes[100];
	int lengthOfSearchIndexes;
	struct DTreeNode *ptrToNextNode;
};

struct DTreeNode
{
	string Name;
	int NoOfDistinctValues;
	struct DTreeValue *PtrToNextValue[30];
};


string **tData = NULL; // [No_of_instances][No_of_atributes]
unsigned long long int temparray[3][35000];
unsigned long long int noOfInstances = 1;
int noOfAttributes = 15;
struct AtttributeMetadata *headOfMetadata = NULL;
set<unsigned long long int> sa;

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
	float locala,localb,localTotal;
	double globalEntropy = calculateEntropyWithRespectToAttr(globala,globalb,globalTotal);
	if(attr->IsContinous == 0)
	{
		for(int i=1;i<attr->NoOfdistinctValues;i++)
		{
			locala = getLessThan(attr,attr->DistinctValues[i],array,count);
			localb = getGreaterThan(attr,attr->DistinctValues[i],array,count);
			localTotal = locala + localb;
			//cout << locala << "---" << localb << "\n";
			globalEntropy += (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
		}
		return globalEntropy;
	}
	else
	{
		for(int i=1;i<attr->NoOfdistinctValues;i++)
		{	/*
			//TO-DO//
			locala = getLessThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localb = getGreaterThanRange(attr,attr->Range[i][1],attr->Range[i][2],array,count);
			localTotal = locala + localb;
			cout << locala << "---" << localb << "\n";
			globalEntropy += (localTotal/globalTotal)*calculateEntropyWithRespectToAttr(locala,localb,localTotal);
			return 0;
			*/
		}
	}
	return 0;
}

int checkForAlgoCompletion()
{
	cout << "done" << "\n";
	struct AtttributeMetadata *check = headOfMetadata;
	int noOfAttributesDone = 0;
	while(check)
	{
		if(check->IsDone == 1)
			noOfAttributesDone++;
		check = check->PtrToNextAttr;
	}
	if(noOfAttributesDone == noOfAttributes)
		return 1;
	else
		return 0;
}


struct DTreeNode* BuildDTree(string **oldArray,int oldCount)
{
	global++;
	if(global == 1)
		return NULL;

	cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	if(checkForAlgoCompletion())
		return NULL;
	else
	{
		struct AtttributeMetadata *temp = headOfMetadata;
		struct AtttributeMetadata *finalOne = NULL;
		float prev = 0;
		double newGain = 0;
		double max = -1;
		int index;

		//printTData(oldArray,oldCount);
		while(temp)
		{
			if(temp->IsDone != 1)
			{
				newGain = calculateInformationGain(temp,oldArray,oldCount);
				cout << temp->Name << " information gain is : " << newGain << "\n";
			}

			if(max < newGain)
			{
				max = newGain;
				finalOne = temp;
			}

			temp = temp->PtrToNextAttr;
		}
		cout << "\nmax is" << finalOne->Name << "and information gain is : " << max << "\n";

		struct DTreeNode *newDtreeNode = new DTreeNode();
		newDtreeNode->Name = finalOne->Name;
		newDtreeNode->NoOfDistinctValues = finalOne->NoOfdistinctValues;
		for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
		{
			struct DTreeValue *newValueNode = new DTreeValue();
			newValueNode->Value = finalOne->DistinctValues[i];
			newValueNode->Index = finalOne->Index;
			newDtreeNode->PtrToNextValue[i] = newValueNode;
		}
		finalOne->IsDone = 1;



		for(int i=1;i<newDtreeNode->NoOfDistinctValues;i++)
		{

			// dynamic allocation
			string** newArray = new string*[oldCount];
			for(int rk = 0; rk <= oldCount; ++rk)
				newArray[rk] = new string[20];
			int newCount = 1;
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
			cout <<"\n";
			cout << "calling with count : " << newCount << " and value : " << newDtreeNode->PtrToNextValue[i]->Value << "\n";
			newDtreeNode->PtrToNextValue[i]->ptrToNextNode = BuildDTree(newArray,newCount);
			delete(newArray);

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

	    for(unsigned long long int i=1;i<noOfInstances;i++)
		{
			//cout << "copying to set " << i << "\n";
			sa.insert(atoi(tData[i][temp->Index].c_str()));
			//temparray[2][i] = tData[i][noOfAttributes];
		}
	    //std::cout << sa.size() << std::endl;
	    set <unsigned long long int, greater <unsigned long long int> > :: iterator itr;
	    int noOfDistinctValues = 1;
	    for (itr = sa.begin(); itr != sa.end(); ++itr)
	        {
	    		temparray[1][noOfDistinctValues] = *itr;
	    		//cout << *itr << "|";
	    		//cout << temparray[1][noOfDistinctValues] << "|";
	            noOfDistinctValues++;
	        }
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
	    	temp->Range[temp->NoOfdistinctValues][1] = 0;
			temp->Range[temp->NoOfdistinctValues][2] = temparray[1][noOfDistinctValues-1]+1;
			temp->NoOfdistinctValues ++;
			return;
	    }
	    double previousRange = 0;
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




void ReadTDataFrmFile()
{

	std::ifstream infile("assg2data.txt");
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



int main()
{
	tData = new string*[35000];
	for(int i = 0; i < 35000; ++i)
		 tData[i] = new string[20];

	ReadMetadata();
	ReadTDataFrmFile();
	//printTData(tData,noOfInstances);
	handleContinousValues();
	struct DTreeNode * headOfDTree = BuildDTree(tData,noOfInstances);
	return 1;

}
