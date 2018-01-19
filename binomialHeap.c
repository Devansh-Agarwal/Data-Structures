#include <stdio.h>
#include <stdlib.h>

// struct for defining each node of a binomial heap which contains 5 attributes
struct bhNode
{
	int data, degree;
	struct bhNode * parent , *rightSibling, * leftChild;
};

struct bhNode * heap = NULL;// declaring root of heap as global variable and initialising as NULL

//declaring all the functions used in the program
//----------------------------------------------------------
struct bhNode * createNode(int);
struct bhNode * findMin();
struct bhNode * mergeHeap(struct bhNode *, struct bhNode *);
void linker(struct bhNode *, struct bhNode *);
struct bhNode * bhUnion(struct bhNode *,struct bhNode *);
void bhInsert(int);
void printHeap(struct bhNode *);
void deleteMin();
//----------------------------------------------------------

//creates an empty node to be added in the heap and initialising the initial values
// and all the pointers in it as NULL
struct bhNode * createNode(int key)
{
	struct bhNode * ptr = (struct bhNode *)malloc(sizeof(struct bhNode));
	if(ptr == NULL)// if there is no memory to spare for node
	{
		printf("Memory Overflow\n");
		exit(0);
	}
	ptr->data = key ;
	ptr->degree = 0;
	ptr->parent = NULL;
	ptr->rightSibling = NULL;
	ptr->leftChild = NULL;
	return ptr;
}

// this functions returns the minimum value int the entire heap, it works by traversing through
// all the roots of the individual heaps that form the binomial heap in a linked list like fashion
struct bhNode * findMin()
{
	struct bhNode * foundIn = heap, *traverser = heap;
	int min;

	// if heap is empty then return NULL
	if(heap == NULL)
		return NULL;
	
	min = heap->data;// initialising the 1st nodes value as min
	
	// finds the node with minimum value
	while(traverser != NULL)
	{
		if(min > traverser->data)//this block updates min and foundIn if a smaller than min key is found
		{	
			min = traverser->data;
			foundIn = traverser;
		}	
		traverser = traverser->rightSibling;//going to the next node
	}
	
	return foundIn;
}

// this merges two heaps and the new heap contains all the heaps in a accending order according
// to there degrees, it is similar to merging two sorted linked lists
// if degree of heap2 is lesser than degree of heap1 at any node(while traversing heap1)
// that particular node of heap1(and the rest of the heap) is replaced by heap2, and the replaced
// part is renamed as heap2
struct bhNode * mergeHeap(struct bhNode * heap1,struct bhNode * heap2)
{
	// tempHeap contains the root of the final node
	struct bhNode * tempHeap = NULL, *swap;

	// if anyone heap is null or both are null that case is handeled here
	if(heap1 == NULL)
		return heap2;
	else if(heap2 == NULL)
		return heap1;

	// making heap1 as the heap which has the 1st node as smaller degree and storing that in tempHeap
	if(heap1->degree > heap2->degree)
	{
		tempHeap = heap2;
		heap2 = heap1;
		heap1 = tempHeap;
	}
	else
		tempHeap = heap1;
	
	// both the heaps are merged here in a proper manner
	while(heap1->rightSibling != NULL)
	{
		if(heap1 ->rightSibling->degree > heap2->degree)
		{
			swap = heap2;
			heap2 = heap1->rightSibling;
			heap1->rightSibling = swap;		
		}
		heap1 = heap1->rightSibling;
	}
	heap1->rightSibling = heap2;// adds the rest of heap2 in heap1 if some part is left of it

	return tempHeap;
}

// links two heaps of the same degree  by making the parent of toBeLinked point to linkedTo 
// and increasing the degree of linkedTo by one
void linker(struct bhNode * toBeLinked, struct bhNode * linkedTo)
{
	toBeLinked-> parent = linkedTo;
	toBeLinked-> rightSibling = linkedTo->leftChild;
	linkedTo->leftChild = toBeLinked;
	linkedTo->degree++;
}

// joins two binomial heaps into one by 1st merging the two binomial heaps and then joining
//the heaps in the main heap which have the same degree two at a time   
struct bhNode * bhUnion(struct bhNode * heap1, struct bhNode * heap2)
{
	struct bhNode * tempHeap = mergeHeap(heap1, heap2);// heap1 & heap2 are merged here
	if(tempHeap == NULL)// this happens if both heap1 & heap2 are empty
		return tempHeap;

	struct bhNode * previous = NULL, *present = tempHeap, *next = present->rightSibling;
	
