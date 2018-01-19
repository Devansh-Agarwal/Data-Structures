#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// contains necessary information to take input as an adjacency list
struct adjGraphNode
{
	int vertix;
	int weight;
	struct adjGraphNode * next;
};

//contains all the vertices in the graph and parent contains the other vertix
//which makes the edge that contains the minimum weight associated with vertix 
struct minHeapNode
{
	int vertix;
	int parent;
	int weight;
};

//declaring all the functions used in the program
//----------------------------------------------------------
void swapMinHeapNode(struct minHeapNode *,struct minHeapNode * );
void swap(int *, int *);
struct adjGraphNode * createNode();
void minheapify(int);
void extractMin();
void graphInput();
void prims();
//----------------------------------------------------------

 struct adjGraphNode ** graph;// array of pointers of struct adjGraphNode* type used for storing adjacency list
 
// numVertix contains total number of vertices,inHeap is used for checking whether  the vertix is already
 int numVertix,*inHeap,*posHeap;//  extracted or not,posHeap contains the location of particular vertix in minHeap
 
 
 int heapSize = 0;
 
//minHeap stores all the vertices and the final MST
 struct minHeapNode * minHeap, min;// min is used for storing the latest value from extractMin
  
// swaps the struct minHeapNode type by swapping the values at there original memory address
void swapMinHeapNode(struct minHeapNode * a, struct minHeapNode * b )
{
	struct minHeapNode temp = *b ;
	*b = *a;
	*a = temp;
}

