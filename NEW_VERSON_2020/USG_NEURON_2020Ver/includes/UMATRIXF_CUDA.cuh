#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef UMATRIXF_CUDA_H__
#define UMATRIXF_CUDA_H__

#define KERNEL <<< blockPerGrid, threadsPerBlock >>>

#pragma once
#include "UMATRIXF.h"
#include <cuda.h>
#include <time.h>
#include <curand_kernel.h>
#include "device_launch_parameters.h"
#include "device_functions.h"
#include "cuda_runtime.h"


#define MULTI 197
#define ADD_COLS 217
#define ADD_ROWS 203
#define TRANS 205
#define CUT_COLS 211
#define CUT_ROWS 218
#define COPY_ROWS 220
using namespace UMF;
namespace UMCF {
	__global__ void UCUDA_SUBTRACT(unsigned int size, float* A, float* B, float* C);
	//A front matrix B is back matrix
	//m -> C's m
	//p -> A's n
	//n -> C's n

	__global__ void UCUDA_MATRIX_MULTI(unsigned int m, unsigned int p, unsigned int n, unsigned int trash, float* A, float* B, float* C);
	//A front matrix B is back matrix
	//m -> A's m or B's m
	//nC -> C's n or B's m
	//nA -> A's n

	__global__ void UCUDA_MATRIX_ADD_ROW(unsigned int m, unsigned int nC, unsigned int nA, unsigned int trash, float* A, float* B, float* C);
	//A front matrix B is back matrix
	//mC -> C's m
	//nC -> C'n or A'n or B'n
	//mA -> A's m

	__global__ void UCUDA_MATRIX_ADD_COLUMN(unsigned int mC, unsigned int nC, unsigned int mA, unsigned int trash, float* A, float* B, float* C);
	//MULTI : A*B
	//ADD_ROW : A + B vertically add extra column
	//ADD_COL : A + B horizontally add extra rows
	//TRANS : A^T , B is NULL, 

	__global__ void UCUDA_MATRIX_TRANS(unsigned int mC, unsigned int nC, unsigned int  mnC, unsigned int trash, float* A, float* trash2, float* C);
	//nA is # column in A 
	//start column number remember 0 is 1, 1 is 2
	//END column number remember 0 is 1, 1 is 2
	//B is NULL

	__global__ void UCUDA_MATRIX_CUT_ROW(unsigned int mA, unsigned int nA, unsigned int start, unsigned int end, float* A, float* trash, float* C);

	__global__ void UCUDA_MATRIX_CUT_COL(unsigned int mA, unsigned int nA, unsigned int start, unsigned int end, float* A, float* trash, float* C);
	//end is the index of A ex)if you want to increase 1 more end must be 1

	__global__ void UCUDA_MATRIX_COPY_ROW(unsigned int mA, unsigned int nA, unsigned int nC, unsigned int end, float* A, float* trash, float* C);
	//start and end are only used for the MATRIX CUT 
	//So in the other case let it as zero or null
	//*[MULTI]   :need A, B	*[CUT_COL] : need start & end  
	//*[ADD_COL] :need A, B	*[CUT_ROW] : need start & end
	//*[ADD_ROW] :need A, B	*[COPY_ROW]: need end is new adding size
	//*[TRANS]   :need Only A	
	//The thread size is 1024 which is the maximum of the cuda

	UMATRIXF* UCUDA_MATRIX_CONVERT(
		UMATRIXF* A, UMATRIXF* B,
		unsigned int start, unsigned int end, const unsigned char FUN);
}
#endif
