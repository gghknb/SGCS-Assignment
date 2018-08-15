#ifndef _MY_SOLVER
#define _MY_SOLVER

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

void program2_1(void);
void program2_2(void);

// HOMEWORK
void program2_2_a(void);
void program2_2_b(void);
void program2_3(void);

#endif // _MY_SOLVER
const double DELTA = 0.000000000001;
const int Nmax = 100;
const double EPSILON = 0.00000000001;
double Fx(int num, double *X, double *Y_arr, double x);
double fx(int num, double u, double a0, double b0, double *X_arr, double *Y_arr);
double Fx2(int num, double *X, double *Y_arr, double x);
double fx2(int num, double u, double a0, double b0, double *X_arr, double *Y_arr);
double fx_Newton(int num, double u, double x1, double *X_arr, double *Y_arr);
double Fx_prime(int num, double *X_arr, double *Y_arr, double x);
