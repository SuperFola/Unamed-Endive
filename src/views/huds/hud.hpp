#ifndef DEF_HUD
#define DEF_HUD

#include <SFML/Graphics.hpp>

class Hud {
private:
    int id;
    bool is_triggered;

public:
    Hud(int);
    virtual bool load() = 0;
    virtual void render(sf::RenderWindow&) = 0;
    virtual int process_event(sf::Event&, sf::Time) = 0;
    virtual void update(sf::RenderWindow&, sf::Time) = 0;
    void setTrigger(bool);
    bool isTriggered();
};

#endif // DEF_HUD
