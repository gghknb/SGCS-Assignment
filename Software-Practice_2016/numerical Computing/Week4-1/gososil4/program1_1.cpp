#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE *fp)
{
	double x0 = DBL_MAX;
	double x1;
	int n;

	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf", &x1);

	for(n=0; ;n++) {		
		fprintf(fp, "%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
		double x2 = x1 - _F(x1)/_FP(x1);
		if(fabs(_F(x2)) < DELTA || n>= Nmax || fabs(x2 - x1) < EPSILON)
				break;
		x1 = x2;
		// TODO
	}
	printf("%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
}