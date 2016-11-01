#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/character.hpp"
#include "huds/menu.hpp"

class DefaultView : public View
{
private:
    // variables
    Character player;
    Map level;
    MenuHUD menu_hud;

public:
    // methods
    DefaultView(const std::string& ="assets/map/1-1441.umd");
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEFAULT_VIEW
