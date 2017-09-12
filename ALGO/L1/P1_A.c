// C++ program to approximate number of people in Birthday Paradox
// problem
#include <stdio.h>
#include <math.h>


// Returns approximate number of people for a given probability
int findnoofpersons(double d,double p)
{
    return ceil(sqrt(2*365*log(1/(1-p))));
}

int main()
{
	double prob,noofdays;
	printf("Enter the desired probability : ");
	scanf("%lf",&prob);
	printf("Enter the no of days :");
	scanf("%lf",&noofdays);
	printf("No. of persons : %d \n",findnoofpersons(noofdays,prob));
}

// C++ program to approximate number of people in Birthday Paradox
// problem


