#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Secant Method
**********************************************/
void program1_2(FILE *fp)
{
	double x0, x1;
	double temp;
	int n;
	fprintf(fp, "\t\t      Secant Method\n");
	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf %lf", &x0, &x1);

	for(n=0; ;n++) {
		fprintf(fp, "%2d  %20.18e  %12.10e\n ", n, x1, fabs(_F(x1)));
		temp = x1 - _F(x1)*(x1-x0)/(_F(x1)-_F(x0));
		x0 = x1;
		x1 = temp;

		
		if (fabs(_F(x1)) < DELTA || fabs(x1 - x0) < EPSILON || n >= Nmax)
		{
			fprintf(fp, "%2d  %20.18e  %12.10e\n", ++n, x1, fabs(_F(x1)));
			fprintf(fp, "\t\t종료조건\n");
			if (fabs(_F(x1)) < DELTA)
				fprintf(fp, "\t f(xn+1) =  %12.10e\n", fabs(_F(x1)));
			if (fabs(x1 - x0) < EPSILON)
				fprintf(fp, "\t xn+1 - xn = %20.18e\n", fabs(x1 - x0));
			if (n >= Nmax)
				fprintf(fp, "\t n= %d", n);

			break;
		}

	
		// TODO
	}
	printf("%2d  %20.18e  %12.10e\n", n, x0, fabs(_F(x0)));
}