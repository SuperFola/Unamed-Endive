#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/character.hpp"
#include "../entities/pnj.hpp"
#include "huds/menu.hpp"
#include "../constants.hpp"

class DefaultView : public View
{
private:
    // variables
    Character player;
    PNJ pnj;
    Map level;
    MenuHUD menu_hud;

public:
    // methods
    DefaultView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    bool hasActiveHud();
    Character* getCharacter();
};

#endif // DEF_DEFAULT_VIEW
