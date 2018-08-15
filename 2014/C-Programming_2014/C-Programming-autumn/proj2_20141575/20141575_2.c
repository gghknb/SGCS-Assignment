#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum Card{credit =0 ,charge };

/* structure about Customer`s Information */ 
typedef struct{
	char custname[30];
	int custID;     
	int custmoney;   //Customer`s remain money
	enum Card card;  //Customer`s card sort
	char custage[6];
}Customer;

/* structure on and off time */
typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int min;
}Time;

/* structure input */
typedef struct{
	int ID;
	char trans[15];//transportation
	int stID; //station ID
	Time time;  
	int transfer_count; //transfer count 
	int onoff; //On and off information
}input;

typedef struct{
	int ID;		// bus ID
	int stID;       // station ID
	char stname[50]; //station name
	float dist; // distance from the previous station 
}transport;

typedef struct _NODE{
	int ID;
	int trans;
	int stID;
	Time time;
	int transfer_count;
	int onoff;
	struct _NODE *link;
}node;

typedef struct {
	int ID;
	int price;
	int balance;
	float dist;
}Output;
node *head=NULL;

void makenode(int ID,char trans[15],int stID,Time time,int transfer_count,int onoff);

float caldistance(int prevstID,int currstID,transport *tr);

void calculate(transport *t10,transport *b604,transport *g5714,transport *sub3,Customer *cust,Output *output);


int main(void)
{
	/* Declare variable */
	Customer *cust;
	input *in;
	int num,num0;
	char sort[7];
	FILE *cu;
	char str[100],str1[100],str2[100],str3[100],str4[100];
	char inputstr[100];
	transport *t10,*b604,*g5714,*sub3;
	FILE *t=fopen("bus_t10.txt","r");
	FILE *b=fopen("bus_b604.txt","r");
	FILE *g=fopen("bus_g5714.txt","r");
	FILE *s=fopen("subway_line3.txt","r");
	int i,num1,num2,num3,num4;

	Output *output;
	/* open transportation file  and initialize */
	fscanf(t,"%d ",&num1);

	t10=(transport *)malloc(sizeof(transport)*num1);
	for(i=0;i<num1;i++)
	{
		fgets(str1,100,t);
		/* initialize ID stationID stationname distance */
		sscanf(str1,"%d %*c %d %*c %[^/] %*c %f",
				&(t10+i)->ID,&(t10+i)->stID,(t10+i)->stname,&(t10+i)->dist);
	}
	fscanf(b,"%d ",&num2);
	b604=(transport *)malloc(sizeof(transport)*num2);
	for(i=0;i<num2;i++)
	{
		fgets(str2,100,b);
		/* initialize ID stationID stationname distance */
		sscanf(str2,"%d %*c %d %*c %[^/] %*c %f",
				&(b604+i)->ID,&(b604+i)->stID,(b604+i)->stname,&(b604+i)->dist);
	}
	fscanf(g,"%d ",&num3);		
	g5714=(transport *)malloc(sizeof(transport)*num3);
	for(i=0;i<num3;i++)
	{
		fgets(str3,100,g);
		/* initialize ID stationID stationname distance */
		sscanf(str3,"%d %*c %d %*c %[^/] %*c %f",
				&(g5714+i)->ID,&(g5714+i)->stID,(g5714+i)->stname,&(g5714+i)->dist);
		
		}
	fscanf(s,"%d ",&num4);
	sub3=(transport *)malloc(sizeof(transport)*num4);
	for(i=0;i<num4;i++)
	{
		fgets(str4,100,s);
		/* initialize ID stationID stationname distance */
		sscanf(str4,"%d %*c %d %*c %[^/] %*c %f",
				&(sub3+i)->ID,&(sub3+i)->stID,(sub3+i)->stname,&(sub3+i)->dist);
	}
	/* open customer file and initialize */
	cu=fopen("customer.txt","r");
	fscanf(cu,"%d ",&num);;
	cust=(Customer *)malloc(sizeof(Customer )*num);
	for(i=0;i<num;i++)
	{
		fgets(str,100,cu);
		sscanf(str,"%d %*c %30[^/] %*c %d %*c %6[^/] %*c %5s",&(cust+i)->custID,(cust+i)->custname,&(cust+i)->custmoney,sort,(cust+i)->custage);
		if(strcmp(sort,"credit")==0)
			(cust+i)->card=credit;
		else if (strcmp(sort,"charge")==0)
			(cust+i)->card=charge;
	}
	/* input and initialize input`s information  */
	fscanf(stdin,"%d ",&num0);
	in=(input *)malloc(sizeof(input)*num0);
	for(i=0;i<num0;i++)
	{
		gets(inputstr);
		/*               ID     trans     stID   year   month  day hour  min   count  onoff */
		sscanf(inputstr,"%d %*c %[^/] %*c %d %*c %d %*c %d %*c %d %d %*c %d %*c %d %*c %d",
				&(in+i)->ID,
				(in+i)->trans,
				&(in+i)->stID,
				&(in+i)->time.year,&(in+i)->time.month,&(in+i)->time.day,&(in+i)->time.hour,&(in+i)->time.min,
				&(in+i)->transfer_count,
				&(in+i)->onoff);
		
				makenode((in+i)->ID,(in+i)->trans,(in+i)->stID,(in+i)->time,(in+i)->transfer_count,(in+i)->onoff);
	}
	
	output=(Output *)malloc(sizeof(Output)*num0);
	fclose(cu);
	calculate(t10,b604,g5714,sub3,cust,output);
	free(cust);
	free(in);
	fclose(t);
	fclose(g);
	fclose(b);
	fclose(s);
	free(output);
	free(t10);
	free(g5714);
	free(b604);
	free(sub3);
	return 0;
}

