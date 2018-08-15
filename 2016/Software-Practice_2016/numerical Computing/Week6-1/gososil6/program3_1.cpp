#include "my_solver.h"

void rpoly(const char *readfilename, const char *writefilename)
{
	int NCOEF, DEGREE;

	double *poly;
	double *zeror, *zeroi;
	long int fail;
	int i,j;	
	FILE *fp_r = fopen(readfilename, "r");
	if(fp_r == NULL) 
	{
		printf("%s file open error...\n", readfilename);
		return;
	}

	FILE *fp_w = fopen(writefilename, "w");
	if(fp_w == NULL) 
	{
		printf("%s file open error...\n", writefilename);
		return;
	}	
	// TODO
	
	fscanf(fp_r,"%d",&DEGREE);
	NCOEF = DEGREE + 1;
	poly = (double *)malloc(sizeof(double) * NCOEF);
	zeror = (double *)malloc(sizeof(double) * DEGREE);
	zeroi = (double *)malloc(sizeof(double) * DEGREE);
	for(int i = 0 ; i < NCOEF ; i++)
	{
		fscanf(fp_r,"%lf",&poly[i]);
	}
	rpoly_(poly,&DEGREE,zeror,zeroi,&fail);

	complex<double> *root = new complex<double>[DEGREE];

	if(!fail)
	{
		fprintf(fp_w,"real     = ");
		for (i = 0; i < DEGREE ; i++) 
		{
			fprintf(fp_w,"%10f  ", zeror[i]);
			root[i].real(zeror[i]);
		}
		fprintf(fp_w,"\n");
		fprintf(fp_w,"Imag    = ");
		for (i = 0; i < DEGREE; i++) 
		{
			fprintf(fp_w,"%10f  ", zeroi[i]);
			root[i].imag(zeroi[i]);

		}
		fprintf(fp_w,"\n");
			
		fprintf(fp_w,"คำf(x)คำ = ");
		for(i = 0 ; i < DEGREE ; i++)
		{
			complex<double> sol(0.0,0.0);
			for(j = 0 ; j < DEGREE ; j++)
			{
				sol += pow(root[i],DEGREE - j)*poly[j];
			}
			sol += poly[DEGREE];
			fprintf(fp_w,"%10f  ",abs(sol));
			
		}
	}
	else fprintf(fp_w,"failed!");

	free(zeroi);
	free(zeror);
	free(poly);

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}

void program3_1()
{
	rpoly("polynomial_3-1_1.txt", "roots_1.txt");
	rpoly("polynomial_3-1_2.txt", "roots_2.txt");
	rpoly("polynomial_3-1_3.txt", "roots_3.txt");
	rpoly("polynomial_3-1_4.txt", "roots_4.txt");
	rpoly("polynomial_3-1_5.txt", "roots_5.txt");
	rpoly("polynomial_3-1_6.txt", "roots_6.txt");
}