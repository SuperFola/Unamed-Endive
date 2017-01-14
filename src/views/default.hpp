#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/character.hpp"
#include "../entities/pnj.hpp"
#include "huds/menu.hpp"
#include "../constants.hpp"
#include "../entities/pnjmanager.hpp"

class DefaultView : public View
{
private:
    // variables
    sf::View view;
    sf::RenderTexture offscreen;
    sf::RenderTexture minimap;
    sf::Sprite offsprite;
    sf::Sprite minisprite;
    Character player;
    Map level;
    PNJManager pnjmgr;
    MenuHUD menu_hud;
    // methods
    void set_view(sf::RenderWindow&);
    void unset_view(sf::RenderWindow&);

public:
    // methods
    DefaultView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    bool hasActiveHud();
    Character* getCharacter();
    PNJManager* getPNJM();
    Map* getMap();
};

#endif // DEF_DEFAULT_VIEW
