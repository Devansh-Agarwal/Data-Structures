#include <stdio.h>
#include <stdlib.h>
void main()
{
int arr[]={64, 22, 12, 22, 11};
int n = 5;
for(int i= 0; i<n-1;i ++)
{
for(int j= 0 ; j<n-i-1;j ++)
{
if(arr[j]>arr[j+1])
{int temp;
temp= arr[j+1];
arr[j+1]=arr[j];
arr[j]=temp;}}}
for ( int i=0; i < n; i++)
        printf("%d ", arr[i]);  
}

