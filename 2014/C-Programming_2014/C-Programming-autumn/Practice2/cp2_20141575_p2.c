#include <stdio.h>
void swap(char **str1,char **str2);

int main()
{
	char *str1="I love vayne";
	char *str2="Even if she is troll";
	
	printf("Before swapping \n");
	printf("\nstr1= %s",str1);
	printf("\nstr2= %s\n",str2);
	swap(&str1,&str2);
	printf("After swapping\n");
	printf("\nstr1 = %s",str1);
	printf("\nstr2 = %s",str2);
	return 0;
}

void swap(char **str1,char **str2)
{
	char *temp;
	temp=*str1;
	*str1=*str2;
	*str2= temp;
	
}
