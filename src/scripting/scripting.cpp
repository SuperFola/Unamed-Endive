#include <iostream>
#include <fstream>

#include "scripting.hpp"
#include "../constants.hpp"

#ifdef PLATFORM_WIN
#include <windows.h>
#endif // PLATFORM_WIN

#ifdef PLATFORM_POSIX
#include <dirent.h>
#endif // PLATFORM_POSIX

#define RETURN_NONE Py_INCREF(Py_None); return Py_None;

namespace PyUnamed
{
    extern "C"
    {
        const char* name = "Unamed";

        // errors
        static PyObject* UnamedError;

        // module functions
        static PyObject* test(PyObject* self, PyObject* args)
        {
            int x = 0;

            if (!PyArg_ParseTuple(args, "i", &x))
            {
                PyErr_SetString(UnamedError, "Can not parse arguments");
                return NULL;
            }

            x *= PyScripting::getValue();
            PyScripting::setValue(PyScripting::getValue() + 1);

            return Py_BuildValue("i", x);
        }

        // module definition
        static PyMethodDef UnamedMethods[] = {
            // ...
            {"test", test, METH_VARARGS, "Return a number time PyScripting::getValue()"},
            // ...
            {NULL, NULL, 0, NULL}  // sentinel
        };
        static PyModuleDef UnamedModule = {
            PyModuleDef_HEAD_INIT,
            name,
            "C++ functions to modify the way some components of the game works",
            -1,
            UnamedMethods
        };

        // init the module
        PyMODINIT_FUNC PyInit_Unamed(void)
        {
            PyObject* m;

            m = PyModule_Create2(&UnamedModule, PYTHON_API_VERSION);
            if (m == NULL)
                return NULL;

            UnamedError = PyErr_NewException("Unamed.error", NULL, NULL);
            Py_INCREF(UnamedError);
            PyModule_AddObject(m, "error", UnamedError);

            return m;
        }
    }
}

PyScripting PyScripting::instance = PyScripting();

PyScripting::PyScripting():
    connected(false)
    , program(L"PyScripter engine")
{
    Py_SetProgramName(this->program);
}

PyScripting& PyScripting::Instance()
{
    return instance;
}

void PyScripting::load_all_modules()
{
    std::string directory = "assets/scripts";

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile((directory + "/*.py").data(), &File);
    if (hSearch != INVALID_HANDLE_VALUE)
    {
        do {
                if (std::string(File.cFileName) != "." && std::string(File.cFileName) != "..")
                    this->modules_names.push_back(directory + "/" + std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #endif // PLATFORM_WIN

    #ifdef PLATFORM_POSIX
    DIR* rep = opendir(directory);

    if (rep != NULL)
    {
        struct dirent* ent;

        while ((ent = readdir(rep)) != NULL)
        {
            this->modules_names.push_back(directory + std::string(ent->d_name));
        }

        closedir(rep);
    }
    #endif // PLATFORM_POSIX

    for (auto& fname: this->modules_names)
    {
        std::ifstream file;
        file.open(fname);

        std::cout << "Loading " << fname << std::endl;

        std::string content;
        while (file)
        {
            std::string line;
            getline(file, line);

            content += line;
            content += "\n";
        }

        file.close();

        this->modules_content.push_back(content);
    }
}

// static methods
bool PyScripting::connect()
{
    if (!instance.connected)
    {
        instance.connected = true;

        PyImport_AppendInittab(PyUnamed::name, PyUnamed::PyInit_Unamed);
        Py_Initialize();
        Py_SetPythonHome(L"assets/scripts/");

        instance.load_all_modules();

        return true;
    }
    return false;  // already running
}

bool PyScripting::disconnect()
{
    if (instance.connected)
    {
        instance.connected = false;
        Py_Finalize();

        return true;
    }
    return false;  // already disconnected
}

int PyScripting::run_code(const char* code)
{
    if (instance.connected)
        return PyRun_SimpleString(code);
    std::cout << "You need to connect your PyScripting instance to Python before using it !" << std::endl;
    return -1;
}

int PyScripting::run_all_modules()
{
    int i = 0;

    for (auto& module_code: instance.modules_content)
    {
        instance.run_code(module_code.data());
        i++;
    }

    std::cout << "Ran " << i << " script(s) one time" << std::endl;

    return 1;
}

void PyScripting::setValue(int val)
{
    instance.value = val;
}

int PyScripting::getValue()
{
    return instance.value;
}
