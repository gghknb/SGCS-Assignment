#include "my_solver.h"
#include <math.h>

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_2( int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag )
{
	// origin function F(x)
	if( *iflag == 1 ){
		//TODO
		fvec[0] = x[0] + x[1] + x[2] - 3;
		fvec[1] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] - 5;
		fvec[2] = exp(x[0]) + x[0]*x[1] - x[0]*x[2] - 1;
	}
	// Jacobi matrix J(x)
	else if( *iflag == 2 ){
		//TODO
		fjac[0] = 1.0;						fjac[3] = 1.0;		fjac[6] = 1.0;
		fjac[1] = 2.0*x[0];					fjac[4] = 2.0*x[1];	fjac[7] = 2.0*x[2];
		fjac[2] = exp(x[0]) + x[1] - x[2];	fjac[5] = x[0];		fjac[8] = -x[0];
	}
}

void program3_2(void)
{
	int n = SOLNUMS;
	double *x ,*x2;
	double *fvec;
	double *fjac = new double[MATCOLS*MATROWS];
	int ldfjac = MATCOLS;
	double tol;
	int info;
	double *wa;
	int lwa;
	FILE *fp_w = fopen("roots_3-2.txt","w");
	if(fp_w == NULL) 
	{
		printf("file open error...");
		return;
	}	
	x = (double *)malloc(sizeof(double) * n);
	x2 = (double *)malloc(sizeof(double) * n);
	fvec = (double *)malloc(sizeof(double) * n);
	tol = TOLERANCE;
	lwa = (n*(n+13))/2;
	wa = (double *)malloc(sizeof(double) * lwa);

	x[0] = 0.1; x[1] = 1.2; x[2] = 2.5;
	hybrj1_(fcn3_2,&n,x,fvec,fjac,&ldfjac,&tol,&info,wa,&lwa);
	fprintf(fp_w,"초기값1\n");
	fprintf(fp_w,"\tx1\tx2\tx3\n");
	fprintf(fp_w,"x값 : %10f  %10f  %10f\n",x[0],x[1],x[2]);
	fprintf(fp_w,"fx값 : %10f %10f %10f\n",fvec[0],fvec[1],fvec[2]);
	x2[0] = 1.0;  x2[1] = 0.0; x2[2] = 1.0; 
	hybrj1_(fcn3_2,&n,x2,fvec,fjac,&ldfjac,&tol,&info,wa,&lwa);
	fprintf(fp_w,"초기값2\n");
	fprintf(fp_w,"\tx1\tx2\tx3\n");
	fprintf(fp_w,"x값 : %10f  %10f  %10f\n",x2[0],x2[1],x2[2]);
	fprintf(fp_w,"fx값 : %10f %10f %10f\n",fvec[0],fvec[1],fvec[2]);

}