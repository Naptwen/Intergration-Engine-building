#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef UMATRIXF_H__
#define UMATRIXF_H__

#include <stdio.h>
#include <fstream>

namespace UMF 
{
	struct UMATRIXF//AVOIDING SHARING PTR TO PROTECT DATA
	{
	public:
		float* vectors = NULL;
		unsigned int m = 0; //row
		unsigned int n = 0; //column

		UMATRIXF();
		//[m] column number
		//[n] row number
		//The matrix coordinate is start from upper left to right bleow
		UMATRIXF(unsigned int m, unsigned int n);
		~UMATRIXF();
		//This Matrix receive the same type of data
		//The other data which out of the size is ignored
		void operator = (const float* matrix);
		void operator = (const UMATRIXF& matrix);
		UMATRIXF* operator * (const UMATRIXF& B);
		UMATRIXF* operator * (const float f);
		UMATRIXF* operator + (const  float f);
		UMATRIXF* T();

		void pinfo();
		void m2txt(const char* name);
		void ZEROS();
		void IDENTITY();
		void ONES();
	};

	UMATRIXF* ROWCUT(UMATRIXF* A, unsigned int st, unsigned int ed);

	UMATRIXF* COLUMCUT(UMATRIXF* A, unsigned int st, unsigned int ed);

	UMATRIXF* MATRIXADDCOL(UMATRIXF* A, UMATRIXF* B);

	UMATRIXF* MATRIXADDROW(UMATRIXF* A, UMATRIXF* B);

	UMATRIXF* GAUSSELI(UMATRIXF* A, int limit);

	UMATRIXF* COPY_ROW(UMATRIXF* A, unsigned int start, unsigned int howmany);

	void swap_rows(UMATRIXF* A, int r1, int r2);

	void Div_rows(UMATRIXF* A, int r1, float num);

	void Multi_Add_rows(UMATRIXF* A, int r1, int r2, float num);
}
#endif