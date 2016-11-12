#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <cmath>
#include <Python.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <vector>
#include <string>

#include <iostream>

#include "types.hpp"
#include "../abstract/texturesmanager.hpp"

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
    std::map<std::string, std::string> modules;
    std::map<std::string, std::map<std::string, std::string>> modules_kinds;
    std::map<std::string, svar_t> globals_vars;
    svar_t empty_svar_t;
    sf::Event event;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::RenderWindow* window;

    // methods
    void load_all_modules();

public:
    static PyScripting& Instance();

    // methods
    static bool connect();
    static bool disconnect();

    static int run_code(const char*);
    static int run_all_modules();
    // specific runners
    static int run_on_start_modules();
    static int run_on_end_modules();
    static int run_until_end_modules();
    static int run_update_modules();
    static int run_event_modules();
    static int run_drawing_modules();

    static void setEvent(sf::Event&);
    static sf::Event getEvent();
    static void setWindow(sf::RenderWindow*);

    static int setModuleKind(const char*, const char*);
    static int loadImage(const char*, const char*);
    static int displayImage(const char*, int, int);
    static int createGlobal(const char*, struct svar_t);
    static svar_t getGlobal(const char*);
};

#endif // DEF_SCRIPTING
