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

void PyScripting::create_and_init_cppModules()
{
    PyImport_AppendInittab(PyUnamed::name, PyUnamed::PyInit_Unamed);
}

// static methods
bool PyScripting::connect()
{
    if (!instance.connected)
    {
        instance.connected = true;
        instance.create_and_init_cppModules();
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
    for (auto& module_code: instance.modules_content)
    {
        instance.run_code(module_code.data());
    }
}

void PyScripting::setValue(int val)
{
    instance.value = val;
}

int PyScripting::getValue()
{
    return instance.value;
}
