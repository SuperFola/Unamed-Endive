#ifndef DEF_PNJ
#define DEF_PNJ

#include <algorithm>
#include <vector>
#include <string>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "../map/map.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../abstract/point.hpp"
#include "../abstract/functions.hpp"
#include "../constants.hpp"
#include "animatedentity.hpp"

#include <algorithm>

enum class PNJkind {
    normal,
    special,
    system,
    me
};

class PNJ : public AnimatedEntity
{
private:
    std::string name;
    std::string text;
    PNJkind kind;
    bool speaking;
    sf::Sprite bckg_speak;
    sf::Font font;
    sf::Text render_text;

public:
    PNJ(std::string, std::string, PNJkind, int x=32, int y=32);
    void _load() override;
    void setDisplayName(const std::string&);
    void speak();
    void render(sf::RenderWindow&);
};

#endif // DEF_PNJ
