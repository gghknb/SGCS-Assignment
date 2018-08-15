
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <assert.h>

const int ELEM_PER_VECTOR = 32;

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
__global__ void combine_two_arrays_GPU( float* x, float* y, float* z )
{
	unsigned int tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	z[ tid ] = 1.0f / (sinf( x[ tid ] )*cosf( y[ tid ] ) + cosf( x[ tid ] )*sinf( y[ tid ] ) );
}

void combine_two_arrays_CPU( float *x, float *y, float *z, int n )
{
	int i;

	for( i = 0; i < n; i++ )
	{
		//z[ i + 0 ] = 1.0f / i;
		z[ i + 0 ] = 1.0f / ( sinf( x[ i + 0 ] )*cosf( y[ i + 0 ] ) + cosf( x[ i + 0 ] )*sinf( y[ i + 0 ] ) );
	}
}

__global__ void combine_two_arrays_GPU_matrix(float *y, float *mat, float *x)
{
	unsigned int tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;

	for(int j=0;j<ELEM_PER_VECTOR;++j)
	{
		y[tid*ELEM_PER_VECTOR + j] = 0;
		for(int k=0;k<ELEM_PER_VECTOR;++k)
			y[tid*ELEM_PER_VECTOR + j] += mat[j*ELEM_PER_VECTOR + k] * x[tid*ELEM_PER_VECTOR + k];
	}
}

void combine_two_arrays_CPU_matrix(float *y,float *mat, float *x,int n)
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<ELEM_PER_VECTOR;++j)
		{
			y[i*ELEM_PER_VECTOR + j] = 0;
			for(int k=0;k<ELEM_PER_VECTOR;++k)
				y[i*ELEM_PER_VECTOR + j] += mat[j*ELEM_PER_VECTOR + k] * x[i*ELEM_PER_VECTOR + k];
		}
	}
}

void main(void) {
	int n_elements;

	TIMER_T compute_time = 0;
	TIMER_T device_time = 0;

	FILE *fp = fopen( "gen.bin", "rb" );

	float *mat = new float[ ELEM_PER_VECTOR * ELEM_PER_VECTOR ];
	int n;
	fread( &n, sizeof( float ), 1, fp );
	float *x = new float[ ELEM_PER_VECTOR * n ];
	fread( x, sizeof( float ), n * ELEM_PER_VECTOR, fp );
	fread( mat, sizeof( float ), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp );

	fclose(fp);

	float *y1 = new float[ n * ELEM_PER_VECTOR ]; //y1
	float *y2 = new float[ n * ELEM_PER_VECTOR ]; //y2

	printf( "*** CPU Works...\n" );
	CHECK_TIME_START();
	combine_two_arrays_CPU_matrix(y1, mat, x, n);
	CHECK_TIME_END( compute_time );
	printf( " - Finish\n\n" );

	CUDA_CALL(cudaSetDevice(0));

	float *cudaY, *cudaX, *cudaMat;
	CUDA_CALL(cudaMalloc(&cudaY, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaX, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaMat, sizeof(float) * ELEM_PER_VECTOR * ELEM_PER_VECTOR));

	printf("*** Copying A and B from host to device...\n");
	CUDA_CALL(cudaMemcpy(cudaX, x, sizeof(float) * n * ELEM_PER_VECTOR, cudaMemcpyHostToDevice));
	CUDA_CALL(cudaMemcpy(cudaMat, mat, sizeof(float) * ELEM_PER_VECTOR * ELEM_PER_VECTOR, cudaMemcpyHostToDevice));
	printf(" - Finish\n\n");

	CHECK_TIME_INIT_GPU();

	size_t n_threads = (1<<10);
	size_t n_blocks = n / n_threads;

	printf("*** kernel call: combine_two_arrays<<< %d, %d >>>()...\n", n_blocks, n_threads);
	CHECK_TIME_START_GPU();
	combine_two_arrays_GPU_matrix<<<n_blocks, n_threads >>>(cudaY, cudaMat, cudaX);
	cuda_error_check("- ", " FAILED: combine_two_arrays()\n\n");
	CHECK_TIME_END_GPU(device_time);
	printf( " - Finish\n\n" );

	printf("*** Time taken = %.6fms(CPU), %.6fms(GPU)\n", compute_time, device_time);

	printf("*** Copying Y from device to host...\n");
	CUDA_CALL(cudaMemcpy(y2, cudaY, sizeof(float)*n * ELEM_PER_VECTOR, cudaMemcpyDeviceToHost));
	CUDA_CALL( cudaDeviceSynchronize() );
	
	int cnt = 0;
	for( int i = 0; i < n * ELEM_PER_VECTOR; ++i )
	{
		if( y1[ i ] != y2[ i ] )
		{
			//fprintf( fp, "[%d] %f != %f\n\n", i, C1[i], C2[i] );
			//printf("[%d] %f != %f\n\n", i, C1[ i ], C2[ i ] );
			cnt++;
		}
	}
	//fclose( fp );

	printf( " - %.2f%% numerical errors...\n", cnt/(float)(n * ELEM_PER_VECTOR)*100 );
	printf(" - Finish\n\n");
	
	cudaFree(cudaY);
	cudaFree(cudaX);
	cudaFree(cudaMat);

	CHECK_TIME_DEST_GPU();
	
	CUDA_CALL(cudaDeviceReset());

	delete[] x;
	delete[] y1;
	delete[] y2;
	delete[] mat;
}