#include <stdio.h>

int main()
{
	float a,b,answer;
	float *x,*y;

	printf("Input a and b: ");
	scanf("%f %f",&a,&b);
	x=&a;
	y=&b;
	answer=(*x+*y)*(*x-*y)*7.2;
		printf("answer : %f\n",answer);
	return 0;
}