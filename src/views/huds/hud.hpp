#ifndef DEF_HUD
#define DEF_HUD

#include <SFML/Graphics.hpp>

class Hud {
private:
    int id;

public:
    Hud(int);
    virtual bool load() = 0;
    virtual void render(sf::RenderWindow&) = 0;
    virtual int process_event(sf::Event&, sf::Time) = 0;
    virtual void update(sf::RenderWindow&, sf::Time) = 0;
};

#endif // DEF_HUD
