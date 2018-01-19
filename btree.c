#include <stdio.h>
#include <stdlib.h>

struct btNode// struct for defining each node of a b tree which contains 4 attributes
{
	int numKeys, isLeaf, *keys;
	struct btNode ** child;

};

// t is half of branching factor
struct btNode * root = NULL,*foundIn =NULL;// foundIn is the pointer to the node which contains the element which is searched
int t, foundAt;// foundAt contains the key no. which contains the requested search key

//declaring all the functions used in the program
//----------------------------------------------------------
void deleteKey(struct btNode *, int);
struct btNode * createNode();
void createTree();
void splitChild(struct btNode *, int);
void insertNonFull(struct btNode *, int);
void insert(int);
void printTree(struct btNode *);
void search(struct btNode *, int);
void takeFromNext(struct btNode *, int);
void takeFromPrevious(struct btNode *, int);
void merge(struct btNode *, int);
void deleteNonLeaf(struct btNode *, int);
//----------------------------------------------------------

// allocates memory for a new node to be inserted in the b tree
struct btNode * createNode()
{
	struct btNode * ptr = (struct btNode *) malloc(sizeof(struct btNode));
	if(ptr == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		exit(0);
	}
	
	ptr -> keys = (int*)malloc(sizeof(int)*(2*t-1));//allocating array for keys stored
	ptr-> child = (struct btNode**)malloc(sizeof(struct btNode *)*2*t);//allocating array for pointers of children stored
	if(ptr->keys == NULL || ptr->child == NULL)// if there is no memory to spare for array
	{
		printf("Memory Overflow\n");
		exit(0);
	}
	return ptr;
}

// creates the b tree by making the root point to the 1st node
void createTree()
{
		root = createNode();
		root -> isLeaf = 1;// if there is only one node then it would be a leaf itself
		root->numKeys = 0;
}

// takes in a nonful internal node parent and index of a child which is full
// a new node is created which keeps one part of the node 
// the child is split about its median key in two parts and the median is 
// adjusted in the parent
void splitChild(struct btNode * parent, int index)
{		
	struct btNode * righHalf = createNode();
	struct btNode * leftHalf = parent->child[index];
	
	righHalf->numKeys = t-1;
	righHalf->isLeaf = leftHalf -> isLeaf;// if the original child is a leaf then the new one will also be one

	//the largest t-1 keys of the original child are copied into rightHalf
	for(int i = 0; i< t-1;i++)
	{
		righHalf->keys[i] = leftHalf-> keys[t+i];
	}

	// if original child is not a leaf then it will have children so the right most t children(corresponding)
	// to the  largest t-1 keys are copied into the rightHalf	
	if(leftHalf->isLeaf == 0)
	{
		for(int i = 0; i< t; i++)
		{
			righHalf->child[i] = leftHalf->child[i+t];
		}
	}

	leftHalf->numKeys = t-1;// both right and left half will have t-1 keys
	
	// all the children of the parent  are shifted by one starting from index +1 
	//  so index +1 can be made free for rightHalf
	for(int i = parent->numKeys; i>= index +1; i--)
	{
		parent->child[i+1] = parent->child[i];
	}
	parent->child[index+1] = righHalf;

	// all the keys of the parent  are shifted by one starting from index 
	//  so index can be made free for the median of the original child
	for(int i =parent->numKeys-1; i>= index;i --  )
	{
		parent->keys[i+1] = parent->keys[i];
	}
	parent->keys[index] = leftHalf->keys[t-1];// storing median in parent
	parent->numKeys = parent->numKeys +1;//increasing the numKeys by one in parent

}

