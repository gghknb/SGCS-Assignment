#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#include <float.h>

__int64 start, freq, end; 
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start) 
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
#define MATDIM 1024
#define DEGREE 10
#define N_X 1048576
double *pMatA, *pMatB, *pMatC, *tempA,*tempB,*a, *x ,*y,*y2,*a3,*x3,*y3;
float *sample_data;
double *sample_data2;
void init_MatMat(void);
void init_poly(void);
void init_taylor(int n);
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize);
void Eval_Poly_Naive(double y[], double x[], int n_x, double a[],int deg);
void Eval_Poly_Horner(double y[], double x[], int n_x, double a[],int deg);
double taylor_poly(double x,int n);
float get_mean(int n,float x[]);
float get_var1(int n,float x[]);
float get_var2(float mean,int n,float x[]);
double get_mean_double(int n,double x[]);
double get_var1_double(int n,double x[]);
double get_var2_double(double mean,int n,double x[]);

int main()
{

	float time_check;
	int wait;
	int n;
	double t,k;
	float mean,var1,var2;
	double mean2,var3,var4;
	/* 실습 1 */
	init_MatMat();

	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(time_check);
	printf("실습 1 - i = %fsec value 512 : %lf\n",time_check/1000,pMatC[512]);
	
	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(time_check);
	printf("실습 1 - ii = %fsec value 512 : %lf\n",time_check/1000,pMatC[512]);

	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(time_check);
	printf("실습 1 - iii = %fsec value 512 : %lf\n",time_check/1000,pMatC[512]);
	
	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(time_check);
	printf("실습 1 - iv = %fsec value 512 : %lf\n",time_check/1000,pMatC[512]);

	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(time_check);
	printf("실습 1 - v = %fsec value 512 : %lf\n",time_check/1000,pMatC[512]);
	 
	/* 실습 2 */
	init_poly();
	CHECK_TIME_START;
	Eval_Poly_Naive(y,x,N_X, a,DEGREE);
	CHECK_TIME_END(time_check);
	printf("Eval_poly_Naive = %fsec\n",time_check/1000);

	CHECK_TIME_START;
	Eval_Poly_Horner(y2,x,N_X, a,DEGREE);
	CHECK_TIME_END(time_check);
	printf("Eval_poly_Horner = %fsec\n",time_check/1000);
	
	for(int i = 0 ; i < N_X ; i++)
	{
		if(abs(y[i] - y2[i]) > DBL_EPSILON)
		{
			printf("%d 번째가 다르다! y[%d] = %lf, y2[%d] = %lf\n",i,i,y[i],i,y2[i]);
			break;
		}
	}
	/* 실습 3 */
	t = -8.3;
	k = taylor_poly(t,24);
	printf("***25개 항f<-8.3> = %e\n",k);
/*	k = taylor_poly(t,29);
	printf("***30개 항f<-8.3> = %e\n",k);
	k = taylor_poly(t,34);
	printf("***35개 항f<-8.3> = %e\n",k);
	k = taylor_poly(t,39);
	printf("***40개 항f<-8.3> = %e\n",k);*/
	k = taylor_poly(t,99);
	printf("***100개 항f<-8.3> = %e\n",k);


	/* 과제 1 */
	/*float */
	printf("과제1\n데이터의 개수를 입력하시오 n : ");
	scanf("%d",&n);
	sample_data = (float *)malloc(sizeof(float)*n);
	srand((unsigned) time(NULL));
	for (int i = 0; i < n; i++)
		sample_data[i] = (float) rand();

	mean = get_mean(n,sample_data);
	printf("------float data------\n");
	printf("mean = %f\n",mean);
	CHECK_TIME_START;
	var1 = get_var1(n,sample_data);
	CHECK_TIME_END(time_check);
	printf("variance1 = %f time = %fsec\n",var1,time_check/1000);
	CHECK_TIME_START;
	var2 = get_var2(mean,n,sample_data);
	CHECK_TIME_END(time_check);
	printf("variance2 = %f time = %fsec\n",var2,time_check/1000);
	/* double */
	sample_data2 = (double *) malloc(sizeof(double)*n);
	srand((unsigned) time(NULL));
	for (int i = 0; i < n; i++)
		sample_data2[i] = (double) rand();
	mean2 = get_mean_double(n,sample_data2);
	printf("------double data------\n");
	printf("mean = %lf\n",mean2);
	CHECK_TIME_START;
	var3 = get_var1_double(n,sample_data2);
	CHECK_TIME_END(time_check);
	printf("variance1 = %lf time = %fsec\n",var3,time_check/1000);
	CHECK_TIME_START;
	var4 = get_var2_double(mean2,n,sample_data2);
	CHECK_TIME_END(time_check);
	printf("variance2 = %lf time = %fsec\n",var4,time_check/1000);
	scanf("%d\n",&wait);
	return 0;


}

