#include "my_solver.h"
#include <math.h>

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001 
double C;
double p[4][5];
void fcn3_1_1(int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		//TODO
		fvec[0] = (x[0] - p[0][0])*(x[0] - p[0][0]) + (x[1] - p[0][1])*(x[1] - p[0][1]) + (x[2] - p[0][2])*(x[2] - p[0][2]) - (C*(p[0][4] + x[3] - p[0][3])) * (C*(p[0][4] + x[3] - p[0][3]));
		fvec[1] = (x[0] - p[1][0])*(x[0] - p[1][0]) + (x[1] - p[1][1])*(x[1] - p[1][1]) + (x[2] - p[1][2])*(x[2] - p[1][2]) - (C*(p[1][4] + x[3] - p[1][3])) * (C*(p[1][4] + x[3] - p[1][3]));
		fvec[2] = (x[0] - p[2][0])*(x[0] - p[2][0]) + (x[1] - p[2][1])*(x[1] - p[2][1]) + (x[2] - p[2][2])*(x[2] - p[2][2]) - (C*(p[2][4] + x[3] - p[2][3])) * (C*(p[2][4] + x[3] - p[2][3]));
		fvec[3] = (x[0] - p[3][0])*(x[0] - p[3][0]) + (x[1] - p[3][1])*(x[1] - p[3][1]) + (x[2] - p[3][2])*(x[2] - p[3][2]) - (C*(p[3][4] + x[3] - p[3][3])) * (C*(p[3][4] + x[3] - p[3][3]));
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		//TODO
		fjac[0] = 2 * (x[0] - p[0][0]); fjac[4] = 2 * (x[1] - p[0][1]); fjac[8] = 2 * (x[2] - p[0][2]); fjac[12] = -2 * (C*(p[0][4] + x[3] - p[0][3])) * C;
		fjac[1] = 2 * (x[0] - p[1][0]); fjac[5] = 2 * (x[1] - p[1][1]); fjac[9] = 2 * (x[2] - p[1][2]); fjac[13] = -2 * (C*(p[1][4] + x[3] - p[1][3])) * C;
		fjac[2] = 2 * (x[0] - p[2][0]); fjac[6] = 2 * (x[1] - p[2][1]); fjac[10] = 2 * (x[2] - p[2][2]);  fjac[14] = -2 * (C*(p[2][4] + x[3] - p[2][3])) * C;
		fjac[3] = 2 * (x[0] - p[3][0]); fjac[7] = 2 * (x[1] - p[3][1]); fjac[11] = 2 * (x[2] - p[3][2]);  fjac[15] = -2 * (C*(p[3][4] + x[3] - p[3][3])) * C;
	}
}

void fcn3_1_2(int *n, double *x, double *fvec, int *iflag)
{
	fvec[0] = (x[0] - p[0][0])*(x[0] - p[0][0]) + (x[1] - p[0][1])*(x[1] - p[0][1]) + (x[2] - p[0][2])*(x[2] - p[0][2]) - (C*(p[0][4] + x[3] - p[0][3])) * (C*(p[0][4] + x[3] - p[0][3]));
	fvec[1] = (x[0] - p[1][0])*(x[0] - p[1][0]) + (x[1] - p[1][1])*(x[1] - p[1][1]) + (x[2] - p[1][2])*(x[2] - p[1][2]) - (C*(p[1][4] + x[3] - p[1][3])) * (C*(p[1][4] + x[3] - p[1][3]));
	fvec[2] = (x[0] - p[2][0])*(x[0] - p[2][0]) + (x[1] - p[2][1])*(x[1] - p[2][1]) + (x[2] - p[2][2])*(x[2] - p[2][2]) - (C*(p[2][4] + x[3] - p[2][3])) * (C*(p[2][4] + x[3] - p[2][3]));
	fvec[3] = (x[0] - p[3][0])*(x[0] - p[3][0]) + (x[1] - p[3][1])*(x[1] - p[3][1]) + (x[2] - p[3][2])*(x[2] - p[3][2]) - (C*(p[3][4] + x[3] - p[3][3])) * (C*(p[3][4] + x[3] - p[3][3]));
}

void program3_1_1() {
	FILE *fp_r;
	FILE *fp_w;
	int n = SOLNUMS;
	double *x;
	double *fvec;
	double *fjac = new double[MATCOLS*MATROWS];
	int ldfjac = MATCOLS;
	double tol;
	int info;
	double *wa;
	int lwa;
	int i, j;
	double b;
	/* initialize variable*/
	x = (double *)malloc(sizeof(double) * n);
	fvec = (double *)malloc(sizeof(double) * n);
	tol = TOLERANCE;
	lwa = (n*(n + 13)) / 2;
	wa = (double *)malloc(sizeof(double) * lwa);

	/* HYBRJ1 GPS_Signal_0.txt*/
	fp_r = fopen("GPS_signal_0.txt", "r");
	fp_w = fopen("GPS_position_3-1_0.txt","w");
	printf("Program3_1_1_0의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r,"%lf",&p[i][j]);
		}
	}
	hybrj1_(fcn3_1_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);

	/* HYBRJ1 GPS_Signal_1.txt*/
	fp_r = fopen("GPS_signal_1.txt", "r");
	fp_w = fopen("GPS_position_3-1_1.txt", "w");
	printf("Program3_1_1_1의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r, "%lf", &p[i][j]);
		}
	}
	hybrj1_(fcn3_1_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);

	/* HYBRJ1 GPS_Signal_2.txt*/
	fp_r = fopen("GPS_signal_2.txt", "r");
	fp_w = fopen("GPS_position_3-1_2.txt", "w");
	printf("Program3_1_1_2의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r, "%lf", &p[i][j]);
		}
	}
	hybrj1_(fcn3_1_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);

	/* HYBRD1 initialize*/

	lwa = (n*(3*n + 13)) / 2;
	wa = (double *)malloc(sizeof(double) * lwa);
	/* HYBRD1 GPS_Signal_0.txt*/
	fp_r = fopen("GPS_signal_0.txt", "r");
	fp_w = fopen("GPS_position_3-2_0.txt", "w");
	printf("Program3_1_2_0의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r, "%lf", &p[i][j]);
		}
	}
	hybrd1_(fcn3_1_2, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);

	/* HYBRD1 GPS_Signal_1.txt*/
	fp_r = fopen("GPS_signal_1.txt", "r");
	fp_w = fopen("GPS_position_3-2_1.txt", "w");
	printf("Program3_1_2_1의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r, "%lf", &p[i][j]);
		}
	}
	hybrd1_(fcn3_1_2, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);

	/* HYBRD1 GPS_Signal_2.txt*/
	fp_r = fopen("GPS_signal_2.txt", "r");
	fp_w = fopen("GPS_position_3-2_2.txt", "w");
	printf("Program3_1_2_2의 초기값을 입력하시오 : ");
	scanf("%lf %lf %lf %lf", &x[0], &x[1], &x[2], &x[3]);
	fscanf(fp_r, "%lf %lf", &C, &b);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(fp_r, "%lf", &p[i][j]);
		}
	}
	hybrd1_(fcn3_1_2, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "x1 : %lf \nx2 : %lf \nx3 : %lf \nx4 : %lf\n", x[0], x[1], x[2], x[3]);
	for (i = 0; i < n; i++) {
		fprintf(fp_w, "f%d = %lf\n", i, fvec[i]);
	}
	fclose(fp_r);
	fclose(fp_w);
}

