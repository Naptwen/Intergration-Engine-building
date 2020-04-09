#pragma once
#include "EMATRIXD.h"

EMATRIXD::EMATRIXD()
{
	this->m = 0;
	this->n = 0;
	vectors = NULL;
}
EMATRIXD::EMATRIXD(unsigned int m, unsigned int n)
{
	this->m = m;
	this->n = n;
	int count = m * n;
	vectors = (double*)calloc(count, sizeof(double));
}
//FREE Dynamic array of this->vector
EMATRIXD::~EMATRIXD()
{
	//if ((this->vectors != NULL) == true)
	//	free(this->vectors);
	vectors = NULL;
}
void EMATRIXD::operator = (const double* matrix)
{
	unsigned int count = this->m * this->n;
	unsigned int size = count * sizeof(double);
	memmove(vectors, matrix, size);
}
void EMATRIXD::operator = (const EMATRIXD& matrix)
{
	if (this->vectors != NULL)
		std::free((void*)vectors);
	unsigned int count = matrix.m * matrix.n;
	unsigned int size = count * sizeof(double);
	this->vectors = (double*)calloc(count, sizeof(double));
	memmove(this->vectors, matrix.vectors, size);
	this->m = matrix.m;
	this->n = matrix.n;
}
EMATRIXD* EMATRIXD::operator * (const  EMATRIXD& B)
{
	if (this->n == B.m)
	{
		EMATRIXD* C = (EMATRIXD*)malloc(sizeof(EMATRIXD));
		//A = m x n and B = n x p So C=AB is  m x p
		unsigned int Cm = this->m;
		unsigned int Cn = B.n;
		unsigned int size_C = Cm * Cn;
		double* matrix_C = (double*)calloc(size_C, sizeof(double));
		unsigned int x = 0, y = 0, k = 0;
		for (unsigned int i = 0; i < size_C; i++)
		{
			x = i / Cn; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			double sum = 0;//														     (1,0)
			for (unsigned int k = 0; k < this->n; k++) // from 0 to n
			{
				unsigned int index_A = k + x * this->n; //Axk
				unsigned int index_B = y + k * B.n;     //Bky
				double temp = this->vectors[index_A] * B.vectors[index_B];

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
		elog << "ERROR * :" + to_string(this->n) + " != " + to_string(B.m) + " MATRIX SIZE IS NOT SAME\n";
		return nullptr;
	}
}
EMATRIXD* EMATRIXD::operator * (const  double f)
{
	EMATRIXD* C = (EMATRIXD*)malloc(sizeof(EMATRIXD));
	unsigned int count = this->m * this->n;
	C->vectors = (double*)malloc(count * sizeof(double));
	memmove(C->vectors, this->vectors, count * sizeof(double));
	unsigned int length = this->m * this->n;
	for (unsigned int k = 0; k < length; k++)// from 0 to n
		C->vectors[k] = C->vectors[k] * f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIXD* EMATRIXD::operator + (const double f)
{
	EMATRIXD* C = (EMATRIXD*)malloc(sizeof(EMATRIXD));
	unsigned int count = this->m * this->n;
	C->vectors = (double*)malloc(count * sizeof(double));
	memmove(C->vectors, this->vectors, count * sizeof(double));
	unsigned int length = this->m * this->n;
	for (unsigned int k = 0; k < length; k++)
		C->vectors[k] = C->vectors[k] + f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIXD* EMATRIXD::T()
{
	unsigned int size_A = m * n;
	unsigned int x = 0, y = 0, temp_m = 0, j = 0, k = 0;
	//-----------------
	double* AT = (double*)calloc(size_A, sizeof(double));
	for (unsigned int i = 0; i < size_A; i++)
	{
		x = i / n;
		y = i % n;
		j = x + y * m; //reverse
		AT[j] = vectors[y + x * n];
	}
	//-----------------
	EMATRIXD* TRANSA = (EMATRIXD*)malloc(sizeof(EMATRIXD));
	TRANSA->m = this->n;
	TRANSA->n = this->m;
	TRANSA->vectors = AT;
	return TRANSA;
}
void EMATRIXD::pinfo() 
{
	cout << "-----------------------------------------\n";
	unsigned int length = this->m * this->n;
	cout<< " m = " + to_string(this->m) + " |  n = " + to_string(this->n) + "\n";
	unsigned int y = 1;
	for (unsigned int i = 0; i < length; i++)
	{
		cout << to_string(this->vectors[i]) + ",";
		if (y == this->n)
		{
			cout << "\n";
			y = 1;
		}
		else
			y++;
	}
	cout << "-----------------------------------------\n";
}
void EMATRIXD::info()
{
	elog << "-----------------------------------------\n";
	unsigned int length = this->m * this->n;
	_logtxt << " m = " + to_string(this->m) + " |  n = " + to_string(this->n) + "\n";
	unsigned int y = 1;
	for (unsigned int i = 0; i < length; i++)
	{
		elog << to_string(this->vectors[i]) + ",";
		if (y == this->n)
		{
			elog << "Next row\n";
			y = 1;
		}
		else
			y++;
	}
	_logtxt << "-----------------------------------------\n";
}
void EMATRIXD::m2txt(string name)
{
	unsigned int length = m * n;
	string out_line;
	ofstream out(name);
	unsigned int x = 1;
	for (unsigned int i = 0; i < length; i++)
	{
		out << to_string(this->vectors[i]) + " , ";
		if (x == this->n)
		{
			out << "\n";
			x = 1;
		}
		else
			x++;
	}
	out.close();
}
void EMATRIXD::COLUMCUT(unsigned int st, unsigned int ed)
{
	if (ed - st > 0 && ed < this->n && st >= 0)
	{
		unsigned int range = ed - st + 1;
		double* press = (double*)malloc(sizeof(double) * range * this->m);
		unsigned int k = 0;
		for (unsigned int j = 0; j < this->m; j++)
			for (unsigned int i = st; i <= ed; i++)
			{
				memmove(&press[k], &this->vectors[i + j * this->n], sizeof(double));
				k++;
			}
		if (this->vectors != NULL)
			free(this->vectors);
		this->vectors = press;
		this->n = ed - st + 1;
	}
	else
	{
		elog << "ERROR:: size is not defined : " + to_string(ed - st) + "\n";
	}
}
void EMATRIXD::ROWCUT(unsigned int st, unsigned int ed)
{
	if (ed - st > 0 && ed < this->m && st >= 0)
	{
		unsigned int range = ed - st + 1;
		double* press = (double*)malloc(sizeof(double) * range * this->n);
		unsigned int k = 0;
		for (unsigned int i = st; i <= ed; i++)
			for (unsigned int j = 0; j < this->n; j++)
			{
				memmove(&press[k], &this->vectors[j + i * this->n], sizeof(double));
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
void EMATRIXD::ZEROS()
{
	if (this->vectors != NULL)
		std::free(this->vectors);
	unsigned int size = 0;
	size = this->m * this->n;
	this->vectors = (double*)calloc(size, sizeof(double));
}
void EMATRIXD::IDENTITY()
{
	unsigned int k = this->m;
	if (this->m < this->n)
		k = this->n;	
	this->ZEROS();
	for (unsigned int i = 0; i < k; i++)
		this->vectors[i + i * this->n] = 1;
}
void EMATRIXD::ONES()
{
	unsigned int size = this->m * this->n;
	for (unsigned int i = 0; i < size; i++)
	{
		this->vectors[i] = 1;
	}
}

EMATRIXD* GAUSSELI(EMATRIXD* A, int limit) {
	//Initializing
	EMATRIXD* G = (EMATRIXD*)malloc(sizeof(EMATRIXD));
	G->m = A->m;
	G->n = A->n;
	int size_G = G->m * G->n;
	G->vectors = (double*)malloc(sizeof(double) * size_G);
	memmove(G->vectors, A->vectors, sizeof(double) * size_G);

	int n = 0;
	int m = 0;
	for (int n = 0; n < limit; n++)
	{
		double current = G->vectors[n + m * G->n];
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
void swap_rows(EMATRIXD* A, int r1, int r2)
{
	if (r1 != r2)
		for (int k = 0; k < A->n; k++)
		{
			float temp = A->vectors[k + r1 * A->n];
			A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n];
			A->vectors[k + r2 * A->n] = temp;
		}
}
void Div_rows(EMATRIXD* A, int r1, double num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] / num;
	}
}
void Multi_Add_rows(EMATRIXD* A, int r1, int r2, double num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n] * num;
	}
}