#include <stdio.h>
#include <stdlib.h>

//declaring all the functions used in the program
//----------------------------------------------------------
int find(int);
void makeUnion(int, int);
//----------------------------------------------------------

int * rank, * parent;// arrays which store the rank and parent info of the set

// finds the representative of a given element in a recursive manner
// and also applies path compression by removing chains by making elements point
// to there representative directly 
int find(int i)
{
	if(parent[i] != i)// goes on till an element pointing to itself is found
		parent[i] = find(parent[i]);	
	return parent[i];	
}

// checks whether two sets are same or not  if not they are combined
void makeUnion(int set1, int set2)
{
	int set1Rep = find(set1);// parent element is found
	int set2Rep = find(set2);//parent element is found
	
	if(set1Rep != set2Rep)// if they are not already in the same set
	{	
		if(rank[set2Rep]> rank[set1Rep])// the smaller ranking set is added to the bigger ranking one
		{
			parent[set1Rep] = parent[set2Rep];
			printf("The new parent of both of them is %d\n",(parent[set2Rep] + 1));
		}
		else if(rank[set2Rep]<rank[set1Rep])
		{
			parent[set2Rep] = parent[set1Rep];
			printf("The new parent of both of them is %d\n",(parent[set1Rep] + 1));
		}
		else
		{
			parent[set1Rep] = parent[set2Rep];// if the rank of both the sets is same then there would be an icrease in the rank
			rank[set2Rep]++;		
			printf("The new parent of both of them is %d\n",(parent[set2Rep] + 1));
		}
	}
	else
		printf("The parent of both of them is %d\n",(parent[set2Rep] + 1));
}

void main()
{
	int numElements, choice, num1, num2;
	printf("Enter the number of elements,so the elements on which operations are done will be 1,2,3...number of elements\n");
	scanf("%d",&numElements);
	
	// dynamically allocating memory of required size
	rank = (int*) malloc(sizeof(int)*numElements);
	parent = (int*) malloc(sizeof(int)*numElements);

	if(parent == NULL || rank == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}

	//intitially putting rank as 0 and making each elements parent as itself
	for(int i = 0; i < numElements; i ++)
	{
		rank[i] = 0;
		parent[i] = i;
	}

	while(1)
		{
			printf("Enter Choice\nMenu:\n1)Find\n2)Union \n3)exit\n\n");// prints the menu
			scanf("%d",&choice);
			switch(choice)
			{
				case 1 : {
							printf("Enter the no. to find parent\n");
							scanf("%d",&num1);
							if(num1> numElements || num1 <1)// the no. is not in range
							{
								printf("The number is not present in the list\n");
								break;
							}
							num1 = find(num1 -1);
							printf("The parent is %d\n",(num1 + 1) );
							break;
						 }
				case 2 :{	
							printf("Enter the number which you want to union\n"	 );
							scanf("%d",&num1);
							scanf("%d",&num2);
							if(num1> numElements || num1 <1 ||num2 > numElements || num2 <1) // the no. is not in range
							{
								printf("The number is not present in the list\n");
								break;
							}
							makeUnion(num1 -1,num2 -1);// numbers are mapped from 1,2,3... to 0,1,2...
							break;
						}		 
				case 3 : {	// freeing all the dynamically allocated memory
							free(parent);
							free(rank);
							return;
						  }		// exit
				default: printf("Wrong Input\n");		 
			}
		}
}