//inserts the key in a leaf node or decends in a recursive manner to an appropriate leaf node in which
//isertion can be done while spliiting full nodes in the way
void insertNonFull(struct btNode* nodeInsert, int num)
{
	int i =nodeInsert->numKeys-1;
	
	// if the current node is leaf then the key is inerted in the appropriate position
	if(nodeInsert->isLeaf == 1)
	{
		//shifting the keys which are greater than num by 1 to there right
		while(i>=0 && num < nodeInsert->keys[i])
		{
			nodeInsert->keys[i+1] = nodeInsert->keys[i--];
		}
		nodeInsert->keys[i+1] = num;// inserting the key so the non decreasing order property is maintained
		nodeInsert->numKeys++;// number of keys increase by 1
	}
	else
	{
		// finding the appropriate child node to descend 
		while(i>=0 && num < nodeInsert->keys[i])
			i--;
		i++;

		// if the child is full we split it and then find which of the 2 children 
		// we need to descend 
		if(nodeInsert->child[i]->numKeys == 2*t-1)
		{
			splitChild(nodeInsert,i);
			if(num >nodeInsert->keys[i])// if it num is bigger then we go descend to the right child
				i++;
		}
		insertNonFull(nodeInsert->child[i], num);// descending to the appropriate child 
	}
}	

// inserts a new key in the btree by taking two cases 1) the root is not full 
//2) the root is full in which case a new node is created and the root is split
// thereby increasing the tree height
void insert(int num)
{
	struct btNode * temp = root;
	
	if(temp->numKeys == (2*t-1))// if the root is full
	{
		struct btNode * newNode = createNode();
		root =  newNode;// new root allocated
		newNode->isLeaf = 0;//wont be a leaf
		newNode->numKeys = 0;
		newNode->child[0] = temp;//setting the old root as the 0 th child of new root
		splitChild(newNode, 0);
		insertNonFull(newNode, num);
	}
	else
		insertNonFull(root, num);
}

//prints the btree in non decreasing order in a recusive manner
void printTree(struct btNode * node)
{
	int i = 0;
	while(i< node->numKeys)// goes through all the keys of the current node
	{
		if( node -> isLeaf == 0)
		{
			printTree(node->child[i]);// goes to the child of the current index node
		}
		printf("%d ",node->keys[i++] );
	}

	// using the above loop only the 1st n childs of the node are accessed so the last
	// child is accessed below
	if(node-> isLeaf == 0)
		printTree(node->child[i]);
}

// searches for a particular key in  the btree by 1st checking if the key exists in the
// current node otherwise descending into the appropriate chil
void search(struct btNode * searchIn, int searchFor)
{
	// this would give index of the key which is equal to or the least biggest key in the
	//the current node with respect searchFor
	int i = 0;
	for(i =0;i<searchIn->numKeys && searchFor>searchIn->keys[i]; i++);

	if(searchIn->keys[i] == searchFor )// searchFor is found
	{	
		foundAt = i;
		foundIn =searchIn;
		return ;
	}
	else if(searchIn->isLeaf == 1)// it is not in the current node and there are no children also so searchFor Not found
		{
			foundIn = NULL;
			return ;
		}
	else
		search(searchIn->child[i],searchFor);// descending to the appropriate child
}

// takes a key from the right sibling(index +1) of the child at index and adds it to the parent at index
// and parents orignal index key goes to child at index
void takeFromNext(struct btNode * node,int index)
{
	struct btNode * child1 = node->child[index], * child2 = node-> child[index+1];
	child1->keys[child1->numKeys]= node->keys[index];// putting parents key in child
	node->keys[index] = child2->keys[0];//putting child at index +1's 0 th key in parent  
	
	// if child1 is not leaf we need to add 1 child of child2 to child1
	if(child1->isLeaf == 0)
		child1->child[child1->numKeys+1] = child2->child[0];

	// if child2 is not a leaf shifting all its children and keys to the left by one
	// otherwise only shifting the keys
	if(child2->isLeaf ==0)
	{
		for(int i =1; i<child2->numKeys; i++)
			child2->keys[i-1] = child2->keys[i];
		for(int i =1; i<=child2->numKeys; i++)
			child2->child[i-1] = child2->child[i];	
	}
	else
		for(int i =1; i<child2->numKeys; i++)
			child2->keys[i-1] = child2->keys[i];
	
	child1->numKeys ++ ;
	child2->numKeys-- ;		
}

