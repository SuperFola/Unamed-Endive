#ifndef DEF_GAME
#define DEF_GAME

#define WIN_TITLE "Unamed Rebirth"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "constants.hpp"
#include "abstract/statemachine.hpp"
#include "scripting/scripting.hpp"
#include "abstract/creatures_loader.hpp"

class Game {
private:
    // variables
    sf::RenderWindow window;
    sf::Clock clock;
    StateMachine sm;
    // others utilities
    CreaturesLoader crea_load;
    sf::CircleShape shape;
    int shape_outline_sickness;
    bool shape_increasing;
    sf::Font font;
    sf::Text loading_text;
    // methods
    void dispatch_events(sf::Event&, sf::Time);
    void resize_window(int, int);
    void render();
    void render_loading();
    void update(sf::Time);
    void update_loading(sf::Time);
    void update_fps(sf::Time, int&);
    void loading();

public:
    // methods
    Game();
    int run();
};

#endif // DEF_GAME
