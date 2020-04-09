#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ELOG.h"

using namespace std;
class EMATRIXF//AVOIDING SHARING PTR TO PROTECT DATA
{
public:
	float* vectors;
	int m; //row
	int n; //column
	EMATRIXF::EMATRIXF();
	EMATRIXF::EMATRIXF(int m, int n);
	EMATRIXF::~EMATRIXF();
	void EMATRIXF::operator = (const float* matrix);
	void EMATRIXF::operator = (const EMATRIXF &matrix);
	EMATRIXF* EMATRIXF:: operator * (const EMATRIXF &B);
	EMATRIXF* EMATRIXF:: operator * (const float f);
	EMATRIXF* EMATRIXF::operator + (const  float f);
	EMATRIXF* EMATRIXF::T();
	void EMATRIXF::info();
	void EMATRIXF::m2txt(std::string name);
	void EMATRIXF::COLUMCUT(int st, int ed);
	void EMATRIXF::ROWCUT(int st, int ed);
	void EMATRIXF::ZEROS();
	void EMATRIXF::ONES();
	void EMATRIXF::IDENTITY();
};
//MATRIX IN ONG DOUBLE
EMATRIXF* MATRIXADDROW(EMATRIXF* A, EMATRIXF* B);
EMATRIXF* MATRIXADDCOL(EMATRIXF* A, EMATRIXF* B);
EMATRIXF* Least_Square_Solution(EMATRIXF* A, EMATRIXF* B);
EMATRIXF* Guassain_elimination(EMATRIXF* A, int limit);
EMATRIXF* PROJECTION(EMATRIXF* A, EMATRIXF* B);

int MAX(int A, int B);
int MIN(int A, int B);
void swap_rows(EMATRIXF* A, int r1, int r2);
void Multi_rows(EMATRIXF* A, int r1, float num);
void Add_rows(EMATRIXF* A, int r1, int r2);
void Sub_rows(EMATRIXF* A, int r1, int r2);
void Div_rows(EMATRIXF* A, int r1, float num);
void Multi_Add_rows(EMATRIXF* A, int r1, int r2, float num);