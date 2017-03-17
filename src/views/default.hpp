#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>
#include <SFML/System/String.hpp>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/character.hpp"
#include "../entities/pnj.hpp"
#include "huds/menu.hpp"
#include "../constants.hpp"
#include "../entities/pnjmanager.hpp"
#include "../abstract/triggersmanager.hpp"
#include "../abstract/functions.hpp"
#include "../json/json.h"

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
    TriggersManager triggsmgr;
    bool display_mmap;
    // methods
    void set_view(sf::RenderWindow&);
    void unset_view(sf::RenderWindow&);

public:
    // methods
    DefaultView();
    bool load(sf::String);
      // we are overriding it but not using it
      bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    bool hasActiveHud();
    Character* getCharacter();
    PNJManager* getPNJM();
    Map* getMap();
    TriggersManager* getTriggsMgr();
    void change_display_mmap(bool);
    bool get_display_mmap();
    void draw_on_offscreen(const sf::Drawable&);
};

#endif // DEF_DEFAULT_VIEW
