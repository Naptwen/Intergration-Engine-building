#pragma once
#include "EMATRIXD.h"
#include "CUDA.cuh"

#define Soft_ReLU 1
#define Noisy_ReLU 2
#define Leaky_ReLU 3
#define ELU 4
#define MEAN 1
#define CROSS 2
class NEURAL_NET
{
public:
	EMATRIXD Input_layer;
	EMATRIXD Hidden_layer;
	EMATRIXD Active_layer;
	EMATRIXD Output_layer;
	int r;
	int e;
	//r - import part
	//e - export part
	NEURAL_NET::NEURAL_NET(int r, int m, int n, int e);
	void NEURAL_NET::NEURAL_IMPORT(EMATRIXD& A);
	void NEURAL_NET::INPUT_VALUE(double value[]);
	EMATRIXD* NEURAL_NET::NEURAL_EXPORT();
};
namespace neural_tool 
{
	//Generating neural
	void FLOW_DATA(NEURAL_NET* A);
	//TYPE : MEAN - Mean square Error, CROSS - Cross Entropy
	//GIVE ME A m*1 matrix
	double ERROR_CHECK(EMATRIXD data, EMATRIXD target, const char TYPE);
	//TYPE : ReLU, NOisyReLU, Leaky_ReLU
	double ACTIVATION_FUN(double A, const char TYPE);
}
namespace data_tool 
{
	//Data Scailing
	void Median_Normalize(double* data, int size);
	void Mean_Normalize(double* data, int size);
	void Quantile_Normarlize(double* data, int size);
	double minArray(double* data, int size);
	double maxArray(double* data, int size);
	void buble_sorting(double* data, int size);
	double Find_Mean(double data[], int size);
	double Standard(double* data, double Ed, int size);
}