#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>
#include <string>
#define MAX 32000;

using namespace std;

char fileName[] = "sampleinput.in";

int noOfRowsInMap = 0;
int noOfColumnsInMap = 0;
int noOfDrones = 0;
int noOfMaxTurns = 0;
int maxPayloadDroneCanCarry = 0;

int noOfProductTypes = 0;
int productTypeWeights[10001];

int noOfWareHouses = 0;
int wareHousesLocation[10001][3];
int stockInWareHouses[10001][10001];

int noOfCustomerOrders = 0;
int customerOrdersLocation[10001][3];
int quantityOrderedByCustomer[10001][10001];
int stockInCustomerLocation[10001][10001];
int totalQuantityOfSpecificCustomerOrder[10001];
bool isCustomerOrderCompleted[10001] = {false};

int dronesLocation[10001][3];
int stockInDrone[10001][10001];
int currentDroneWeight[10001] = {0};

int calculateDistance(int x1,int y1,int x2,int y2)
{
	int roundedOfDistance = round(sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) ));
	return roundedOfDistance;
}


void readInputsFromFile()
{
	ifstream infile("sampleinput.in");
	string line;
	int lineNo=1;
	int count1 = 1;
	int count2 = 1;
	int currentWarehouse = 1;
	int currentCustomerOrder = 1;
	while (getline(infile, line))
	{
		istringstream ss(line);
		string token;
		//cout << line << endl;
		if(lineNo == 1)
		{
			string arr[5];
			int i=0;
			while(getline(ss, token, ' '))
			{
				arr[i] = token;
				i++;
			}
			noOfRowsInMap = atoi(arr[0].c_str());
			noOfColumnsInMap = atoi(arr[1].c_str());
			noOfDrones = atoi(arr[2].c_str());
			noOfMaxTurns = atoi(arr[3].c_str());
			maxPayloadDroneCanCarry = atoi(arr[4].c_str());
		}
		else if(lineNo == 2)
		{
			noOfProductTypes = atoi(line.c_str());
		}
		else if(lineNo == 3)
		{
			int i=1;
			while(getline(ss, token, ' '))
			{
				productTypeWeights[i] = atoi(token.c_str());
				i++;
			}
		}
		else if(lineNo == 4)
		{
			noOfWareHouses = atoi(line.c_str());
		}
		else if(lineNo != (5+noOfWareHouses*2) && (currentWarehouse != (noOfWareHouses+1)))
		{
			string arr[10001];
			int i=1;
			while(getline(ss, token, ' '))
			{
				arr[i] = token;
				i++;
			}
			if(count1%2 == 1)
			{
				wareHousesLocation[currentWarehouse][1] = atoi(arr[1].c_str());
				wareHousesLocation[currentWarehouse][2] = atoi(arr[2].c_str());

			}
			else
			{
				for(int i=1;i<=noOfProductTypes;i++)
					stockInWareHouses[currentWarehouse][i] = atoi(arr[i].c_str());
				currentWarehouse++;
			}
			count1++;
		}
		else if(lineNo == (5+noOfWareHouses*2))
		{
			noOfCustomerOrders = atoi(line.c_str());
		}
		else if((lineNo != ((5+noOfWareHouses*2)+(noOfCustomerOrders*3)+1)) && (currentCustomerOrder!=(noOfCustomerOrders+1)))
		{
			string arr[10001];
			int i=1;
			while(getline(ss, token, ' '))
			{
				arr[i] = token;
				i++;
			}
			if(count2%3 == 1)
			{
				customerOrdersLocation[currentCustomerOrder][1] = atoi(arr[1].c_str());
				customerOrdersLocation[currentCustomerOrder][2] = atoi(arr[2].c_str());
			}
			else if(count2%3 == 2)
			{
				totalQuantityOfSpecificCustomerOrder[currentCustomerOrder] = atoi(line.c_str());
			}
			else
			{
				for(int i=1;i<=totalQuantityOfSpecificCustomerOrder[currentCustomerOrder];i++)
				{
					int productType = atoi(arr[i].c_str()) + 1;
					(quantityOrderedByCustomer[currentCustomerOrder][productType])++;
				}
				currentCustomerOrder++;
			}
			count2++;
		}
		lineNo++;
	}
	for(int i=1;i<=noOfDrones;i++)
	{
		dronesLocation[i][1] = wareHousesLocation[1][1];
		dronesLocation[i][2] = wareHousesLocation[1][2];
	}
}