// takes a key from the left sibling(index -1) of the child at index and adds it to the parent at index-1
// and parents orignal index-1 key goes to child at index
void takeFromPrevious(struct btNode * node,int index)
{
	struct btNode * child1 = node->child[index], * child2 = node-> child[index-1];
	
	// if child1 is not a leaf shifting all its children and keys to the right by one
	// otherwise only shifting the keys
	if(child1->isLeaf == 0)
	{
		for(int i =child1->numKeys-1; 0<=i; i--)
			child1->keys[i+1] = child1->keys[i];
		for(int i =child1->numKeys; 0<=i; i--)
			child1->child[i+1] = child1->child[i];	
	}
	else	
		for(int i =child1->numKeys-1;0<=i; i--)
			child1->keys[i+1] = child1->keys[i];
	
	child1->keys[0] = node->keys[index-1];// putting parents key in child
	if(node->isLeaf ==0)// transfering child2's rightmost child if it exists
		child1->child[0] =child2->child[child2->numKeys];

	node->keys[index-1] = child2 ->keys[child2->numKeys-1];//putting child at index -1's last key in parent
	child1->numKeys ++ ;
	child2->numKeys-- ;			
}

// this merges child indexed at index and index +1 by shifting the key in node at index to child1 
// and freeing child2
void merge(struct btNode * node, int index)
{
	struct btNode * child1 = node->child[index], * child2 = node-> child[index+1];
	child1->keys[t-1] = node->keys[index];//putting the nodes key into child1
	
	// if child1 is not leaf then copying all the keys and children of child2 in child1 starting from 
	// location t otherwise only copying the keys
	if(child1->isLeaf == 0)
	{
		for(int i =0; i<child2->numKeys; i++)
			child1->keys[i+t] = child2->keys[i];
		for(int i =0; i<=child2->numKeys; i++)	
			child1->child[i+t] = child2->child[i];
	}
	else
		for(int i =0; i<child2->numKeys; i++)
			child1->keys[i+t] = child2->keys[i];

	//shifting all the keys and children of node to the left by one
	// thereby deleting 1 key and 1 child
	for(int i = index +2; i<=node->numKeys;i++)
		node->child[i-1] = node->child[i];
	for(int i =index +1; i<node->numKeys;i++)
		node->keys[i-1] = node->keys[i];

	child1->numKeys += child2->numKeys+1;
	node->numKeys -- ;
	free(child2);
}

//deletes a key from a non leaf node indexed at index
void deleteNonLeaf(struct btNode * node,int index)
{
	int num = node->keys[index];
	struct btNode * temp;
	
	// if the child at index has more than t-1 keys, predecessor of num is found and num is 
	// replaced by predecessor and the predecessor left in child is recursively deleted
	// predecessor is found by going to the child at index and then going to rightmost child
	// until leaf child is reached, the right most key of that child will be predecessor
	if(node->child[index]->numKeys >= t)
	{
		for(temp =node->child[index];temp->isLeaf == 0;temp = temp ->child[temp->numKeys]);
		int predecesor = temp->keys[temp->numKeys-1];
		node->keys[index] =predecesor;
		deleteKey(node->child[index],predecesor);
	}
	
	// if the child at index +1 has more than t-1 keys ,successor of num is found and num is 
	// replaced by successor and the successor left in child is recursively deleted
	// successor is found by going to the child at index +1 and then going to leftmost child
	// until leaf child is reached, the left most key of that child will be successor
	else if(node->child[index+1]->numKeys >= t)
	{
		for(temp = node->child[index+1];temp->isLeaf == 0;temp = temp->child[0]);
		int successor = temp->keys[0];	
		node->keys[index] = successor;
		deleteKey(node->child[index+1],successor);
	}
	
	// otherwise merge the children at index and index +1 and now child at index will have the key
	// to be deleted
	else
	{
		merge(node,index);
		deleteKey(node->child[index],num);
	}
}

