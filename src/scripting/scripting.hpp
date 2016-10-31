#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <Python.h>
#include <vector>
#include <string>

class PyScripting
{
private:
    // variables
    bool connected;
    wchar_t* program;
    std::vector<std::string> modules_names;
    std::vector<std::string> modules_content;
    // methods
    void load_all_modules();
    void create_and_init_cppModules();

public:
    // methods
    PyScripting();
    bool connect();
    bool disconnect();
    int run_code(const char*);

    // methods to interract C++ <-> Python
    PyObject* test(PyObject*, PyObject*);
};

#endif // DEF_SCRIPTING
