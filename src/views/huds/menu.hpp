#ifndef DEF_MENU_HUD
#define DEF_MENU_HUD

#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "hud.hpp"
#include "../../abstract/texturesmanager.hpp"

class MenuHUD : public Hud
{
private:
    // variables
    std::vector<int> menus;
    int current;
    sf::Font font;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    std::map<std::string, sf::Text> texts;

    const std::string BACKGROUND = "background";
    const std::string BG_CATEGORY = "category";
    const std::string BG_CATEGORY_SELECTED = "category selected";
    const std::string TXT_CREA = "creatures";
    const std::string TXT_INVENT = "inventory";
    const std::string TXT_MAP = "map";
    const std::string TXT_SAVE = "save";
    const std::string TXT_DEX = "dex";
    const std::string TXT_BACK = "back";

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
