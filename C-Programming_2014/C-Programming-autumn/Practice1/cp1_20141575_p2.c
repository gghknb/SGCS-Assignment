#include <stdio.h>

int main()
{
	float a,b;
	float *x,*y;

	printf("Input a and b : ");
	scanf("%f %f",&a,&b);
	x=&a;
	y=&b;
	printf("\n");
	printf("%f + %f = %f\n",*x,*y,*x+*y);
	printf("%f - %f = %f\n",*x,*y,*x-*y);
	printf("%f * %f = %f\n",*x,*y,(*x)*(*y));
	printf("%f / %f = %f\n",*x,*y,(*x)/(*y));
	return 0;
}
