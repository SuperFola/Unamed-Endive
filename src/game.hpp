#ifndef DEF_GAME
#define DEF_GAME

#define WIN_W 640
#define WIN_H 540
#define WIN_TITLE "Unamed"

#include <SFML/Graphics.hpp>

#include "abstract/statemachine.hpp"

class Game {
private:
    // variables
    sf::RenderWindow window;
    sf::CircleShape shape;
    sf::Clock clock;
    StateMachine sm;
    // methods
    int prepare();
    void dispatch_events(sf::Event&);
    void resize_window(int, int);
    void render();

public:
    // methods
    Game();
    int run();
};

#endif // DEF_GAME
