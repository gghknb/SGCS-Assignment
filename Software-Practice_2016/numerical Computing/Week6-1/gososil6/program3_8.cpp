#include "my_solver.h"
#include <math.h>

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C_2, b_2;
double p11_2,p12_2,p13_2,p21_2,p22_2,p23_2,p31_2,p32_2,p33_2,p41_2,p42_2,p43_2;
double t1_2,tr1_2,t2_2,tr2_2,t3_2,tr3_2,t4_2,tr4_2;

void fcn3_7( int *n, double *x, double *fvec, int *iflag )
{	
		fvec[0] = pow(x[0]-p11_2,2)+pow(x[1]-p12_2,2)+pow(x[2]-p13_2,2)-pow(C_2*(tr1_2+x[3]-t1_2),2);
		fvec[1] = pow(x[0]-p21_2,2)+pow(x[1]-p22_2,2)+pow(x[2]-p23_2,2)-pow(C_2*(tr2_2+x[3]-t2_2),2);
		fvec[2] = pow(x[0]-p31_2,2)+pow(x[1]-p32_2,2)+pow(x[2]-p33_2,2)-pow(C_2*(tr3_2+x[3]-t3_2),2);
		fvec[3] = pow(x[0]-p41_2,2)+pow(x[1]-p42_2,2)+pow(x[2]-p43_2,2)-pow(C_2*(tr4_2+x[3]-t4_2),2);
}

void program3_7(void)
{
	int i, n = SOLNUMS;
	double x[SOLNUMS];	//need to initilize x0
	double fvec[SOLNUMS];	
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 *SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 *SOLNUMS + 13)) / 2;	FILE* fp_r[3];
	FILE* fp_w[3];

	char readfile[] = "GPS_signal_i.txt";
	char writefile[] = "GPS_position_3-2_i.txt";
	for(i=0;i<3;i++) {
		 readfile[11] = i+48;
		 writefile[17] = i+48;

		 fp_r[i] = fopen(readfile,"r");
		 fp_w[i] = fopen(writefile,"w");
	}
	for(i=0 ; i<3 ; i++)
	{


	fscanf(fp_r[i], "%lf %lf", &C_2, &b_2);
	fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p11_2,&p12_2,&p13_2,&t1_2,&tr1_2);
	fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p21_2,&p22_2,&p23_2,&t2_2,&tr2_2);
	fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p31_2,&p32_2,&p33_2,&t3_2,&tr3_2);
	fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p41_2,&p42_2,&p43_2,&t4_2,&tr4_2);
	
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	x[3] = b_2;
		

	hybrd1_( fcn3_7, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w[i], "	   x		 y	       z	       b	      fvec[0]  fvec[1]	 fvec[2]  fvec[3]\n");
	fprintf(fp_w[i],"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	fprintf(fp_w[i], "%.6lf %.6lf %.6lf %.6lf\t %.6lf %.6lf %.6lf %.6lf\n",x[0], x[1], x[2], x[3], fvec[0], fvec[1], fvec[2], fvec[3]);
	

	
	}
}