void init_MatMat(void) {
	double *ptr;
    
	pMatA = (double *) malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *) malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *) malloc(sizeof(double)*MATDIM*MATDIM);
	tempA = (double *) malloc(sizeof(double)*MATDIM*MATDIM);
	tempB = (double *) malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned) time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double) rand()/((double) RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double) rand()/((double) RAND_MAX);
	
}

void init_poly(void) 
{
	double *ptr;
	a =  (double *) malloc(sizeof(double)*(DEGREE+1));
	x =  (double *) malloc(sizeof(double)*(N_X));
	y =  (double *) malloc(sizeof(double)*(N_X));
	y2 =  (double *) malloc(sizeof(double)*(N_X));
	srand((unsigned) time(NULL));
	ptr = a;
	for (int i = 0; i < DEGREE+1; i++)
		*ptr++ = (double) rand()/((double) RAND_MAX);
	ptr = x;
	for (int i = 0; i < N_X; i++)
		*ptr++ = (double) rand()/((double) RAND_MAX);
}
/*
void init_taylor(int n)
{
	double *ptr;
	a3 =  (double *) malloc(sizeof(double)*(n));
	x3 =  (double *) malloc(sizeof(double)*(1));
	y3 =  (double *) malloc(sizeof(double)*(1));
	ptr = a3;
	a3[0] = 1;
	for (int i = 1; i < DEGREE+1; i++)
		a3[i] = 1/i;
	x3[0] = -8.3;

}
*/

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize)
{
	int i,j,k;

	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ;j < MatSize ; ++j)
		{
			pDestMatrix[i*MatSize+j] = 0;
			for(k = 0 ; k < MatSize ; ++k)
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightMatrix[k * MatSize + j];
		}
	}
	return ;
}

void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize)
{
	int i,j,k;
	/* transpose */ 
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ; j < MatSize ; ++j)
		{
			 tempB[i * MatSize + j] = pRightMatrix[j * MatSize + i];
		}
	}
	/* multiple */
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ;j < MatSize ; ++j)
		{
			pDestMatrix[i*MatSize+j] = 0;
			for(k = 0 ; k < MatSize ; ++k)
				pDestMatrix[i*MatSize + j] += pLeftMatrix[i*MatSize + k] * tempB[j*MatSize + k];
		}
	}
	return ;
}

void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize)
{
	int i,j,k;
	/* transpose */ 
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ; j < MatSize ; ++j)
		{
			 tempB[i * MatSize + j] = pRightMatrix[j * MatSize + i];
		}
	}
	/* multiple */
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ;j < MatSize ; ++j)
		{
			pDestMatrix[i*MatSize+j] = 0;
			for(k = 0 ; k < MatSize ; k = k + 4)
			{
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k] * tempB[j * MatSize + k];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+1] * tempB[j * MatSize + k+1];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+2] * tempB[j * MatSize + k+2];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+3] * tempB[j * MatSize + k+3];
			}
		}
	}
	return ;
}

void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize)
{
	int i,j,k;
	/* transpose */ 
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ; j < MatSize ; ++j)
		{
			 tempB[i * MatSize + j] = pRightMatrix[j * MatSize + i];
		}
	}
	/* multiple */
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ;j < MatSize ; ++j)
		{
			pDestMatrix[i*MatSize+j] = 0;
			for(k = 0 ; k < MatSize ; k = k + 8)
			{
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k] * tempB[j * MatSize + k];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+1] * tempB[j * MatSize + k+1];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+2] * tempB[j * MatSize + k+2];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+3] * tempB[j * MatSize + k+3];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+4] * tempB[j * MatSize + k+4];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+5] * tempB[j * MatSize + k+5];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+6] * tempB[j * MatSize + k+6];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+7] * tempB[j * MatSize + k+7];

			}
		}
	}
	return ;
}

