
/* 
 * File:   randomselection.cpp
 * Author: benjamin
 *
 * Created on 11 September, 2017, 9:06 PM
 */

#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

int random_selection(int *ptr, int size, int median);
int partition(int *ptr, int size);
void swap(int *a, int *b);
bool check(int *ptr, int index, int size);
void set_array(int *ptr, int size);

int arr[10000];


int random_selection(int *ptr, int size, int median){
    int l = 0;
    int r = size - 1;
    int index = 0;
    int res = 0;
    srand (time(NULL));
    bool flag = false;
    if(size == 1)
        return arr[0];
    else{
        while(!flag){
            index = rand() % size;
            flag = check(ptr, index, size);
        }
        swap(ptr[l], ptr[index]);
        res = partition(ptr, size);
        if( res == median)
            return ptr[res];
        else if( res < median){
            int t1 = 0,t2 = 0;
            l = res + 1;
            t1 = size - (l + 1);
            t2 = median - (res + 1);
            return random_selection((ptr + l), t1, t2);
        }
        else if( res > median){
            int t1;
            r = res - 1;
            t1 = size - (res + 1);
            return random_selection(ptr, t1, median);
        }
    }            
}

int partition(int *ptr, int size){
    int l = 0;
    int r = size - 1;
    int p = 0;
    int x = ptr[p];
    while (l < r){
        while (l < size && arr[l] <= x)
            l += 1;
        while (r > -1 && arr[r] > x)
            r -= 1;
        if(l < r)
            swap(arr[l], arr[r]);
    }
    swap(arr[p], arr[r]);
    return r;
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

bool check(int *ptr, int index, int size){
    int i;
    int count = 0;
    for(i = 0;i < size ; ++i){
        if(ptr[i] < ptr[index])
            ++count;
    }
    if( count <= (int)floor(0.75*size) && count >= (int)floor(0.25*size))
        return true;
    return false;
}


int getmedian(int size){
    int ans = (int)floor(size/2);
    if(ans % 2 == 0)
        --ans;
    return ans;
}

void set_array(int *ptr, int size){
    int i = 0;
    srand (time(NULL));
    for(i = 0;i < size;++i)
        ptr[i] = (rand() % 50000);
}





int main(int argc, char** argv) {
    
    int size = 10;
    set_array(arr, size);
    int median = getmedian(size);
    int ans = random_selection(arr, size, median);
    cout<<"The Median for given input is\n";
    cout<<ans;
    return 0;
}

