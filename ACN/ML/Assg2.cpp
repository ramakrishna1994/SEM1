#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>

using namespace std;



struct AtttributeMetadata{
	int index;
	string Name;
	string DistinctValues[21000];
	int NoOfdistinctValues;
	int IsContinous;
	struct AtttributeMetadata *PtrToNextAttr;
};
string tData[50000][20]; // [No_of_instances][No_of_atributes]
unsigned long long int noOfInstances = 1;
int noOfAttributes = 15;
struct AtttributeMetadata *headOfMetadata = NULL;

void ReadTDataFrmFile();
void printTData();
void handleContinousValues();
void ReadMetadata();
struct AtttributeMetadata *insertMetadata();
void makeItDiscreteValues(struct AtttributeMetadata* temp);
void printArray(unsigned long long int (&array)[3][35000],int length);
unsigned long long int findCount(int index,unsigned long long int value,string compare);
void startAlgo();
float calculateInformationGain(struct AtttributeMetadata *attr);
float calculateGlobalEntropyWithRespectToAttr(struct Attribute* attr);




int getLessThan()
{
	int count=0;
	for(int i=1;i<noOfInstances;i++)
		if(tData[i][noOfAttributes] == "<=50K")
			count++;
	return count;
}

int getGreaterThan()
{
	int count=0;
	for(int i=1;i<noOfInstances;i++)
		if(tData[i][noOfAttributes] == ">50K")
			count++;
	return count;

}

double calculateGlobalEntropyWithRespectToAttr(double a,double b,double total)
{

		double part1 = (double)a/(double)noOfInstances;
		double part2 = (double)b/(double)noOfInstances;
		double entropy =  -(part1 * log(part1))-(part2 * log(part2));
		cout << "float is " << entropy << "\n";
		return entropy;

}


float calculateInformationGain(struct AtttributeMetadata *attr)
{
	float a = getLessThan();
	float b = getGreaterThan();
	double globalEntropy = calculateGlobalEntropyWithRespectToAttr(a,b,a+b);
	for(int i=1;i<attr->NoOfdistinctValues;i++)
	{
		globalEntropy +=
	}

}



void startAlgo()
{
	struct AtttributeMetadata *temp = headOfMetadata;
	float prev = 0;
	while(temp)
	{
		float newGain = calculateInformationGain(temp);
		temp = temp->PtrToNextAttr;
	}
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
		newnode->index = index;
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

void handleContinousValues()
{
	struct AtttributeMetadata* temp = headOfMetadata;
	while(temp)
	{
		if(temp->IsContinous == 1)
		{
			makeItDiscreteValues(temp);
		}
		temp = temp->PtrToNextAttr;
	}
}

void makeItDiscreteValues(struct AtttributeMetadata* temp)
{
	unsigned long long int temparray[3][35000];
	for(int i=1;i<noOfInstances;i++)
	{
		//cout << "copying " << i << "\n";
		temparray[1][i] = atoi(tData[i][temp->index].c_str());
		//temparray[2][i] = tData[i][noOfAttributes];
	}
	//findHowManyDiscreteValues(temparray);


	    std::set<int> sa(&temparray[1][1], &temparray[1][noOfInstances]);
	    //std::cout << sa.size() << std::endl;
	    set <int, greater <int> > :: iterator itr;
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
	    	unsigned long long int lessThanCount = findCount(temp->index,temparray[1][i],"<=50K");
			unsigned long long int greaterThanCount = findCount(temp->index,temparray[1][i],">50K");
			if(lessThanCount <= greaterThanCount)
				temparray[2][i] = 1; // 1 indicates >50K
			else
				temparray[2][i] = 0; // 0 indicates <=50K
			/*cout << "less than count is " << lessThanCount << "\n";
			cout << "greater than count is " << greaterThanCount << "\n";
			cout << "-----------------------\n";*/
	    }
	    printArray(temparray,noOfDistinctValues);
	    int prev = temparray[2][1];
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
	    if(count+2 == noOfDistinctValues && temparray[2][1]==1)
	    {
			//cout << "average is " << temparray[1][noOfDistinctValues]+1 << "\n";
			temp->DistinctValues[noOfDistinctValues] = temparray[1][noOfDistinctValues]+1;
			temp->NoOfdistinctValues ++;
	    }
	    if(count+2 == noOfDistinctValues && temparray[2][1]==0)
		{
	    	//cout << "count is "<< count <<"\n";
			//cout << "average is " << temparray[1][1]-1 << "\n";
			temp->DistinctValues[noOfDistinctValues] = temparray[1][1]-1;
			temp->NoOfdistinctValues ++;
		}
	    for(int i=2;i<noOfDistinctValues;i++)
	    {
	    	if(prev != temparray[2][i])
	    		{
	    			double average = ((double)temparray[1][i-1] + (double)temparray[1][i])/2;
	    			//cout << "average is " << average << "\n";
	    			prev = temparray[2][noOfDistinctValues];
	    			temp->DistinctValues[noOfDistinctValues] = average;
	    			temp->NoOfdistinctValues ++;

	    		}
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

void printTData()
{
	for(int i=1;i<noOfInstances;i++)
		{
			for(int j=1;j<=noOfAttributes;j++)
				cout<<tData[i][j]<<"|";
			cout<<"\n";
		}
	cout << "no of instances are " << noOfInstances << "\n";
}

int main()
{


	ReadMetadata();
	ReadTDataFrmFile();
	//printTData();
	//handleContinousValues();
	startAlgo();
	return 1;

}
