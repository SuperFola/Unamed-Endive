#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

extern "C" {
    #include <Python.h>
}

#include <vector>
#include <string>

#include "functions.hpp"

class PyScripting
{
private:
    PyScripting& operator=(const PyScripting&) {}
    PyScripting(const PyScripting&) {}

    static PyScripting instance;
    PyScripting();

    // variables
    int value;
    bool connected;
    wchar_t* program;
    std::vector<std::string> modules_names;
    std::vector<std::string> modules_content;

    // methods
    void load_all_modules();
    void create_and_init_cppModules();

public:
    static PyScripting& Instance();

    // methods
    static bool connect();
    static bool disconnect();
    static int run_code(const char*);
    static int run_all_modules();
    static void setValue(int);
    static int getValue();
};

#endif // DEF_SCRIPTING
