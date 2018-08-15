#include "my_solver.h"
#include <math.h>

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C, b;
double p11,p12,p13,p21,p22,p23,p31,p32,p33,p41,p42,p43;
double t1,tr1,t2,tr2,t3,tr3,t4,tr4;


 void fcn3_6( int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag )
{
	if( *iflag == 1 ){
		fvec[0] = pow(x[0]-p11,2)+pow(x[1]-p12,2)+pow(x[2]-p13,2)-pow(C*(tr1+x[3]-t1),2);
		fvec[1] = pow(x[0]-p21,2)+pow(x[1]-p22,2)+pow(x[2]-p23,2)-pow(C*(tr2+x[3]-t2),2);
		fvec[2] = pow(x[0]-p31,2)+pow(x[1]-p32,2)+pow(x[2]-p33,2)-pow(C*(tr3+x[3]-t3),2);
		fvec[3] = pow(x[0]-p41,2)+pow(x[1]-p42,2)+pow(x[2]-p43,2)-pow(C*(tr4+x[3]-t4),2);
		
	}
	else if( *iflag == 2 ){

		fjac[0] = 2*(x[0]-p11);
		fjac[1] = 2*(x[0]-p21);
		fjac[2] = 2*(x[0]-p31);
		fjac[3] = 2*(x[0]-p41);
		
		fjac[4] = 2*(x[1]-p12);
		fjac[5] = 2*(x[1]-p22);
		fjac[6] = 2*(x[1]-p32);
		fjac[7] = 2*(x[1]-p42);
		
		fjac[8] = 2*(x[2]-p13);
		fjac[9] = 2*(x[2]-p23);
		fjac[10] = 2*(x[2]-p33);
		fjac[11] = 2*(x[2]-p43);
		
		fjac[12] = -2*C*C*(tr1+x[3]-t1);
		fjac[13] = -2*C*C*(tr2+x[3]-t2);
		fjac[14] = -2*C*C*(tr3+x[3]-t3);
		fjac[15] = -2*C*C*(tr4+x[3]-t4);
	}
}


void program3_6(void)
{
	int i, n = SOLNUMS;
	double x[SOLNUMS] ;	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS+13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS+13)) / 2;
	

	FILE* fp_r[3];
	FILE* fp_w[3];

	char readfile[] = "GPS_signal_i.txt";
	char writefile[] = "GPS_position_3-1_i.txt";

	for(i=0;i<3;i++) {
		 readfile[11] = i+48;
		 writefile[17] = i+48;

		 fp_r[i] = fopen(readfile,"r");
		 fp_w[i] = fopen(writefile,"w");
	}

	for(i=0;i<3;i++) {

	
		fscanf(fp_r[i], "%lf %lf", &C, &b);
		fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p11,&p12,&p13,&t1,&tr1);
		fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p21,&p22,&p23,&t2,&tr2);
		fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p31,&p32,&p33,&t3,&tr3);
		fscanf(fp_r[i], "%lf %lf %lf %lf %lf",&p41,&p42,&p43,&t4,&tr4);
	
		scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
		x[3]= b;
		

		hybrj1_( fcn3_6, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
		fprintf(fp_w[i], "	   x		 y	       z	       b	         fvec[0]      fvec[1]  	 fvec[2]    fvec[3]\n");
		fprintf(fp_w[i],"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		fprintf(fp_w[i], "%.6lf     %.6lf     %.6lf     %.6lf\t %.6lf   %.6lf   %.6lf   %.6lf\n",x[0], x[1], x[2], x[3], fvec[0], fvec[1], fvec[2], fvec[3]);
	}



	
}