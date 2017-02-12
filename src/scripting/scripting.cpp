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
            char result[256];
            if (!PyArg_ParseTuple(args, "ss", &kind, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the kind of the script, and a const char* representing the id of the script");
                return NULL;
            }
            if (PyScripting::setModuleKind(kind, id) == -1)
            {
                strcpy(result, "You have already defined a kind for this script : ");
                strcat(result, id);
                PyErr_SetString(UnamedError, result);
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* print(PyObject* self, PyObject* args)
        {
            PyObject* content;
            char result[256];
            if (!PyArg_ParseTuple(args, "O", &content))
            {
                strcpy(result, "Can not parse argument : need a single argument, of any type");
                PyErr_SetString(UnamedError, result);
                return NULL;
            }
            PyObject* objectsRepresentation = PyObject_Repr(content);
            const char* parsed = PyUnicode_AsUTF8(objectsRepresentation);
            Py_XDECREF(objectsRepresentation);

            PyScripting::print(parsed);

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
                strcpy(result, "Can not find the image at ");
                strcat(result, name);
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
                strcpy(result, "Can not find the image with id ");
                strcat(result, id);
                PyErr_SetString(UnamedError, result);
                return NULL;
            }

            RETURN_NONE
        }

        static PyObject* display_with_view(PyObject* self, PyObject* args)
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
            if (PyScripting::displayImageWithView(id, x, y) == -1)
            {
                strcpy(result, "Can not find the image with id ");
                strcat(result, id);
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
                strcpy(result, "Could not find the global var named ");
                strcat(result, name);
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

        static PyObject* is_spawn(PyObject* self, PyObject* args)
        {
            int x;
            int y;
            if (!PyArg_ParseTuple(args, "ii", &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two int representing the position of the block to test");
                return NULL;
            }
            return Py_BuildValue("i", PyScripting::map_is_spawn(x, y));
        }

        static PyObject* is_tp(PyObject* self, PyObject* args)
        {
            int x;
            int y;
            if (!PyArg_ParseTuple(args, "ii", &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two int representing the position of the block to test");
                return NULL;
            }
            return Py_BuildValue("i", PyScripting::map_is_tp(x, y));
        }

        static PyObject* getSpawnFrom(PyObject* self, PyObject* args)
        {
            int mid;
            if (!PyArg_ParseTuple(args, "ii", &mid))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the map id to find in the spawns list");
                return NULL;
            }

            int rpos = PyScripting::map_getSpawnFrom(mid);

            if (rpos == -1)
                return Py_BuildValue("i", -1);

            int x = rpos % PyScripting::getMapWidth();
            int y = rpos / PyScripting::getMapHeight();

            return Py_BuildValue("(ii)", x, y);
        }

        static PyObject* getMapFromTp(PyObject* self, PyObject* args)
        {
            int x;
            int y;
            if (!PyArg_ParseTuple(args, "ii", &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two int representing the position of the teleporter");
                return NULL;
            }
            int v = -1;
            if (PyScripting::map_is_tp(x, y))
                v = PyScripting::map_getMapFromTp(x, y);
            return Py_BuildValue("i", v);
        }

        static PyObject* map_width(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("i", PyScripting::getMapWidth());
        }

        static PyObject* map_height(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("i", PyScripting::getMapHeight());
        }

        static PyObject* getMapId(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("i", PyScripting::getMapId());
        }

        static PyObject* changeBlockAttrib(PyObject* self, PyObject* args)
        {
            int rid;
            const char* id;
            int v;
            if (!PyArg_ParseTuple(args, "isp", &rid, &id, &v))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the case (rpos), a string representing the attribute to change, and an int for the value (0 | 1)");
                return NULL;
            }

            PyScripting::changeBlockAttrib(rid, id, v);

            RETURN_NONE
        }

        static PyObject* getPlayerName(PyObject* self, PyObject* args)
        {
            return Py_BuildValue("s", PyScripting::getPlayerName());
        }

        static PyObject* getTrigger(PyObject* self, PyObject* args)
        {
            int mid;
            int rid;
            if (!PyArg_ParseTuple(args, "ii", &mid, &rid))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the map (mid), and a integer representing the position of the case (rid)");
                return NULL;
            }

            return Py_BuildValue("s", PyScripting::getTrigger(mid, rid));
        }

        static PyObject* is_notrigger(PyObject* self, PyObject* args)
        {
            const char* id;
            if (!PyArg_ParseTuple(args, "s", &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need a string representing the id of a trigger given by getTrigger(..)");
                return NULL;
            }

            return Py_BuildValue("p", PyScripting::is_notrigger(id));
        }

        static PyObject* addTrigger(PyObject* self, PyObject* args)
        {
            int mid;
            int rid;
            const char* id;
            if (!PyArg_ParseTuple(args, "iis", &mid, &rid, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two integers (mid and rid), and a string representing a new id for your trigger");
                return NULL;
            }

            PyScripting::addTrigger(mid, rid, id);

            RETURN_NONE
        }

        static PyObject* tpPlayerOnSpawn(PyObject* self, PyObject* args)
        {
            int mid;
            int sid;
            if (!PyArg_ParseTuple(args, "ii", &mid, &sid))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two integers (mid and sid)");
                return NULL;
            }

            PyScripting::map_tpPlayerOnSpawn(mid, sid);

            RETURN_NONE
        }

        static PyObject* tpPlayerOn(PyObject* self, PyObject* args)
        {
            int x;
            int y;
            if (!PyArg_ParseTuple(args, "ii", &x, &y))
            {
                PyErr_SetString(UnamedError, "Can not parse argument, need two integers (x, y)");
                return NULL;
            }

            PyScripting::map_tpPlayerOn(x + y * PyScripting::getMapWidth());

            RETURN_NONE
        }

        // module definition
        static PyMethodDef UnamedMethods[] = {
            // ...
            {"upr", print, METH_VARARGS, "Print function using std::cout"},
            {"registerScript", registerScript, METH_VARARGS, "Register a script in the PyScripting singleton, as a specific kind given as an argument, with an id also given"},
            {"loadImage", loadTexture, METH_VARARGS, "Load an image using a given path, and assigne it to a given id"},
            {"displayImage", displayTexture, METH_VARARGS, "Display an image loaded before using loadImage with its id, and its position (2 integers, x and y)"},
            {"displayWithView", display_with_view, METH_VARARGS, "Display an image loaded before, relative to the top left corner of the view. Need the same argument as displayImage(...)"},
            {"createGlobal", createGlobal, METH_VARARGS, "Create a global value from a given id (of type char*), with a specified value (int, float and char* are currently supported)"},
            {"getGlobal", getGlobal, METH_VARARGS, "Return a global value with the name given"},
            {"getCurrentMusicName", getCurrentMusicName, METH_VARARGS, "Return the name of the current playing music, if one is playing"},
            {"getCurrentViewId", getCurrentView, METH_VARARGS, "Return the current view id"},
            {"stopMusic", stopMusic, METH_VARARGS, "Stop the current, if one is playing"},
            {"playMusic", playMusic, METH_VARARGS, "Start a song from its name"},
            {"hasActiveHud", hasActiveHud, METH_VARARGS, "Check if a view, with a given id, has currently an active HUD"},
            {"getEvent", getEvent, METH_VARARGS, "Get the current event and return it using a dict"},
            {"createPNJ", createPNJ, METH_VARARGS, "Create a PNJ (refer to the wiki to learn more about the PNJkind)"},
            {"isSpawn", is_spawn, METH_VARARGS, "Return 1 if the block at the pos x y is a spawn, otherwise 0"},
            {"isTp", is_tp, METH_VARARGS, "Return 1 if the block at the pos x y is a tp, otherwise 0"},
            {"getSpawnPosFromMapId", getSpawnFrom, METH_VARARGS, "Return a tuple of two int representing the position of the spawn. If the spawn can not be found, return -1"},
            {"getMapIdFromTpPos", getMapFromTp, METH_VARARGS, "Return a map id from a tp pos. If the tp can not be found, return -1"},
            {"getMapWidth", map_width, METH_VARARGS, "Return the width of the map (in blocks)"},
            {"getMapHeight", map_height, METH_VARARGS, "Return the height of the map (in blocks)"},
            {"getMapId", getMapId, METH_VARARGS, "Return the id of the map (int)"},
            {"changeBlockAttribute", changeBlockAttrib, METH_VARARGS, "To change attributes of a specified block on the current map"},
            {"getPlayerName", getPlayerName, METH_VARARGS, "Return the name of the player"},
            {"getTrigger", getTrigger, METH_VARARGS, "Take two integers, mid and rid, and return a string representing the identifier of a trigger. Remember to check if the trigger is valid using is_notrigger(id)"},
            {"is_notrigger", is_notrigger, METH_VARARGS, "Take a string, given by getTrigger(..), and return True or False whether the id represents a valid trigger or not"},
            {"addTrigger", addTrigger, METH_VARARGS, "Take two integers (mid and rid) and a string representing the id of your new trigger"},
            {"tpPlayerOnSpawn", tpPlayerOnSpawn, METH_VARARGS, "Take a map id and a spawn id (integer and string). Will teleport the player to this map (can be the actual map), on the position of the spawn"},
            {"tpPlayerOn", tpPlayerOn, METH_VARARGS, "Take two integers (x, y). Will teleport the player on this position, on the current map"},
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

    // just to be sure to start from nothing
    this->modules = std::map<std::string, std::string>();

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
        // Py_SetPythonHome(L"assets/scripts/");
        Py_Initialize();

        PyScripting::run_code("import errno, os, stat, shutil\n\
def handleRemoveReadonly(func, path, exc):\n\
  excvalue = exc[1]\n\
  if func in (os.rmdir, os.remove) and excvalue.errno == errno.EACCES:\n\
      os.chmod(path, stat.S_IRWXU| stat.S_IRWXG| stat.S_IRWXO);func(path)\n\
  else: raise\n\
remove = lambda filename : shutil.rmtree(filename, ignore_errors=False, onerror=handleRemoveReadonly)");

        return true;
    }
    return false;  // already running
}

void PyScripting::load()
{
    instance.load_all_modules();
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

void PyScripting::reload_all()
{
    instance.load_all_modules();
}

int PyScripting::run_code(const char* code)
{
    if (instance.connected)
        return PyRun_SimpleString(code);
    std::cout << "You need to connect your PyScripting instance to Python before using it !" << std::endl;
    return -1;
}

const char* PyScripting::run_code_and_get_out(const char* code)
{
    if (instance.connected)
    {
        std::stringstream ss;
        auto oldout = std::cout.rdbuf(ss.rdbuf());

        // running code
        PyRun_SimpleString(code);

        // setting back the old stdout
        std::cout.rdbuf(oldout);

        return ss.str().c_str();
    }
    std::cout << "You need to connect your PyScripting instance to Python before using it !" << std::endl;
    return "None";
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

void PyScripting::setTriggsMgr(TriggersManager* triggs)
{
    std::cout << "Adding a pointer on the triggers manager to the PyScripting singleton" << std::endl;
    instance.triggsmgr = triggs;
}

void PyScripting::setPlayer(Character* player)
{
    std::cout << "Adding a pointer on the player to the PyScripting singleton" << std::endl;
    instance.player = player;
}

 int PyScripting::setModuleKind(const char* kind, const char* id)
 {
     std::string tkind = std::string(kind);
     std::string tid = std::string(id);

     std::cout << "Registering " << tid << " as " << tkind << std::endl;

     if (instance.modules_kinds.find(tkind) != instance.modules_kinds.end())
     {
         if (instance.modules_kinds[tkind].find(tid) == instance.modules_kinds[tkind].end())
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

 int PyScripting::displayImageWithView(const char* id, int x, int y)
 {
     std::string tid = std::string(id);

     if (instance.sprites[tid].getPosition().x != x || instance.sprites[tid].getPosition().y != y)
     {
        sf::Vector2f p2 = instance.window->mapPixelToCoords(sf::Vector2i(x, y));
        instance.sprites[tid].setPosition(p2);
     }
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

int PyScripting::map_is_spawn(int x, int y)
{
    return (instance.level->is_spawn(x, y)) ? 1 : 0;
}

int PyScripting::map_is_tp(int x, int y)
{
    return (instance.level->is_tp(x, y)) ? 1 : 0;
}

int PyScripting::map_getSpawnFrom(int mid)
{
    return instance.level->getSpawnFromMap(mid);  // rpos !
}

int PyScripting::map_getMapFromTp(int x, int y)
{
    return instance.level->getMapFromTp(x + y * instance.level->getWidth());
}

int PyScripting::getMapWidth()
{
    return instance.level->getWidth();
}

int PyScripting::getMapHeight()
{
    return instance.level->getHeight();
}

int PyScripting::getMapId()
{
    return instance.level->getId();
}

void PyScripting::changeBlockAttrib(int rid, const char* identifier, int value)
{
    instance.level->setBlockAttrib(rid, std::string(identifier), bool(value));
}

const char* PyScripting::getPlayerName()
{
    return instance.player->getName().c_str();
}

const char* PyScripting::getTrigger(int mid, int rid)
{
    return instance.triggsmgr->get_trigg(mid, rid).c_str();
}

void PyScripting::addTrigger(int mid, int rid, const char* identifier)
{
    instance.triggsmgr->add_trigger(mid, rid, std::string(identifier));
}

int PyScripting::is_notrigger(const char* identifier)
{
    return (instance.triggsmgr->is_notrigger(std::string(identifier))) ? 1 : 0;
}

void PyScripting::map_tpPlayerOnSpawn(int mid, int sid)
{
    if (instance.level->getId() != mid)
    {
        // we need to load this new map
        std::stringstream ss;
        ss << "assets/map/" << mid << ".umd";
        instance.level->load_map_at(ss.str());
    }
    int rpos = instance.level->getSpawnFromMap(sid);
    instance.player->setPos(
                            rpos % instance.level->getWidth()
                            , rpos / instance.level->getWidth()
                            );
}

void PyScripting::map_tpPlayerOn(int rid)
{
    instance.player->setPos(
                            rid % instance.level->getWidth()
                            , rid / instance.level->getWidth()
                            );
}

void PyScripting::print(const char* thing)
{
    std::cout << thing << std::endl;
}

