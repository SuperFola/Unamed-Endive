#ifndef DEF_SCRIPTING
#define DEF_SCRIPTING

#include <cmath>

#include "../constants.hpp"
#ifdef PLATFORM_POSIX
    #include <python3.6/Python.h>
#elif defined(PLATFORM_WIN)
    #include <Python.h>
#endif

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

#include "../abstract/convert_events.hpp"

#define RETURN_NONE Py_INCREF(Py_None); return Py_None;

// forward declaration
class Game;

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
    sf::Font font;
    Container<sf::Text> texts;

    sf::RenderWindow* window;
    MusicPlayer* music_player;
    StateMachine* sm;
    PNJManager* pnjm;
    Map* level;
    Character* player;
    Game* game;

    // methods
    void load_all_modules();
    void uninitialize();

public:
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
    static int run_update_modules();
    static int run_event_modules();
    static int run_drawing_modules();

    static int setModuleKind(const char*, const char*);
    static int unloadModule(const char*);

    static void setWindow(sf::RenderWindow*);
    static void setMusicPlayer(MusicPlayer*);
    static void setStateMachine(StateMachine*);
    static void setPnjManager(PNJManager*);
    static void setMap(Map*);
    static void setPlayer(Character*);
    static void setGame(Game*);

    static int createGlobal(const char*, struct svar_t);
    static svar_t getGlobal(const char*);
    static void print(const char*);

    static void setEvent(sf::Event&);
    static sf::Event getEvent();
    static int map_is_tp(int, int);
    static int map_getMapFromTp(int, int);
    static void map_tpPlayerOn(int);
    static int getMapWidth();
    static int getMapHeight();
    static int getMapId();
    static void changeBlockAttrib(int, const char*, int);

    static int loadImage(const char*, const char*);
    static int displayImage(const char*, int, int);
    static int displayImageWithView(const char*, int, int);

    static int getCurrentView();
    static int hasActiveHud(int);

    static const char* getCurrentMusicName();
    static void stopMusic();
    static int playMusic(const char*);

    static void createPNJ(int, const char*, const char*, int, const char*, int, int);

    static const char* getPlayerName();
    static const char* getPlayerFolder();

    static const char* screenshot();

    static std::string exec_net_req_getstr(const char*, const char*);
    static int exec_net_req_getint(const char*, const char*);
    static Json::Value exec_net_req_getjson(const char*, const char*);
    static std::vector<int> exec_net_req_getvectorint(const char*, const char*);
    static std::vector<std::string> exec_net_req_getvectorstr(const char*, const char*);

    static std::string sha256crypt(const char*);

    static void setCurrentView(int);
    static int countCreaturesInEquip();
    static int countCreaturesInPC();
    static void giveObject(int, int, int);
    static void turnPlayer(int);
    static void createText(int, int, int, const char*, int, int, const char*);
    static void writeText(const char*);

    static void setFightEnv(int);
    static void setFightEscape(bool);

    static void triggerBalloonPrompt(const std::wstring&, int&, int);
    static const char* balloonPromptGetOuput();

    static void set_fight_opponents(fight_opponent);
    static void set_fight_opponents(fight_opponent, fight_opponent);
    static void set_fight_opponents(fight_opponent, fight_opponent, fight_opponent);

    static int set_creature_name(int, int, const char*);

    static void remove_object_from_pocket(int, int, int);
    static void add_object_to_pocket(int, int, int);

    static void setPlayerName(const char*);

    static int getTileId(int);
};

#endif // DEF_SCRIPTING
