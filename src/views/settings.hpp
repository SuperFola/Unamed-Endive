#ifndef DEF_SETTINGS_VIEW
#define DEF_SETTINGS_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../json/json.h"
#include "../abstract/functions.hpp"
#include "../abstract/config.hpp"
#include "../abstract/musicplayer.hpp"

#define SET_START_X 147.0f
#define SET_START_X2 345.0f
#define SET_START_Y 82.0f
#define SET_SPACE_Y 56.0f

#define k_none 0
#define k_menu 1
#define k_up 2
#define k_dwn 3
#define k_ri 4
#define k_le 5

class SettingsView : public View
{
private:
    Container<sf::Texture> textures;
    Container<sf::Text> texts;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;

    int key_needed;
    std::string key;

    MusicPlayer* mplayer;

    const std::string BCKG = "background";
    const std::string CHECKED = "checked";
    const std::string UNCHECKED = "unchecked";
    const std::string MUSIC = "musique";
    const std::string AA = "aa";
    const std::string VS = "vs";
    const std::string SHADER = "shader";
    const std::string FPS = "fps";
    const std::string MENU = "menu";
    const std::string UP = "up";
    const std::string DW = "dwn";
    const std::string RI = "right";
    const std::string LE = "left";
    const std::string VMU = "valuemusique";
    const std::string VAA = "valueaa";
    const std::string VVS = "valuevsync";
    const std::string VSHADER = "valueshader";
    const std::string VFPS = "valuefps";
    const std::string VMENU = "valuemenu";
    const std::string VUP = "valueup";
    const std::string VDW = "valuedw";
    const std::string VRI = "valueright";
    const std::string VLE = "valueleft";

    void update_texts();

public:
    SettingsView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;

    void set_music_player(MusicPlayer*);

    static std::string convert_textentered_to_value(sf::Uint32 e);
};

#endif // DEF_SETTINGS_VIEW
