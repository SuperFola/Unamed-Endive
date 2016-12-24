#ifndef DEF_DEX_VIEW
#define DEF_DEX_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../objects/dex.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/creatures_loader.hpp"

class DexView : public View
{
private:
    // variables
    bool displaying_crea;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    std::vector<std::tuple<sf::Text, sf::Sprite, std::string>> dex_content;
    sf::Font font;
    sf::Text text;
    Dex* dex;
    CreaturesLoader* crealoader;
    int selected;

    const std::string BCKG = "background";
    const std::string TYPES_BTN = "types_button";
    const std::string CREA_BTN = "crea_btn";

    // methods
    void draw_content(sf::RenderWindow&);
    int index_of(const std::string&);

public:
    // methods
    DexView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_dex(Dex*);
    void add_crealoader(CreaturesLoader*);
    void load_dex_content();
};

#endif // DEF_DEX_VIEW

