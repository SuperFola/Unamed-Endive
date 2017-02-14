#ifndef DEF_CREA_VIEW
#define DEF_CREA_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../abstract/equip.hpp"

class CreaView : public View
{
private:
    // variables
    bool displaying_crea;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text text;
    Equip* equip;

    const std::string BCKG = "background";
    const std::string BCKG_PC = "background_pc";
    const std::string BTN_PC = "btn_pc";
    const std::string BTN_CREA = "btn_crea";

    // methods
    void draw_content(sf::RenderWindow&);

public:
    // methods
    CreaView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_equip(Equip*);
};

#endif // DEF_CREA_VIEW



