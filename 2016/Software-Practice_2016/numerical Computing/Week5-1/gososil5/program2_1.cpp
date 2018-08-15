#include "my_solver.h"

void program2_1()
{
	FILE *fp_r, *fp_w;
	int num,i;
	double x,y;
	double x_arr[100],y_arr[100];
	double interval;
	double area = 0,sum_to_1 = 0,sum_to_5 = 0;
	double x_temp,x_temp2;
	double new_interval;
	fp_r = fopen("sampling.txt", "r");		
	if(fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf.txt", "w");
	fscanf(fp_r,"%d %lf",&num,&interval);
	for(i = 0; i < num ; i++)
	{
		fscanf(fp_r,"%lf %lf",&x,&y);
		if(i == 0 || i == (num -1))
		{
			area += interval/2 * y;
			x_arr[i] = x;
			y_arr[i] = y;
		}
		else
		{
			area += interval * y;
			x_arr[i] = x;
			y_arr[i] = y;
		}
	}
	x_temp = x_arr[num-1] - x_arr[0];
	x_temp2 = x_arr[0];
	for(i = 0 ; i < num ; i++)
	{
		x_arr[i] = (x_arr[i] - x_temp2) / x_temp;
		y_arr[i] = y_arr[i] / area * x_temp;
		if(x_arr[i] <= 0.5)
			sum_to_5 += y_arr[i] / num;
		else if(x_arr[i] > 0.5)
			sum_to_1 += y_arr[i] / num;
	}
	sum_to_5 -= y_arr[0] / num / 2;
	sum_to_1 -= y_arr[num-1] / num / 2;

	printf("*** Integration of pdf from 0.0 to 1.0 = %lf\n",sum_to_1 + sum_to_5);
	printf("*** Integration of pdf from 0.5 to 1.0 = %lf\n",sum_to_1);
	printf("*** Integration of pdf from 0.0 to 0.5 = %lf\n",sum_to_5);

	new_interval = x_arr[1] - x_arr[0];
	fprintf(fp_w,"%d %lf\n",num,new_interval);
	for(i = 0 ; i < num ; i++)
	{
		fprintf(fp_w,"%lf %lf\n",x_arr[i],y_arr[i]);
	}
	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}