//checks if the current node contains the key or not if it contains then
//it does a leaf kind or non leaf kind deletion accordingly, if the current 
// node doesnt contains the key it goes to the appropriate child 
void deleteKey(struct btNode * node, int num)
{
	// this would give index of the key which is equal to or the least biggest key in the
	//the current node with respect searchFor
	int index;
	for(index = 0; node->keys[index]<num && index < node->numKeys ; index++);//?

	// in case key is found i
	if(index<node->numKeys && node->keys[index] == num)
	{
		if(node->isLeaf == 0)// if not a leaf
			deleteNonLeaf(node, index);
		else if(node->isLeaf == 1)// if it is a leaf
			{
				// all the keys greater than num are shifted to there left by one, thereby removing num from node
				int i = index;
				while(i < node->numKeys-1) 
					node->keys[i] = node->keys[(i++)+1];

				node->numKeys--;//total keys decrease by one			
			}
	}
	else
	{
		if(node->isLeaf == 1)//the node is a leaf and num is not present in the node
			printf("The given key is not present in the tree\n");
		else
		{
			int temp1 = node->numKeys, temp2 = index;
			
			// checking if the child we need to descend has less than t keys i.e t-1 we
			// increase the number of keys such that they are >= t
			if(node->child[index]->numKeys< t)
			{
				//if the child we need to descend is not the rightmost child and the next child has
				// more than t keys we take a number from the next child
				if(index != node->numKeys && node->child[index+1]->numKeys >=t)
				{
					takeFromNext(node,index);
				}
				//if the child we need to descend is not the leftmost child and the previous child has
				// more than t keys we take a number from the next child
				else if(index != 0 && node->child[index-1]->numKeys >=t)
				{
					takeFromPrevious(node, index);
				}				
				// otherwise merge merge with its next sibling if its not the rightmost child
				//if rightmost merge with previous sibling
				else
				{
					if(index!= node->numKeys)
						merge(node,index);
					else
						merge(node,index-1);
				}
			}
			
			//if rightMost child was merged we have to descend to index-1
			// otherwise we descend to index
			if(temp1 ==temp2 &&index >node->numKeys )
				deleteKey(node->child[index-1],num);
			else
				deleteKey(node->child[index],num);
		}
	}	

	// when the root node has 0 keys and it is not a leaf make  new root as roots 1st child
	// if it is leaf free the root
	if(root->numKeys == 0)
	{
		struct btNode * temp = root;
		if(root->isLeaf == 1)
		{
			root = NULL;
		}
		else
			root = root->child[0];
		free(temp);
	}
}

//contains the menu other basic stuff
void main()
{
	int bFactor, choice, key;
	printf("Enter the branching factor\n");
	scanf("%d",&bFactor);
	t = bFactor/2;	

	createTree();// creating an empty tree intitially

	while(1)
	{
		printf("Enter Choice\nMenu:\n1)Insert\n2)Search\n3)delete\n4)exit\n\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 : {
						printf("Enter the key\n");
						scanf("%d",&key);
						if(root == NULL)// if the tree hass been deleted
							createTree();
						insert(key);
						printTree(root);
						printf("\n");
						break;
					 }
			case 2 : {
						printf("Enter key\n");
						scanf("%d",&key);
						if(root == NULL)
						{
							printf("The b tree is empty\n");
							break;
						}
						search(root,key);
					 	if(foundIn == NULL)
					 		printf("Not present in tree\n");
					 	else
					 		printf("Found :: %d\n",foundIn->keys[foundAt]);
						break;
						
					 }
			case 3 :{
						printf("Enter the key\n");
						scanf("%d",&key);
						if(root == NULL)
						{
							printf("The b tree is empty\n");
							break;
						}
						deleteKey(root, key);
						if(root != NULL)
							printTree(root);
						printf("\n");
						break;
					}
			case 4 :{
						// freeing all the dynamically allocated memory by calling delete
						// at the zeroth key of the b tree till it becomes empty
						while(root!= NULL)
							deleteKey(root,root->keys[0]);
						
						return;		 		 
					}	
		 
			default: printf("Wrong Input\n");		 
		}
	}
}