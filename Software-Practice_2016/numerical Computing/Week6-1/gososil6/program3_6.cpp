#include "my_solver.h"
#include <math.h>

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_5( int *n, double *x, double *fvec, int *iflag )
{	
	fvec[0] = 2.0*x[0]*x[0]*x[0] - 12.0*x[0] - x[1] - 1.0;
	fvec[1] = 3.0*x[1]*x[1] - 6.0*x[1] - x[0] - 3.0;		
}

void program3_5(void)
{
	//TODO
	FILE *fp_w = fopen("roots_3-5.txt", "w");
	if(fp_w == NULL) 
	{
		printf("roots_3-5.txt file open error...\n");
		return;
	}	

	double i,j;

	int n = SOLNUMS,info;
	double x[SOLNUMS], fvec[SOLNUMS], tol = TOLERANCE;
	double wa[(SOLNUMS * (3 *SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 *SOLNUMS + 13)) / 2;

	for(i=-4;i<=4;i+=0.5) {
		for(j=-5;j<=5;j+=0.5) {
			x[0] = i; x[1] = j;
			hybrd1_( fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);
			fprintf(fp_w,"�ʱⰪ : %lf %lf ",i,j);
			fprintf(fp_w," (x,y) = %lf %lf ", x[0], x[1]);
			fprintf(fp_w,"       %lf %lf\n", fvec[0], fvec[1]);	
		}
	}


	if(fp_w != NULL) fclose(fp_w);
}