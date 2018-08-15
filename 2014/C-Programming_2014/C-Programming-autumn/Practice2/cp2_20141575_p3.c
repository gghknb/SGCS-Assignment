#include <stdio.h>

void *min(void *ptr1,void *ptr2,char option);

int main()
{
	int num1,num2;
	float num3,num4;
	char option;
	void *minimum;
	printf("Enter the type of number : ");
	scanf("%c",&option);
	if(option=='i')
	{
		printf("Enter the 1st number : ");
		scanf("%d",&num1);
		printf("Enter the 2nd number : ");
		scanf("%d",&num2);
		minimum=min(&num1,&num2,option);
		printf("\n\n\nminimum value is %d\n",*((int *)minimum));
	}
	else if(option=='f')
	{
		printf("Enter the 1st number : ");
		scanf("%f",&num3);
		printf("Enter the 2nd number : ");
		scanf("%f",&num4);
		minimum=min(&num3,&num4,option);
		printf("\n\n\nminimum value is %f\n",*((float *)minimum));
	}
	return 0;
}
void *min(void *ptr1,void *ptr2,char option)
{
		if(option=='i')
		{
			if(*(int *)ptr1>*(int *)ptr2)
				return (int *)ptr2;
			else
				return (int *)ptr1;
		}

		else if(option=='f')
		{
			
			if(*(float *)ptr1>*(float *)ptr2)
				return (float *)ptr2;
			else
				return (float *)ptr1;
		}
}
