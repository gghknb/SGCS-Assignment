#include <stdio.h>

int main()
{
	int a,b;
	int *x,*y;
	printf("Input 2 integers : ");
	scanf("%d %d",&a,&b);	
	x=&a;
	y=&b;
	if(a<b)
	{
		x=&b;
		y=&a;
	}
	
	
	printf("smaller value is %d\n",*y);
	printf("%d is stored at address %d\n",*y,y);
	printf("bigger value is %d\n",*x);
	printf("%d is stored at address %d",*x,x);
	return 0;

}
	