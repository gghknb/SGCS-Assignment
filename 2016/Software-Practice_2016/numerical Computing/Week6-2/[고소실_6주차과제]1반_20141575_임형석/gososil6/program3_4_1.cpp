#include "my_solver.h"


void program3_4_1(){
	FILE *fp_r;
	FILE *fp_w;
	int n;
	int i, j;
	int ia;
	int *l;
	float *a;
	float *b;
	float *x, *s;
	fp_r = fopen("linear_system.txt","r");
	fp_w = fopen("linear_system_solution.txt","w");
	fscanf(fp_r,"%d",&n);
	b = (float *)malloc(sizeof(float)*n);
	x = (float *)malloc(sizeof(float)*n);
	s = (float *)malloc(sizeof(float)*n);
	l = (int *)malloc(sizeof(int)*n);
	ia = n;
	a = (float *)malloc(sizeof(float)*(n*n));
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			fscanf(fp_r, "%f", &a[i + n*j]);
	}
	for (i = 0; i < n; i++)
		fscanf(fp_r,"%f",&b[i]);

	gespp_(&n, a, &ia, l, s);
	solve_(&n, a, &ia, l, b, x);

	fprintf(fp_w,"%d\n",n);
	for (i = 0; i < n; i++)
	{
		fprintf(fp_w,"%f\n",x[i]);
	}
}

