#include <stdio.h>

int main()
{
	int x,y,z,a,b,c;
	printf("Enter 2 numbers: ");
    scanf("%d %d",&a,&b);
    if(a%10+b%10>=10)	
		x=1;
	else
	  x=0;
	if((a%100)+(b%100)>=100)
	   y=1;
	else
		y=0;
	if(a+b>=1000)
		z=1;
	else
		z=0;
    c=x+y+z;
	printf("%d carry operations\n",c);
	return 0;
}