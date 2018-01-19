#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct bstNode {
	struct bstNode * child[2];//0 is left 1 is right
	int name ;
	char gender[7];
	char schoolName[50];
};

int found ;
struct bstNode * foundPtr;

//declaring all the functions used in the program
//----------------------------------------------------------
struct bstNode * createBstNode();
void printInorder( struct bstNode *);
struct bstNode * insertBstNode(struct bstNode *, struct bstNode *);
void search(struct bstNode *, int);
struct bstNode * delete( struct bstNode *, int);
//---------------------------------------------------------- 

// creates a new node which can be added in the bst and assigns value to the node
struct bstNode * createBstNode()
{
	struct bstNode * p = (struct bstNode*)malloc(sizeof(struct bstNode));//dynamically allocates memory
	
	if( p == NULL)// in case of memory overflow null pointer is given
	{
		printf("Memory Overflow\n");
		return p ; 
	}
	
	//assigning the inital values
	printf("Enter Name\n");
	scanf("%d",&p->name);
	printf("Enter gender\n");
	scanf("%s",p->gender);
	printf("Enter School name\n");
	scanf("%s",p->schoolName);
	printf("\n");
	p->child[0] = NULL;// putting left pointer as null
	p->child[1] = NULL;// putting right pointer as null
	return p; 
}

void printInorder(struct bstNode* root)// prints the tree in an inorder(non decreasing order) fashion using recursion
{
	if(root == NULL)// when the end of tree is reached
		return;
	printInorder(root -> child[0]);
	printf("Name: %d\nSchool Name: %s\nGender: %s\n",root->name,root->schoolName,root->gender);
	printInorder(root -> child[1]);
	
}

struct bstNode * insertBstNode(struct bstNode* root,struct bstNode* ptr)//inserts a node in the tree in a recursive manner
{
	if(root == NULL)// when the end of tree is reached
		return ptr;
		
	else if((root->name) >= (ptr->name) )// if the given name is lesser than or equal to root then it will be added in the left subtree
		root ->child[0] = insertBstNode(root->child[0],ptr);
	
	else if((root->name) < (ptr->name) )// if the given name is greater than root then it will be added in the right subtree
		root ->child[1] =	insertBstNode(root->child[1],ptr);
	
	return root;

}

void search(struct bstNode* root,int num)// searches for a given name using recursion
{
	if(root == NULL)// the root does not exist as tree end has been reached
	{	
		found = 0;
		return;
	}
	
	if(root ->name == num)// if the root is found
	{
		foundPtr = root;
		found = 1;
		return;
	}
	
	if(root->name < num)// goes to the right sub tree if given key is larger
		search(root->child[1],num);
	if(root->name >num)// goes to the left sub tree if the given key is lesser than or equal
		search(root->child[0],num);
}
struct bstNode * delete(struct bstNode* root, int num)// deletes a particular key from the tree
{
	// 1st three conditional statements are used for finding the root
	if(root ==	NULL)
	{
		printf("The given name is not in the tree\n");
		return root;
	}
	else if(root ->name > num)
		root ->child[0] = delete(root->child[0], num);
	else if(root -> name <num)
		root->child[1] = delete(root -> child[1], num);
	else// if root is found, deletion is implemented using three cases
	{

		if(root-> child[0] == NULL && root -> child[1] == NULL)// if  the node to be deleted has no child
		{
			free(root);// removes the node
			root = NULL;
		}// if the node to be deleted has 1 child
		else if( root -> child[1] == NULL)// no right child
		{
			struct bstNode * temp = root;
			root = root ->child[0];// skipping the node to be deleted
			free(temp);// removes the node
		}
		else if(root ->child[0] == NULL)// no left child
		{
			struct bstNode * temp = root;
			root = temp ->child[1];// skipping the node to be deleted
			free(temp);	// removes the node
		}
		else// if both the children are there
		{
			// finding the max number in the left sub tree
			struct bstNode * temp = root -> child[0];
			while(temp-> child[1] != NULL )
				temp = temp -> child[1];
			//copying the found node in the node to be deleted
			root->name	= temp-> name;
			strcpy(root->gender, temp -> gender);
			strcpy(root->schoolName, temp -> schoolName);
			// reccursively deleting the found node from its subtree
			root->child[0] = delete(root->child[0], temp-> name);
		}
	}
	return root;
}

// frees all the dynamically allocated memory i.e. deletes the entire tree
// it performs a preorder traverser and frees memory while doing it
void deleteAll(struct bstNode * traverser)
{
	if(traverser == NULL)
		return;
	//first the subtrees are deleted and then the root of those subtrees
	deleteAll(traverser->child[0]);
	deleteAll(traverser->child[1]);
	free(traverser);
}
void main()
{
	int choice, num;
	struct bstNode *ptr;
	struct bstNode *root = NULL;

	while(1)
	{
		printf("Enter Choice\nMenu:\n1)Insert\n2)Search\n3)Delete\n4)exit\n\n");// prints menu
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 : {

						ptr = createBstNode();
						if( ptr ==NULL )// checks for memory overflow
							break;
						root = insertBstNode(root,ptr);
						printInorder(root);
						break;
					 }
			case 2 :{
						printf("Enter name\n");
						scanf("%d",&num);
						search(root,num);
						if(found == 0)
							printf("The given name is not in the tree\n");
						if(found == 1)
							printf("Name: %d\nSchool Name: %s\nGender: %s\n",foundPtr->name,foundPtr->schoolName,foundPtr->gender);	
						break;
					}		 
			case 3 :{
					    printf("Enter name to be deleted\n");
					    scanf("%d",&num);
					    root =  delete(root, num);
					    printInorder(root);
					    break;
					}
			case 4 :{
			 			deleteAll(root);// all the memory is freed
			 			return;
		 			}		
			default: printf("Wrong Input\n");		 
		}
	}
}



















































































































































































































































































































































































































































































