void makenode(int ID,char trans[15],int stID,Time time,int transfer_count,int onoff)

{
	node *tail,*new;
	new=(node *)malloc(sizeof(node));

	new->ID=ID;
	if( strcmp(trans,"t10 ")==0)
		new->trans=10;
	else if(strcmp(trans,"g5714 ")==0)
		new->trans=5714;
	else if(strcmp(trans,"b604 ")==0)
		new->trans=604;
	else if(strcmp(trans,"3 ")==0)
		new->trans=3;
	new->time=time;
	new->stID=stID;
	new->transfer_count=transfer_count;
	new->onoff=onoff;
	new->link=NULL;
	if(!head)
	{
		head=new;
	}
	else
	{
		for(tail=head;tail->link;tail=tail->link)
		;
		tail->link=new;
	}
}

/* calculate Amount & balance(remain money) & distance and save to output file*/
void calculate(transport *t10,transport *b604,transport *g5714,transport *sub3,Customer *cust,Output *output)
{
	FILE *file;
	node *tail;
	int t;
	int i=0;
	int prev_trans;  //variable that previous transportation
	int prev_stID; // variable that previous stationID(start station)
	int curr_stID; // variable that cuurent stationID (take down station)
	int sum=0;
	file=fopen("reader_output.txt","w");
	for(tail=head;tail!=NULL;tail=tail->link)
	{
		t=tail->ID;
		/* initialize structure output(distance , balance , ID) */
		(output+i)->ID=t;		
		(output+i)->balance=(cust+t)->custmoney;
		(output+i)->price=0;
		(output+i)->dist=0;
		/*       on     generate addtional fee when transfer to other transportation  */
		if(tail->onoff==0)
		{
			prev_stID=tail->stID;
					/* Not use transfer,only use one bus or one subway */
			if(tail->transfer_count==0)
			{
				/*only use t10 */
				if(tail->trans==10)
				{
					prev_trans=10;
					/*    adult fee  */
					if(strcmp((cust+t)->custage,"adult")==0)
						(output+i)->price=850;
					/*    teens fee   */
					else if(strcmp((cust+t)->custage,"teens")==0)
						(output+i)->price=560;
					/*    child fee   */
					else if(strcmp((cust+t)->custage,"child")==0)
						(output+i)->price=350;
				}
				/* only use b604 */
				else if(tail->trans==604)
				{
					prev_trans=604;
					if(strcmp((cust+t)->custage,"adult")==0)
						(output+i)->price=1050;
					else if(strcmp((cust+t)->custage,"teens")==0)
						(output+i)->price=720;
					else if(strcmp((cust+t)->custage,"child")==0)
						(output+i)->price=450;
				}
				/* only use g5714 */
				else if(tail->trans==5714)
				{
					prev_trans=5714;
					if(strcmp((cust+t)->custage,"adult")==0)
						(output+i)->price=1050;
					else if(strcmp((cust+t)->custage,"teens")==0)
						(output+i)->price=720;
					else if(strcmp((cust+t)->custage,"child")==0)
						(output+i)->price=450;
				}
				/* only use subway line 3 */
				else if (tail->trans==3)
				{	
					prev_trans=3;
					if(strcmp((cust+t)->custage,"adult")==0)
						(output+i)->price=1050;
					else if(strcmp((cust+t)->custage,"teens")==0)
						(output+i)->price=720;
					else if(strcmp((cust+t)->custage,"child")==0)
						(output+i)->price=450;
				}
			}
			/* use transfer */
			else if(tail->transfer_count!=0)
			{
				/* 10-> 10                     not generate addtional fee */
				/* 10-> 604,g5714,subwayline3  generate additional fee */
				if(prev_trans==10)
				{
					if(tail->trans==10)
					{
						(output+i)->price=0;
						prev_trans= 10;
					}
					else if(tail->trans==5714)
					{
						prev_trans=5714;
						if(strcmp((cust+t)->custage,"adult")==0)
							(output+i)->price=1050-850;
						else if(strcmp((cust+t)->custage,"teens")==0)
							(output+i)->price=720-560;
						else if(strcmp((cust+t)->custage,"child")==0)
							(output+i)->price=450-350;
					}
					else if(tail->trans==604)
					{
						prev_trans=604;
						if(strcmp((cust+t)->custage,"adult")==0)
							(output+i)->price=1050-850;
						else if(strcmp((cust+t)->custage,"teens")==0)
							(output+i)->price=720-560;
						else if(strcmp((cust+t)->custage,"child")==0)
							(output+i)->price=450-350;
					}
					else if(tail->trans==3)
					{
						prev_trans=3;
						if(strcmp((cust+t)->custage,"adult")==0)
							(output+i)->price=1050-850;
						else if(strcmp((cust+t)->custage,"teens")==0)
							(output+i)->price=720-560;
						else if(strcmp((cust+t)->custage,"child")==0)
							(output+i)->price=450-350;
					}
				}
				else /* Not generate addtional price   b604,g5714,subway line3 -> 
													   b604, g5714,subway line3,t10      */
				{
					(output+i)->price=0;
					if(tail->trans==10)
						prev_trans=10;
					else if(tail->trans==5714)
						prev_trans=5714;
					else if(tail->trans==604)
						prev_trans=604;
					else if(tail->trans==3)
						prev_trans=3;
				}
			}
			(cust+t)->custmoney -= (output+i)->price;
			(output+i)->balance = (cust+t)->custmoney; 
		}
		/*      off      generate addtional fee beacuase of distance which over 10 km */
		else if(tail->onoff==1)
		{
			
			curr_stID=tail->stID;
				/* Only use bus  Not generate addtitional fee 
			 * use subway line 3 generate addtitional fee because of distance */
			if(tail->transfer_count==0)
			{
				if(tail->trans==10)
				{
					prev_trans=10;
					(output+i)->price=0;
					(cust+t)->custmoney -= (output+i)->price;
					(output+i)->balance = (cust+t)->custmoney;
					(output+i)->dist=caldistance(prev_stID,curr_stID,t10);
				}
				else if(tail->trans==5714)
				{
					prev_trans=5714;
					(output+i)->price=0;
					(cust+t)->custmoney-=(output+i)->price;
					(output+i)->balance = (cust+t)->custmoney;
					(output+i)->dist=caldistance(prev_stID,curr_stID,t10);
				}
				else if(tail->trans==604)
				{
					prev_trans=604;
					(output+i)->price=0;
					(cust+t)->custmoney-=(output+i)->price;
					(output+i)->balance = (cust+t)->custmoney;
					(output+i)->dist=caldistance(prev_stID,curr_stID,t10);
				}
				else if(tail->trans==3)
				{
					prev_trans=3;
					(output+i)->price=0;
					/* find start and last station and distance */
					(output+i)->dist=caldistance(prev_stID,curr_stID,sub3);		  
					/* if  10<=dist<40 additional fee 100won per 5km */
					if((output+i)->dist>=10 && (output+i)->dist<40 )
					{
						(output+i)->price = ((int)(output->dist-10)/5+1)*100;
					}
					/* if dist>40 addtional fee 100won per 10km + 10 to 40km addtional fee */
					else if((output+i)->dist>40)
					{
						(output+i)->price = 600 + ((int)(output->dist-40)/10+1)*100;
					}

					/* if teens sale 80% child sale 50% */
					if(strcmp((cust+t)->custage,"teens")==0)
							(output+i)->price *= 0.8;
					else if (strcmp((cust+t)->custage,"child")==0)
							(output+i)->price *= 0.5;

					(cust+t)->custmoney -= (output+i)-> price;
					(output+i)->balance = (cust+t)->custmoney;
				}
				sum+=(output+i)->dist;
			}
			/* Use transfer system 
			 * t10, b604, g5714 , subway line 3 */
			else if(tail->transfer_count!=0)
			{
				if(tail->trans==10)
				{
				
					(output+i)->dist=caldistance(prev_stID,curr_stID,t10);
					prev_trans=10;
				}
				if(tail->trans==604)
				{
					
					(output+i)->dist=caldistance(prev_stID,curr_stID,b604);
					prev_trans=604;
				}
				if(tail->trans==5714)
				{			
					(output+i)->dist=caldistance(prev_stID,curr_stID,g5714);
				
					

					prev_trans=5714;
				}
				if(tail->trans==3)
				{
					(output+i)->dist=caldistance(prev_stID,curr_stID,sub3);	
					prev_trans=3;
				}
					sum+= (output+i)->dist;
				/* if dist>=10 addtional fee 100won per 5km */
					if(sum<10)
						(output+i)->price=0;
					else if (sum >=10)
					{
						(output+i)->price =  ((int)(sum-10)/5+1)*100;
						if(strcmp((cust+t)->custage,"teens")==0)
								(output+i)->price *=0.8;
						else if(strcmp((cust+t)->custage,"child")==0)
								(output+i)->price *=0.5;
					}
					(cust+t)->custmoney -= (output+i)->price;
					(output+i)->balance = (cust+t)->custmoney;
			}
			tail->transfer_count++;
			tail->transfer_count= tail->transfer_count % 5;
		}
			fprintf(file,"%d / %d / %d.%d.%d. %d:%d / %d ",
					tail->trans,tail->ID,
					tail->time.year,tail->time.month,tail->time.day,tail->time.hour,tail->time.min,
					tail->stID);
			if((cust+tail->ID)->card ==charge)
				fprintf(file,"/ credit / %s / %d\n",(cust+tail->ID)->custage,(output+i)->price);
			else if( (cust+tail->ID)->card == credit)
				fprintf(file,"/ charge / %s / %d\n",(cust+tail->ID)->custage,(output+i)->price);
		
			printf("%d : %d, %d, %.1f\n",(output+i)->ID,(output+i)->price,(output+i)->balance,(output+i)->dist);
		i++;
	}
	fclose(file);
}
/* calculate distance previous station to stop station */ 
float caldistance(int prevstID,int currstID,transport *tr)
{
	float dist=0;
	int i,t;

	for(i=0;;i++)
	{
		if((tr+i)->stID == prevstID)
			break;

	}
	for(t=i;;t++)
	{
		if((tr+t)->stID == currstID)
			break;
		dist += (tr+t+1)->dist;

	}
	return dist;
} 
