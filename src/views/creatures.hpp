#ifndef DEF_CREA_VIEW
#define DEF_CREA_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"

class CreaView : public View
{
private:
    // variables
    bool displaying_crea;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text text;

    const std::string BCKG = "background";
    const std::string BTN_PC = "btn_pc";
    const std::string BTN_TO_PC = "btn_to_pc";
    const std::string BTN_CREA = "btn_crea";
    const std::string BTN_TO_CREA = "btn_to_crea";

    // methods
    void draw_content(sf::RenderWindow&);

public:
    // methods
    CreaView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_CREA_VIEW



