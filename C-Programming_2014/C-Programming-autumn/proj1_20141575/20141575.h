#include <stdio.h>
#include <stdlib.h>

/* Basic Function */
double** addMatrix(double** A, double** B, int n);
double** subMatrix(double** A, double** B, int n);
double** mulMatrix(double** A, double** B, int n);
double detA(double** A, int n);
double** inverseA(double** A, int n);
double** inverseA_gaussian(double** A, int n);
double** inverseA55(double** A);
double** matrix_LU(double **A, double **L, double **U, int n);
/* add Function */
double** transposeA(double** A, int n);
double** getMijFromA(double** A, int i, int j, int n);
double detA44(double** A);
double detA55(double** A);
double detA4(double **A);
