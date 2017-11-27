/*
 * Author : Saradhi Ramakrishna (2017H1030081H)
 * Practical implementation of Google Hashcode Qualification problem
 * 1. Select Nearest warehouse to customer.
 * 2. Select Nearest drone to warehouse.
 * 3. Check if distance covered by drone to warehouse and customer will exceed maxturns a drone can take.
 * 4. If yes, repeat 1 and 2.
 * 5. If No, then go to warehouse and load the products in drone.
 * 6. Then go to customer and deliver the products.
 * 7. Update drone location and drone distance.
 * 8. Repeat it until the customer order is satisfied.
 * 9. Repeat the entire process again for all customers.
 * Note : 	1. Drone will load partial customer orders from warehouse.
 * 			2. Drone will always contain not more than what a customer wants.
 *
 * Tests run and results :
 * 			1. redundancy.in 				--- 	508 out of total 1000
 * 			2. mother_of_all_warehouses.in	---		16  out of total 800
 * 			3. busy_day.in					---		108 out of total 1250
 * 			4. sampleinput.in				---		3   out of total 3
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <set>
#include <cmath>
#include <string>
#include <limits>

using namespace std;

const int MAX = numeric_limits<int>::max();
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
int distanceTravelledByDrone[10001] = {0};
int temporaryDistanceCheckForDrone[10001] = {0};

void readInputsFromFile()
{
	//ifstream infile("mother_of_all_warehouses.in");
	//ifstream infile("busy_day.in");
	ifstream infile("redundancy.in");
	//ifstream infile("sampleinput.in");
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

int calculateDistance(int x1,int y1,int x2,int y2)
{
	int roundedOfDistance = round(sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) ));
	return roundedOfDistance;
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

int isWarehouseContainingSatisfyingMinimumCustomerOrder(int customerOrderId, int warehouseId)
{
	int remainingRequiredForCustomer = 0;
	int totalProductsSatisfiedByWarehouse = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		remainingRequiredForCustomer = quantityOrderedByCustomer[customerOrderId][i] - stockInCustomerLocation[customerOrderId][i];
		if(remainingRequiredForCustomer > 0)
		{
			if(stockInWareHouses[warehouseId][i] > 0)
				totalProductsSatisfiedByWarehouse ++;
		}
	}
	if(totalProductsSatisfiedByWarehouse > 0)
		return 1;
	else
		return 0;
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
		if(min > distance && isWarehouseContainingSatisfyingMinimumCustomerOrder(customerOrderId,warehouseId))
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
		if(min > distance && temporaryDistanceCheckForDrone[droneId]!=1)
		{
			min = distance;
			selectedDroneId = droneId;
		}
	}
	return selectedDroneId;
}

int isWeightOfDroneExceedingMaxPayload(int droneId)
{
	int totalWeight = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		totalWeight += stockInDrone[droneId][i] * productTypeWeights[i];
	}
	cout << "total weight in drone : " << droneId <<" is "<< totalWeight << endl;
	if(totalWeight >= maxPayloadDroneCanCarry)
		return 1;
	else
		return 0;
}

void loadProductsFromWarehouseIntoDrone(int customerOrderId,int wareHouseId,int droneId)
{
	int remainingRequiredForCustomer = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		remainingRequiredForCustomer = quantityOrderedByCustomer[customerOrderId][i] - stockInCustomerLocation[customerOrderId][i];

		if(remainingRequiredForCustomer > 0)
		{
			cout << "remaining for product type " << i << " is "<< remainingRequiredForCustomer << endl;
			int remainingRequiredInDroneToSatisfyCustomerOrder = remainingRequiredForCustomer - stockInDrone[droneId][i];
			if(remainingRequiredInDroneToSatisfyCustomerOrder > 0)
			{
				if(remainingRequiredInDroneToSatisfyCustomerOrder <= stockInWareHouses[wareHouseId][i])
				{
					cout << "remaining required in drone to satisfy customer for product type " << i << " is "<< remainingRequiredInDroneToSatisfyCustomerOrder << endl;
					cout << "Stock in warehouse of product type " << i << " is " << stockInWareHouses[wareHouseId][i] << endl;
					stockInDrone[droneId][i] += remainingRequiredInDroneToSatisfyCustomerOrder;
					stockInWareHouses[wareHouseId][i] -= remainingRequiredInDroneToSatisfyCustomerOrder;
					if(isWeightOfDroneExceedingMaxPayload(droneId))
					{
						cout << "weight exceeded for drone : " << droneId << " so, removing product type : " << i << endl;
						stockInDrone[droneId][i] -= remainingRequiredInDroneToSatisfyCustomerOrder;
						stockInWareHouses[wareHouseId][i] += remainingRequiredInDroneToSatisfyCustomerOrder;
					}

				}
				else
				{
					int temp = stockInWareHouses[wareHouseId][i];
					cout << "remaining required in drone to satisfy customer for product type " << i << " is "<< remainingRequiredInDroneToSatisfyCustomerOrder << endl;
					cout << "Stock in warehouse of product type " << i << " is " << stockInWareHouses[wareHouseId][i] << endl;
					stockInDrone[droneId][i] += temp;
					stockInWareHouses[wareHouseId][i] = 0;
					if(isWeightOfDroneExceedingMaxPayload(droneId))
					{
						cout << "weight exceeded for drone : " << droneId << " so, removing product type : " << i << endl;
						stockInDrone[droneId][i] -= temp;
						stockInWareHouses[wareHouseId][i] += temp;
					}
				}
			}

		}

	}
}

void checkForCustomerOrderCompletionAndSetTheFlag(int customerOrderId)
{
	int noOfProductTypesSatisfied = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		if(stockInCustomerLocation[customerOrderId][i] == quantityOrderedByCustomer[customerOrderId][i])
			noOfProductTypesSatisfied ++;
	}
	if(noOfProductTypesSatisfied == noOfProductTypes)
		{
			isCustomerOrderCompleted[customerOrderId] = 1;
			cout << "customer order completed" << endl;
		}
}

void updateDroneLocation(int customerOrderId, int droneId)
{
	dronesLocation[droneId][1] = customerOrdersLocation[customerOrderId][1];
	dronesLocation[droneId][2] = customerOrdersLocation[customerOrderId][2];
	cout << "drone : " << droneId << " location updated to  customer : " << customerOrderId << endl;
}


void updateDistanceTravelledByDrone(int customerOrderId, int warehouseId,int droneId)
{
	int x1,y1,x2,y2;
	x1 = customerOrdersLocation[customerOrderId][1];
	y1 = customerOrdersLocation[customerOrderId][2];
	x2 = wareHousesLocation[warehouseId][1];
	y2 = wareHousesLocation[warehouseId][2];
	int distanceBetweenCustomerAndWarehouse = calculateDistance(x1,y1,x2,y2);
	x1 = dronesLocation[droneId][1];
	y1 = dronesLocation[droneId][2];
	x2 = wareHousesLocation[warehouseId][1];
	y2 = wareHousesLocation[warehouseId][2];
	int distanceBetweenDroneAndWarehouse = calculateDistance(x1,y1,x2,y2);
	distanceTravelledByDrone[droneId] += distanceBetweenCustomerAndWarehouse + distanceBetweenDroneAndWarehouse;
	cout << "Updating Distance Travelled by drone : " << droneId << endl;
}

void deliverProductsToCustomer(int customerOrderId,int selectedDroneId)
{
	int remainingRequiredForCustomer = 0;
	for(int i=1;i<=noOfProductTypes;i++)
	{
		//remainingRequiredForCustomer = quantityOrderedByCustomer[customerOrderId][i] - stockInCustomerLocation[customerOrderId][i];
		if(stockInDrone[selectedDroneId][i] > 0)
		{
			cout << "delivering product type " << i << " is " << stockInDrone[selectedDroneId][i] << endl;
			stockInCustomerLocation[customerOrderId][i] += stockInDrone[selectedDroneId][i];
			stockInDrone[selectedDroneId][i] = 0;
		}
	}
	checkForCustomerOrderCompletionAndSetTheFlag(customerOrderId);
	updateDroneLocation(customerOrderId,selectedDroneId);
}

int willDroneDistanceBeExhaustedBeforeItCanComplete(int customerOrderId,int warehouseId, int droneId)
{
	int x1,y1,x2,y2;
	x1 = customerOrdersLocation[customerOrderId][1];
	y1 = customerOrdersLocation[customerOrderId][2];
	x2 = wareHousesLocation[warehouseId][1];
	y2 = wareHousesLocation[warehouseId][2];
	int distanceBetweenCustomerAndWarehouse = calculateDistance(x1,y1,x2,y2);
	x1 = dronesLocation[droneId][1];
	y1 = dronesLocation[droneId][2];
	x2 = wareHousesLocation[warehouseId][1];
	y2 = wareHousesLocation[warehouseId][2];
	int distanceBetweenDroneAndWarehouse = calculateDistance(x1,y1,x2,y2);
	int newDistance = distanceTravelledByDrone[droneId] + distanceBetweenCustomerAndWarehouse + distanceBetweenDroneAndWarehouse;
	if(newDistance > noOfMaxTurns)
	{
		temporaryDistanceCheckForDrone[droneId] = 1;
		return 1;
	}
	else
		return 0;

}

void resetTemporaryDistanceCheckForDrone()
{
	for(int i=1;i<=noOfDrones;i++)
		temporaryDistanceCheckForDrone[i] = 0;
}

int areAllDroneDistancesExhausted()
{
	int count = 0;
	for(int i=1;i<=noOfDrones;i++)
		if(temporaryDistanceCheckForDrone[i] == 1)
			count++;
	if(count == noOfDrones)
		return 1;
	else
		return 0;
}
void startAlgo()
{
	for(int customerOrderId=1;customerOrderId<=noOfCustomerOrders;customerOrderId++)
	{
		cout << "=============================running for customer: " << customerOrderId << "===============================================" << endl;
		resetTemporaryDistanceCheckForDrone();
		while(1){
			int warehouseIDCloseToCustomer = findWarehouseCloseToSpecificCustomer(customerOrderId);
			cout << "WareHouse close to customer:" << customerOrderId << " is warehouse:" << warehouseIDCloseToCustomer << endl;
			int droneCloseToWarehouse = findDroneCloseToSpecificWarehouse(warehouseIDCloseToCustomer);
			cout << "Drone Close to Warehouse:" << warehouseIDCloseToCustomer << " is drone:" << droneCloseToWarehouse << endl;
			if(!willDroneDistanceBeExhaustedBeforeItCanComplete(customerOrderId,warehouseIDCloseToCustomer,droneCloseToWarehouse))
			{
				loadProductsFromWarehouseIntoDrone(customerOrderId,warehouseIDCloseToCustomer,droneCloseToWarehouse);
				deliverProductsToCustomer(customerOrderId,droneCloseToWarehouse);
				updateDistanceTravelledByDrone(customerOrderId,warehouseIDCloseToCustomer,droneCloseToWarehouse);
			}
			else
				cout << "Drone : " << droneCloseToWarehouse << " distance will be exhausted before it can complete . So selecting new drone" << endl;
			if(isCustomerOrderCompleted[customerOrderId] == 1)
				break;
			if(areAllDroneDistancesExhausted())
			{
				cout << "All Drones distances are exhaused " << endl;
				cout << "Customer order cannot be completed" << endl;
				break;
			}
			cout << "----------" << endl;
		}
	}
}

int noOfCustomerOrdersSatisfied()
{
	int count = 0;
	for(int i=1;i<=noOfCustomerOrders;i++)
		if(isCustomerOrderCompleted[i] == 1)
			count++;
	return count;
}

void printDistancesOfDrones()
{
	int max = -1;
	for(int i=1;i<=noOfDrones;i++)
	{
		cout << " Distance of drone " << i << " is " << distanceTravelledByDrone[i] << endl;
		if(max < distanceTravelledByDrone[i])
			max = distanceTravelledByDrone[i];
	}
	cout << "==============================================================================" << endl;
	cout << "Total Time or Turns taken to complete the orders is " << max << endl;

}

int main()
{
	readInputsFromFile();
	//printData();
	startAlgo();
	cout << "==============================================================================" << endl;
	//cout << "No of customer Orders satisfied are " << noOfCustomerOrdersSatisfied() << " out of total " << noOfCustomerOrders << endl;
	cout << "==============================================================================" << endl;
	printDistancesOfDrones();

}
