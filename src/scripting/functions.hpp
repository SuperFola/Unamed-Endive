#ifndef DEF_SCRIPT_FUNCS
#define DEF_SCRIPT_FUNCS

extern "C" {
    #include <Python.h>
}

#include "scripting.hpp"

namespace PyUnamed
{
    extern "C"
    {
        const char* name = "Unamed";

        // init the module
        PyMODINIT_FUNC PyInit_Unamed(void);

        // module functions
        static PyObject* test(PyObject*, PyObject*);

        // module definition
        static PyMethodDef UnamedMethods[] = {
            // ...
            {"test", test, METH_VARARGS, "Return a number time PyScripting::getValue()"},
            // ...
            {NULL, NULL, 0, NULL}  // sentinel
        };
        static struct PyModuleDef UnamedModule = {
            PyModuleDef_HEAD_INIT,
            name,
            "C++ functions to modify the way some components of the game works",
            -1,
            UnamedMethods
        };
    }
}

#endif // DEF_SCRIPT_FUNCS
