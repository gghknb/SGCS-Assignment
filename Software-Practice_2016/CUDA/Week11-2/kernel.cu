#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <assert.h>


#define N_EQUATIONS (1<<25)

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;

#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START() { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif
		
#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL( cudaEventDestroy( cuda_timer_start ) );
	CUDA_CALL( cudaEventDestroy( cuda_timer_stop ) );
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif


__host__ void cuda_error_check(const char * prefix, const char * postfix)
{
	if (cudaPeekAtLastError() != cudaSuccess)
	{
		printf("%s%s%s", prefix, cudaGetErrorString(cudaGetLastError()), postfix);
		cudaDeviceReset();
		//wait_exit();
		exit(1);
	}
}

#define MAX_N_ELEMENTS	(1 << 25)

void generate_random_float_array(float *array, int n) {

	int i;

	for (i = 0; i < n; i++) {
		array[i] = 3.1415926f*((float)rand() / RAND_MAX);
	}
}

// gpu code
void find_roots_CPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1, int n)
{
	int i;
	float a, b, c, d, x0, x1, tmp;
	for (i = 0; i < n; i++)
	{
		a = A[i]; b = B[i]; c = C[i];
		d = sqrtf(b*b - 4.0f*a*c);
		tmp = 1.0f / (2.0f*a);
		X0[i] = x0 = (-b - d) * tmp;
		X1[i] = x1 = (-b + d) * tmp;
		FX0[i] = (a*x0 + b)*x0 + c;
		FX1[i] = (a*x1 + b)*x1 + c;
	}
}

__global__ void find_roots_GPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1)
{
	unsigned int tid = (blockIdx.x * blockDim.x) + threadIdx.x;
	float a,b,c,d,x0,x1,tmp;

	a = A[tid]; b = B[tid], c = C[tid];
	d = sqrtf(b*b - 4.0f*a*c);
	tmp = 1.0f / (2.0f*a);
	X0[tid] = x0 = (-b - d) * tmp;
	X1[tid] = x1 = (-b + d) * tmp;
	FX0[tid] = (a*x0 + b)*x0 + c;
	FX1[tid] = (a*x1 + b)*x1 + c;
}

