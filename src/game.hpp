#ifndef DEF_GAME
#define DEF_GAME

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "constants.hpp"
#include "scripting/scripting.hpp"
#include "abstract/creatures_loader.hpp"
#include "abstract/functions.hpp"
#include "bag/objectstable.hpp"
#include "abstract/musicplayer.hpp"
#include "abstract/types_table.hpp"
#include "abstract/container.hpp"
#include "objects/messenger.hpp"

class Game {
private:
    // variables
    sf::RenderWindow window;
    sf::Clock clock;
    StateMachine sm;
    MusicPlayer mplayer;
    TypesTable ttable;
    CreaturesLoader crea_load;
    Container<sf::Texture> textures;
    bool has_requested_quit;
    // others utilities
      // loading
    sf::CircleShape shape;
    int shape_outline_sickness;
    bool shape_increasing;
    sf::Text loading_text;
      // global
    sf::Font font;
      // cheats
    bool cheat_on;
    bool _got_coderet;
    sf::Text cmd;
    sf::String cmd_str;
      // menu
    sf::Text menu_user;
    sf::Text menu_text;
    sf::Text cursor;
    sf::String menu_userentry;
    sf::Text menu_ask_user;
    sf::Sprite menu_bckg_s;
    sf::Sprite menu_logo_s;
    sf::Sprite menu_btn_del_s;
    sf::Sprite menu_btn_new_s;
    sf::Sprite menu_btn_start_s;
    sf::Sprite alphablack;
    sf::Sprite deletewarn;
    sf::Sprite validatebtn;
    int blinking;
    bool blink;
    int menu_game_selected;
    // methods
      // commons methods
    void handle_std_events(sf::Event&, sf::Time);
    void dispatch_events(sf::Event&, sf::Time);
    void render();
    void update(sf::Time);
    void update_fps(sf::Time, int&);
      // loading interface methods
    void update_loading(sf::Time);
    void render_loading();
    void loading();
      // menu interface methods
    void update_menu(sf::Time, int, bool);
    void render_menu(const std::vector<std::string>&, bool, bool, bool, bool);
    void menu();
      // others methods
    void take_screenshot();
    void post_load();
    void on_end();

public:
    // methods
    Game();
    int run();
};

#endif // DEF_GAME
