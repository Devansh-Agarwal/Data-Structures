#include <stdio.h>
#include <stdlib.h>
struct rbNode//declaring rb Node
{
	struct rbNode* parent, *left, *right; 
	int key,red;// 1 is for red , 0 is black
};

//declaring all the functions used in the program
//----------------------------------------------------------
struct rbNode * createRbNode(int);
void search(struct rbNode *,int);
void leftRotate(struct rbNode *);
void rightRotate(struct rbNode *);
void rbInsert(struct rbNode *);
void rbInsertBst(struct rbNode *);
void printInorder(struct rbNode *);
void deleteAll(struct rbNode *);
//----------------------------------------------------------
// root is the start of tree
//nil is used as sentinal node
struct rbNode * nil, *foundPtr, *root;//found and foundPtr are used in search function
int found ;

struct rbNode * createRbNode(int key)
{
	struct rbNode * ptr = (struct rbNode*)malloc(sizeof(struct rbNode));//dynamically allots memory for node
	
	if( ptr == NULL) // in case of memory overflow null pointer is given
	{
		printf("Memory Overflow");
		return ptr ; 
	}
	//inserting the inital values
	ptr->key = key;
	ptr->parent = nil;
	ptr->left = nil;
	ptr->right = nil;
	ptr->red = 1;//initially setting node as red as only red nodes are added
	return ptr;
}

void search(struct rbNode* root,int num)//recursively searching the given node
{
	if(root == nil)
	{	
		found = 0;
		return;
	}
	
	if(root -> key == num)
	{
		foundPtr = root;
		found = 1;
		return;
	}
	
	if(root->key < num)
		search(root->right,num);
	if(root->key >num)
		search(root->left,num);
}
//leftRotate makes the right child of x to be the parent of x and fixes other pointers
void leftRotate(struct rbNode * x)//this accepts node to rotate on
{
	struct rbNode * y = x ->right;// getting access to right child of x
	
	x-> right = y->left;//changing the left child of y to right child of x
	if(y->left != nil)
		(y->left)->parent = x;//if y's left child is not null puting its parent as x

	y->parent = x->parent;//setting x's parent as y's parent
	//setting x's parents link to y using 3 cases
	if(x->parent == nil)// if x is the root ,setting y as root
		root = y;
	else if(x == (x->parent)->left)//if x is the left child of its parent
		(x->parent)->left = y;//setting x's parent's left child as y
	else if(x == (x->parent)->right)//if x is the right child of its parent								
		(x->parent)->right = y;//setting x's parent's right child as y
	
	y->left = x;// setting y's left child as x 
	x->parent = y; // changing x's parent to y

}

//rightRotate makes the left child of y ,the parent of y and fixes other pointers
void rightRotate(struct rbNode * y)////this accepts the node to rotate on
{
	struct rbNode * x = y-> left;// getting access to left child of y 
	
	y->left = x->right;//changing the 	right child of x to left child of y
	if(x->right != nil)
		(x->right->parent) = y;//if x's right child is not null puting its parent as y
	
	x->parent = y->parent;//setting y's parent as x's parent
	//setting y's parents link to x using 3 cases
	if(y->parent == nil)// if y is the root ,setting x as root
		root = x;
	else if(y == (y->parent)->left)//if y is the left child of its parent
		(y->parent)->left = x;//setting y's parent's left child as x
	else if(y == (y->parent)->right)//if y is the right child of its parent
		(y->parent)->right = x;//setting y's parent's right child as x
	
	x->right = y;// setting x's right child as y
	y->parent = x;// changing y's parent to x	
}

// does the normal bst type insert without taking care of the color related properties
void rbInsertBst(struct rbNode * new)
{
	struct rbNode * findLoc = nil;//used for finding location of parent of the node to be attached(y)
	struct rbNode * traverser = root;//used to traverse the tree(x)
	
	// used for finding the parent node whose child the new node will be made
	while(traverser != nil)// traverser reaches the sentinal 
	{
		findLoc = traverser;
		if((new->key) <(traverser->key))
			traverser = traverser -> left;
		else
			traverser = traverser -> right;
	}

	new -> parent = findLoc;//assigning the new node's parent 
	if(findLoc == nil)// tree is empty
		root = new;
	else if((new -> key) < findLoc->key)//checking if left child
		findLoc->left = new;
	else//if it is right child
		findLoc->right = new;
}

