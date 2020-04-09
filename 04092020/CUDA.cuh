#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "EMATRIXF.h"
#include "EMATRIXD.h"

#define MULTI 197
#define ADD_COL 201
#define ADD_ROW 202
#define TRANS 205
#define CUT_COL 211
#define CUT_ROW 218
#define COPY_COL 220

//A front matrix B is back matrix
//m -> C's m
//p -> A's n
//n -> C's n
__global__
void CUDA_MATRIX_MULTI(int m, int p, int n, int trash, double* A, double* B, double* C);
//A front matrix B is back matrix
//m -> A's m or B's m
//nC -> C's n or B's m
//nA -> A's n
__global__
void CUDA_MATRIX_ADD_COLUMN(int m, int nC, int nA, int trash, double* A, double* B, double* C);
//A front matrix B is back matrix
//mC -> C's m
//nC -> C'n or A'n or B'n
//mA -> A's m
__global__
void CUDA_MATRIX_ADD_ROW(int mC, int nC, int mA, int trash, double* A, double* B, double* C);
//MULTI : A*B
//ADD_ROW : A + B vertically add extra column
//ADD_COL : A + B horizontally add extra rows
//TRANS : A^T , B is NULL, 
__global__
void CUDA_MATRIX_TRANS(int mC, int nC, int  mnC, int trash, double* A, double* trash2, double* C);
//nA is # column in A 
//start column number remember 0 is 1, 1 is 2
//END column number remember 0 is 1, 1 is 2
//B is NULL
__global__
void CUDA_MATRIX_CUT_COL(int mA, int nA, int start, int end, double* A, double* trahs, double* C);
//start row number remember 0 is 1, 1 is 2
//END row number remember 0 is 1, 1 is 2
//B is NULL
__global__
void CUDA_MATRIX_CUT_ROW(int mA, int nA, int start, int end, double* A, double* trash, double* C);
//end is the index of A ex)if you want to increase 1 more end must be 1
__global__
void CUDA_MATRIX_COPY_COL(int mA, int nA, int trash_int, int end, double* A, double* trash, double* C);
//start and end are only used for the MATRIX CUT 
//So in the other case let it as zero or null
//*[MULTI]   :need A, B	*[CUT_COL] : need start end  
//*[ADD_COL] :need A, B	*[CUT_ROW] : need start end
//*[ADD_ROW] :need A, B	*[COPY_COL]: need end(end is index)
//*[TRANS]   :need Only A	
EMATRIXD* CUDA_MATRIX_CONVERT(EMATRIXD* A, EMATRIXD* B, int start, int end, const unsigned char FUN);