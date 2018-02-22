#include <stdio.h>
#include <stdlib.h>
void merge(int arr[],int l , int m ,int r)
{
  int n1= m-l +1,n2=r-m,k=l;
  int arr1[n1],arr2[n2];
  for(int i =0; i<n1;i++)
  {
    arr1[i]=arr[l+i];
  }
  for(int i =0; i<n2;i++)
  {
     arr2[i]=arr[m+i+1];
  }
  int i =0, j =0;
  while(i<n1&&j<n2)
  {
        if(arr1[i]<arr2[j])
          arr[k++]= arr1[i++];
        if(arr1[i]>arr2[j])
          arr[k++]= arr2[j++];
        if(arr1[i]==arr2[j])
        {
           arr[k++]= arr1[i++];
           arr[k++] = arr2[j++];
        }
   }	
  while(i<n1)
    arr[k++]=arr1[i++];
  while(j<n2)
    arr[k++]=arr2[j++];
  //printf("ee");
}
void mergesort(int arr[], int l , int r)
{
  if(l<r)
  {
    int m = (l+r)/2;
    mergesort(arr,l,m);	
    mergesort(arr,m+1,r);
    merge(arr,l,m,r);
  }
}
void main()
{
  int arr[]={64, 25, 12, 22, 11};
  int n = 5;
  mergesort(arr,0,4);
  for ( int i=0; i < n; i++)
         printf("%d ", arr[i]);  
}


