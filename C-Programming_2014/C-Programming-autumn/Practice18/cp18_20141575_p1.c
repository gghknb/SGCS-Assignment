#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union utemp{
	int i;
	float f;
	double d;
};

typedef struct{
	int type;
	char name[20];
	union utemp u;
}stemp;

void power(stemp *s);

int main(void)
{
	int n,i;
	stemp *st;
	
	printf("# of iterations\n");
	scanf("%d",&n);
	
	st=(stemp *)malloc(sizeof(stemp)*n);
	
	for(i=0;i<n;i++)
	{
		printf("Type [1 for int, 2 for float, 3 for double] : ");
		scanf("%d",&(st+i)->type);
		
		switch((st+i)->type){
			case 1 : 
				strcpy((st+i)->name,"int"); 
				break;
			case 2 : 
				strcpy((st+i)->name,"float"); 
				break;
			case 3 : 
				strcpy((st+i)->name,"double"); 
				break;
		};
		
		printf("Enter a %s value : ",(st+i)->name);
		switch((st+i)->type){
			case 1 : 
				scanf("%d",&(st+i)->u.i); 
				break;
			case 2 : 
				scanf("%f",&(st+i)->u.f);
				break;
			case 3 :
				scanf("%lf",&(st+i)->u.d);
				break;
		};
		power(st+i);
	}
	
	printf("-------------------Result--------------------\n");
	for(i=0;i<n;i++)
	{
		switch((st+i)->type){
			case 1 :
				printf("%-10s : %7d\n",(st+i)->name,(st+i)->u.i);
				break;
		    case 2 :
				printf("%-10s : %7.4f\n",(st+i)->name,(st+i)->u.f);
				break;
		    case 3 :
				printf("%-10s : %7.4lf\n",(st+i)->name,(st+i)->u.d);
				break;
		};
	}
	return 0;
}

void power(stemp *s)
{
	switch(s->type){
		case 1: 
			s->u.i = (s->u.i)*(s->u.i);
			break;
		case 2:
			s->u.f = (s->u.f)*(s->u.f);
			break;
		case 3: 
		   s->u.d= (s->u.d)*(s->u.d);
		   break;
	};
}	
