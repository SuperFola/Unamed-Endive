#ifndef DEF_GAME
#define DEF_GAME

#define WIN_W 640
#define WIN_H 540
#define WIN_TITLE "test fenetre"

#include <SFML/Graphics.hpp>

class Game {
private:
    // variables
    sf::RenderWindow window;

public:
    // methodes
    Game();
    int run();
};

#endif // DEF_GAME
