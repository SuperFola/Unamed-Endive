#ifndef DEF_DEX_VIEW
#define DEF_DEX_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../objects/dex.hpp"

class DexView : public View
{
private:
    // variables
    bool displaying_crea;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text text;
    Dex* dex;

    const std::string BCKG = "background";
    const std::string TYPES_BTN = "types_button";
    const std::string CREA_BTN = "crea_btn";

    // methods
    void draw_content(sf::RenderWindow&);

public:
    // methods
    DexView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_dex(Dex*);
};

#endif // DEF_DEX_VIEW

