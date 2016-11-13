#ifndef DEF_PNJ
#define DEF_PNJ

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "../abstract/texturesmanager.hpp"

enum class PNJkind {
    normal,
    special,
    system,
    me
};

class PNJ
{
private:
    std::string name;
    std::string text;
    PNJkind kind;
    TexturesManager textures;
    std::vector<sf::Sprite> sprites;
    sf::Sprite bckg_speak;
    sf::Font font;
    sf::Text render_text;

public:
    PNJ(std::string, std::string, PNJkind);
    bool load();
    void speak();
};

#endif // DEF_PNJ
