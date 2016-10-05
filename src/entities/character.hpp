#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include <string>
#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "../constants.hpp"
#include "../abstract/point.hpp"
#include "../map/map.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../bag/bag.hpp"

enum class MvState {
    idle,
    walking,
    idle2,
    walking2
};

enum class ChState {
    idle,
    walking,
    running
};

class Character {
private:
    // variables
    std::string name;
    Point pos;
    int speed;
    DIR direction;
    TexturesManager textures;
    std::vector<sf::Sprite> sprites;
    ChState state;
    MvState anim_cursor;
    std::string path;
    Bag bag;
    // methods
    void update_walk_anim();
    void update_run_anim();
    void load_character_textures();

public:
    // methods
    Character();
    Character(const std::string&, const std::string&);
    int move(DIR, Map, sf::Time);
    int save();
    sf::Sprite& getCurrentSprite();
    void update(sf::RenderWindow&, sf::Time);
};

#endif // DEF_CHARACTER
