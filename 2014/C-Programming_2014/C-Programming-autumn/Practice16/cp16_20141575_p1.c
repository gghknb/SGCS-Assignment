#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Sex{male,female};


typedef struct{
	float mid;
	float final;
	float proj;
}Points;

typedef struct{
	int studentId;
	char name[40];
	enum Sex sex;
	Points subject;
	char grade[3];
}Grades;

int main()
{
	int n,i,j;
	char *str;
	char *gender;
	int s;
	float *score;
	float temp;
	Grades *grad;
	Grades tempstruct;
	str=(char *)malloc(sizeof(char)*100);
	gender=(char *)malloc(sizeof(char)*10);
	fscanf(stdin,"%d ",&n);
	grad=(Grades*)malloc(sizeof(Grades)*n);
	score=(float *)malloc(sizeof(float)*n);
	printf("\n--Student List --\n");

	for(i=0;i<n;i++)
	{	
		gets(str);
		sscanf(str,"%d %*c %39[^/]  %*c %10[^/]  %*c %f  %*c %f %*c %f"
				,&(grad+i)->studentId,(grad+i)->name,gender,&(grad+i)->subject.mid,&(grad+i)->subject.final,&(grad+i)->subject.proj);
		*(score+i)=(((grad+i)->subject.mid)*0.3+((grad+i)->subject.final)*0.3+((grad+i)->subject.proj)*0.4);
	}
	if(strcmp(gender,"Male")==0)
		(grad+i)->sex=0;
	else if(strcmp(gender,"Female")==0)
		(grad+i)->sex=1;
	
	for(i=0;i<n;i++) //버블소트
	{
		for(j=i+1;j<n;j++)
		{
			if(*(score+i)<*(score+j))
			{
				temp=*(score+i);
				*(score+i)=*(score+j);
				*(score+j)=temp;
				tempstruct=*(grad+i);
				*(grad+i)=*(grad+j);
				*(grad+j)=tempstruct;
			}
		}
	}
	for(i=0;i<=n*30/100;i++)
		strcpy((grad+i)->grade,"A+");
	
	for(i=n*30/100;i<=n*70/100;i++)
		strcpy((grad+i)->grade,"B+");
	
	for(i=n*70/100;i<n;i++)
		strcpy((grad+i)->grade,"C+");
	
	for(i=0;i<n;i++)
	{
		if(*(score+i)<50)
			strcpy((grad+i)->grade,"F");
	}
    
	for(i=0;i<n;i++)
	{
		printf("\nId : %d\n",(grad+i)->studentId);
		printf("Name : %s",(grad+i)->name);
		if((grad+i)->sex==0)
			printf(" (Male)\n");
		else if((grad+i)->sex==1)
			printf(" (Female)\n");
		printf("Grade(mid) : %.2f\n",(grad+i)->subject.mid);
		printf("Grade(final) : %.2f\n",(grad+i)->subject.final);
		printf("Grade(project) : %.2f\n",(grad+i)->subject.proj);
		printf("Grade : %s(%.2f, %d)",(grad+i)->grade,*(score+i),i+1);
	}

		
	return 0;
}

