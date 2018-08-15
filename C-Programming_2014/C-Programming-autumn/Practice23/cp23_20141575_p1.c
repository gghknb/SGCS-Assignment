#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALID 0
#define HISTORY 1
#define QUIT 2
#define INVALID 3

typedef struct node{
		char command[10];
		struct node* next;
}queue_node;


typedef struct{
	queue_node *front;
	int count;
	queue_node *rear;
}queue;

int CheckCommand(char *command);
void EnqueueCommand(queue *pQueue,char *dataIn);

int main(void)
{
	char command[10];
	queue *pQueue;
	char *data;
	data=(char *)malloc(sizeof(char)*10);
	pQueue=(queue *)malloc(sizeof(queue));
	
	pQueue->front=NULL;
	pQueue->count=0;
	pQueue->rear=NULL;
	while(1)
	{
		printf(">>");
		scanf("%s",data);
		switch(CheckCommand(data))
		{
			case VALID : 
				printf("[Valid] %s\n",data);
				EnqueueCommand(pQueue,data);
				break;

			case HISTORY :
				printf("queue count = %d\n",pQueue->count);
				break;

			case QUIT : 
				return 0;
			case INVALID :
				printf("[Invalid]\n");
		}
	}
	return 0;
}

int CheckCommand(char *command)
{
	if(strcmp(command,"help")==0 || strcmp(command,"dir")==0 
		||strcmp(command,"mkdir")==0 ||strcmp(command,"cd")==0)
		return 0;
	else if(strcmp(command,"history")==0 ||strcmp(command,"h")==0 )
		return 1;
	else if(strcmp(command,"q")==0 ||strcmp(command,"quit")==0)
		return 2;
	else return 3;
}
void EnqueueCommand(queue *pQueue,char *dataIn)
{
	queue_node *ptr;
	ptr=(queue_node *)malloc(sizeof(queue_node));

	strcpy(ptr->command,dataIn);
	ptr->next=NULL;

	if(pQueue->count==0)
		pQueue->front=ptr;
	else 
		pQueue->rear->next=ptr;
	(pQueue->count++);
	pQueue->rear=ptr;
}
