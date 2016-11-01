#include "functions.hpp"

// module loader
PyMODINIT_FUNC PyUnamed::PyInit_Unamed(void)
{
    return PyModule_Create2(&PyUnamed::UnamedModule, PYTHON_API_VERSION);
}

// module functions
PyObject* PyUnamed::test(PyObject* self, PyObject* args)
{
    int x = 0;

    PyArg_ParseTuple(args, "i", &x);

    x *= PyScripting::getValue();

    return Py_BuildValue("i", x);
}
