#include <stdio.h>
#include <stdlib.h>
void main ()
{
	
	int arr[5],key;
	for(int i =0 ; i < 5 ; i++)
		scanf("%d",&arr[i]);
	scanf("%d",&key);	
	int low =0,high = 4, mid =0 ;
	while(low <=high)
	{
		mid = (low + high)/2;
		if(key == arr[mid] )
		{
			printf("\n%d",(mid +1));
				exit (0);
		}
		if(key > arr[mid])
			low = mid+1;
		if(key < arr[mid])
			high = mid-1;
}
printf(" not found");
}