// calls the normal bst type insert and
//fixes the color properties that are violated because of insertion of a red node 
void rbInsert(struct rbNode * new)
{
	rbInsertBst(new);// this will do the basic bst insert
	struct rbNode * uncle;
	while(((new->parent)->red) == 1)//no problem if parent is black
	{
		if(new->parent == (((new->parent)->parent)->left))// if parent is the left child of its parent
		{
			uncle = ((new->parent)->parent)->right;// then uncle will be the right child of grandParent(new node's)
			
			if(uncle->red == 1)// if uncle is red ,we put both parent and uncle as black and grand parent as red
			{
				((new->parent)->red) = 0;
				uncle->red = 0 ;
				(((new->parent)->parent)->red) = 1;
				new = ((new->parent)->parent);// shifting the problem node to grandparent
			}

			else
			{ 
				if(new == ((new->parent)->right))//rotates left if the problem node is right child of parent
				{
					new = new -> parent;
					leftRotate(new);
				}

			(new->parent)->red = 0;//setting parent as black
			((new->parent)->parent) ->red = 1;//setting grandparent as red
			rightRotate(((new->parent)->parent));
			}
		}
		else// if parent is the right child of its parent
		{
			uncle = ((new->parent)->parent)->left;// then uncle will be the left child of grandParent(new node's)
			if(uncle->red == 1)
			{
				((new->parent)->red) = 0;// if uncle is red ,we put both parent and uncle as black and grand parent as red
				uncle->red = 0 ;
				(((new->parent)->parent)->red) = 1;
				new = ((new->parent)->parent);
			}
			else
			{
				if(new == ((new->parent)->left))//rotates left if the problem node is left child of parent
				{
					new = new -> parent;
					rightRotate(new);
				}

			(new->parent)->red = 0;//setting parent as black
			((new->parent)->parent)->red = 1;//setting grandparent as red
			leftRotate(((new->parent)->parent));
			}
		}
	}
	root->red = 0;//setting root as black as it may have changed
}

void printInorder(struct rbNode* root)//recursively prints rb tree in inorder
{
	if(root == nil)
		return;
	printInorder(root -> left);
	printf("key: %d ",root->key);
	if(root->red == 0)//checking for color of root
		printf("Color : Black\n");
	else
		printf("Color :red \n");
	printInorder(root -> right);
	
}

// frees all the dynamically allocated memory i.e. deletes the entire tree
// it performs a preorder traverser and frees memory while doing it
void deleteAll(struct rbNode * traverser)
{
	if(traverser == nil)
		return;
	//first the subtrees are deleted and then the root of those subtrees
	deleteAll(traverser->left);
	deleteAll(traverser->right);
	free(traverser);
}

void main()
{
	int choice;
	struct rbNode* ptr;
	int key;
	nil = (struct rbNode*)malloc(sizeof(struct rbNode));
	nil->key = 999999;
	nil->parent = NULL;
	nil->left = NULL;
	nil->right = NULL;
	nil->red = 0;
	root = nil;
	while(1)
	{
		printf("Enter Choice\nMenu:\n1)Insert\n2)Search\n3)exit\n\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 : {
						printf("Enter the key\n");
						scanf("%d",&key);
						ptr = createRbNode(key);//will create a rb node
						if( ptr ==NULL )// in case of memory overflow 
							break;
						rbInsert(ptr);
						printInorder(root);
						break;
					 }
			case 2 : {
						printf("Enter key\n");
						scanf("%d",&key);
						search(root,key);
						if(found == 0)// if given element not found
							printf("The given key is not in the tree\n");
						if(found == 1)//if given element found
						{
							printf("Value: %d",foundPtr->key);
							if(foundPtr->red == 0)
								printf("Color : Black\n");
							else
								printf("Color :red \n")	;
						}
						break;
						
					 }
			case 3 : {
						deleteAll(root);// all the memory is freed
						return;		 		 
					 }		
		 
			default: printf("Wrong Input\n");		 
		}
	}
}
		
