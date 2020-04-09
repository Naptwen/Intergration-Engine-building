#include "CUDA.cuh"

__global__
void CUDA_MATRIX_MULTI(int m, int p, int n, int trash, double* A, double* B, double* C)
{
	int k = threadIdx.x + blockIdx.x * blockDim.x; //index of C

	if (k < m * n) {
		double temp = 0;
		for (int t = 0; t < p; t++) {
			temp += A[t + k / n * p] * B[k % n + t * n];
		}
		C[k] = temp;
	}
}
__global__
void CUDA_MATRIX_ADD_COLUMN(int m, int nC, int nA, int trash, double* A, double* B, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int mnC = m * nC;
	int nB = nC - nA;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		if (y < nA)
			C[y + x * nC] = A[y + x * nA];
		else
			C[y + x * nC] = B[y - nA + x * nB];
	}
}
__global__
void CUDA_MATRIX_ADD_ROW(int mC, int nC, int mA, int trash, double* A, double* B, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int mnC = mC * nC;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		if (x < mA)
			C[y + x * nC] = A[y + x * nC];
		else
			C[y + x * nC] = B[y + (x - mA) * nC];
	}
}
__global__
void CUDA_MATRIX_TRANS(int mC, int nC, int  mnC, int trash, double* A, double* trash2, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		C[y + x * nC] = A[x + y * mC];
	}
}
__global__
void CUDA_MATRIX_CUT_COL(int mA, int nA, int start, int end, double* A, double* trahs, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{
		int x = i / nA;
		int y = i % nA;
		if (y >= start && y <= end)
			C[y - start + x * (end - start + 1)] = A[i];
	}
}
__global__
void CUDA_MATRIX_CUT_ROW(int mA, int nA, int start, int end, double* A, double* trash, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{
		int x = i / nA;
		int y = i % nA;
		if (x >= start && x <= end)
			C[y + (x - start) * nA] = A[i];
	}
}
__global__
void CUDA_MATRIX_COPY_COL(int mA, int nA, int nC, int end, double* A, double* trash, double* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{	
		for (int k = 0; k <= end; k++)
		{
			C[k + i * nC] = A[i];
		}
	}
}
EMATRIXD* CUDA_MATRIX_CONVERT(EMATRIXD* A, EMATRIXD* B, int start, int end, const unsigned char FUN)
{
	double* CUDA_A = NULL;
	double* CUDA_B = NULL;
	double* CUDA_C = NULL;
	int size_A = 0, size_B = 0, size_C = 0;
	if (A != nullptr)
	{

		size_A = sizeof(double) * A->m * A->n;
		cudaMalloc(&CUDA_A, size_A);
		cudaMemcpy(CUDA_A, A->vectors, size_A, cudaMemcpyHostToDevice);
	}
	if (B != nullptr)
	{

		size_B = sizeof(double) * B->m * B->n;
		cudaMalloc(&CUDA_B, size_B);
		cudaMemcpy(CUDA_B, B->vectors, size_B, cudaMemcpyHostToDevice);

	}
	EMATRIXD* C = (EMATRIXD*)malloc(sizeof(EMATRIXD));
	void (*temp)(int, int, int, int, double*, double*, double*);
	long int bx = 0, by = 0, bz = 0, bi = 0;
	switch (FUN)
	{
	case MULTI:
		if (A->n != B->m)
		{
			elog << "[ERROR] MULTI MATRIX:" + to_string(A->n) + to_string(B->m) + "is not equal\n";
			return nullptr;
		}
		temp = CUDA_MATRIX_MULTI;
		size_C = A->m * B->n;
		C->m = A->m;
		C->n = B->n;
		bx = C->m;
		by = A->n;
		bz = C->n;
		break;
	case ADD_COL:
		if (A->m != B->m)
		{
			elog << "[ERROR] ADD COLUMN:" + to_string(A->m) + " : " + to_string(B->m) + "is not equal\n";
			return nullptr;
		}
		temp = CUDA_MATRIX_ADD_COLUMN;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m;
		C->n = A->n + B->n;
		bx = C->m;
		by = C->n;
		bz = A->n;
		break;
	case ADD_ROW:
		if (A->n != B->n)
		{
			elog << "[ERROR] ADD ROW:" + to_string(A->n) + " : " + to_string(B->n) + "is not equal\n";
			return nullptr;
		}
		temp = CUDA_MATRIX_ADD_ROW;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m + B->m;
		C->n = A->n;
		bx = C->m;
		by = C->n;
		bz = A->m;
		break;
	case TRANS:
		temp = CUDA_MATRIX_TRANS;
		size_C = A->n * A->m;//for the speed
		C->m = A->n;
		C->n = A->m;
		bx = C->m;
		by = C->n;
		bz = C->m * C->n;
		break;
	case CUT_COL:
		if (start <0 && end > A->n)
		{
			elog << "[ERROR] CUT COL:" + to_string(start) + " : " + to_string(end) + "is out of range\n";
			return nullptr;
		}
		temp = CUDA_MATRIX_CUT_COL;
		size_C = A->m * (end - start + 1);
		C->m = A->m;
		C->n = (end - start + 1);
		bx = A->m;
		by = A->n;
		bz = start;
		bi = end;
		break;
	case CUT_ROW:
		if (start <0 && end > A->m)
		{
			elog << "[ERROR] CUT ROW:" + to_string(start) + " : " + to_string(end) + "is out of range\n";
			return nullptr;
		}
		temp = CUDA_MATRIX_CUT_ROW;
		size_C = A->n * (end - start + 1);
		C->m = (end - start + 1);
		C->n = A->n;
		bx = A->m;
		by = A->n;
		bz = start;
		bi = end;
		break;
	case COPY_COL:
		temp = CUDA_MATRIX_COPY_COL;
		size_C = (A->n + end)* A->m;
		C->m = A->m;
		C->n = (A->n + end);
		bx = A->m;
		by = A->n;
		bz = C->n;
		bi = end;
		break;
	}

	cudaMalloc(&CUDA_C, sizeof(double) * size_C);
	dim3 blockPerGrid(512, 1, 1);
	dim3 threadsPerBlock(512, 1, 1);

	temp << <blockPerGrid, threadsPerBlock >> > (bx, by, bz, bi, CUDA_A, CUDA_B, CUDA_C);

	C->vectors = (double*)malloc(sizeof(double) * size_C);
	cudaMemcpy(C->vectors, CUDA_C, sizeof(double) * size_C, cudaMemcpyDeviceToHost);
	if (CUDA_A != NULL)
		cudaFree(CUDA_A);
	if (CUDA_B != NULL)
		cudaFree(CUDA_B);
	if (CUDA_C != NULL)
		cudaFree(CUDA_C);
	return C;
}