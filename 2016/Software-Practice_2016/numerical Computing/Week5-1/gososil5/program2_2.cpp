#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

// global variables
const double DELTA = 0.000000000001;
const int Nmax = 100;
const double EPSILON = 0.00000000001;
double Fx(int num,double *X,double *Y_arr,double x);
double Fx1(int num,double *X,double *Y_arr,double x);
double fx(int num,double u,double a0,double b0,double *X_arr,double *Y_arr);

void program2_2()
{
	FILE *fp_r, *fp_w;
	int U_num,i;
	int num;
	double interval;
	double *U,*X;
	double X_arr[100],Y_arr[100];
	
	fp_r = fopen("pdf.txt", "r");
	fscanf(fp_r,"%d %lf",&num,&interval);
	for(i = 0 ; i < num ;i ++)
	{
		fscanf(fp_r,"%lf %lf",&X_arr[i],&Y_arr[i]);
	}

	fp_w = fopen("pdf_and_rand.txt", "w");
	printf("nr의 개수를 입력 : ");
	scanf("%d",&U_num);
	U = (double *)malloc(sizeof(double)*U_num);
	X = (double *)malloc(sizeof(double)*U_num);

	for( i = 0 ; i < U_num ; i++)
	{
		U[i] = (double)rand()/ RAND_MAX;
	}
	
	for( i = 0 ; i < U_num ; i++)
	{
		X[i] = fx(num,U[i],X_arr[0],X_arr[num-1],X_arr,Y_arr);
	}


	fprintf(fp_w,"%d\n",U_num);
	for(i = 0 ; i < U_num ; i++)
	{
		fprintf(fp_w,"%.15lf\n",X[i]);
	}
	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}

double Fx(int num,double *X_arr,double *Y_arr,double x)
{
	int i;
	double area = 0;

	if( x == 0.0)
	{
		return 0; 
	}
	for(i = 0 ; i < num ; i++)
	{
		if(X_arr[i] <= x)
		{
			area += Y_arr[i] *(X_arr[1] - X_arr[0]);
		}
	}
	area -= Y_arr[0] / num / 2;
	return area;
}

double fx(int num,double u,double a0,double b0,double *X_arr,double *Y_arr)
{
	double x1;
	int n;
	double temp,temp2,temp3;
	double ratio;
	for(n = 0 ; ;n++)
	{
		x1 = (a0 + b0)/2.0;
		temp = Fx(num,X_arr,Y_arr,x1) - u;
		temp2 = Fx(num,X_arr,Y_arr,a0) - u;	
		temp3 = Fx(num,X_arr,Y_arr,b0) - u;
		if( temp2*temp > 0 )
			a0 = x1;
		else if(temp3*temp > 0)
			b0 = x1;
		if (fabs(temp) < DELTA || fabs(b0 - a0) < EPSILON)
			break;
		
	}
	
	return x1;
}