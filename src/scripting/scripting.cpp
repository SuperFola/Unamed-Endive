#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "scripting.hpp"
#include "../constants.hpp"

#ifdef PLATFORM_WIN
#include <windows.h>
#endif // PLATFORM_WIN

#ifdef PLATFORM_POSIX
#include <dirent.h>
#endif // PLATFORM_POSIX

#include "../game.hpp"

namespace PyUnamed {
    #include "pythonscripting.hpp"
}

PyScripting PyScripting::instance = PyScripting();

PyScripting::PyScripting():
    connected(false)
    , program(L"PyScripter engine")
{
    this->empty_svar_t.kind = "e";

    Py_SetProgramName(this->program);
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

        DebugLog(SH_INFO, "Loading script : " << mod);

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
    // this->modules.clear();
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

        instance.font.loadFromFile(FONTPATH);

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
        instance.uninitialize();

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
    DebugLog(SH_WARN, "You need to connect the PyScripting singleton to Python before using it !");
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
    DebugLog(SH_WARN, "You need to connect the PyScripting singleton to Python before using it !");
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
    DebugLog(SH_INFO, "Ran " << i << " modules at the start of the game");

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
    DebugLog(SH_INFO, "Ran " << i << " modules at the end of the game");

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

void PyScripting::setWindow(sf::RenderWindow* win)
{
    DebugLog(SH_INFO, "Adding a pointer on the window to the PyScripting singleton");
    instance.window = win;
}

void PyScripting::setMusicPlayer(MusicPlayer* mp)
{
    DebugLog(SH_INFO, "Adding a pointer on the music player to the PyScripting singleton");
    instance.music_player = mp;
}

void PyScripting::setStateMachine(StateMachine* sm)
{
    DebugLog(SH_INFO, "Adding a pointer on the statemachine to the PyScripting singleton");
    instance.sm = sm;
}

void PyScripting::setPnjManager(PNJManager* pnjm)
{
    DebugLog(SH_INFO, "Adding a pointer on the pnjmanager to the PyScripting singleton");
    instance.pnjm = pnjm;
}

void PyScripting::setMap(Map* level)
{
    DebugLog(SH_INFO, "Adding a pointer on the map to the PyScripting singleton");
    instance.level = level;
}

void PyScripting::setPlayer(Character* player)
{
    DebugLog(SH_INFO, "Adding a pointer on the player to the PyScripting singleton");
    instance.player = player;
}

void PyScripting::setGame(Game* game)
{
    DebugLog(SH_INFO, "Adding a pointer on the game to the PyScripting singleton");
    instance.game = game;
}

int PyScripting::setModuleKind(const char* kind, const char* id)
{
    std::string tkind = std::string(kind);
    std::string tid = std::string(id);

    DebugLog(SH_OK, "Registering " << tid << " as " << tkind);

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

int PyScripting::unloadModule(const char* id)
{
    std::string sid = std::string(id);
    DebugLog(SH_INFO, "Unloading " << sid);

    std::string kind = "";

    for (auto const& p : instance.modules_kinds)
    {
        for (auto const& s : p.second)
        {
            if (s.first == sid)
            {
                kind = p.first;
                break;
            }
        }
        if (kind != "")
            break;
    }
    if (kind != "")
    {
        instance.modules_kinds[kind].erase(sid);
    }

    return (kind != "") ? 0 : 1;
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

void PyScripting::print(const char* thing)
{
    std::cout << thing;
}

void PyScripting::uninitialize()
{
    instance.window = NULL;
    instance.music_player = NULL;
    instance.sm = NULL;
    instance.pnjm = NULL;
    instance.level = NULL;
    instance.player = NULL;
}