void MultiplySquareMatrices_5(double *pDestMatrix, double *pLeftMatrix,double *pRightMatrix, int MatSize)
{
	int i,j,k;
	/* transpose */ 
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ; j < MatSize ; ++j)
		{
			 tempB[i * MatSize + j] = pRightMatrix[j * MatSize + i];
		}
	}
	/* multiple */
	for(i = 0 ; i < MatSize ; ++i)
	{
		for(j = 0 ;j < MatSize ; ++j)
		{
			pDestMatrix[i*MatSize+j] = 0;
			for(k = 0 ; k < MatSize ; k = k + 16)
			{
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k] * tempB[j * MatSize + k];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+1] * tempB[j * MatSize + k+1];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+2] * tempB[j * MatSize + k+2];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+3] * tempB[j * MatSize + k+3];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+4] * tempB[j * MatSize + k+4];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+5] * tempB[j * MatSize + k+5];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+6] * tempB[j * MatSize + k+6];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+7] * tempB[j * MatSize + k+7];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+8] * tempB[j * MatSize + k+8];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+9] * tempB[j * MatSize + k+9];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+10] * tempB[j * MatSize + k+10];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+11] * tempB[j * MatSize + k+11];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+12] * tempB[j * MatSize + k+12];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+13] * tempB[j * MatSize + k+13];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+14] * tempB[j * MatSize + k+14];
				pDestMatrix[i * MatSize +j] += pLeftMatrix[i * MatSize + k+15] * tempB[j * MatSize + k+15];
			}
		}
	}
	return ;
}

void Eval_Poly_Naive(double y[], double x[], int n_x, double a[],int deg)
{
	int i,j;

	for(i = 0; i < n_x ; ++i)
	{
		y[i] = 0;
		for(j = 0 ; j < deg + 1 ; j++)
			y[i] += a[j] * pow(x[i],j);
	}
	return ;
}


void Eval_Poly_Horner(double y[], double x[], int n_x, double a[],int deg)
{
	int i,j;
	for(i = 0; i < n_x ; ++i)
	{
		y[i] = 0;
		for(j = 0 ; j < deg; ++j)
		{
			y[i] += a[deg-j];
			y[i] *= x[i];
		/*if(j != deg)
				y[i] *= x[i];*/
		}
		y[i] += a[0];
	}
	return ;
}

double taylor_poly(double x,int n)
{
	double result = 1;
	for(int i = n ; i > 0 ; i--)
	{
		result *= x/(double)i;
		result += 1;
	}
	return result;
}

float get_mean(int n,float xx[])
{
	float sum = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += xx[i];

	sum /= (float)n;
	return sum;
}
float get_var1(int n,float xx[])
{
	float sum = 0;
	float sum2 = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += xx[i] * xx[i];
	sum *= n;
	for(i = 0 ; i < n ; i++)
		sum2 += xx[i];
	sum2 *= sum2;
	sum -= sum2;
	sum /= (float)(n*(n-1));

	return sum;
}
float get_var2(float mean,int n,float xx[])
{
	float sum = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += (xx[i] - mean)*(xx[i] - mean);
	sum /= (float)(n-1);

	return sum;
}

double get_mean_double(int n,double xx[])
{
	double sum = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += xx[i];
	sum /= (double)n;
	return sum;
}
double get_var1_double(int n,double xx[])
{
	double sum = 0;
	double sum2 = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += xx[i] * xx[i];
	sum *= n;
	for(i = 0 ; i < n ; i++)
		sum2 += xx[i];
	sum2 *= sum2;
	sum -= sum2;
	sum /= (double)(n*(n-1));

	return sum;
}
double get_var2_double(double mean,int n,double xx[])
{
	double sum = 0;
	int i;
	for(i = 0 ; i < n ; i++)
		sum += (xx[i] - mean)*(xx[i] - mean);
	sum /= (double)(n-1);
	return sum;
}