#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Card{credit=0,charge};
/* structure about Customer`s Information */
typedef struct{
	int ID;		// bus ID
	int stID;       // station ID
	char stname[20]; //station name
	int dist; // distance from the previous station 
}transport;

typedef struct{
	char custname[30];
	int custID;     
	int custmoney;   //Customer`s remain money
	enum Card card;  //Customer`s card sort
	char custage[6];
}Customer;
/* structure about on and off time */
typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
}Time;
/* structure about input */
typedef struct{
	int ID;
	char trans[15]; // trans is transportation`s name
	int stID; //stID is input Station ID
	Time time; // on and off time
	float dis;   //distance
	int price; // Input price;
}Input;

typedef struct _NODE{
	int ID;
	int price;
	struct _NODE *link;
}node;

node *head =NULL;

void makenode(int ID, int price);
void writetotxt(Customer *cust,int num);

int main(void)
{
     /* Declaration  */
	int i,num1,num2;  
	Customer *cust;  //cust is customer`s Information
	FILE *cu;       
	char *str; 
	char *inputstr;
	Input *info; 
	char sort[7];
	node *tail;	
	str=(char *)malloc(sizeof(char)*100);		
	inputstr=(char *)malloc(sizeof(char)*100);
	/* open the file to read */
	cu=fopen("customer.txt","r"); //open file Customer`s Information txt file
	
	/* input the information to variables */
	fscanf(cu,"%d ",&num1);
	cust=(Customer *)malloc(sizeof(Customer)*num1);
	for(i=0;i<num1;i++)   //
	{
		fgets(str,100,cu);
		/* ID / name / money / card sort / age */
		sscanf(str,"%d %*c %20[^/] %*c %d %*c %6[^/] %*c %5s",&(cust+i)->custID,(cust+i)->custname,&(cust+i)->custmoney,sort,(cust+i)->custage);
		if(strcmp(sort,"credit")==0)
			(cust+i)->card=credit;
		else if (strcmp(sort,"charge")==0)
			(cust+i)->card=charge;
	}

	fscanf(stdin,"%d ",&num2);
	info=(Input *)malloc(sizeof(Input)*num2);
	/* input the inputstring to structure */
	for(i=0;i<num2;i++)
	{
		gets(inputstr);
		/* user ID / trans / year.month.day.hour.min / distance / price	 */
		sscanf(inputstr,"%d %*c %s %*c %d %*c %d %*c %d %*c %d %d %*c %d %*c %f %*c %d",
				&(info+i)->ID,
				(info+i)->trans,
				&(info+i)->stID,
				&(info+i)->time.year,&(info+i)->time.month,&(info+i)->time.day,&(info+i)->time.hour,&(info+i)->time.minute,
				&(info+i)->dis,
				&(info+i)->price);
		if((cust + ((info+i)->ID))->card == credit)
			(cust + (info+i)->ID)->custmoney += (info+i)->price;
		else if( (cust + ((info+i)->ID))->card == charge)
			(cust + (info+i)->ID)->custmoney -= (info+i)->price;
		makenode( (info+i)->ID , (cust+ (info+i)->ID )-> custmoney);
	}
	
	/* print the result */
	
	for(tail=head;tail;tail=tail->link)
	{
		printf("%d : %d\n",tail->ID,tail->price);
	}
	/* free and fclose file */
	fclose(cu);
	writetotxt(cust,num1);
	free(str);
	free(inputstr);
	free(cust);
	free(info);
	return 0;
}
/* make node to implement on and off */
void makenode(int ID,int price)
{
	node *new,*tail;

	new=(node *)malloc(sizeof(node));
	new->link=NULL;
	new->ID=ID;
	new->price=price;
	if(!head)
	{
		head=new;
	}
	else
	{
		for(tail=head;tail->link!=NULL;tail=tail->link)
			;
		tail->link=new;
	}
}
/* write changed information to Customer.txt */ 
void writetotxt(Customer *cust,int num)  
{
	int i;
	FILE *fp;
	/* open Customer.txt to write */
	fp=fopen("customer.txt","w");
	/* write to Customer.txt */
	fprintf(fp,"%d\n",num);
	for(i=0;i<num;i++)
	{
		if((cust+i)->card == credit)
		{
			fprintf(fp,"%d / %s/ %d / %s / %s\n",(cust+i)->custID,(cust+i)->custname,(cust+i)->custmoney,"credit",(cust+i)->custage);
		}
		else if ((cust+i)->card == charge)
		{
			fprintf(fp,"%d / %s/ %d / %s / %s\n",(cust+i)->custID,(cust+i)->custname,(cust+i)->custmoney,"charge",(cust+i)->custage);
		}
	}
	fclose(fp);
}

