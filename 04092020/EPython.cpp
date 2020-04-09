#include "EPyhton.h"

int RUN_PYTHON(string python_name) 
{
    Py_Initialize();
    const char* name = python_name.c_str();
    PyObject* obj = Py_BuildValue("s", name);
    FILE* file = _Py_fopen_obj(obj, "r+");
    if (file != NULL) {
        PyRun_SimpleFile(file, name);
    }
    else
    {
        Py_Finalize();
        return 0;
    }
    Py_Finalize();
    return 1;
}