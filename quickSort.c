#include<iostream>
#include<stdlib.h>
int partition(int *arr,int l , int high)
{
int pivot = arr[high];
int i = l -1;
for(int j =l ; j< high; j++)
{
if(arr[j]<=pivot)
{int temp = arr[j];
arr[j]= arr[++i];
arr[i]=temp;
} }
int temp = arr[++i];
arr[i]= arr[high];
arr[high]=temp;
return i;
}
void quicksort(int *arr,int l , int high)
{if(l<high)
{int pi = partition(arr, l,high);
quicksort(arr,l,pi-1);
quicksort(arr,pi+1,high);
}}
void main()
{int arr[]={64, 25, 12, 22, 11};
 int n = 5;
quicksort(arr,0,4);
for ( int i=0; i < n; i++)
        cout<< arr[i];  
}
