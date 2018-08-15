#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void bubblesort(char **name,char **gender,int *grade,int num);

int main(){
		int i,num,average;
		char **name;
		char **gender;
		int  *grade;
		float score=0;
		char result[100];
		char str[100];
		fscanf(stdin,"%d ",&num);
		name=(char **)malloc(sizeof(char*)*num);
		gender=(char **)malloc(sizeof(char*)*num);
		grade=(int *)malloc(sizeof(int)*num);
	
		for(i=0;i<num;i++)
		{
			*(name+i)=(char *)malloc(sizeof(char)*31);
			*(gender+i)=(char *)malloc(sizeof(char)*7);
		
		}
	
		for(i=0;i<num;i++){
			gets(str);
			sscanf(str,"%30[^/] %*c %6[^/] %*c %d",*(name+i),*(gender+i),(grade+i));
			score+=*(grade+i);
		}
	
		bubblesort(name,gender,grade,num);
		printf("-----Student List-----\n");
		for(i=0;i<num;i++){
			printf("%d /%s /%s /%d \n",20140001+i,*(name+i),*(gender+i),*(grade+i));
		}
		
		printf("----------------------\naverage = %f\n",score/(num));
		return 0;
}
void bubblesort(char **name,char **gender,int *grade,int num)
{
	int i,j;
	char *temp_name;
	int temp_grade;
	char *temp_gender;
	temp_name=(char *)malloc(sizeof(char)*31);
	temp_gender=(char *)malloc(sizeof(char)*7);
	for(i=0;i<num;i++){
		for(j=i+1;j<num;j++){
			if(strcmp(*(name+i),*(name+j))>0){
				strcpy(temp_name,*(name+i));
				strcpy(*(name+i),*(name+j));
				strcpy(*(name+j),temp_name);
				strcpy(temp_gender,*(gender+i));
				strcpy(*(gender+i),*(gender+j));
				(*(gender+j),temp_gender);
				temp_grade=*(grade+i);
				*(grade+i)=*(grade+j);
				*(grade+j)=temp_grade;
			}
		}
	}
}

	