// swaps the int type type by swapping the values at there original memory address
void swap(int* a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

//creates a adjGraphNode dynamically as per requirement
struct adjGraphNode * createNode()
{
	struct adjGraphNode * p = (struct adjGraphNode*)malloc(sizeof(struct adjGraphNode));//dynamically allocates memory
	
	if( p == NULL)// in case of memory overflow null pointer is given
	{
		printf("Memory Overflow\n");
		exit(0) ; 
	}
	return p;
}

//minHeapify() assumes  that the binary trees rooted at left and right child are min heaps
//minHeapify is performed with respect to weights associated with respected vertices
void minHeapify(int index)
{
	
	int smallest = index, leftChild = (index*2) + 1, rightChild = (index*2) + 2;//calculating the left and right child location in array
	
	if((leftChild<heapSize) && (minHeap[leftChild].weight < minHeap[index].weight))// checking if leftChild is smaller than index and if there exists a left child
		smallest = leftChild;
	if((rightChild<heapSize) && (minHeap[rightChild].weight < minHeap[smallest].weight))// checking if rightChild is smaller than smallest and if there exists a right child
		smallest = rightChild;

	if(smallest != index)//if one of the children is smaller than index
	{
		swap(&posHeap[minHeap[index].vertix],&posHeap[minHeap[smallest].vertix]);//also swapping the value in posHeap array
		swapMinHeapNode(&minHeap[index],&minHeap[smallest]);// swap smaller child and index
		minHeapify(smallest);//applying minheapify on smaller childs location
	}

}

// gives and removes the minimum value from minHeap based on weights associated with respected vertices
//1st element is swapped with last element of heap and heapsize is decreased by one
void extractMin()
{
	if(heapSize <=0)// if the heap is empty ,so no element to delete
	{
		printf("heap underflow , no element present in heap\n");
		return;
	}	
	
	min = minHeap[0];
	if(heapSize ==1)// if heapsize is one then heapsize is made zero
	{
		heapSize--;
		return;
	}
	
	//swapping both the members of minheap and respected posHeap member
	posHeap[minHeap[heapSize-1].vertix] = 0; 
	posHeap[minHeap[0].vertix] = heapSize-1;
	minHeap[0] = minHeap[--heapSize]; // puts the last element as root and decreasing the heapsize by 1
	minHeap[heapSize] = min;
	minHeapify(0);// calling minHeapify to fix the heap property if distorted
}

// used for taking adjacency list as input each new adjgraphNode is added in a fashion similar to linked list
// at the position of the respected vertix
void graphInput()
{
	// numEdges stores number of edges and source, desti, size are temorary variables used during scanning, p is used
	// storing pointer of new node 
	int numEdges, source, desti, size;
	struct adjGraphNode * p;

	printf("Enter the no. of vertices\n");
	scanf("%d",&numVertix);
	graph = (struct adjGraphNode **)malloc(sizeof(struct adjGraphNode *) *numVertix);// allocating memory for adjacency list

	if(graph == NULL)// in case there is no memory left
	{
		printf("Memory Overflow\n");
		exit(0);
	}
	
	for(int i =0; i< numVertix; i++)// initially putting all pointers as NULL
		graph[i] = NULL;

	// taking input of the edges and storing in adjacency list
	printf("Enter the number of edges\n");
	scanf("%d",&numEdges);

	if(numEdges< numVertix - 1)// invalid input case
	{
		printf("Number of edges cannot less than numVertix -1");
		exit(0);
	}
	printf("Now enter the edges(take vertix name as 0,1,2...) and there weights one by one in format : \nsource 0 \ndestination 2 \nweight 5\n");
	for(int i =0; i< numEdges; i++)
	{
		printf("Enter source\n");
		scanf("%d",&source);
		printf("Enter destination\n"); 
		scanf("%d",&desti);
		printf("Enter weight\n");
		scanf("%d",&size);
		p = createNode();
		if(source == desti)// source and destination are same
		{
			printf("Source and destination vertices are same\n");
			exit(0);
		}

		p-> next = graph[source];
		p-> weight = size;
		p-> vertix = desti;
		graph[source] = p;

		p = createNode();
		p-> next = graph[desti];
		p-> weight = size;
		p-> vertix = source;
		graph[desti] = p;

	}
}

// uses prims algorithm to find MST of the given graph
void prims()
{
	struct adjGraphNode * crawl;// used for going through all adjacent vertices of a given vertix
	
	// traverse through all adjacent vertices of the vertix given by extractMin and updates weight
	//associated with them if a weight lesser than the present weight is found
	while(heapSize>0)
	{
		extractMin();
		crawl = graph[min.vertix];
		inHeap[min.vertix] = 0;	// specifying that min.vertix is no more a part of minHeap
		
		//traversing the list associated with given vertix and updating weights appropriately
		while(crawl != NULL)
		{
			//checks whether new weight is lesser and if the vertix is in minHeap if true update the weight and position
			if(crawl->weight <minHeap[posHeap[crawl->vertix]].weight && inHeap[crawl->vertix] == 1)
			{
				//weight and parent are updated 
				minHeap[posHeap[crawl->vertix]].weight = crawl-> weight;
				minHeap[posHeap[crawl->vertix]].parent = min.vertix;
				
				//the heap property is restored by swapping with parents if the parent is greater than vertix
				for(int i = posHeap[crawl->vertix]; (i>0)  && (minHeap[i].weight < minHeap[(i-1)/2].weight );i = (i-1)/2 )
				{
					// both elements in posHeap and minHeap are swapped
					swap(&posHeap[minHeap[i].vertix],&posHeap[minHeap[(i-1)/2].vertix]);
					swapMinHeapNode(&minHeap[i],&minHeap[(i-1)/2]);
				}			
			}
			crawl = crawl->next;// crawl is updated it works in a simillar manner as in linked list
		}	
	}
}

void main()
{
	graphInput();// input of graph is taken as adjacency list
	heapSize = numVertix;
	
	// allocating memory for inHeap,posHeap and minHeap
	inHeap = (int *)malloc(sizeof( int) *numVertix);
	posHeap = (int *)malloc(sizeof( int) *numVertix);	
	minHeap = (struct minHeapNode *)malloc(sizeof(struct minHeapNode) * numVertix);

	if ( minHeap == NULL || inHeap == NULL ||posHeap == NULL)// if there is no memory  to allocate
	{
		printf("Memory Overflow\n");
		exit(0);
	}
	heapSize = numVertix;
	
	//allocating initial values to minHeap
	for(int i =0; i < numVertix; i++)
	{
		minHeap[i].vertix = i ;
		inHeap[i] = 1;// initally all the vertices will be in minHeap
		
		// putting all weights as INT_MAX except 1st one
		if(i == 0)
			minHeap[0].weight = 0;
		else	
			minHeap[i].weight = INT_MAX; 
		minHeap[i].parent = -1;
		posHeap[i] = i;// initial postion is same as array index
	}	
	
	prims();// calling prims
	
	//printing the MST
	for(int i = 0; i < numVertix ; i++)
	{
		if(minHeap[i].parent != -1)	
			printf("%d---%d; weight::%d\n",minHeap[i].vertix,minHeap[i].parent,minHeap[i].weight);
	}

	// freeing all the dynamically allocated memory
	free(minHeap);
	free(inHeap);
	free(posHeap);
	for(int j = 0;j< numVertix ;j++)// freeing the adjacency list
	{
		struct adjGraphNode * i = graph[j];
		while(i != NULL)
		{
			struct adjGraphNode * p = i;
			i = i->next;
			free(p);
		}
		graph[j] = NULL;
	}
}