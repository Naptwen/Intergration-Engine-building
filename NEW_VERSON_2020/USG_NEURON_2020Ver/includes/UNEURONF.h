#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef UNEURONF_H__
#define UNEURONF_H__


#pragma once
#include <random>//c++ style random
#include "UMATRIXF.h"


#define ReLU 16
#define Soft_ReLU 17
#define Noisy_ReLU 18
#define Leaky_ReLU 19
#define ELU 4
#define SWISH 11
#define LOGISTIC 5
#define HYPERBOLIC 3
#define NONE 23
#define SIGMOID 67

#define MEAN 1
#define CROSS 2

#define XAVIER 87
#define HENORM 13

#define MOMEN 77
#define STOCHASTIC 79

#define NESTEROV 73
#define NEST 71

using std::vector;
using namespace UMF;
namespace UNF {
	
	//Using multiple process omp

	void Median_Normalize(float* data, unsigned int size);
	//Using multiple process omp

	void Mean_Normalize(float* data, unsigned int size);
	//Using multiple process omp

	void Quantile_Normarlize(float* data, unsigned int size);
	//Using multiple process omp

	int minArray(float* data, unsigned int size);
	//Using multiple process omp

	int maxArray(float* data, unsigned int size);
	//Using multiple process omp

	void buble_sorting(float* data, unsigned int size);

	float Find_Mean(float data[], unsigned int size);
	//Using multiple process omp

	float Standard(float* data, float Ed, unsigned int size);
	//Using multiple process omp

	float average(float* data, unsigned int size);
	//Using multiple process omp

	float variance(float* data, float avg, unsigned int size);

	float z_score(float true_mean, float avg, float var);

	float cumulative_std(float z);

	float Z_SCORE_TEST(float* list, unsigned int size);

	float STANDARD(float* list, unsigned int size);
	//****************************************************************************
	//When it is operated by '=' it is deep copy its elements
	//----------Provided algorithm list----------------
	//[Layer] - you can edit the layer size but always the last and first layer is output and input nodes
	//[Error type] - Cross entropy, Mean square
	//[Activation] - Soft_ReLU, Noisy_ReLU, Leaky_ReLU, ELU, SWISH, LOGISTIC, HYPERBOLIC, SIGMOID, NONE
	//[Back propagaion] - Stochastic Gradient Descent,  Momentum, Nesterov
	//****************************************************************************
	struct NEURON_BLOCKF
	{
	private:
		//This function return the result of the activation function by ACT_TYPE
		//The ACT_TYPE is listed in the activation function type list
		float ACTIVATION(float y, const char ACT_TYPE);
		//Using multi processor omp
		//This function return the result of the activation function to the Z matrix from the input matrix(TEMP)
		//The act_type is as same as the list of the activation function
		void ACT_FUN(UMATRIXF* TEMP, UMATRIXF* Z, const char act_type);
		//Using multi processor omp
		//This function calculate the partial derivative of Error function by the error type
		void E_A_CAL(float* E_A_LIST, float target[]);
		//Using multi processor omp
		//This function calculate the partial derivative of activation function by
		//It references the each layer's activatino functino type
		void A_Z_CAL(float* A_Z_LIST, unsigned int k);
		//Using multi processor omp
		//This function calculate the partial derivative of the sum of the weight equation
		void Z_W_CAL(float* Z_W_LIST, unsigned int k);
		//Using multi processor omp
		//This function calcuate the partial derivative for the Error value in the sum of weight equation
		void TOTAL_E(float* E_TOTAL_LIST, float* E_A_LIST, float* A_Z_LIST, unsigned int e_size, unsigned int k);
		//Using multi processor omp
		//This function change the value of the number 'k' row's weight by the partial derivative of the sum of weight equation
		void UPDATE_WEIGHT(float* ERROR_LIST, float* A_Z_LIST, float* Z_W_LIST, unsigned int  k);

		//This function just change the constant number of TYPE in text in const char
		const char* TYPE2STR(const char TYPE);
	public:
		UMATRIXF TOTAL_B_LAYER;//Bias layer
		UMATRIXF TOTAL_W_LAYER;//hidden layer
		UMATRIXF TOTAL_Z_LAYER;//z = w1a + w1b +w1c
		UMATRIXF TOTAL_A_LAYER;//a = f(z)
		UMATRIXF TOTAL_M_LAYER;//momentum velocity layer

		vector<char> ACT_TYPE_LIST;
		vector<unsigned int> LAYER_M_LIST;
		float vr = 0; //FOR THE MOMENTUM, VELOCITY RATE
		float lr = 0; //LEARNING RATE
		char BTYPE = 0; // BACK PROPAGATION TYPE
		char ERROR_TYPE = 0; //ERROR TYPE
		//It deep copy the value

		void operator = (const NEURON_BLOCKF COPY);
		//This function allow to add the layer to the current Neural block
		//[activation type list]
		//ReLU,		Soft_ReLU,	Noisy_ReLU 
		//Leaky_ReLU,	ELU,		SWISH (google published)
		//LOGISTIC,	HYPERBOLIC,	SIGMOID 
		//NONE 
		void ADD_LAYER(unsigned int m, const char activation);
		//Using multi processor omp
		//This function intialize the weight layer except the first weight layer
		//[Type list]
		//XAVIER	HENORM	NONE
		//NONE give between 0~1 number by the float
		//The random function is based on the c++
		void W_INIT(const char TYPE);
		//Using multi processor omp
		//This function import the input data to the first weight layer
		void INPUT(float input[], unsigned int size);
		//This function required the previous steps; ADD LAYER, W_INIT, INPUT, LEARN_SET
		//This function fully used the cpu process
		void CPU_RUN(bool NOMALIZATION);
		//Using multi processor omp
		//This give output data of NEURON BLOCK (Run function is already included)
		//Data_norm - whether or not the normalizationing the data after activation function
		void OUTPUT(float* output, unsigned int size);
		//This function set the learning methods
		//lr - learning rate
		//vr - velocity of momentum
		//ERROR TYPE :  [CROSS] or [MEAN] defulat is MEAN
		//B_TYPE [NONE], [MOMEN], [STOCHASTIC], [NEST]
		void LEARN_SET(float vr_, float lr_, const char ERROR_TYPE, const char B_TYPE);
		//This function return the error value in double type
		//This function use the cpu process
		//MEAN - FOR GENERAL
		//CROSS - FOR BERNUii
		double LOSS_CHECK(float* target);
		//This function update the weight layer by backpropagation
		//The back propagation type must be declared in previous function LEARN_SET
		void CPU_BACK_PROPA(float target[]);
		//This function printf the matrix function
		void pinfo();
		//This function save the neural block class's elements in CSV format(.csv is required)
		void NEURON_SAVE(const char* name);
		//Using multi processor omp double loop
		//This functino check any layer has nan of inf value
		//If it has return true.
		bool CHECK_VAR();
	};
}
#endif

