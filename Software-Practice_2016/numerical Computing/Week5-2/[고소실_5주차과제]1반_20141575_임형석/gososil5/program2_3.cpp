#include "my_solver.h"

// global variables
double *U;
double *X;
// HOMEWORK
void program2_3()
{
	FILE *fp_r1, *fp_r2, *fp_w;
	double init_value = 0;
	double interval;
	double *rand;
	int num,num2;
	int rand_num = 0;
	int total_num = 0;
	int U_num = 0;
	fp_r1 = fopen("pdf.txt", "r");
	fp_r2 = fopen("pdf_and_rand_2_2_a.txt","r");
	fp_w = fopen("histrogram.txt","w");
	fscanf(fp_r1, "%d %lf", &num, &interval);
	fscanf(fp_r2, "%d", &num2);
	if (fp_w)
		printf("program2_3 : Histogram created\n");
	else  printf("program2_3 : Failed\n");


	rand = (double *)malloc(sizeof(double)*num2);
	for (int i = 0; i < num2; i++)
	{
		fscanf(fp_r2, "%lf", &rand[i]);
	}
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num2; j++)
		{
			if (init_value <= rand[j] && rand[j] < init_value + interval)
			{
				rand_num++;
			}
			if (init_value <= U[j] && U[j] < init_value + interval)
			{
				U_num++;
			}
		}
		fprintf(fp_w, "%lf ~ %lf : pdf_and_rand : %d          난수 : %d\n", init_value, init_value + interval, rand_num,U_num);
		total_num += rand_num;
		rand_num = 0;
		U_num = 0;
		init_value += interval;
	}
	fprintf(fp_w, "Total : %d\n",total_num);
}

// HOMEWORK
void program2_2_a()
{
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;

	// something to do...
	FILE *fp_r, *fp_w;
	int U_num, i;
	int num;
	double interval;
	double X_arr[100], Y_arr[100];

	fp_r = fopen("pdf.txt", "r");
	fscanf(fp_r, "%d %lf", &num, &interval);
	for (i = 0; i < num; i++)
	{
		fscanf(fp_r, "%lf %lf", &X_arr[i], &Y_arr[i]);
	}

	fp_w = fopen("pdf_and_rand_2_2_a.txt", "w");
	printf("program2_2_a \nnr의 개수를 입력 : ");
	scanf("%d", &U_num);
	U = (double *)malloc(sizeof(double)*U_num);
	X = (double *)malloc(sizeof(double)*U_num);

	for (i = 0; i < U_num; i++)
	{
		U[i] = (double)rand() / RAND_MAX;
	}

	for (i = 0; i < U_num; i++)
	{
		X[i] = fx2(num, U[i], X_arr[0], X_arr[num - 1], X_arr, Y_arr);
	}
	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", U_num);
	for (i = 0; i < U_num; i++)
	{
		fprintf(fp_w, "%.15lf\n", X[i]);
	}
	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	printf("The program2_2_a run time is %f(ms)..\n", resultTime*1000.0);
}

double Fx2(int num, double *X_arr, double *Y_arr, double x)
{
	int i;
	double area = 0;

	if (x == 0.0)
	{
		return 0;
	}
	for (i = 0; i < num; i++)
	{
		if (X_arr[i] <= x)
		{
			area += Y_arr[i] * (X_arr[1] - X_arr[0]);
		}
	}
	area -= Y_arr[0] / num / 2;
	return area;
}

double fx2(int num, double u, double a0, double b0, double *X_arr, double *Y_arr)
{
	double x1;
	int n;
	double temp, temp2, temp3;
	double ratio;
	for (n = 0; ; n++)
	{
		x1 = (a0 + b0) / 2.0;
		temp = Fx2(num, X_arr, Y_arr, x1) - u;
		temp2 = Fx2(num, X_arr, Y_arr, a0) - u;
		temp3 = Fx2(num, X_arr, Y_arr, b0) - u;
		if (temp2*temp > 0)
			a0 = x1;
		else if (temp3*temp > 0)
			b0 = x1;
		
		if (fabs(temp) < DELTA || fabs(b0 - a0) < EPSILON)
		{
			break;
		}
	}
	ratio = fabs(temp2) / (temp3 - temp2) * (b0 - a0) + a0;
	return ratio;
}

void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;

	// something to do...
	FILE *fp_r, *fp_w;
	int i;
	int num;
	double interval;
	double *X2;
	double X_arr[100], Y_arr[100];

	fp_r = fopen("pdf.txt", "r");
	fscanf(fp_r, "%d %lf", &num, &interval);
	for (i = 0; i < num; i++)
	{
		fscanf(fp_r, "%lf %lf", &X_arr[i], &Y_arr[i]);
	}

	fp_w = fopen("pdf_and_rand_2_2_b.txt", "w");
	printf("program2_2_b \nnr의 개수를 입력 : ");
	scanf("%d", &num);
	X2 = (double *)malloc(sizeof(double)*num);
	for (i = 0; i < num; i++)
	{
		//X2[i] = fx_Newton(num, U[i],X[i], X_arr, Y_arr);
		X2[i] = fx_Newton(num, U[i], 0.5, X_arr, Y_arr);
	}

	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", num);
	for (i = 0; i < num; i++)
	{
		fprintf(fp_w, "%.15lf\n", X2[i]);
	}

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
	printf("The program2_2_b run time is %f(ms)..\n", resultTime*1000.0);
}

double Fx_prime(int num, double *X_arr, double *Y_arr, double x)
{
	int px1, px2,i;
	double s;
	for (i = 0; i < num; i++)
	{
		if (X_arr[i] <= x)
		{
			px1 = i;
			px2 = i+1;
		}
		else break;
	}
	s = (x - X_arr[px1]) / (X_arr[px2] - X_arr[px1]);
	return (1 - s)*(Y_arr[px1]) + s*(Y_arr[px2]);
}

double fx_Newton(int num, double u, double x1, double *X_arr, double *Y_arr)
{
	double x2;
	double _F, _FP;
	for (int n = 0;; n++)
	{	
			_F = Fx2(num, X_arr, Y_arr, x1) - u;
			_FP = Fx_prime(num, X_arr, Y_arr, x1);
			x2 = x1 - _F / _FP;
			if (fabs(Fx2(num, X_arr, Y_arr, x2) - u) < DELTA || fabs(x2 - x1) < EPSILON || n>=20)
			{
				break;
			}
			x1 = fabs(x2);
	}
	return x1;
}