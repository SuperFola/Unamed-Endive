#ifndef DEF_GAME
#define DEF_GAME

#define WIN_W 640
#define WIN_H 540
#define WIN_TITLE "Unamed"

#include <SFML/Graphics.hpp>

#include "abstract/statemachine.hpp"
#include "constants.hpp"
#include "views/default.hpp"

class Game {
private:
    // variables
    sf::RenderWindow window;
    sf::Clock clock;
    StateMachine sm;
    DefaultView def_view {};
    // methods
    void dispatch_events(sf::Event&, sf::Time);
    void resize_window(int, int);
    void render();
    void update(sf::Time);
    void update_fps(sf::Time, int&);

public:
    // methods
    Game();
    int run();
};

#endif // DEF_GAME
