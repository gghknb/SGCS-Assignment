#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _NODE{
	char DATA;
	struct _NODE *LINK;
}NODE;

NODE *head;

void DeleteNodeAtFront();
void DeleteNodeAtBack();
void InsertNodeAtFront(char a);
void InsertNodeAtBack(char b);
void PrintList();

int main(void)
{
	DeleteNodeAtFront();
		DeleteNodeAtBack();
		InsertNodeAtFront('n');
		InsertNodeAtFront('e');
		InsertNodeAtFront('p');
		InsertNodeAtBack('l');
		InsertNodeAtFront('o');
		InsertNodeAtBack('a');
		InsertNodeAtBack('b');
		printf("----Insertion Complete-----\n");
		PrintList();
		DeleteNodeAtFront();
		DeleteNodeAtBack();
		DeleteNodeAtBack();
		DeleteNodeAtBack();
		printf("\n----Delete Complete-----\n");
		PrintList();
		return 0;
}
	
void InsertNodeAtFront(char a)
{
	NODE *new0;

	new0=(NODE *)malloc(sizeof(NODE));
	new0->DATA=a;
	new0->LINK=NULL;
		
	new0->LINK= head;
	head=new0;
}

void InsertNodeAtBack(char b)
{
	NODE *new1;
	NODE *tail;
	
	new1=(NODE *)malloc(sizeof(NODE));
	new1->DATA=b;
	new1->LINK=NULL;

	if(head==NULL)
		head=new1;
	else
	{
		for(tail=head;tail->LINK!=NULL;tail=tail->LINK)
			;
		tail->LINK=new1;
	}
	
}

void PrintList()
{
	NODE *tail;
	for(tail=head;tail;tail=tail->LINK)
		printf("%c",tail->DATA);
	printf("\n");
}

void DeleteNodeAtFront()
{
	NODE *new2;

	new2=(NODE *)malloc(sizeof(NODE));
	if(head)
	{
		new2=head;
		head=head->LINK;
		free(new2);
	}
}

void DeleteNodeAtBack()
{
	NODE *pPre,*pCur;
	pCur=head;
	pPre=head;
	if(head)
	{
		while(pCur->LINK!=NULL)
		{
			pPre=pCur;
			pCur=pCur->LINK;
		}
		pPre->LINK=pCur->LINK;
	free(pCur);
	}
}
