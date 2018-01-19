#include <stdio.h>
#include <stdlib.h>

int maxSize,*minHeap,heapSize = 0;// declaring the 3 global variables which give information about the heap

//declaring all the functions used in the program
//----------------------------------------------------------
void swap(int *, int *);
void printMinHeap();
void minHeapify(int);
void insertMinHeap(int);
void findMin();
void deleteMin();
//----------------------------------------------------------

//swaps the value at two memory addresses 
void swap(int* a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// used to print the array which stores the minheap
void printMinHeap()
{
	if(heapSize <= 0 )// if there is no element in the heap
	{
		printf("The heap is empty\n");
		return;
	}	
	for(int i = 0; i<heapSize;i++)// prints all the elements from 0 to heapsize -1
		printf("%d ",minHeap[i]);
	printf("\n");
}

void minHeapify(int index)
{
	
	int smallest = index, leftChild = (index*2) + 1, rightChild = (index*2) + 2;//calculating the left and right child location in array
	
	if((leftChild<heapSize) && (minHeap[leftChild] < minHeap[index]))// checking if leftChild is smaller than index and if there exists a left child
		smallest = leftChild;
	if((rightChild<heapSize) && (minHeap[rightChild] < minHeap[smallest]))// checking if rightChild is smaller than smallest and if there exists a right child
		smallest = rightChild;

	if(smallest != index)//if one of the children is smaller than index
	{
		swap(&minHeap[index],&minHeap[smallest]);// swap smaller child and index
		minHeapify(smallest);//applying minheapify on smaller childs location
	}

}

//insertMinHeap() is used to insert an element in the heap
void insertMinHeap(int num)
{
	if(heapSize >= maxSize)// if there is no space left in the heap
	{
		printf("Min Heap Overflow\n");
		return;
	}
	minHeap[heapSize++] = num;// the new element is added to the end of array and size of heap is increased by one
	
	// used to fix the ordering property if it is destroyed by checking swapping the parent and child if the child is smaller
	// doing it till the root is reached or the parent is no more larger than child 
	for(int i = heapSize-1; (i>0)  && (minHeap[i] < minHeap[(i-1)/2] );i = (i-1)/2)
	{
		swap(&minHeap[i],&minHeap[(i-1)/2]);
	}	
}

// returns the minimum element of the heap i.e. the root
void findMin()
{
	if(heapSize <=0)// if the heap is empty ,so no element is returned
	{
		printf("heap underflow , no element present in heap\n");
		return;
	}
	else
		printf("Min element = %d\n",minHeap[0]);

}

// deletes the minimum element of the heap i.e. the root
//1st element is swapped with last element of heap and heapsize is decreased by one
void deleteMin()
{
	if(heapSize <=0)// if the heap is empty ,so no element to delete
	{
		printf("heap underflow , no element present in heap\n");
		return;
	}
	
	if(heapSize ==1)// if heapsize is one then heapsize is made zero
	{
		heapSize--;
		return;
	}
	minHeap[0] = minHeap[--heapSize]; // puts the last element as root and decreasing the heapsize by 1
	minHeapify(0);// min heap property is restored by this which might have been destroyed by the above swap		
}
void main()
{
	int choice, num;
	
	printf("Enter the max capacity of heap\n");
	scanf("%d",&maxSize);
	minHeap = (int*)malloc(sizeof(int)*maxSize);// dynamically allocating array of required size
	if(minHeap == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}

	while(1)
		{
			printf("Enter Choice\nMenu:\n1)Insert\n2)Find Min\n3)Delete Min\n4)exit\n\n");// prints the menu
			scanf("%d",&choice);
			switch(choice)
			{
				case 1 : {// in case of insertion
							printf("Enter the no. to be inserted in the min heap\n");
							scanf("%d",&num);
							insertMinHeap(num);
							printMinHeap();
							break;
						 }
				case 2 :{// in case of find min
							findMin();	
							break;
						}		 
				case 3 :{// in case of delete min
						    deleteMin();
						    printMinHeap();
						    break;
						}
				case 4 :{	// freeing all the dynamically allocated memory
							free(minHeap);
							return;		// exit
						}	
				default: printf("Wrong Input\n");		 
			}
		}

}