#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char dept_name[25];
	int dept_no;
}dept;

enum Gender {Male=0,Female};

typedef struct{
	int employee_id;
	char name[25];
	enum Gender gender;
	dept department;
	double salary;
}employee_data;

int search(employee_data *e,int num,int keyword);
employee_data update(employee_data e,int n);

int main()
{
	int num,i,id,check,depnum;
	FILE *fp;
	char *str;
	char *gen;
	char con;
	employee_data *data;
	str=(char *)malloc(sizeof(char)*200);
	gen=(char *)malloc(sizeof(char)*7);
	fp=fopen("input.txt","r");
	
	fscanf(fp,"%d",&num);	
	data=(employee_data *)malloc(sizeof(employee_data)*num);
	
	for(i=0;i<num+1;i++)
	{
		fgets(str,200,fp);
		sscanf(str,"%d %*c %25[^/] %*c %6[^ ] %*c %10[^ ] %*c %d %*c %lf",
				&(data+i)->employee_id,(data+i)->name,gen,
				(data+i)->department.dept_name,&(data+i)->department.dept_no,&(data+i)->salary);
		if(strcmp(gen,"Male")==0)
			(data+i)->gender=Male;
		else if(strcmp(gen,"Female")==0)
			(data+i)->gender=Female;
	}
	
	while(1)
	{
		printf("Employee id : ");
		scanf("%d",&id);
		for(i=0;i<num;i++)
		{
			if(search((data+i),num,id)==0)
				break;
		}
		if(search((data+i),num,id)==0)
		{
			printf("------Employee data------\n");
			printf("%d / %s /",(data+i)->employee_id,(data+i)->name);
			if((data+i)->gender=Male)
				printf(" Male /");
			else printf(" Female /");
			printf(" %s / %d / %.2lf\n",(data+i)->department.dept_name,(data+i)->department.dept_no,(data+i)->salary);
			printf("Update information\n");
			printf("Input the department number : ");
			
			scanf("%d",&depnum);
			*(data+i)=update(*(data+i),depnum);
			printf("---Updated employee data---\n");
			printf("%d / %s /",(data+i)->employee_id,(data+i)->name);
			if((data+i)->gender=Male)
				printf(" Male /");
			else printf(" Female /");
			printf(" %s / %d / %.2lf\n",(data+i)->department.dept_name,(data+i)->department.dept_no,(data+i)->salary);
			printf("-------------------------\n");
		}
		else if(search((data+i),num,id)==1)
		{
			printf("No employee\n");
		}
		printf("continue?(y/n)");
		getchar();
		scanf("%c",&con);
		if(con=='y')
			continue;
		else break;

	}
	return 0;
}

int search(employee_data *e,int num,int keyword)
{
	if(keyword == e->employee_id)
		return 0;
	else return 1;
}

employee_data update(employee_data e,int n)
{
	if(n==1)
	{
		e.department.dept_no=1;
		strcpy(e.department.dept_name,"FINANCE");
	}
	else if(n==2)
	{
		e.department.dept_no=2;
		strcpy(e.department.dept_name,"SALES");
	}
	else if(n==3)
	{
		e.department.dept_no=3;
		strcpy(e.department.dept_name,"RESEARCH");
	}
	else if(n==4)
	{
		e.department.dept_no=4;
		strcpy(e.department.dept_name,"PLANNING");
	}
	return e;
}
