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

    const std::string BG_CAT_CREA = "categorie crea";
    const std::string BG_CAT_SEL_CREA = "categorie selected crea";
    const std::string BG_CAT_DEX = "categorie dex";
    const std::string BG_CAT_SEL_DEX = "categorie selected dex";
    const std::string BG_CAT_INVENTORY = "categorie inventory";
    const std::string BG_CAT_SEL_INVENTORY = "categorie selected inventory";
    const std::string BG_CAT_MAP = "categorie map";
    const std::string BG_CAT_SEL_MAP = "categorie selected map";
    const std::string BG_CAT_BACK = "categorie back";
    const std::string BG_CAT_SEL_BACK = "categorie selected back";
    const std::string BG_CAT_SAVE = "categorie save";
    const std::string BG_CAT_SEL_SAVE = "categorie selected save";
    const std::string TXT_CREA = "creatures";
    const std::string TXT_INVENT = "inventory";
    const std::string TXT_MAP = "map";
    const std::string TXT_SAVE = "save";
    const std::string TXT_DEX = "dex";
    const std::string TXT_BACK = "back";

    // methods
    void clic(int, int);
    int id_to_vid(int);

public:
    // methods
    MenuHUD();
    bool load() override;
    void render(sf::RenderTexture&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_MENU_HUD
