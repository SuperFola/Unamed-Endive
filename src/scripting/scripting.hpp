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
#include "../abstract/musicplayer.hpp"
#include "../abstract/statemachine.hpp"
#include "../entities/pnjmanager.hpp"
#include "../map/map.hpp"

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
    MusicPlayer* music_player;
    StateMachine* sm;
    PNJManager* pnjm;
    Map* level;

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
    static void setMusicPlayer(MusicPlayer*);
    static void setStateMachine(StateMachine*);
    static void setPnjManager(PNJManager*);
    static void setMap(Map*);

    static int map_is_spawn(int, int);
    static int map_is_tp(int, int);
    static int map_getSpawnFrom(int);
    static int map_getMapFromTp(int, int);
    static int getMapWidth();
    static int getMapHeight();

    static int setModuleKind(const char*, const char*);
    static int loadImage(const char*, const char*);
    static int displayImage(const char*, int, int);
    static int createGlobal(const char*, struct svar_t);
    static svar_t getGlobal(const char*);
    static const char* getCurrentMusicName();
    static int getCurrentView();
    static int hasActiveHud(int);
    static void stopMusic();
    static int playMusic(const char*);
    static void createPNJ(int, const char*, const char*, int, const char*, int, int);
};

#endif // DEF_SCRIPTING
