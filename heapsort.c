#include <stdio.h>
#include <stdlib.h>

//swaps the value at teo memory addresses 
void swap(int* a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

//minHeapify() assumes  that the binary trees rooted at left and right child are min heaps
void minHeapify(int *arr, int size, int index)
{
	
	int smallest = index, leftChild = (index*2) + 1, rightChild = (index*2) + 2;//calculating the left and right child location in array
	
	if((leftChild<size) && (arr[leftChild] < arr[index]))// checking if leftChild is smaller than index and if there exists a left child
		smallest = leftChild;
	if((rightChild<size) && (arr[rightChild] < arr[smallest]))// checking if rightChild is smaller than smallest and if there exists a right child
		smallest = rightChild;

	if(smallest != index)//if one of the children is smaller than index
	{
		swap(&arr[index],&arr[smallest]);// swap smaller child and index
		minHeapify(arr,size,smallest);//applying minheapify on smaller childs location
	}

}

// buildMinHeap() coverts array into a minheap in bottom - up manner using minHeapify()
void buildMinHeap(int *arr, int size)
{
	for(int i = ((size/2)-1); i>=0; i--)// traversing array in a bottom up manner from the 1st non-leaf node to root
		minHeapify(arr, size, i);// calling minHeapify() for each non-leaf node
}

//builds a min heap in the begining and then returns an array sorted in decending order
void heapsort(int * arr, int size)
{
	buildMinHeap(arr, size);// builds a min heap

	// loop used for creating a decreasing order array from min heap
	for(int i =size-1; i>=1; i--)// goes from (size -1)  to 1 as the last element would automatically be sorted 
	{
		swap(&arr[0], &arr[i]);// swaps the root of min heap with min heap's last entry
		minHeapify(arr, i, 0);//applied only on root as only root has been changed and size of heap is also decreased for further calls 
	}
}

void main()
{
	int size, i= 0;
	
	printf("Enter the size of the array\n");
	scanf("%d",&size);
	int * arr = (int*)malloc(sizeof(int)*size);// dynamically allocating array of required size
	
	if(arr == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}
	printf("Enter the elements to heapsort\n");			
	while(i<size)// used for taking the array input
		scanf("%d",&arr[i++]);

	heapsort(arr,size);// sorts the array in descending order
	
	for(i= 0; i<=(((size-1))/2) ; i++)// reverses the array hence we get an accending order array
	{
	  swap(&arr[i], &arr[size-1-i]);//swapping the element from front and back in an symmetric manner
	}
	for(i= 0; i < size; i++)// printing the array
		printf("%d ",arr[i]);
}