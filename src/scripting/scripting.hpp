#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <Python.h>

#include <vector>
#include <string>

class PyScripting
{
private:
    PyScripting& operator=(const PyScripting&) {return instance;}
    PyScripting(const PyScripting&) {}

    static class PyScripting instance;
    PyScripting();

    // variables
    bool connected;
    wchar_t* program;
    std::vector<std::string> modules_names;
    std::vector<std::string> modules_content;

    // methods
    void load_all_modules();

    int value;

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
