#ifndef DEF_GAME
#define DEF_GAME

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "constants.hpp"
#include "abstract/statemachine.hpp"
#include "scripting/scripting.hpp"
#include "abstract/creatures_loader.hpp"
#include "abstract/functions.hpp"
#include "bag/objectstable.hpp"
#include "abstract/musicplayer.hpp"
#include "abstract/types_table.hpp"
#include "abstract/triggersmanager.hpp"

class Game {
private:
    // variables
    sf::RenderWindow window;
    sf::Clock clock;
    StateMachine sm;
    MusicPlayer mplayer;
    TypesTable ttable;
    CreaturesLoader crea_load;
    // others utilities
    sf::CircleShape shape;
    int shape_outline_sickness;
    bool shape_increasing;
    sf::Font font;
    sf::Text loading_text;
    bool cheat_on;
    bool _got_coderet;
    sf::Text cmd;
    sf::String cmd_str;
    // methods
    void dispatch_events(sf::Event&, sf::Time);
    void render();
    void render_loading();
    void update(sf::Time);
    void update_loading(sf::Time);
    void update_fps(sf::Time, int&);
    void loading();
    void take_screenshot();
    void post_load();
    void on_end();

public:
    // methods
    Game();
    int run();
};

#endif // DEF_GAME
