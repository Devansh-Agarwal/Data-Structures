#include <stdio.h>
#include <stdlib.h>

struct edgeNode// describing a struct which contains information of an edge
{
	int vertix1;
	int vertix2;
	int weight;
	
};

//declaring all the functions used in the program
//----------------------------------------------------------
void swap(struct edgeNode * , struct edgeNode *);
void minheapify(int, int);
void buildMinHeap();
void heapsort();
void graphInput();
int  find(int);
void makeUnion(int, int);
void kruskal();
//----------------------------------------------------------

// parent and rank are used in disjoint set implementation
int * parent,*rank, numVertix,numEdges;
struct edgeNode * graph, *mst;// graph stores initial input and mst stores final Min spanning tree

//swaps the value at two memory addresses 
void swap(struct edgeNode * a, struct edgeNode *b)
{
	struct edgeNode temp = *a;
	*a = *b;
	*b = temp;
}

// minHeapify() assumes  that the binary trees rooted at left and right child are min heaps
// it compares current value with its children and if they are smaller, the smallest one 
// is replaced by the current value and if a swap happens then the recursion descends
// to the location of the child which is swapped  
void minHeapify(int size, int index)
{
	
	int smallest = index, leftChild = (index*2) + 1, rightChild = (index*2) + 2;//calculating the left and right child location in array
	
	if((leftChild<size) && (graph[leftChild].weight < graph[index].weight))// checking if leftChild is smaller than index and if there exists a left child
		smallest = leftChild;
	if((rightChild<size) && (graph[rightChild].weight < graph[smallest].weight))// checking if rightChild is smaller than smallest and if there exists a right child
		smallest = rightChild;

	if(smallest != index)//if one of the children is smaller than index
	{
		swap(&graph[index],&graph[smallest]);// swap smaller child and index
		minHeapify(size,smallest);//applying minheapify on smaller childs location
	}

}

// buildMinHeap() coverts array into a minheap in bottom - up manner using minHeapify()
void buildMinHeap()
{
	for(int i = ((numEdges/2)-1); i>=0; i--)// traversing array in a bottom up manner from the 1st non-leaf node to root
		minHeapify(numEdges,i);// calling minHeapify() for each non-leaf node
}

//builds a min heap in the begining and then returns an array sorted in decending order
void heapsort()
{
	buildMinHeap();// builds a min heap

	// loop used for creating a decreasing order array from min heap
	for(int i =numEdges-1; i>=1; i--)// goes from (size -1)  to 1 as the last element would automatically be sorted 
	{
		swap(&graph[0], &graph[i]);// swaps the root of min heap with min heap's last entry
		minHeapify(i,0);//applied only on root as only root has been changed and size of heap is also decreased for further calls 
	}
}

// takes input of edges in an 1d array of struct edgeNode
void graphInput()
{
	printf("Enter the no. of vertices\n");
	scanf("%d",&numVertix);
	printf("Enter the number of edges\n");
	scanf("%d",&numEdges);
	
	if(numEdges< numVertix - 1)// invalid input case
	{
		printf("Number of edges cannot less than numVertix -1");
		exit(0);
	}
	graph = (struct edgeNode *)malloc(sizeof(struct edgeNode) *numEdges);//allocating a dynamic array for inputing the edges
	if(graph == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}
	
	//inputing all the edges one by one
	printf("Now enter the edges(take vertix name as 0,1,2...) and there weights one by one in format : \nsource 0 \ndestination 2 \nweight 5\n");
	for(int i =0; i< numEdges; i++)
	{
		printf("Enter source\n");
		scanf("%d",&graph[i].vertix1);
		printf("Enter destination\n"); 
		scanf("%d",&graph[i].vertix2);
		printf("Enter weight\n");
		scanf("%d",&graph[i].weight);
		if(graph[i].vertix1 == graph[i].vertix2)// source and destination are same
		{
			printf("Source and destination vertices are same\n");
			exit(0);
		}
	}
}

// finds recursivelly the parent(representative) of i
int find(int i)
{
	if(parent[i] != i)
		parent[i] = find(parent[i]);
	return parent[i];	
}

// checks whether two sets are same or not  if not they are combined
void makeUnion(int set1, int set2)
{
	int set1Rep = find(set1);// parent element is found
	int set2Rep = find(set2);//parent element is found
	
	if(rank[set2Rep]> rank[set1Rep])// the smaller ranking set is added to the bigger ranking one
	{
		parent[set1Rep] = parent[set2Rep];
	}
	else if(rank[set2Rep]<rank[set1Rep])
	{
		parent[set2Rep] = parent[set1Rep];
	}
	else
	{
		parent[set1Rep] = parent[set2Rep];// if the rank of both the sets is same then there would be an icrease in the rank
		rank[set2Rep]++;		
	}
}

// used for finding MST by using traversing tgrough a sorted array acc. to there weights
void kruskal()
{
	int traverser =0,v1Rep,v2Rep ;// traverser is used for going through the heap
	
	for(int edges = 0; edges < numVertix-1;)// the number of edges in MST will be 1 less than no. of vertices
	{
		v1Rep = find(graph[traverser].vertix1);// parent of set is found
		v2Rep = find(graph[traverser].vertix2);// parent of set is found
		

		if(v1Rep != v2Rep)// if there is no cycle i.e both the vertices of the edge to be added are already not in MST 
		{
			mst[edges++] = graph[traverser];// the edge is added to mst
			makeUnion(v1Rep,v2Rep);// the edge is added to the main set
		}
		traverser ++;
	}

}
void main()
{
	graphInput();// input of graph is taken
	
	parent = (int *)malloc(sizeof( int) *numVertix);// dynamically allocated to maintain the parents in disjoint set
	rank = (int *)malloc(sizeof( int) *numVertix);// dynamically allocated to maintain the rank in disjoint set
	
	if(parent == NULL || rank == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}
	
	for(int i = 0; i <numVertix; i++)// allocating the initial values
	{
		parent[i] = i;// all the elemets are parents of themselves in the begining
		rank[i] = 0;// inital rank is 0
	}
	
	heapsort();// sorts the array in descending order according to weight of edges
	
	for(int i= 0; i<=(((numEdges-1))/2) ; i++)// reverses the array hence we get an accending order array
	{
	  swap(&graph[i], &graph[numEdges-1-i]);//swapping the element from front and back in an symmetric manner
	}
	
	mst =  (struct edgeNode *)malloc(sizeof(struct edgeNode) *(numVertix-1));// allocating array for storing MST
	if(mst == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		return;
	}
	
	kruskal();// MST is calculated
	
	for(int i =0; i < numVertix-1; i++)// the MST is printed here
	{
		printf("%d---%d, weight = %d\n\n",mst[i].vertix1,mst[i].vertix2,mst[i].weight);
	}
	printf("\n");	
	
	//freeing all the dynamically allocated memory
	free(mst);
	free(graph);
	free(parent);
	free(rank);
}
