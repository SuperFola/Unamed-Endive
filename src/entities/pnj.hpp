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
    Point pos;
    ChState state;
    MvState anim_cursor;
    DIR direction;
    bool speaking;
    float speed;
    std::string path;
    TexturesManager textures;
    std::vector<sf::Sprite> sprites;
    sf::Sprite bckg_speak;
    sf::Font font;
    sf::Text render_text;
    sf::Time elapsed_mvt_time;
    sf::Time not_moving_time;

    void update_anim(sf::Time);
    void update_walk_anim();
    void update_run_anim();

public:
    PNJ(std::string, std::string, PNJkind, int x=32, int y=32);
    bool load();
    int move(DIR, Map, sf::Time);
    void setDisplayName(const std::string&);
    void speak();
    sf::Sprite& getCurrentSprite();
    void render(sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
    void setDir(DIR);
};

#endif // DEF_PNJ
