#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_Node(int id,char *name);

typedef struct _Node{
		int st_id;
		char *st_name;
		struct _Node *link;
}Node;

Node *head = NULL;

int main(void)
{
	int n,i,id;
	char *str;
	char *name;
	Node *tail;
	str=(char *)malloc(sizeof(char)*200);
	name=(char *)malloc(sizeof(char)*25);
	scanf("%d ",&n);
	printf("--------Student list--------------\n");
	for(i=0;i<n;i++)
	{
		gets(str);
		sscanf(str,"%d %*c %[^\n]",&id,name);
		create_Node(id,name);
	}
	for(tail=head;tail;tail=tail->link)
	{
		printf("%d / %s\n",tail->st_id,tail->st_name);
	}
	return 0;
}

void create_Node(int id,char *name)
{
	Node *new;
	Node *tail;

	new=(Node *)malloc(sizeof(Node));
	new->st_name=(char *)malloc(sizeof(char)*25);
	
	new->st_id = id;
	strcpy(new->st_name, name);
	new->link=NULL;

	if(head==NULL)
		head=new;
	else 
	{
		for(tail=head;tail->link!=NULL;tail=tail->link)
			;
			tail->link=new;
	}
}


