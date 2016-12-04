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
            char result[128];

            if (!PyArg_ParseTuple(args, "ss", &name, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the path to the image, and a const char* giving the id of the image");
                return NULL;
            }
            if (PyScripting::loadImage(name, id) == -1)
            {
                strcat(result, "Can not find the image at '");
                strcat(result, name);
                strcat(result, "'");
                PyErr_SetString(UnamedError, result);
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* displayTexture(PyObject* self, PyObject* args)
        {
            const char* id;
            int x;
            int y;
            char result[128];

            if (!PyArg_ParseTuple(args, "sii", &id, &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the id of the image, and 2 integers representing the position of the image (relative to the upper right corner of the map)");
                return NULL;
            }
            if (PyScripting::displayImage(id, x, y) == -1)
            {
                strcat(result, "Can not find the image with id '");
                strcat(result, id);
                strcat(result, "'");
                PyErr_SetString(UnamedError, result);
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
                            PyErr_SetString(UnamedError, "Can not parse argument, need either an int, a float, or a char*");
                            return NULL;
                        } else gvar.kind = "p";
                    } else gvar.kind = "s";
                } else gvar.kind = "f";
            } else gvar.kind = "i";
            // if we are here, we successfully casted the python var into a C/C++ one
            int v = PyScripting::createGlobal(name, gvar);
            return PyLong_FromLong(v);
        }

        static PyObject* getGlobal(PyObject* self, PyObject* args)
        {
            const char* name;
            if (!PyArg_ParseTuple(args, "s", &name))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the global var to get");
                return NULL;
            }
            char result[128];
            svar_t gvar = PyScripting::getGlobal(name);
            PyObject* v;

            switch (*gvar.kind)
            {
            case 'e':
                goto failed;
                break;

            case 'p':
                v = Py_BuildValue(gvar.kind, gvar.bvar);
                break;

            case 's':
                v = Py_BuildValue(gvar.kind, gvar.cvar);
                break;

            case 'f':
                v = Py_BuildValue(gvar.kind, gvar.fvar);
                break;

            case 'i':
                v = Py_BuildValue(gvar.kind, gvar.ivar);
                break;

            default:
                goto failed;
                break;
            }
            return v;

            failed:
            {
                strcat(result, "Could not find the global var named '");
                strcat(result, name);
                strcat(result, "'");
                PyErr_SetString(UnamedError, result);
                return NULL;
            }
        }

        static PyObject* playMusic(PyObject* self, PyObject* args)
        {
            const char* name;
            if (!PyArg_ParseTuple(args, "s", &name))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the song to play");
                return NULL;
            }
            return Py_BuildValue("i", PyScripting::playMusic(name));
        }

        static PyObject* stopMusic(PyObject* self, PyObject* args)
        {
            PyScripting::stopMusic();

            RETURN_NONE
        }

        static PyObject* getCurrentMusicName(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("s", PyScripting::getCurrentMusicName());
        }

        static PyObject* getCurrentView(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("i", PyScripting::getCurrentView());
        }

        static PyObject* hasActiveHud(PyObject* self, PyObject* args)
        {
            int vid;
            if (!PyArg_ParseTuple(args, "i", &vid))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the id of the view to check");
                return NULL;
            }
            return Py_BuildValue("i", PyScripting::hasActiveHud(vid));
        }

        static PyObject* getEvent(PyObject* self, PyObject* args)
        {
            sf::Event ev = PyScripting::getEvent();
            PyObject* d = PyDict_New();

            //PyDict_SetItem(d, "type", static_cast<int>(ev.type));

            return d;
        }

        static PyObject* createPNJ(PyObject* self, PyObject* args)
        {
            int mid;
            const char* name;
            const char* text;
            int pnjkind_i;
            const char* dname;
            int x;
            int y;
            if (!PyArg_ParseTuple(args, "issisii", &mid, &name, &text, &pnjkind_i, &dname, &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the id of the map, a const char* for the filename, a const char* for the text of the pnj, an int representing the kind of pnj, a const char* for its display name and two int for its position (x and y)");
                return NULL;
            }
            PyScripting::createPNJ(mid, name, text, pnjkind_i, dname, x, y);

            RETURN_NONE
        }

        // module definition
        static PyMethodDef UnamedMethods[] = {
            // ...
            {"registerScript", registerScript, METH_VARARGS, "Register a script in the PyScripting singleton, as a specific kind given as an argument, with an id also given"},
            {"loadImage", loadTexture, METH_VARARGS, "Load an image using a given path, and assigne it to a given id"},
            {"displayImage", displayTexture, METH_VARARGS, "Display an image loaded before using loadImage with its id, and its position (2 integers, x and y)"},
            {"createGlobal", createGlobal, METH_VARARGS, "Create a global value from a given id (of type char*), with a specified value (int, float and char* are currently supported)"},
            {"getGlobal", getGlobal, METH_VARARGS, "Return a global value with the name given"},
            {"getCurrentMusicName", getCurrentMusicName, METH_VARARGS, "Return the name of the current playing music, if one is playing"},
            {"getCurrentViewId", getCurrentView, METH_VARARGS, "Return the current view id"},
            {"stopMusic", stopMusic, METH_VARARGS, "Stop the current, if one is playing"},
            {"playMusic", playMusic, METH_VARARGS, "Start a song from its name"},
            {"hasActiveHud", hasActiveHud, METH_VARARGS, "Check if a view, with a given id, has currently an active HUD"},
            {"getEvent", getEvent, METH_VARARGS, "Get the current event and return it using a dict"},
            {"createPNJ", createPNJ, METH_VARARGS, "Create a PNJ (refer to the wiki to learn more about the PNJkind)"},
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
    this->empty_svar_t.kind = "e";

    Py_SetProgramName(this->program);
}

PyScripting& PyScripting::Instance()
{
    return instance;
}

void PyScripting::load_all_modules()
{
    std::string directory = "assets/scripts/";
    std::vector<std::string> mods;

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile((directory + "*.py").data(), &File);
    if (hSearch != INVALID_HANDLE_VALUE)
    {
        do {
                if (std::string(File.cFileName) != "." && std::string(File.cFileName) != "..")
                    mods.push_back(directory + std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #endif // PLATFORM_WIN

    #ifdef PLATFORM_POSIX
    DIR* rep = opendir(directory.data());

    if (rep != NULL)
    {
        struct dirent* ent;

        while ((ent = readdir(rep)) != NULL)
        {
            mods.push_back(directory + std::string(ent->d_name));
        }

        closedir(rep);
    }
    #endif // PLATFORM_POSIX

    for (auto& mod: mods)
    {
        std::ifstream file;
        file.open(mod);

        std::cout << "Loading script : " << mod << std::endl;

        std::string content;
        while (file)
        {
            std::string line;
            getline(file, line);

            content += line;
            content += "\n";
        }

        file.close();

        this->modules[mod.substr(directory.size())] = content;
    }
    PyScripting::run_code(this->modules["register.py"].data());
    this->modules.clear();
}

// static methods
bool PyScripting::connect()
{
    if (!instance.connected)
    {
        instance.connected = true;

        PyImport_AppendInittab(PyUnamed::name, PyUnamed::PyInit_Unamed);
        Py_Initialize();
        // Py_SetPythonHome(L"assets/scripts/");

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
        for (auto& var: instance.modules_kinds)
        {
            var.second.clear();
        }
        instance.modules_kinds.clear();

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

    for (auto& kinds: instance.modules_kinds)
    {
        for (auto& code: instance.modules_kinds[kinds.first])
        {
            instance.run_code(code.second.data());
            i++;
        }
    }

    return 1;
}

int PyScripting::run_on_start_modules()
{
    int i = 0;
    for (auto& module_code: instance.modules_kinds["runOnceWhenStarting"])
    {
        instance.run_code(module_code.second.data());
        i++;
    }
    std::cout << "Ran " << i << " modules at the start of the game" << std::endl;

    return 1;
}

int PyScripting::run_on_end_modules()
{
    int i = 0;
    for (auto& module_code: instance.modules_kinds["runOnceWhenClosing"])
    {
        instance.run_code(module_code.second.data());
        i++;
    }
    std::cout << "Ran " << i << " modules at the end of the game" << std::endl;

    return 1;
}

int PyScripting::run_until_end_modules()
{
    return 1;
}

int PyScripting::run_update_modules()
{
    for (auto& module_code: instance.modules_kinds["runWhenUpdatingGame"])
    {
        instance.run_code(module_code.second.data());
    }

    return 1;
}

int PyScripting::run_event_modules()
{
    for (auto& module_code: instance.modules_kinds["runWhenProcessingEvents"])
    {
        instance.run_code(module_code.second.data());
    }

    return 1;
}

int PyScripting::run_drawing_modules()
{
    for (auto& module_code: instance.modules_kinds["runWhenRenderingView"])
    {
        instance.run_code(module_code.second.data());
    }

    return 1;
}

void PyScripting::setEvent(sf::Event& ev)
{
    instance.event = ev;
}

sf::Event PyScripting::getEvent()
{
    return instance.event;
}

void PyScripting::setWindow(sf::RenderWindow* win)
{
    std::cout << "Adding a pointer on the window to the PyScripting singleton" << std::endl;
    instance.window = win;
}

void PyScripting::setMusicPlayer(MusicPlayer* mp)
{
    std::cout << "Adding a pointer on the music player to the PyScripting singleton" << std::endl;
    instance.music_player = mp;
}

void PyScripting::setStateMachine(StateMachine* sm)
{
    std::cout << "Adding a pointer on the statemachine to the PyScripting singleton" << std::endl;
    instance.sm = sm;
}

void PyScripting::setPnjManager(PNJManager* pnjm)
{
    std::cout << "Adding a pointer on the pnjmanager to the PyScripting singleton" << std::endl;
    instance.pnjm = pnjm;
}

void PyScripting::setMap(Map* level)
{
    std::cout << "Adding a pointer on the map to the PyScripting singleton" << std::endl;
    instance.level = level;
}

 int PyScripting::setModuleKind(const char* kind, const char* id)
 {
     std::string tkind = std::string(kind);
     std::string tid = std::string(id);

     std::cout << "Registering " << tid << " as " << tkind << std::endl;

     if (instance.modules_kinds.find(tkind) != instance.modules_kinds.end())
     {
         if (instance.modules_kinds[tkind].find(tid) != instance.modules_kinds[tkind].end())
         {
             instance.modules_kinds[tkind][tid] = instance.modules[tid];
             return 0;
         }
         else
            return -1;  // error the module already exists
     }
     else
     {
         instance.modules_kinds[tkind][tid] = instance.modules[tid];
         return 0;
     }
 }

 int PyScripting::loadImage(const char* name, const char* id)
 {
     std::string tname = std::string(name);
     std::string tid = std::string(id);

     std::cout << "Loading an image from the python binding (c++ code here) path: " << tname << ", id: " << tid << std::endl;

     sf::Texture tex;
     if (!tex.loadFromFile(tname))
        return -1;
     instance.textures.addTexture(tid, tex);
     instance.sprites[tid] = sf::Sprite(instance.textures.getTexture(tid));

     return 0;
 }

 int PyScripting::displayImage(const char* id, int x, int y)
 {
     std::string tid = std::string(id);

     if (instance.sprites[tid].getPosition().x != x || instance.sprites[tid].getPosition().y != y)
        instance.sprites[tid].setPosition(x, y);
    instance.window->draw(instance.sprites[tid]);

    return 0;
 }

 int PyScripting::createGlobal(const char* name, struct svar_t value)
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

const char* PyScripting::getCurrentMusicName()
{
    return instance.music_player->getCurrentName().data();
}

int PyScripting::getCurrentView()
{
    return instance.sm->getId();
}

int PyScripting::hasActiveHud(int vid)
{
    return instance.sm->hasActiveHud(instance.sm->getId());
}

void PyScripting::stopMusic()
{
    return instance.music_player->stop();
}

int PyScripting::playMusic(const char* name)
{
    return int(instance.music_player->play(std::string(name)));
}

void PyScripting::createPNJ(int mid, const char* name, const char* text, int pnjkind_i, const char* dname, int x, int y)
{
    PNJkind kind;
    switch (pnjkind_i)
    {
    case 0:
        kind = PNJkind::normal;
        break;

    case 1:
        kind = PNJkind::special;
        break;

    case 2:
        kind = PNJkind::system;
        break;

    default:
        kind = PNJkind::normal;
        break;
    }

    instance.pnjm->add_pnj_on_map(mid, std::string(name), std::string(text), kind, std::string(dname), x, y);
}
