#include "ENueron.h"
#include <limits>

NEURAL_NET::NEURAL_NET(int r, int m, int n, int e)
{
	this->Input_layer = EMATRIXD(m,1);
	this->Hidden_layer = EMATRIXD(m, n);
	this->Active_layer = EMATRIXD(m, n);
	this->Output_layer = EMATRIXD(m, 1);
	this->r = r;
	this->e = e;
	for (int i = 0; i < r; i++)
		this->Input_layer.vectors[i] = 1;
	this->Hidden_layer.ONES();
	for (int i = 0; i < e; i++)
		this->Output_layer.vectors[i] = 1;
}
void NEURAL_NET::NEURAL_IMPORT(EMATRIXD& A)
{
	memmove(this->Input_layer.vectors, A.vectors, this->Input_layer.n);
	this->Input_layer = A;
}
void NEURAL_NET::INPUT_VALUE(double value[])
{
	for (int i = 0; i < this->r; i++)
		this->Input_layer.vectors[i] = value[i];
}
EMATRIXD* NEURAL_NET::NEURAL_EXPORT() 
{
	return NULL;
}

namespace neural_tool
{
	void FLOW_DATA(NEURAL_NET* A)
	{
		int i = 0;
		int _m = A->Hidden_layer.m;
		int _n = A->Hidden_layer.n;
		EMATRIXD TEMP(_m, 1);

		//FIRST RECIVE VALUE
		for (int k = 0; k < _m; k++)
			TEMP.vectors[k] = A->Input_layer.vectors[k];
		//NOW OTHER
		for (int i = 0; i < _n; i++)
		{
			//CUT
			EMATRIXD* X = CUDA_MATRIX_CONVERT(&A->Hidden_layer, NULL, i, i, CUT_COL);
			//EXTEND
			EMATRIXD* X_ = CUDA_MATRIX_CONVERT(X, 0, 0, _m - 1, COPY_COL);
			//X * DATA
			EMATRIXD* XA = CUDA_MATRIX_CONVERT(X_, &TEMP, 0, 0, MULTI);
			//ACTIVATION TO DATA
			for (int j = 0; j < _m; j++)
			{
				TEMP.vectors[j] = ACTIVATION_FUN(XA->vectors[j], Soft_ReLU);
				//DATA SAVE
				A->Active_layer.vectors[i + j * _n] = TEMP.vectors[j];
			}
			free(X);
			free(X_);
			free(XA);
		}
		for (int p = 0; p < A->e; p++)
			A->Output_layer.vectors[p] = TEMP.vectors[p];
	}
	double ERROR_CHECK(EMATRIXD data, EMATRIXD target, const char TYPE)
	{
		switch (TYPE)
		{
		case MEAN:
		{
			double sum = 0.0;
			for (int i = 0; i < data.m; i++)
				sum += pow(data.vectors[i] - target.vectors[i], 2);
			return sum / data.m;
		}
		case CROSS:
		{
			data_tool::Quantile_Normarlize(data.vectors, data.m);
			double sum = 0.0;
			for (int i = 0; i < data.m; i++)
				sum -= log(data.vectors[i]) * (target.vectors[i]);
			return sum / data.m;
		}
		default:
			return NULL;
		}
	}

	double ACTIVATION_FUN(double y, const char TYPE)
	{
		switch (TYPE)
		{
		case ELU:
			return ((0 < y) ? 0.01 * (exp(y) - 1) : y);
		case Leaky_ReLU:
			return ((0 < y) ? 0.01 * y : y);
		case Noisy_ReLU:
			return ((0 < y) ? 0 : y);
		case Soft_ReLU:
			return 1 / (1 + exp(-y));
		default:
			if (y > 0)
				return 1;
			else
				return 0;
		}
		return 0;
	}
}
//----------Data Tool----------------------------
namespace data_tool
{
	void Median_Normalize(double* data, int size)
	{
		double* temp = (double*)malloc(sizeof(double) * size);
		memmove(temp, data, sizeof(double) * size);
		double Medium = Find_Mean(temp, size);
		free(temp);

		//standard diviation
		double sd = Standard(data, Medium, size);
		//Normalize
		for (int i = 0; i < size; i++)
			data[i] = (data[i] - Medium) / sd;
	}
	void Mean_Normalize(double* data, int size)
	{
		double sum = 0;
		for (int i = 0; i < size; i++)
			sum += data[i];
		double mean = sum / size;
		double sd = Standard(data, mean, size);
		for (int i = 0; i < size; i++)
			data[i] = (data[i] - mean) / sd;
	}
	void Quantile_Normarlize(double* data, int size)
	{
		double max = maxArray(data, size);
		double min = minArray(data, size);
		if (max - min != 0)
			for (int i = 0; i < size; i++)
				data[i] = (data[i] - min) / (max - min);
	}
	double minArray(double* data, int size)
	{
		double min = data[0];
		for (int i = 1; i < size; i++)
			if (min > data[i])
				min = data[i];
		return min;
	}
	double maxArray(double* data, int size)
	{
		double max = data[0];
		for (int i = 1; i < size; i++)
			if (max < data[i])
				max = data[i];
		return max;
	}
	void buble_sorting(double* data, int size)
	{
		for (int i = 0; i < size - 1; i++)
		{
			double temp = data[i];
			if (data[i + 1] < data[i])
			{
				data[i] = data[i + 1];
				data[i + 1] = temp;
				i = 0;
			}
		}
	}
	double Find_Mean(double data[], int size)
	{
		buble_sorting(data, size);
		double Mean = 0;
		if (size % 2 == 0)
		{

			int A = size / 2;
			int B = size / 2 + 1;
			Mean = data[(A + B) / 2];
		}
		else
		{
			Mean = data[size / 2 + 1];
		}
		return Mean;
	}
	double Standard(double* data, double Ed, int size)
	{
		double sd = 0;
		for (int i = 0; i < size; i++)
			sd += pow(data[i] - Ed, 2);
		sd = sd / size;
		sd = sqrt(sd);
		return sd;
	}
}

/* least square algorithm for projection
		EMATRIXD* AT = CUDA_MATRIX_CONVERT(&data, NULL, 0, 0, TRANS);
		EMATRIXD* ATdata = CUDA_MATRIX_CONVERT(AT, &data, 0, 0, MULTI);
		EMATRIXD* ATtarget = CUDA_MATRIX_CONVERT(AT, &target, 0, 0, MULTI);
		EMATRIXD* SUM = CUDA_MATRIX_CONVERT(ATdata, ATtarget, 0, 0, ADD_COL);
		EMATRIXD* SOL=  GAUSSELI(SUM, ATdata->m);
		EMATRIXD* X_ = CUDA_MATRIX_CONVERT(SOL, NULL, 0, 0, CUT_COL);
		EMATRIXD* P = CUDA_MATRIX_CONVERT(AT, X_, 0, 0, MULTI);
*/