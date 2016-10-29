#ifndef DEF_MENU_HUD
#define DEF_MENU_HUD

#include <vector>
#include <SFML/Graphics.hpp>

#include "hud.hpp"

class MenuHUD : public Hud
{
private:
    // variables
    std::vector<int> menus;
    int current;
    sf::Font font;
    std::vector<sf::Texture*> textures;
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Text*> texts;

    const int BACKGROUND = 0;
    const int BG_CATEGORY = 1;
    const int BG_CATEGORY_SELECTED = 2;
    const int TXT_CREA = 0;
    const int TXT_INVENT = 1;
    const int TXT_MAP = 2;
    const int TXT_SAVE = 3;
    const int TXT_DEX = 4;
    const int TXT_BACK = 5;

    // methods
    void clic(int, int);

public:
    // methods
    MenuHUD();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_MENU_HUD
