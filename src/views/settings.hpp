#ifndef DEF_SETTINGS_VIEW
#define DEF_SETTINGS_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../json/json.h"

class SettingsView : public View
{
private:
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;

public:
    SettingsView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_SETTINGS_VIEW