void main(void) {

	TIMER_T compute_time = 0;
	TIMER_T device_time = 0;
	
	FILE *fp = fopen( "abc.bin", "rb" );

	int n;
	fread( &n, sizeof( int ), 1, fp );
	float *A = new float[ N_EQUATIONS ];
	float *B = new float[ N_EQUATIONS ];
	float *C = new float[ N_EQUATIONS ];
	float *X0 = new float[ N_EQUATIONS ];
	float *X1 = new float[ N_EQUATIONS ];
	float *FX0 = new float[ N_EQUATIONS ];
	float *FX1 = new float[ N_EQUATIONS ];
	
	float *CPU_X0 = new float[ N_EQUATIONS ];
	float *CPU_X1 = new float[ N_EQUATIONS ];
	float *CPU_FX0 = new float[ N_EQUATIONS ];
	float *CPU_FX1 = new float[ N_EQUATIONS ];
	fread( A, sizeof( float ), N_EQUATIONS, fp );
	fread( B, sizeof( float ), N_EQUATIONS, fp );
	fread( C, sizeof( float ), N_EQUATIONS, fp );
	
	fclose(fp);

	printf( "*** CPU Works...\n" );
	CHECK_TIME_START();
	find_roots_CPU(A,B,C,CPU_X0,CPU_X1,CPU_FX0,CPU_FX1,n);
	CHECK_TIME_END( compute_time );
	printf( " - Finish\n\n" );

	CUDA_CALL(cudaSetDevice(0));
	
	float *cudaA,*cudaB,*cudaC,*cudaX0,*cudaX1,*cudaFX0,*cudaFX1;
	CUDA_CALL(cudaMalloc(&cudaA, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaB, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaC, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaX0, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaX1, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaFX0, sizeof(float) * N_EQUATIONS));
	CUDA_CALL(cudaMalloc(&cudaFX1, sizeof(float) * N_EQUATIONS));

	printf("*** Copying A and B and C from host to device...\n");
	CUDA_CALL(cudaMemcpy(cudaA, A, sizeof(float) * N_EQUATIONS, cudaMemcpyHostToDevice));
	CUDA_CALL(cudaMemcpy(cudaB, B, sizeof(float) * N_EQUATIONS, cudaMemcpyHostToDevice));
	CUDA_CALL(cudaMemcpy(cudaC, C, sizeof(float) * N_EQUATIONS, cudaMemcpyHostToDevice));
	printf(" - Finish\n\n");

	CHECK_TIME_INIT_GPU();

	size_t n_threads = (1<<10);
	size_t n_blocks = n / n_threads;

	printf("*** kernel call: Get X0 X1 FX0 FX1 <<< %d, %d >>>()...\n", n_blocks, n_threads);
	CHECK_TIME_START_GPU();
	find_roots_GPU<<<n_blocks, n_threads >>>(cudaA,cudaB,cudaC,cudaX0,cudaX1,cudaFX0,cudaFX1);
	cuda_error_check("- ", " FAILED:  X0 X1 FX0 FX()\n\n");
	CHECK_TIME_END_GPU(device_time);
	printf( " - Finish\n\n" );

	printf("*** Time taken = %.6fms(CPU), %.6fms(GPU)\n", compute_time, device_time);

	printf("*** Copying Y from device to host...\n");
	CUDA_CALL(cudaMemcpy(X0, cudaX0, sizeof(float) * N_EQUATIONS, cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaMemcpy(X1, cudaX1, sizeof(float) * N_EQUATIONS, cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaMemcpy(FX0, cudaFX0, sizeof(float) * N_EQUATIONS, cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaMemcpy(FX1, cudaFX1, sizeof(float) * N_EQUATIONS, cudaMemcpyDeviceToHost));
	CUDA_CALL( cudaDeviceSynchronize() );
	/* ∞À¡ı */
	int cnt = 0;
	for( int i = 0; i < N_EQUATIONS ; ++i )
	{
		if(  fabs(X0[ i ] - CPU_X0[ i ]) > 0.00001 )
			cnt++;
		
	}
	printf( "X0 - %.2f%% numerical errors...\n", cnt/(float)(N_EQUATIONS)*100 );

	 cnt = 0;
	for( int i = 0; i < N_EQUATIONS ; ++i )
	{
		if(fabs(X1[ i ] - CPU_X1[ i ]) > 0.00001 )
			cnt++;
		
	}
	printf( "X1 - %.2f%% numerical errors...\n", cnt/(float)(N_EQUATIONS)*100 );

	cnt = 0;
	for( int i = 0; i < N_EQUATIONS ; ++i )
	{
		if( fabs(FX0[ i ] - CPU_FX0[ i ]) > 0.00001 )
			cnt++;
		
	}
	printf( "FX0 - %.2f%% numerical errors...\n", cnt/(float)(N_EQUATIONS)*100 );
	
	cnt = 0;
	for( int i = 0; i < N_EQUATIONS ; ++i )
	{
		if( fabs(FX1[ i ] - CPU_FX1[ i ]) > 0.00001 )
			cnt++;
		
	}
	printf("FX1 - %.2f%% numerical errors...\n", cnt/(float)(N_EQUATIONS)*100 );
	
	printf(" - Finish\n\n");

	FILE *fp1 = fopen( "X0.bin", "wb" );
	fwrite( X0, sizeof( float ), N_EQUATIONS, fp1);
	fclose(fp1);

	FILE *fp2 = fopen( "X1.bin", "wb" );
	fwrite( X1, sizeof( float ), N_EQUATIONS, fp2);
	fclose(fp2);

	FILE *fp3 = fopen( "FX0.bin", "wb" );
	fwrite( FX0, sizeof( float ), N_EQUATIONS, fp3);
	fclose(fp3);

	FILE *fp4 = fopen( "FX1.bin", "wb" );
	fwrite( FX1, sizeof( float ), N_EQUATIONS, fp4);
	fclose(fp4);
	
	cudaFree(cudaA);
	cudaFree(cudaB);
	cudaFree(cudaC);
	cudaFree(cudaX0);
	cudaFree(cudaX1);
	cudaFree(cudaFX0);
	cudaFree(cudaFX1);

	CHECK_TIME_DEST_GPU();
	
	CUDA_CALL(cudaDeviceReset());

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] X0;
	delete[] X1;
	delete[] FX0;
	delete[] FX1;

}