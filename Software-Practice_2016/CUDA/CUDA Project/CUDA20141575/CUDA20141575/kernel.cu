
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <assert.h>


const int ELEM_PER_VECTOR = 32;
#define ELEM_PER_POINT (1 << 5)
#define N_ELEMS (1<<22)
#define N_POINTS (N_ELEMS / ELEM_PER_POINT)

typedef struct
{
	float *elem[ELEM_PER_POINT];
}POINTS_SOA;

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

inline float absIEEE754( float f)
{
	return ( float& )( ( int& )f &= 0x7fffffff );
}

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
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
		exit(1);
	}
}

#define MAX_N_ELEMENTS	(1 << 25)

/* CPU */
void matrix_multiplication_cpu(float *vecY, float (*matA), float *vecX, int n)
{
	for (int i = 0; i<n; ++i)
	{
		for (int j = 0; j<ELEM_PER_VECTOR; ++j)
		{
			vecY[i*ELEM_PER_VECTOR + j] = 0;
			for (int k = 0; k<ELEM_PER_VECTOR; ++k)
				vecY[i*ELEM_PER_VECTOR + j] += matA[j*ELEM_PER_VECTOR + k] * vecX[i*ELEM_PER_VECTOR + k];
		}
	}
}
/* GPU1 */
__global__ void matrix_multiplication_gpu_global_memory(float* vecY, float(*matA), float* vecX)
{
	unsigned tid = (blockIdx.x * blockDim.x) + threadIdx.x;

	float result;
	for (unsigned i = 0; i < ELEM_PER_VECTOR; ++i)
	{
		result = 0.0f;
		for (unsigned j = 0; j < ELEM_PER_VECTOR; ++j)
		{
			result += matA[i*ELEM_PER_VECTOR + j] * vecX[tid * ELEM_PER_VECTOR + j];
		}
		vecY[tid * ELEM_PER_VECTOR + i] = result;
	}
}
/* GPU2 */
__global__ void matrix_multiplication_gpu_shared_memory(float* vecY, float* matA, float* vecX)
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = ( tid % ELEM_PER_VECTOR ) + ( tid / 1024 * ELEM_PER_VECTOR ); // blockDim.x
	unsigned eid = ( tid % 1024 / ELEM_PER_VECTOR );

	__shared__ float sharedVecX[ 1024 ];
	__shared__ float sharedMatA[ 1024 ];

	unsigned ratio = 1024 / blockDim.x; // num of elements in 32x32 Matrix
	int accessID = ( threadIdx.x / ELEM_PER_VECTOR ) * ( ELEM_PER_VECTOR * ratio ) + ( threadIdx.x % ELEM_PER_VECTOR );
	for( unsigned i = 0; i < ratio; ++i )
		sharedMatA[ accessID + i * ELEM_PER_VECTOR ] = ( ( float* )matA )[ accessID + i * ELEM_PER_VECTOR ];

	sharedVecX[ threadIdx.x ] = vecX[ tid ];
	unsigned svid = threadIdx.x % ELEM_PER_VECTOR * ELEM_PER_VECTOR;
	__syncthreads( );

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += sharedMatA[ eid * ELEM_PER_VECTOR + j ] * sharedVecX[ svid + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}
/* GPU3*/
__global__ void matrix_multiplication_gpu_SOA_shared_memory(POINTS_SOA vecY, float* matA, POINTS_SOA vecX)
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	__shared__ float sharedVecX[ 1024 ];
	__shared__ float sharedMatA[ 1024 ];                

	unsigned ratio = 1024 / blockDim.x; // num of elements in 32x32 Matrix
	int accessID = ( threadIdx.x / ELEM_PER_VECTOR ) * ( ELEM_PER_VECTOR * ratio ) + ( threadIdx.x % ELEM_PER_VECTOR );
	for( unsigned i = 0; i < ratio; ++i )
		sharedMatA[ accessID + i * ELEM_PER_VECTOR ] = ( ( float* )matA )[ accessID + i * ELEM_PER_VECTOR ];

	sharedVecX[ threadIdx.x ] = vecX.elem[eid][ vid ];
	unsigned svid = threadIdx.x / ELEM_PER_VECTOR * ELEM_PER_VECTOR; 
	__syncthreads();

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += sharedMatA[ eid * ELEM_PER_VECTOR + j ] * sharedVecX[ svid + j ];
	}
	vecY.elem[eid][vid] = result;
}

