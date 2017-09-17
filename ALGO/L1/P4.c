#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NO_OF_TRIALS 1

int *A,sizeofarray,medianindex,currentphase=0,previousphase=0,count;

int findphase(int low,int high);
int partition(int low,int high);
void printarray(int low,int high);
void swap(int *a,int *b);
int check_for_j_within_50_percent(int low,int high,int index);
void findmedian(int low,int high);
void sortarray();

void main()
{
	printf("Enter the size of array : ");
	scanf("%d",&sizeofarray);
	medianindex = ceil((float)sizeofarray / (float)2);
	printf("median index is %d\n",medianindex);
	A = (int *)malloc((sizeofarray+1)*sizeof(int));
	srand(time(NULL));
	for(int i=1;i<=sizeofarray;i++)
	{
		A[i] = rand() % (sizeofarray*1000) + 1;
	}

	findmedian(1,sizeofarray);
	printf("+++++++++++++++++++++++++++++++++++\n");
	sortarray();
	printf("Actual Median if array is sorted : %d\n",A[medianindex]);

}

void findmedian(int low,int high)
{
		printf("+++++++++++++++++++++++++++++++++++\n");
		printf("low is %d and high is %d\n",low,high);
		currentphase = findphase(low,high);
		if(currentphase == previousphase)
			count++;
		else
		{
			count = 1;
			previousphase = currentphase;
		}
		printf("We are in phase %d\n",currentphase);
		printf("Count in phase (X-%d) is %d\n",currentphase,count);
		//printarray(low,high);
		int pivotindex = partition(low,high);
		int istrue = check_for_j_within_50_percent(1,sizeofarray,pivotindex);
		//printf("%d\n",pivotindex);
		if(istrue)
			printf("we have got final pivot index as %d which is with in 50 percent range of median\n",pivotindex);
		else
			printf("we have got final pivot index as %d which is not within 50 percent range of median, so discarding the pivot and trying again\n",pivotindex);
		if(istrue)
		{
			if(pivotindex == medianindex)
			{
				printf("+++++++++++++++++++++++++++++++++++\n");
				printf("Median found from random experiment: %d\n",A[pivotindex]);
				return;
			}
			if(pivotindex >= medianindex)
			{
				findmedian(low,pivotindex-1);
			}
			else
			{
				findmedian(pivotindex+1,high);
			}
		}
		else
		{
			findmedian(low,high);
		}
}

int check_for_j_within_50_percent(int low,int high,int index)
{
	int length = high - low + 1;
	int quarterpartition = length / 4;
	int startingcount = medianindex - quarterpartition;
	int endingcount = medianindex + quarterpartition;
	if(index >= startingcount && index <= endingcount)
		return 1;
	else
		return 0;
}

void swap(int *a,int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int low,int high)
{
	int randomindex = rand() % (high - low) + low + 1; // Will generate low<=x<=high numbers only
	printf("random index is %d and pivot is %d\n",randomindex,A[randomindex]);
	int pivot = A[randomindex];
	    int i = low, j = high;

	    while (1)
	    {
	        // Find leftmost element greater than
	        // or equal to pivot
	        while (A[i] < pivot)
	        {
	        	i++;
	        }
	        //printf("i stopped at %d\n",i);
	        // Find rightmost element smaller than
	        // or equal to pivot
	        while (A[j] > pivot)
	        {
	        	j--;
	        }
	        //printf("j stopped at %d\n",j);
	        // If two pointers met.
	        if (i >= j)
	        {
	        	//swap(&A[randomindex],&A[j]);
	        	//printarray(low,high);
	        	return j;
	        }


	        swap(&A[i],&A[j]);
	        //printarray(low,high);
	    }
	//return randomindex;
}

/* Function to print an array */
void printarray(int low,int high)
{
    for (int i=low; i <=high; i++)
        printf("%d ", A[i]);
    printf("\n");
}

void sortarray()
{
	for(int i=1;i<=sizeofarray;i++)
	{
		for(int j=1;j<=sizeofarray;j++)
		{
			if(A[i] <= A[j])
				swap(&A[i],&A[j]);
		}

	}
}

int findphase(int low,int high)
{
	int length = high - low + 1;
	for(int i=0;i<sizeofarray;i++)
	{
		int startingcount = pow((double)0.75,(double)i)*sizeofarray;
		int endingcount = pow((double)0.75,(double)i+1)*(double)sizeofarray;
		//printf("starting is %d and ending is %lf and length is %d\n",startingcount,endingcount,length);
		if(length >= endingcount && length <= startingcount)
			return i;
	}
	return -1;
}


