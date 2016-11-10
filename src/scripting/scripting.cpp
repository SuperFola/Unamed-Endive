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

        static PyObject* registerScript(PyObject* self, PyObject* args)
        {
            const char* kind;
            const char* id;
            if (!PyArg_ParseTuple(args, "ss", &kind, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the kind of the script, and a const char* representing the id of the script");
                return NULL;
            }
            if (PyScripting::setModuleKind(kind, id) == -1)
            {
                PyErr_SetString(UnamedError, "You have already defined a kind for this script");
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* loadTexture(PyObject* self, PyObject* args)
        {
            const char* name;
            const char* id;

            if (!PyArg_ParseTuple(args, "ss", &name, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the path to the image, and a const char* giving the id of the image");
                return NULL;
            }
            if (PyScripting::loadImage(name, id) == -1)
            {
                PyErr_SetString(UnamedError, "Can not find the image at '" + name + "'");
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* displayTexture(PyObject* self, PyObject* args)
        {
            const char* id;
            int x;
            int y;

            if (!PyArg_ParseTuple(args, "sii", &id, &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the id of the image, and 2 integers representing the position of the image (relative to the upper right corner of the map)");
                return NULL;
            }
            if (PyScripting::displayImage(id, x, y) == -1)
            {
                PyErr_SetString(UnamedError, "Can not find the image with id '" + id + "'");
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* createGlobal(PyObject* self, PyObject* args)
        {
            const char* name;
            svar_t gvar;

            if (!PyArg_ParseTuple(args, "si", &name, &gvar.ivar))
            {
                if (!PyArg_ParseTuple(args, "sf", &name, &gvar.fvar))
                {
                    if (!PyArg_ParseTuple(args, "ss", &name, &gvar.cvar))
                    {
                        if (!PyArg_ParseTuple(args, "sp", &name, &gvar.bvar))
                        {
                            goto failed;
                        } else gvar.kind = "p";
                    } else gvar.kind = "s";
                } else gvar.kind = "f";
            } else gvar.kind = "i";
            // if we are here, we successfully casted the python var into a C/C++ one
            return PyLong_FromLong(PyScripting::createGlobal(name, svar_t));

            failed:
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need either an int, a float, or a char*");
                return NULL;
            }
        }

        static PyObject* getGlobal(PyObject* self, PyObject* args)
        {
            const char* name;
            if (!PyArg_ParseTuple(args, "s", &name))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the global var to get");
                return NULL;
            }
            svar_t gvar = PyScripting::getGlobal(name);
            PyObject* v;

            switch (gvar.kind)
            {
            case "empty":
                goto failed;
                break;

            case "p":
                v = Py_BuildValue(gvar.kind, gvar.bvar);
                break;

            case "s":
                v = Py_BuildValue(gvar.kind, gvar.cvar);
                break;

            case "f":
                v = Py_BuildValue(gvar.kind, gvar.fvar);
                break;

            case "i":
                v = Py_BuildValue(gvar.kind, gvar.ivar);
                break;

            default:
                goto failed;
                break;
            }
            return v;

            failed:
            {
                PyErr_SetString(UnamedError, "Could not find the global var named '" + name + "'");
                return NULL;
            }
        }

        // module definition
        static PyMethodDef UnamedMethods[] = {
            // ...
            {"test", test, METH_VARARGS, "Return a number time PyScripting::getValue()"},
            {"registerScript", registerScript, METH_VARARGS, "Register a script in the PyScripting singleton, as a specific kind given as an argument, with an id also given"},
            {"loadImage", loadTexture, METH_VARARGS, "Load an image using a given path, and assigne it to a given id"},
            {"displayImage", displayTexture, METH_VARARGS, "Display an image loaded before using loadImage with its id, and its position (2 integers, x and y)"},
            {"createGlobal", createGlobal, METH_VARARGS, "Create a global value from a given id (of type char*), with a specified value (int, float and char* are currently supported)"},
            {"getGlobal", getGlobal, METH_VARARGS, "Return a global value with the name given"},
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
    this->empty_svar_t.kind = "empty";

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

 int PyScripting::setModuleKind(const char* kind, const char* id)
 {
     std::string tid = std::string(id);
     if (instance.modules_kinds.find(tid) != instance.modules_kinds.end())
        return -1;  // error the key already exists
     instance.modules_kinds[tid)] = std::string(kind);  // otherwise everything is fine <3
     return 0;
 }

 int PyScripting::loadImage(const char* name, const char* id)
 {
     return 0;
 }

 int PyScripting::displayImage(const char* id, int x, int y)
 {
     return 0;
 }

 int PyScripting::createGlobal(const char* name, svar_t value)
 {
     std::string tname = std::string(name);
     if (instance.globals_vars.find(tname) != instance.globals_vars.end())
     {
         instance.globals_vars[tname] = value;
         return 1;  // for python consistency
     }
     return 0;  // same
 }

svar_t PyScripting::getGlobal(const char* name)
{
    std::string tname = std::string(name);
    if (instance.globals_vars.find(tname) != instance.globals_vars.end())
        return instance.globals_vars[tname];
    return instance.empty_svar_t;
}
