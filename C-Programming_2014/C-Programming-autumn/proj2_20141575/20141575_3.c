#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum Card{credit =0 ,charge };

typedef struct{
	char custname[30];
	int custID;     
	int custmoney;   //Customer`s remain money
	enum Card card;  //Customer`s card sort
	char custage[6];
}Customer;

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int min;
}Time;

typedef struct {
	int transID;
	int ID;
	Time time;
	int stID;
	char card[7];
	char sort[7];
	int price;
}Output;

int main()
{
	/* declare */
	Output *output;
	FILE *fn;
	FILE *cu;
	int num,num2;
	FILE *fp;
	int i=0,cs,t;
	char str[100];
	char inputstr[100];
	char cardsort[7];
	int ID,chargemoney;
	char name[30];
	int money[50]={0};
	int cnt=0;
	char strname[10];
	Customer *cust;
	/* open file */
	fp=fopen("reader_output.txt","r");
	cu=fopen("customer.txt","r");
	/*initailize */
	fscanf(cu,"%d ",&num);
	cust=(Customer *)malloc(sizeof(Customer)*num);
	for(i=0;i<num;i++)   //
	{
		fgets(str,100,cu);
		/* ID / name / money / card sort / age */
		sscanf(str,"%d %*c %20[^/] %*c %d %*c %6[^/] %*c %5s",&(cust+i)->custID,(cust+i)->custname,&(cust+i)->custmoney,cardsort,(cust+i)->custage);
		if(strcmp(cardsort,"credit")==0)
			(cust+i)->card=credit;
		else if (strcmp(cardsort,"charge")==0)
			(cust+i)->card=charge;
	}
	fclose(cu);
	
	output=(Output *)malloc(sizeof(Output)*100);
	/* initailize reader_output.txt`s variable */
	
	while(fgets(inputstr,100,fp) !=NULL)
	{
		cnt++;
	}
	fclose(fp);
	fp=fopen("reader_output.txt","r");
	for(i=0;i<cnt;i++)
	{
			fgets(inputstr,100,fp);
			sscanf(inputstr,"%d %*c %d %*c %d %*c %d %*c %d %*c %d %*c %d %*c %d %*c %6s %*c %s %*c %d",
				&(output+i)->transID,&(output+i)->ID,
				&(output+i)->time.year,&(output+i)->time.month,&(output+i)->time.day,
				&(output+i)->time.hour,&(output+i)->time.min,
				&(output+i)->stID,(output+i)->card,(output+i)->sort,&(output+i)->price);
	}
	
		/* input */
	scanf("%d",&num2);
	for(t=0;t<num2;t++)
	{
		scanf("%d",&cs);
		/* charge */
		if(cs==1)
		{
			scanf(" %*c %d %*c %d",&ID,&chargemoney);
			if((cust+ID)->card == credit)
				printf("fail\n");
			else 
			{
				printf("%d : %d , ",ID,(cust+ID)->custmoney);
				(cust+ID)->custmoney+=chargemoney;
				printf("%d",(cust+ID)->custmoney);
			}

		}
		/* credit initailize */
		else if(cs==2)
		{
			for(i=0;i<num;i++)
			{
				if((cust+i)->card == credit)
				 {
					(cust+i)->custmoney += money[(cust+i)->custID];
					 printf("%d : %s / %d\n",(cust+i)->custID,(cust+i)->custname,(cust+i)->custmoney);
					
				 }
			}
		}
		/* last stop system */
		else if (cs==3)
		{
			scanf(" %*c %s",name);
		
			for(i=0;i<cnt;i++)
			{		
				switch((output+i)->transID)
				{
					case 10 : strcpy(strname,"t10");
							  break;
					case 5714 : strcpy(strname,"g5714");
								break;
					case 604 : strcpy(strname,"b604");
							   break;
					case 3 : strcpy(strname,"3");
							 break;
				}
			
				if(strcmp(name,strname)==0)
				{
					printf("%d : %d\n",(output+i)->ID,(output+i)->price);
					money[(output+i)->ID] += (output+i)-> price;
				}
			}
		}	
	}
	
		/* rewrite to customer.txt */
		fn=fopen("customer.txt","w");
		fprintf(fn,"%d\n",num);
		for(i=0;i<num;i++)
		{
			fprintf(fn,"%d / %s/ %d ",(cust+i)->custID,(cust+i)->custname,(cust+i)->custmoney);
			if((cust+i)->card == charge)
			fprintf(fn,"/ charge / %s\n",(cust+i)->custage);
			else if ((cust+i)->card == credit)
			fprintf(fn,"/ credit / %s\n",(cust+i)->custage);
		}		
		free(cust);
		free(output);
		fclose(fp);
		fclose(fn);
		return 0;
}

