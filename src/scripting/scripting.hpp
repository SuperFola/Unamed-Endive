#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <Python.h>

#include <map>
#include <vector>
#include <string>

#include "types.hpp"

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
    std::map<std::string, std::string> modules_kinds;
    std::map<std::string, svar_t> globals_vars;
    svar_t empty_svar_t;

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

    static int setModuleKind(const char*, const char*);
    static int loadImage(const char*, const char*);
    static int displayImage(const char*, int, int);
    static int createGlobal(const char*, svar_t);
    static svar_t getGlobal(const char*);
};

#endif // DEF_SCRIPTING
