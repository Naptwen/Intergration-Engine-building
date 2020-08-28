#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef UNEURONF_CUDA_H__
#define UNEURONF_CUDA_H__

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>//--------------------
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

#pragma once
#include "UNEURONF.h"
#include "UMATRIXF_CUDA.cuh"
#include  <device_atomic_functions.h>
using namespace UNF;
using namespace UMCF;
namespace UNCF {
	//-------------------CUDA BACK FUNCTIONS-------------------------------------

	__global__ void UCUDA_E_A_CAL(float* E_A_LIST, float* target, float* TOTAL_A_VEC, int _n, int size, const char ERROR_TYPE);

	__global__ void UCUDA_A_Z_CAL(float* TOTAL_Z_VEC, float* A_Z_LIST, int _n, int size, int k, char fn_TYPE);

	__global__ void UCUDA_Z_W_CAL(float* TOTAL_Z_VEC, float* Z_W_LIST, int _n, int k, int size);

	__global__ void UCUDA_TOTAL_E(float* E_TOTAL_LIST, float* E_A_LIST, float* A_Z_LIST, float* TOTAL_W_VEC, int _m, int _n, int e_size, int k);

	__global__ void UCUDA_UPDATE_W(float* ERROR_LIST, float* A_Z_LIST, float* Z_W_LIST, float* TOTAL_W_VEC, float* TOTAL_M_VEC, float* TOTAL_B_VEC, int _m, int _n, int k, float vr, float lr, char BTYPE);

	void GPU_BACK_PROPA(NEURON_BLOCKF* temp, float* target);
	//-------------------CUDA RUN FUNCTIONS-------------------------------------

	__global__ void UCUDA_1D22D(float* trg, float* src_z, int _m, int _n, int k);

	__global__ void UCUDA_ACTFN(float* trg, float* src_z, int _m, const char ACT_TYPE);
	//------------------------Statistics---------------------------------------

	__global__ void UCUDA_ATOMIC_ADD(float* src_z, float* sum, int _m);

	__global__ void UCUDA_ATOMIC_VAR(float* src_z, float* sum, float* std, int _m);

	__global__ void UCUDA_ATOMIC_NORM(float* src_z, float* std, float* sum, float* trg, int _m);
	//list cuda malloc
		//trg cuda malloc

	void CUDA_NORMALIZATION(float* list, float* trg, unsigned int _m);
	//Using GPGPU of NVIDIA
		//The factors of the algorithm are basse on the NEURON_BLOCK 
		//So the previous steps are required
		//The NORMALIZATION is ture of false

	void CUDA_RUN(NEURON_BLOCKF* temp, bool NORMALIZATION);
}
#endif