void printData()
{
	cout << noOfRowsInMap << " " << noOfColumnsInMap << " " << noOfDrones << " " << noOfMaxTurns << " " << maxPayloadDroneCanCarry << endl;
	cout << noOfProductTypes << endl;
	for(int i=1;i<=noOfProductTypes;i++)
		cout << productTypeWeights[i] << " ";
	cout << endl;
	cout << noOfWareHouses << endl;
	for(int i=1;i<=noOfWareHouses;i++)
	{
		cout << wareHousesLocation[i][1] << " " << wareHousesLocation[i][2] << endl;
		for(int j=1;j<=noOfProductTypes;j++)
			cout << stockInWareHouses[i][j] << " " ;
		cout << endl;
	}
	cout << noOfCustomerOrders << endl;
	for(int i=1;i<=noOfCustomerOrders;i++)
	{
		cout << customerOrdersLocation[i][1] << " " << customerOrdersLocation[i][2] << endl;
		cout << totalQuantityOfSpecificCustomerOrder[i] << endl;
		for(int j=1;j<=noOfProductTypes;j++)
		{
			if(quantityOrderedByCustomer[i][j] > 0)
				cout << j << " ";
		}
		cout << endl;
	}
}

int findWarehouseCloseToSpecificCustomer(int customerOrderId)
{
	int min = MAX;
	int selectedWareHouseId = -1;
	int x1 = customerOrdersLocation[customerOrderId][1];
	int y1 = customerOrdersLocation[customerOrderId][2];
	int x2,y2,distance;
	for(int warehouseId=1;warehouseId <= noOfWareHouses;warehouseId++)
	{
		x2 = wareHousesLocation[warehouseId][1];
		y2 = wareHousesLocation[warehouseId][2];
		distance = calculateDistance(x1,y1,x2,y2);
		if(min > distance)
		{
			min = distance;
			selectedWareHouseId = warehouseId;
		}
	}
	return selectedWareHouseId;
}

int findDroneCloseToSpecificWarehouse(int warehouseId)
{
	int min = MAX;
	int selectedDroneId = -1;
	int x1 = wareHousesLocation[warehouseId][1];
	int y1 = wareHousesLocation[warehouseId][2];
	int x2,y2,distance;
	for(int droneId = 1;droneId<=noOfDrones;droneId++)
	{
		x2 = dronesLocation[droneId][1];
		y2 = dronesLocation[droneId][2];
		distance = calculateDistance(x1,y1,x2,y2);
		if(min > distance)
		{
			min = distance;
			selectedDroneId = droneId;
		}
	}
	return selectedDroneId;
}


void serviceCustomerOrder(int customerOrderId,int wareHouseId,int droneId)
{
	int remainingRequiredForCustomer = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		remainingRequiredForCustomer = stockInCustomerLocation[customerOrderId][i] - stockInDrone[droneId][i];
		if(remainingRequiredForCustomer <= stockInWareHouses[wareHouseId][i])
		{
			stockInDrone[droneId][i] += remainingRequiredForCustomer;

		}
		else
		{
			stockInDrone[droneId][i] += stockInWareHouses[wareHouseId][i];
		}
	}
}

void startAlgo()
{
	for(int customerOrderId=1;customerOrderId<=noOfCustomerOrders;customerOrderId++)
	{
		int warehouseIDCloseToCustomer = findWarehouseCloseToSpecificCustomer(customerOrderId);
		cout << "WareHouse close to customer:" << customerOrderId << " is warehouse:" << warehouseIDCloseToCustomer << endl;
		int droneCloseToWarehouse = findDroneCloseToSpecificWarehouse(warehouseIDCloseToCustomer);
		cout << "Drone Close to Warehouse:" << warehouseIDCloseToCustomer << " is drone:" << droneCloseToWarehouse << endl;
		serviceCustomerOrder(customerOrderId,warehouseIDCloseToCustomer,droneCloseToWarehouse);

	}
}


int main()
{
	readInputsFromFile();
	//printData();
	startAlgo();

}
