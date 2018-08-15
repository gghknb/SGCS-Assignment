#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double x0 = DBL_MAX;
	double a0, b0, x1, temp;
	int n;
	fprintf(fp,"\t\t      Bisection Method\n");
	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf %lf", &a0, &b0);
	for(n=0; ;n++){
		x1 = (a0 + b0) / 2.0;
		temp = _F(x1);
		fprintf(fp, "%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
			if (_F(a0) * temp > 0)
			{
				a0 = x1;
			}
			else if (_F(a0) * temp < 0)
			{
				b0 = x1;
			}

			if (fabs(temp) < DELTA || n >= Nmax || fabs(b0 - a0) < EPSILON)
			{
				fprintf(fp, "\t\t종료조건\n");
				if (fabs(_F(temp)) < DELTA)
					fprintf(fp, "\t f(xn+1) =  %12.10e\n", fabs(_F(temp)));
				if (fabs(b0 - a0) < EPSILON)
					fprintf(fp, "\t xn+1 - xn = %20.18e\n", fabs(b0 - a0));
				if (n >= Nmax)
					fprintf(fp, "\t n= %d", n);
				break;
			}
		// TODO

	}
	printf("%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
}