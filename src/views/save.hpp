#ifndef DEF_SAVE_VIEW
#define DEF_SAVE_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../json/json.h"
#include "../abstract/functions.hpp"
#include "../entities/pnj.hpp"
#include "default.hpp"

class SaveView : public View
{
private:
    // variables
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    DefaultView* defv;
    sf::Font font;
    sf::Text text;
    PNJ npc;
    sf::Time last;
    int npcdoing;
    bool npcway;
    int _c;

public:
    // methods
    SaveView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void setdefv(DefaultView*);
    void save();
};

#endif // DEF_SAVE_VIEW


