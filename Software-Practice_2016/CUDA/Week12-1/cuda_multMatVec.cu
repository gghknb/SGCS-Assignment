#include "cuda_multMatVec.cuh"


__global__ void MultMatVec_GPU_GlobalMemoryWithoutRegister_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;

	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		vecY[ tid * ELEM_PER_VECTOR ] = 0.0f; // No use register
		for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
		{
			vecY[ tid * ELEM_PER_VECTOR + i ] += matA[ i ][ j ] * vecX[ tid * ELEM_PER_VECTOR + j ];
		}
	}
}

__global__ void MultMatVec_GPU_GlobalMemoryWithRegister_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;

	float result;
	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		result = 0.0f;
		for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
		{
			result += matA[ i ][ j ] * vecX[ tid * ELEM_PER_VECTOR + j ];
		}
		vecY[ tid * ELEM_PER_VECTOR + i ] = result;
	}
}

__global__ void MultMatVec_GPU_GlobalMemoryWithoutRegister_Element32ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = blockIdx.x * blockDim.x + threadIdx.x;

	vecY[ tid ] = 0.0f;
	for( unsigned j = 0; j < blockDim.x; ++j )
	{
		vecY[ tid ] += matA[ threadIdx.x ][ j ] * vecX[ blockIdx.x * ELEM_PER_VECTOR + j ];
	}
}

__global__ void MultMatVec_GPU_GlobalMemoryWithRegister_Element32ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	float result = 0.0f;
	for( unsigned j = 0; j < blockDim.x; ++j )
	{
		result += matA[ threadIdx.x ][ j ] * vecX[ blockIdx.x * ELEM_PER_VECTOR + j ];
	}
	vecY[ blockIdx.x * blockDim.x + threadIdx.x ] = result;
}

__global__ void MultMatVec_GPU_GlobalMemoryWithRegister_Element1024ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += matA[ eid ][ j ] * vecX[ vid * ELEM_PER_VECTOR + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}

__global__ void MultMatVec_GPU_GlobalMemoryWithAtomic_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		vecY[ vid * ELEM_PER_VECTOR + i ] = 0.0f;
		atomicAdd( &vecY[ vid * ELEM_PER_VECTOR + i ], matA[ i ][ eid ] * vecX[ vid * ELEM_PER_VECTOR + eid ] );
	}
}

__global__ void MultMatVec_GPU_StridedGlobalMemory_Element1024ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned n = gridDim.x * blockDim.x;
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned stride = ( n / ELEM_PER_VECTOR );

	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid / stride;

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += matA[ eid ][ j ] * vecX[ vid * ELEM_PER_VECTOR + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}

__constant__ float constantMatA[ ELEM_PER_VECTOR ][ ELEM_PER_VECTOR ];
void GenerateConstantMatrix( IN float( *matA )[ ELEM_PER_VECTOR ] )
{
	cudaMemcpyToSymbol( constantMatA, matA, sizeof( float ) * ELEM_PER_VECTOR * ELEM_PER_VECTOR );
}

__global__ void MultMatVec_GPU_SimpleConstantMatrixWithRegister_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;

	float result;
	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		result = 0.0f;
		for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
		{
			result += constantMatA[ i ][ j ] * vecX[ tid * ELEM_PER_VECTOR + j ];
		}
		vecY[ tid * ELEM_PER_VECTOR + i ] = result;
	}
}

__global__ void MultMatVec_GPU_SimpleConstantMatrixWithAtomic_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		vecY[ vid * ELEM_PER_VECTOR + i ] = 0.0f;
		atomicAdd( &vecY[ vid * ELEM_PER_VECTOR + i ], constantMatA[ i ][ eid ] * vecX[ vid * ELEM_PER_VECTOR + eid ] );
	}
}

__global__ void MultMatVec_GPU_SimpleConstantMatrixWithRegister_Element1024ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += constantMatA[ eid ][ j ] * vecX[ vid * ELEM_PER_VECTOR + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}

