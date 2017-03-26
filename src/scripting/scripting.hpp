#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <cmath>
#include <Python.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

#include <iostream>

#include "types.hpp"
#include "../abstract/container.hpp"
#include "../abstract/musicplayer.hpp"
#include "../abstract/statemachine.hpp"
#include "../entities/pnjmanager.hpp"
#include "../entities/character.hpp"
#include "../map/map.hpp"
#include "../abstract/triggersmanager.hpp"

#include "../abstract/convert_events.hpp"

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
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;

    sf::RenderWindow* window;
    MusicPlayer* music_player;
    StateMachine* sm;
    PNJManager* pnjm;
    Map* level;
    TriggersManager* triggsmgr;
    Character* player;

    // methods
    void load_all_modules();

public:
    static PyScripting& Instance();

    // methods
    static bool connect();
    static void load();
    static bool disconnect();
    static void reload_all();

    static int run_code(const char*);
    static const char* run_code_and_get_out(const char*);
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
    static void setTriggsMgr(TriggersManager*);
    static void setPlayer(Character*);

    static int map_is_tp(int, int);
    static int map_getMapFromTp(int, int);
    static void map_tpPlayerOnSpawn(int, int);
    static void map_tpPlayerOn(int);
    static int getMapWidth();
    static int getMapHeight();
    static int getMapId();
    static void changeBlockAttrib(int, const char*, int);

    static const char* getTrigger(int, int);
    static void addTrigger(int, int, const char*);
    static int is_notrigger(const char*);

    static int setModuleKind(const char*, const char*);

    static int loadImage(const char*, const char*);
    static int displayImage(const char*, int, int);
    static int displayImageWithView(const char*, int, int);

    static int createGlobal(const char*, struct svar_t);
    static svar_t getGlobal(const char*);

    static int getCurrentView();
    static int hasActiveHud(int);

    static const char* getCurrentMusicName();
    static void stopMusic();
    static int playMusic(const char*);

    static void createPNJ(int, const char*, const char*, int, const char*, int, int);

    static const char* getPlayerName();

    static void print(const char*);

    static const char* screenshot();
};

#endif // DEF_SCRIPTING
