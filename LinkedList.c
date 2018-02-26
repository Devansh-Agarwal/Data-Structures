#include <stdio.h>
#include <stdlib.h>

struct node
{
	int val;
	struct node *next;
	
};
struct node *head = NULL ;
struct node* createNode()// creates a node
{
	struct node *p= (struct node *)malloc(sizeof(struct node));
	if(p==NULL)// in case of memory overflow
	{
		printf("Memory Overflow");
		exit(0) ;
	}
	return p;
}
void insertNodeAtEndOfList()
{	
	int num;
	printf("enter element to be inserted\n");
	scanf("%d",&num);
	struct node *p= createNode();
	p->	next = NULL;	
	p-> val = num;
	if(head==NULL)//checking for empty linked list
	{
		head = p ;
	}
	else
	{
		struct node* i;
		for( i =head; i->next !=NULL;i = i->next)
		{}
		i-> next = p;
	}
}
void insertNodeAtBeginningOfList()
{
	int num;
	printf("enter element to be inserted\n");
	scanf("%d",&num);
	struct node *p= createNode();
	p-> val = num;
	if(head==NULL)
	{
		head = p ;
		p->	next = NULL;

	}
	else
	{
		p-> next =head;
		head = p;
	} 

}
void insertNodeAfterFirstOccurence()
{
	int num,num2;
	printf("enter the number which you want to insert\n");
    scanf("%d", &num);
    printf("enter after which number you want to insert\n");
    scanf("%d", &num2);
    struct node *p= createNode();
    struct node* i;
   for( i =head; i!=NULL;i = i->next)
		{
			if(i->val==num2)//checks for 1st occurence 
			{
				p->next= i->next;
				p->val = num;
				i->next =p;
				return; 
			}
		}
	printf("the number after which you wanted to added the node is not present \n");

}
void deleteLastOccurence()
{	
	int num;
	printf("enter the number which you want to DELETE\n");
    scanf("%d", &num);
    struct node* i,*p=NULL,*prev =NULL;
    for( i =head; i!=NULL;i = i->next)// traversing the list
    {	
   		if(i==head&&i->val==num&&i->next==NULL)//1st element is last occurence
   			{
   				prev= i;
   				break;
   			}
   		if(i==head&&i->val==num)
   			{
   				prev= i;

   			}
   		p= i->next;
   		if(p!=NULL)
   			if(p->val == num)
   				prev=i;
    }
    if(prev!=NULL)// linked list is empty 
    {
    	if(prev==head&&prev->next==NULL)//1 element linked list
    	{
    		head = NULL;
    		free(prev);
    		return;
    	}	
    	if(prev==head&&prev->next!=NULL)
        {
            head=head->next;
            free(prev);
            return;
        }
    	i=prev->next;
    	prev->next= i->next;
    	free(i);
    }
    else
    	printf("element does not exist in the linked list\n");
}
void deleteAllElements()
{
	struct node *i= head;
	while(i!=NULL)
	{
		
		struct node* p= i ;
		i = i->next;
		free(p);
	}
	head=NULL;
}
void printAllElements()
{
	if(head==NULL)
		printf("there are no elements in this list\n");
	for(struct node*i=head; i!=NULL; i=i->next)
	{
		if(i->next!=NULL)// no comma after last element
			printf("%d,", i->val );
		else
			printf("%d",i->val);
	}
	printf("\n");
}
void main()
{
	int choice;
	while(1)
	{
		printf("1 - insert a node at the end of the list\n");
		printf("2 - insert a node at the beginning of the list\n");
		printf("3 - insert node after the first occurance of an element\n");
		printf("4 - delete the last occurance of an element\n");
		printf("5 - delete all elements\n");
		printf("6 - print all elements\n");
		printf("7 - exit\n");
        
        printf("enter your choice\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:     insertNodeAtEndOfList();
						printAllElements();break;
			            

			case 2:    	insertNodeAtBeginningOfList();
			            printAllElements();break;

			case 3:  	insertNodeAfterFirstOccurence();
			           	printAllElements();break;               

			case 4:     deleteLastOccurence();
			           	printAllElements();break;

			case 5:     deleteAllElements();
			            printAllElements();break;

			case 6:     printAllElements();break;                                  

            case 7:     deleteAllElements();
            			exit (0);

			default:    printf("enter valid option\n");  
	        
		}
	}
}