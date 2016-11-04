#ifndef DEF_INVENT_VIEW
#define DEF_INVENT_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../bag/bag.hpp"

class InventView : public View
{
private:
    // variables
    int current;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text text;
    sf::Text current_pocket_name;
    Bag* bag;

    const std::string BCKG = "backgroud";
    const std::string LARROW = "left_arrow";
    const std::string RARROW = "right_arrow";
    const std::string POCKET1 = "pocket1";
    const std::string POCKET2 = "pocket2";
    const std::string POCKET3 = "pocket3";
    const std::string POCKET4 = "pocket4";
    const std::string POCKET5 = "pocket5";

    // methods
    void draw_content(sf::RenderWindow&);

public:
    // methods
    InventView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_bag(Bag*);
};

#endif // DEF_INVENT_VIEW




