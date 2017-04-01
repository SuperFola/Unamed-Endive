#ifndef DEF_SHOP_VIEW
#define DEF_SHOP_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../json/json.h"

class ShopView : public View
{
private:
    // variables
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;

public:
    ShopView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_SHOP_VIEW
