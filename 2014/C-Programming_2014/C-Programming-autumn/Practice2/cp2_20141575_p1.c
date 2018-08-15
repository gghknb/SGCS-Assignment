#include <stdio.h>

void calAddSub(int num1,int num2,int *add,int *sub);

int main()
{
	int num1,num2;
	int add,sub;
	printf("Enter the 1st number : ");
	scanf("%d",&num1);
	printf("Enter the 2nd number : ");
	scanf("%d",&num2);

	calAddSub(num1,num2,&add,&sub);
	printf("sum of %d and %d is %d\n",num1,num2,add);
	printf("diff of %d and %d is %d\n",num1,num2,sub);


	return 0;
}

void calAddSub(int num1,int num2,int *add,int *sub)
{
	*add=num1+num2;

	if(num2>num1)
	*sub=num2-num1;
	else *sub=num1-num2;
}
