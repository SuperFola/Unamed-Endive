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

PyScripting::PyScripting():
    connected(false)
    , program(L"PyScripter engine")
{
    Py_SetProgramName(this->program);
}

PyObject* PyScripting::test(PyObject* self, PyObject* args)
{
    int x = 0;

    PyArg_ParseTuple(args, "i", &x);

    x *= 2;

    return Py_BuildValue("i", x);
}

bool PyScripting::connect()
{
    if (!this->connected)
    {
        this->connected = true;
        Py_Initialize();
        Py_SetPythonHome(L"assets/scripts/");
        this->load_all_modules();
        this->create_and_init_cppModules();
        return true;
    }
    return false;  // already running
}

bool PyScripting::disconnect()
{
    if (this->connected)
    {
        this->connected = false;
        Py_Finalize();
        return true;
    }
    return false;  // already disconnected
}

int PyScripting::run_code(const char* code)
{
    if (this->connected)
        return PyRun_SimpleString(code);
    std::cout << "You need to connect your PyScripting instance to Python before using it !" << std::endl;
    return -1;
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
                if (File.cFileName != "." && File.cFileName != "..")
                    this->modules_names.push_back(std::string(File.cFileName));
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
            this->modules_names.push_back(ent->d_name);
        }

        closedir(rep);
    }
    #endif // PLATFORM_POSIX

    for (auto& fname: this->modules_names)
    {
        std::ifstream file;
        file.open(fname);

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

void PyScripting::create_and_init_cppModules()
{
    PyMethodDef module[] = {{"test", this->test, METH_VARARGS, NULL},
                                                {NULL, NULL, 0, NULL}};
    Py_InitModule("Unamed", module);
}
