#include "my_solver.h"

extern double (*_F)(double);
extern double (*_FP)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE *fp)
{
	double x0 = DBL_MAX;
	double x1,x2;
	int n;
	fprintf(fp, "\t\t      New-Rapson Method\n");
	fprintf(fp, " n              xn1                  |f(xn1)|\n");
	scanf("%lf", &x1);

	for(n=0; ;n++) {		
		fprintf(fp, "%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));
	
		x2 = x1 - _F(x1) / _FP(x1);
		
		if (fabs(_F(x2)) < DELTA || fabs(x2 - x1) < EPSILON || n >= Nmax)
		{
			fprintf(fp, "%2d  %20.18e  %12.10e\n", ++n, x2, fabs(_F(x2)));
			fprintf(fp,"\t\t종료조건\n");
			if (fabs(_F(x2)) < DELTA)
				fprintf(fp, "\t f(xn+1) =  %12.10e\n", fabs(_F(x2)));
			if (fabs(x2 - x1) < EPSILON)
				fprintf(fp, "\t xn+1 - xn = %20.18e\n", fabs(x2 - x1));
			if (n >= Nmax)
				fprintf(fp, "\t n= %d", n);
			break;
		}
	
		x1 = x2;
		// TODO
	}
	printf("%2d  %20.18e  %12.10e\n", n, x1, fabs(_F(x1)));

}