/* GPU4*/
__global__ void matrix_multiplication_gpu_optimized(float* vecY, float* matA, float* vecX)
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;
	//unsigned vid = ( tid % ELEM_PER_VECTOR ) + ( tid / 1024 * ELEM_PER_VECTOR ); // blockDim.x
	//unsigned eid = ( tid % 1024 / ELEM_PER_VECTOR );

	__shared__ float sharedVecX[ 1024 ];
	__shared__ float sharedMatA[ 1024 ];

	unsigned ratio = 1024 / blockDim.x; // num of elements in 32x32 Matrix
	int accessID = ( threadIdx.x / ELEM_PER_VECTOR ) * ( ELEM_PER_VECTOR * ratio ) + ( threadIdx.x % ELEM_PER_VECTOR );
	int accessID_transpose = ( threadIdx.x % ELEM_PER_VECTOR ) * ( ELEM_PER_VECTOR * ratio ) + ( threadIdx.x / ELEM_PER_VECTOR );

	for( unsigned i = 0; i < ratio; ++i )
		sharedMatA[ accessID_transpose + i * ELEM_PER_VECTOR ] = ( ( float* )matA )[ accessID + i * ELEM_PER_VECTOR ];

	sharedVecX[ threadIdx.x ] = vecX[ tid ];
	unsigned svid = threadIdx.x / ELEM_PER_VECTOR * ELEM_PER_VECTOR;
	__syncthreads( );

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += sharedMatA[ j * ELEM_PER_VECTOR + eid ] * sharedVecX[ svid + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}

