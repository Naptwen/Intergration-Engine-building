#pragma once
#include "EMATRIXF.h"

EMATRIXF::EMATRIXF()
{	
	this->m = 0;
	this->n = 0;
	vectors = NULL;
}
EMATRIXF::EMATRIXF(int m, int n)
{
	this->m = m;
	this->n = n;
	int count = m * n;
	vectors = (float*)calloc(count, sizeof(float));
}
//FREE Dynamic array of this->vector
EMATRIXF::~EMATRIXF()
{
	//if ((this->vectors != NULL) == true)
	//	free(this->vectors);
	vectors = NULL;
}
void EMATRIXF::operator = (const float* matrix)
{
	int count = this->m * this->n;
	int size = count * sizeof(float);
	memmove(vectors, matrix, size);
}
void EMATRIXF::operator = (const EMATRIXF &matrix)
{
 	if (this->vectors != NULL)
		std::free((void*)vectors);
	int count = matrix.m * matrix.n;
	int size = count * sizeof(float);
	this->vectors = (float*)calloc(count, sizeof(float));
	memmove(this->vectors, matrix.vectors, size);
	this->m = matrix.m;
	this->n = matrix.n;
}
EMATRIXF* EMATRIXF::operator * (const  EMATRIXF &B)
{
	if (this->n == B.m)
	{
		EMATRIXF* C = (EMATRIXF*)malloc(sizeof(EMATRIXF));
		//A = m x n and B = n x p So C=AB is  m x p
		int Cm = this->m;
		int Cn = B.n;
		int size_C = Cm * Cn;
		float* matrix_C = (float*)calloc(size_C, sizeof(float));
		int x = 0, y = 0, k = 0;
		for (int i = 0; i < size_C; i++)
		{
			x = i / Cn; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			float sum = 0;//														     (1,0)
			for (int k = 0; k < this->n; k++) // from 0 to n
			{
				int index_A = k + x * this->n; //Axk
				int index_B = y + k * B.n;     //Bky
				float temp = this->vectors[index_A] * B.vectors[index_B];

				sum += temp;
			}
			matrix_C[i] = sum; //Cxy = Sigma (Axk * Bky)
		}
		C->m = Cm;
		C->n = Cn;
		C->vectors = matrix_C;
		return C;
	}
	else
	{
		elog << "ERROR * :"+ to_string(this->n) + " != " + to_string(B.m) + " MATRIX SIZE IS NOT SAME\n";
		return nullptr;
	}
}
EMATRIXF* EMATRIXF::operator * (const  float f)
{
	EMATRIXF* C = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	int count = this->m * this->n;
	C->vectors = (float*)malloc(count * sizeof(float));
	memmove(C->vectors, this->vectors, count * sizeof(float));
	int length = this->m * this->n;
	for (int k = 0; k < length; k++)// from 0 to n
		C->vectors[k] = C->vectors[k] * f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIXF* EMATRIXF::operator + (const  float f)
{
	EMATRIXF* C = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	int count = this->m * this->n;
	C->vectors = (float*)malloc(count * sizeof(float));
	memmove(C->vectors, this->vectors, count * sizeof(float));
	int length = this->m * this->n;
	for (int k = 0; k < length; k++)
		C->vectors[k] = C->vectors[k] + f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIXF* EMATRIXF::T()
{
	int size_A = m * n;
	int x = 0, y = 0, temp_m = 0,  j = 0, k = 0;
	//-----------------
	float* AT = (float*)calloc(size_A, sizeof(float));
	for (int i = 0; i < size_A; i++)
	{
		x = i / n;
		y = i % n;
		j = x + y * m; //reverse
		AT[j] = vectors[y + x * n];
	}
	//-----------------
	EMATRIXF* TRANSA = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	TRANSA->m = this->n;
	TRANSA->n = this->m;
	TRANSA->vectors = AT;
	return TRANSA;
}
void EMATRIXF::info()
{
	elog << "-----------------------------------------\n";
	int length = this->m * this->n;
	_logtxt << " m = " + to_string(this->m) + " |  n = " + to_string(this->n) + "\n";
	int y = 1;
	for (int i = 0; i < length; i++)
	{
		elog << to_string(this->vectors[i]) + ",";
		//cout << (float) this->vectors[i] << ", ";
		if (y == this->n)
		{
			elog << "Next row\n";
			y = 1;
		}
		else
			y++;
	}
	_logtxt<< "-----------------------------------------\n";
}
void EMATRIXF::m2txt(string name)
{
	int length = m * n;
	string out_line;
	ofstream out(name);
	int x = 1;
	for (int i = 0; i < length; i++)
	{
		out << to_string(this->vectors[i]) + " , ";
		if (x == this->n)
		{
			out <<"\n";
			x = 1;
		}
		else
			x++;
	}
	out.close();
}
void EMATRIXF::COLUMCUT(int st, int ed)
{
	if (ed - st > 0 && ed < this->n && st >= 0)
	{
		int range = ed - st + 1;
		float* press = (float*)malloc(sizeof(float) * range * this->m);
		int k = 0;
		for (int j = 0; j < this->m; j++)
			for (int i = st; i <= ed; i++)
			{
				memmove(&press[k], &this->vectors[i + j * this->n], sizeof(float));
				k++;
			}
		if(this->vectors != NULL)
			free(this->vectors);
		this->vectors = press;
		this->n = ed - st + 1;
	}
	else
	{
		elog << "ERROR:: size is not defined : " + to_string(ed-st) + "\n";
	}
}
void EMATRIXF::ROWCUT(int st, int ed)
{
	if (ed - st > 0 && ed < this->m && st >= 0)
	{
		int range = ed - st + 1;
		float* press = (float*)malloc(sizeof(float) * range * this->n);
		int k = 0;
		for (int i = st; i <= ed; i++)
			for (int j = 0; j < this->n; j++)
			{
				memmove(&press[k], &this->vectors[j + i * this->n], sizeof(float));
				k++;
			}
		if (this->vectors != NULL)
			free(this->vectors);
		this->vectors = press;
		this->m = ed - st + 1;
	}
	else
	{
		elog << "ERROR:: size is not defined\n";
	}
}
void EMATRIXF::ZEROS() 
{
	if (this->vectors != NULL)
		std::free(this->vectors);
	int size = 0;
	size = this->m* this->n;
	this->vectors = (float*)calloc(size, sizeof(float));
}
void EMATRIXF::IDENTITY()
{
	int k = MIN(this->m,this->n);
	this->ZEROS();
	for (int i = 0; i < k; i++)
		this->vectors[i + i * this->n] = 1;
}
void EMATRIXF::ONES() 
{
	int size = this->m * this->n;
	for (int i = 0; i < size; i++)
	{
		this->vectors[i] = 1;
	}
}

EMATRIXF* MATRIXADDROW(EMATRIXF* A, EMATRIXF* B)
{
	if (A->n != B->n)
	{
		elog << "ERROR! A->n and B->n is not equal\n";
		return nullptr;
	}
	else
	{
		int size_AlB = ((A->m) * (A->n) + (B->m) * (B->n));
		int AlB_m = A->m + B->m;
		int AlB_n = A->n;
		EMATRIXF* AlB = (EMATRIXF*)malloc(sizeof(EMATRIXF));
		AlB->m = AlB_m;
		AlB->n = AlB_n;
		AlB->vectors = (float*)malloc(sizeof(float) * size_AlB);
		for (int i = 0; i < size_AlB; i++)
		{
			int m = i / AlB_n;
			int n = i % AlB_n;
			if (m < A->m)
				AlB->vectors[i] = A->vectors[n + m * A->n];
			else
			{
				int Bm = m - A->m;
				AlB->vectors[i] = B->vectors[n + Bm * B->n];
			}
		}
		return AlB;
	}
}
EMATRIXF* MATRIXADDCOL(EMATRIXF* A, EMATRIXF* B)
{
	if (A->m != B->m)
	{
		elog << "ERROR! A->m and B->m is not equal\n";
		return nullptr;
	}
	else
	{
		int size_A_B = ((A->m) * (A->n) + (B->m) * (B->n));
		int A_B_m = A->m;
		int A_B_n = A->n + B->n;
		EMATRIXF* A_B = (EMATRIXF*)malloc(sizeof(EMATRIXF));
		A_B->m = A_B_m;
		A_B->n = A_B_n;
		A_B->vectors = (float*)malloc(sizeof(float) * size_A_B);
		for (int i = 0; i < size_A_B; i++)
		{
			int m = i / A_B_n;
			int n = i % A_B_n;
			if (n < A->n)
				A_B->vectors[i] = A->vectors[n + m * A->n];
			else
			{
				int Bn = n - A->n;
				A_B->vectors[i] = B->vectors[Bn + m * B->n];
			}
		}
		return A_B;
	}
}
EMATRIXF* Least_Square_Solution(EMATRIXF* A, EMATRIXF* B)
//A(m,n) and B(n,p) must be required
{
	EMATRIXF* AT = A->T();
	EMATRIXF* ATA = (*AT) * (*A);
	EMATRIXF* ATB = (*AT) * (*B);
	EMATRIXF* AB = MATRIXADDCOL(ATA, ATB);
	EMATRIXF* G = Guassain_elimination(AB, AB->n - ATB->n);
	G->COLUMCUT(AB->n -ATB->n , AB->n - 1);

	free(AT);
	free(ATA);
	free(ATB);
	free(AB);
	return G;
}
EMATRIXF* Guassain_elimination(EMATRIXF* A, int limit) {
	//Initializing
	EMATRIXF* G = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	G->m = A->m;
	G->n = A->n;
	int size_G = G->m * G->n;
	G->vectors = (float*)malloc(sizeof(float) * size_G);
	memmove(G->vectors, A->vectors, sizeof(float) * size_G);

	int n = 0;
	int m = 0;
	for (int n = 0; n < limit; n++)
	{
		float current = G->vectors[n + m * G->n];
		//If it is Zero
		if (current == 0)
			for (int k = 0; k < G->m; k++)
				if (G->vectors[n + k * G->n] != 0)
					swap_rows(G, k, m);
		//IF the column has at least one nonzero rows.
		if (current != 0)
		{
			//if it is no 1
			if (current != 1)
				Div_rows(G, m, G->vectors[n + m * G->n]);
			//Seaching other nonzero in the same column
			for (int k = 0; k < G->m; k++)
				if (m != k && G->vectors[n + k * G->n] != 0)
					Multi_Add_rows(G, k, m, -G->vectors[n + k * G->n]);
			//increase the row 
			//Since in Linear Algebra # pivot < # colum
			m++;
		}
	}
	return G;
}
EMATRIXF* PROJECTION(EMATRIXF* A, EMATRIXF* B)
{
	EMATRIXF* AT = A->T();
	EMATRIXF* ATA = (*AT) * (*A);
	EMATRIXF* ATB = (*AT) * (*B);
	EMATRIXF* AB = MATRIXADDCOL(ATA, ATB);
	EMATRIXF* G = Guassain_elimination(AB, AB->n - ATB->n);
	G->COLUMCUT(AB->n - ATB->n, AB->n - 1);
	EMATRIXF* P = (*A) * (*G);
	free(AT);
	free(ATA);
	free(ATB);
	free(AB);
	free(G);
	return P;
}






//calculation
int MAX(int A, int B) { if (A > B) return A; return B; }
int MIN(int A, int B) { if (A > B) return B; return A; }
//Row Operator
void swap_rows(EMATRIXF* A, int r1, int r2)
{
	if(r1 != r2)
		for (int k = 0; k < A->n; k++)
		{
			float temp = A->vectors[k + r1 * A->n];
			A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n];
			A->vectors[k + r2 * A->n] = temp;
		}
}
void Add_rows(EMATRIXF* A, int r1, int r2)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n];
	}
}
void Sub_rows(EMATRIXF* A, int r1, int r2)//subtract R1- R2
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] =A->vectors[k + r1 * A->n] - A->vectors[k + r2 * A->n];
	}
}
void Multi_rows(EMATRIXF* A, int r1, int r2)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n] * A->vectors[k + r1 * A->n];
	}
}
void Div_rows(EMATRIXF* A, int r1, float num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n]/num;
	}
}
void Multi_Add_rows(EMATRIXF* A, int r1, int r2, float num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n]*num;
	}
}
