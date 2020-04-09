#pragma once
#include <iostream>
#include <Python.h>

void py()
{
	Py_Initialize();
	if (Py_IsInitialized())
	{
		char* str = "temp = 20\nprint tmp2";
		PyRun_SimpleString("print('Hello, python')");
		PyRun_SimpleString("temp1 =10");
		PyRun_SimpleString("print tmp1");
		Py_Finalize();
	}
}