void main(void) {
	int n_elements;
	TIMER_T compute_time = 0;
	TIMER_T device_time_1 = 0;
	TIMER_T device_time_2 = 0;
	TIMER_T device_time_3 = 0;
	TIMER_T device_time_4 = 0;
	FILE *fp = fopen("gen.bin", "rb");

	/* initialize array & variable*/
	float *mat  = new float[ELEM_PER_VECTOR * ELEM_PER_VECTOR];
	int n;
	fread(&n, sizeof(float), 1, fp);
	float *vecX = new float[ELEM_PER_VECTOR * n];
	fread(vecX, sizeof(float), n * ELEM_PER_VECTOR, fp);
	fread(mat, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);

	fclose(fp);
	
	float *vecY_cpu  = new float[n * ELEM_PER_VECTOR];
	float *vecY_1 = new float[n * ELEM_PER_VECTOR]; //y1
	float *vecY_2 = new float[n * ELEM_PER_VECTOR]; //y2
	float *vecY_3 = new float[n * ELEM_PER_VECTOR]; //y3
	float *vecY_4 = new float[n * ELEM_PER_VECTOR]; //y4

	/* CPU exec */
	CHECK_TIME_START();
	matrix_multiplication_cpu(vecY_cpu,mat,vecX,n);
	CHECK_TIME_END(compute_time);
	/* CPU output*/
	printf("Elapsed Time by CPU is %.6f(s)\n",compute_time/1000);


	/* initialize Cuda variable */
	CUDA_CALL(cudaSetDevice(0));

	float *cudaY_1, *cudaY_2, *cudaY_4;
	float *cudaX, *cudaMat;
	CUDA_CALL(cudaMalloc(&cudaY_1, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaY_2, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaY_4, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaX, sizeof(float) * n * ELEM_PER_VECTOR));
	CUDA_CALL(cudaMalloc(&cudaMat, sizeof(float) * ELEM_PER_VECTOR * ELEM_PER_VECTOR));

	//*** Copying A and B from host to device... copy to Cuda varible 
	CUDA_CALL(cudaMemcpy(cudaX, vecX, sizeof(float) * n * ELEM_PER_VECTOR, cudaMemcpyHostToDevice));
	CUDA_CALL(cudaMemcpy(cudaMat, mat, sizeof(float) * ELEM_PER_VECTOR * ELEM_PER_VECTOR, cudaMemcpyHostToDevice));
	CHECK_TIME_INIT_GPU();


	size_t n_threads = (1 << 10);
	size_t n_blocks = n / n_threads;

	size_t _1024Threads = ( 1 << 10 );
	size_t _1024Blocks_perElement = ( n * ELEM_PER_VECTOR ) / _1024Threads;
	size_t _1024Blocks_perVector = n / _1024Threads;

	/* GPU1 exec*/
	CHECK_TIME_START_GPU();
	matrix_multiplication_gpu_global_memory <<<_1024Blocks_perVector, _1024Threads >>>(cudaY_1, cudaMat, cudaX);
	cuda_error_check("- ", " FAILED: matrix_multiplication_gpu_global_memory()\n\n");
	CHECK_TIME_END_GPU(device_time_1);

	/* GPU2 exec*/
	CHECK_TIME_START_GPU();
	matrix_multiplication_gpu_shared_memory <<<_1024Blocks_perElement, _1024Threads >>>(cudaY_2, cudaMat, cudaX);
	cuda_error_check("- ", " FAILED: matrix_multiplication_gpu_shared_memory()\n\n");
	CHECK_TIME_END_GPU(device_time_2);
	
	/* GPU3 exec*/
	POINTS_SOA CudaX3,CudaY3,SoaX,SoaY;
	for(int i = 0 ; i < ELEM_PER_VECTOR ; ++i)
	{
		SoaX.elem[i] = (float*)malloc(sizeof(float) * n);
		SoaY.elem[i] = (float*)malloc(sizeof(float) * n);
		CUDA_CALL(cudaMalloc(&CudaX3.elem[i],sizeof(float) * n));
		CUDA_CALL(cudaMalloc(&CudaY3.elem[i],sizeof(float) * n));
	}

	for(int i = 0 ; i < n ; ++i)
	{
		for(int j = 0 ; j < ELEM_PER_VECTOR ; ++j)
		{
			SoaX.elem[j][i] = vecX[i * ELEM_PER_VECTOR + j];
		}
	}
	for(int i = 0 ; i < ELEM_PER_VECTOR ; ++i)
		CUDA_CALL(cudaMemcpy(CudaX3.elem[i], SoaX.elem[i], sizeof(float) * n, cudaMemcpyHostToDevice));

	CHECK_TIME_START_GPU();
	matrix_multiplication_gpu_SOA_shared_memory <<<_1024Blocks_perElement, _1024Threads >>>(CudaY3,cudaMat,CudaX3);
	cuda_error_check("- ", " FAILED: matrix_multiplication_gpu_SOA_shared_memory()\n\n");
	CHECK_TIME_END_GPU(device_time_3);
	
	/* GPU4 exec*/
	CHECK_TIME_START_GPU();
	matrix_multiplication_gpu_optimized <<<_1024Blocks_perElement, _1024Threads >>>(cudaY_4,cudaMat,cudaX);
	cuda_error_check("- ", " FAILED: matrix_multiplication_gpu_optimized()\n\n");
	CHECK_TIME_END_GPU(device_time_4);

	/* *** Copying Y from device to host...*/
	CUDA_CALL(cudaMemcpy(vecY_1, cudaY_1, sizeof(float)*n * ELEM_PER_VECTOR, cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaMemcpy(vecY_2, cudaY_2, sizeof(float)*n * ELEM_PER_VECTOR, cudaMemcpyDeviceToHost));
	for(int i = 0 ; i < ELEM_PER_VECTOR ; ++i)
		CUDA_CALL(cudaMemcpy(SoaY.elem[i], CudaY3.elem[i], sizeof(float) * n,cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaMemcpy(vecY_4, cudaY_4, sizeof(float)*n * ELEM_PER_VECTOR, cudaMemcpyDeviceToHost));
	CUDA_CALL(cudaDeviceSynchronize());

	/* GPU1 output*/
	float EPSILON = 0.0001f;
	int cnt = 0;
	for (int i = 0; i < n * ELEM_PER_VECTOR; ++i)
	{
		if ( fabs(vecY_cpu[i] - vecY_1[i]) > EPSILON )
		{
			cnt++;
		}
	}

	printf("Elapsed Time by GPU1 is %.6f(s)", device_time_1/1000); 
	printf(" Error rate is %.2f%%\n", cnt / (float)(n * ELEM_PER_VECTOR) * 100);

	/* GPU2 output*/
	cnt = 0;
	for (int i = 0; i < n * ELEM_PER_VECTOR; ++i)
	{
		if( fabs(vecY_cpu[i] - vecY_2[i]) > EPSILON)		
		{
			cnt++;
		}
	}
	printf("Elapsed Time by GPU2 is %.6f(s)",device_time_2/1000);
	printf(" Error rate is %.2f%%\n", cnt / (float)(n * ELEM_PER_VECTOR) * 100);

	/* GPU3 output*/
	cnt = 0;
	for (int i = 0; i < n ; ++i)
	{
		for(int j = 0 ; j < ELEM_PER_VECTOR; ++j)
		{
			if (absIEEE754(SoaY.elem[j][i] - vecY_cpu[i*ELEM_PER_VECTOR + j]) > EPSILON)
			{
				cnt++;
			}
		}
	}
	printf("Elapsed Time by GPU3 is %.6f(s)",device_time_3/1000);
	printf(" Error rate is %.2f%%\n", cnt / (float)(n * ELEM_PER_VECTOR) * 100);

	/* GPU4 output*/
	cnt = 0;
	for (int i = 0; i < n * ELEM_PER_VECTOR; ++i)
	{
		if (fabs(vecY_cpu[i] - vecY_4[i]) > EPSILON)
		{
			cnt++;
		}
	}

	printf("Elapsed Time by GPU4 is %.6f(s)",device_time_4/1000);
	printf(" Error rate is %.2f%%\n", cnt / (float)(n * ELEM_PER_VECTOR) * 100);

	/* free */
	cudaFree(cudaY_1);
	cudaFree(cudaY_2);
	cudaFree(cudaY_4);
	cudaFree(cudaX);
	cudaFree(cudaMat);

	CHECK_TIME_DEST_GPU();

	CUDA_CALL(cudaDeviceReset());

	delete[] vecX;
	delete[] vecY_cpu;
	delete[] vecY_1;
	delete[] vecY_2;
	delete[] vecY_3;
	delete[] vecY_4;
	delete[] mat;
}