__global__ void MultMatVec_GPU_StridedConstantMatrixWithRegister_Element1024ThreadsPerBlock( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	// BLOCK DIM: 128 (CC:3.5)
	unsigned n = gridDim.x * blockDim.x;
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned stride = ( n / ELEM_PER_VECTOR );
	
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid / stride;

	float result = 0.0f;
	for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
	{
		result += constantMatA[ eid ][ j ] * vecX[ vid * ELEM_PER_VECTOR + j ];
	}
	vecY[ vid * ELEM_PER_VECTOR + eid ] = result;
}

// 이거 아직 실험해볼 코드가 구성이 안됨
__global__ void MultMatVec_GPU_BroadcastConstantMatrixWithRegister_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;

	float result;
	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		result = 0.0f;
		for( unsigned j = 0; j < ELEM_PER_VECTOR; ++j )
		{
			result += constantMatA[ i ][ j ] * vecX[ tid * ELEM_PER_VECTOR + j ];
		}
		vecY[ tid * ELEM_PER_VECTOR + i ] = result;
	}
}

__global__ void MultMatVec_GPU_GlobalMemoryWithRegister_Vector_Direction( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = blockIdx.x * blockDim.x + threadIdx.x;

	float result = 0.0f;
	
	
}

//extern __shared__ unsigned char sharedBuffer[];
//__shared__ float sharedVecX[];
__global__ void MultMatVec_GPU_SharedMemoryWithAtomic_Vector( OUT float* vecY, IN float( *matA )[ ELEM_PER_VECTOR ], IN float* vecX )
{
	unsigned tid = ( blockIdx.x * blockDim.x ) + threadIdx.x;
	unsigned vid = tid / ELEM_PER_VECTOR;
	unsigned eid = tid % ELEM_PER_VECTOR;

	unsigned ratio = 1024 / blockDim.x; // num of elements in 32x32 Matrix
	__shared__ float sharedMatA[ 1024 ];
	__shared__ float sharedVecY[ 128 ];

	// normally access
	// GTX 690, 29.x ms
	//for( unsigned i = 0; i < ratio; ++i )
	//	sharedMatA[ threadIdx.x * ratio + i ] = ( ( float* )matA )[ threadIdx.x * ratio + i ];
	//__syncthreads();

	// coalesed access
	// GTX 690, 25.x ms
	int accessID = ( threadIdx.x / ELEM_PER_VECTOR ) * (ELEM_PER_VECTOR * ratio) + ( threadIdx.x % ELEM_PER_VECTOR );
	for(unsigned i = 0; i < ratio; ++i )
		sharedMatA[ accessID + i * ELEM_PER_VECTOR ] = ( ( float* )matA )[ accessID + i * ELEM_PER_VECTOR ];
	__syncthreads( );

	for( unsigned i = 0; i < ELEM_PER_VECTOR; ++i )
	{
		sharedVecY[ threadIdx.x ] = 0.0f;
		atomicAdd( &sharedVecY[ threadIdx.x ], sharedMatA[ i * ELEM_PER_VECTOR + eid ] * vecX[ vid * ELEM_PER_VECTOR + eid ] );
		vecY[ vid * ELEM_PER_VECTOR + i ] = sharedVecY[ threadIdx.x ];

		//vecY[ vid * ELEM_PER_VECTOR + i ] = 0.0f;
		//atomicAdd( &vecY[ vid * ELEM_PER_VECTOR + i ], sharedMatA[ i * ELEM_PER_VECTOR + eid ] * vecX[ vid * ELEM_PER_VECTOR + eid ] );
	}
}

// Coded by 20141500
// Wrong calculation
__global__ void gpu_haaam_1D( OUT float *y, IN float( *M )[ ELEM_PER_VECTOR ], IN float *x )
{
	unsigned j = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned n = blockDim.x * gridDim.x;
	//if( j >= n )
	//	return;

	for( unsigned i = 0; i<ELEM_PER_VECTOR; ++i ) {
		float result = 0;
		for( unsigned k = 0; k<ELEM_PER_VECTOR; ++k ) {
			result += M[ i ][ k ] * x[ k * n + j ];
		}
		y[ i * n + j ] = result;
	}
}