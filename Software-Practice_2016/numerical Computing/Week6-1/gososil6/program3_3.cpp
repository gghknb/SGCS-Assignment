#include "my_solver.h"
#include <math.h>

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_3( int *n, double *x, double *fvec, int *iflag )
{	
	fvec[0] = 10.0*x[0] - 2.0*x[1]*x[1] + x[1] -2.0*x[2] - 5.0;
	fvec[1] = 8.0*x[1]*x[1] + 4.0*x[2]*x[2] - 9.0;
	fvec[2] = 8.0*x[1]*x[2] + 4.0;	
}

void program3_3(void)
{
	//TODO
	int n = SOLNUMS;
	double *x ,*x2;
	double *fvec;
	double tol;
	int info;
	double *wa;
	int lwa;
	FILE *fp_w = fopen("roots_3-3.txt","w");
	if(fp_w == NULL) 
	{
		printf("file open error...");
		return;
	}	
	x = (double *)malloc(sizeof(double) * n);
	x2 = (double *)malloc(sizeof(double) * n);
	fvec = (double *)malloc(sizeof(double) * n);
	tol = TOLERANCE;
	lwa = (n*(3*n+13))/2;
	wa = (double *)malloc(sizeof(double) * lwa);

	x[0] = 1.0; x[1] = -1.0; x[2] = 1.0;
	hybrd1_(fcn3_3,&n,x,fvec,&tol,&info,wa,&lwa);
	fprintf(fp_w,"초기값1\n");
	fprintf(fp_w,"\tx1\tx2\tx3\n");
	fprintf(fp_w,"x값 : %10f  %10f  %10f\n",x[0],x[1],x[2]);
	fprintf(fp_w,"fx값 : %10f %10f %10f\n",fvec[0],fvec[1],fvec[2]);
	x2[0] = 1.0;  x2[1] = 1.0; x2[2] = -1.0; 
	hybrd1_(fcn3_3,&n,x2,fvec,&tol,&info,wa,&lwa);
	fprintf(fp_w,"초기값2\n");
	fprintf(fp_w,"\tx1\tx2\tx3\n");
	fprintf(fp_w,"x값 : %10f  %10f  %10f\n",x2[0],x2[1],x2[2]);
	fprintf(fp_w,"fx값 : %10f %10f %10f\n",fvec[0],fvec[1],fvec[2]);
	
}