	//same degree heaps are joined two make a degree +1 heap here, two at a time
	while(next != NULL)
	{
		//condition before || checks if two consecutive	degrees are not same and second one checks whether 3 consecutive 
		//heaps have same degree in which case last 2 heaps are merged
		if((present->degree != next->degree)||(next->rightSibling != NULL && next->rightSibling->degree == present->degree) )
		{
			previous = present;
			present = next;
		}
		else 
		{
			if(present->data <= next->data)// if true then next is joined to present and presents degree is increased 
			{
				present->rightSibling = next->rightSibling;
				linker(next, present);// makes next a part of present
			}
			else
			{
				if(previous == NULL) // in case of 1ste individual heap in binomila heap
					tempHeap = next;
				else
					previous->rightSibling = next;
				linker(present, next);// makes present a part of next
				present = next;		
			}
		}
		next = present-> rightSibling;
	} 
	return tempHeap;

}

// inserts a new key in the binomial heap by creating a new heap which contains only the new key
// and calling the bhUnion function on the new heap and already existing heap
// which combines the two heaps
void bhInsert(int key)
{
	struct bhNode * newHeap = createNode(key);
	heap = bhUnion(heap,newHeap);
}

// this functions prints the entire binomial heap in a recursive manner by calling the leftChild and
// then printing the child and all the node which can be reached using rightSibling 
void printHeap(struct bhNode * hp)
{
	if(heap == NULL )// if the binomial heap is empty
		{
			printf("The heap is empty");
			return;
		}
	
	while(hp != NULL)
	{
		printf("%d ",hp-> data );
		printHeap(hp->leftChild);// recursive call
		hp= hp->rightSibling;
	}
}

//deletes the minimum key in the binomial Heap
// 1st the minimum node with minimum key is found then it is removed from the root list of binomial heap
// allong with its respective heap nodes, then its childrens list is reversed and the right most child is
// made the new head and it is joined with the original heap without the min key in root list
void deleteMin()
{
	struct bhNode * foundIn = heap, *previous = NULL, *traverser = heap,*hNoMin = heap,*hMin = NULL;
	int min;
	
	if(heap == NULL)// in case the heap is empty so nothing to delete
		return;
	
	//minimum key, its node and its previous node is found in a manner similar to 
	//defined in findMin()
	min = heap ->data;
	while(traverser->rightSibling != NULL)
	{
		if(min>traverser->rightSibling->data)
		{
			foundIn = traverser->rightSibling;
			previous = traverser;
			min = foundIn->data;
		}
		traverser = traverser ->rightSibling;
	}

	// the node contiaing min is removed from root list of binomial heap here
	if(previous == NULL && foundIn->rightSibling ==NULL)
			hNoMin = NULL;
	else if(previous == NULL)
			hNoMin = foundIn->rightSibling;
	else
		previous->rightSibling = foundIn->rightSibling;
	
	// the children list is reversed here by making the traversers's rightChild as previous
	// and making new head as the initial right most child
	if(foundIn->leftChild != NULL)
	{
		struct bhNode * next;
		traverser = foundIn->leftChild;
		previous = NULL;
		while(traverser != NULL)
		{
			next = traverser ->rightSibling;
			traverser->rightSibling = previous;
			previous = traverser;
			traverser = next;
		}
		hMin = previous;
		foundIn->leftChild = NULL;	
	}

	free(foundIn);//the node containing the min value is deleted
	heap =	bhUnion(hNoMin,hMin);//both the heaps are joined here
}

void main()
{
	int choice, num;
	struct bhNode * foundIn;
	while(1)
		{
			printf("Enter Choice\nMenu:\n1)Insert\n2)Find Min\n3)Delete Min\n4)exit\n\n");// prints the menu
			scanf("%d",&choice);
			switch(choice)
			{
				case 1 : {// in case of insertion
							printf("Enter the no. to be inserted in the binomial heap\n");
							scanf("%d",&num);
							bhInsert(num);
							printHeap(heap);
							printf("\n");
							break;
						 }
				case 2 :{// in case of find min
							foundIn = findMin();	
							if(foundIn == NULL)//the heap is empty
								printf("The Heap is empty.\n");
							else
								printf("The minimum element is %d\n",foundIn->data);
							break;
						}		 
				case 3 :{// in case of delete min
						    deleteMin();
						    printHeap(heap);
						    printf("\n");
						    break;
						}
				case 4 :{	// freeing all the dynamically allocated memory
							while(heap != NULL)
								{
									deleteMin();
								}	
							return;		// exit
						}	
				default: printf("Wrong Input\n");		 
			}
		}

}


