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
#include "../abstract/functions.hpp"

enum class Sex {
    Female,
    Male
};

class Character {
private:
    // variables
    std::string name;
    Point pos;
    float speed;
    DIRECTION direction;
    TexturesManager textures;
    std::vector<sf::Sprite> sprites;
    ChState state;
    MvState anim_cursor;
    Sex sex;
    std::string path;
    Bag bag;
    sf::Time elapsed_mvt_time;
    sf::Time not_moving_time;
    // methods
    void update_anim(sf::Time);
    void update_walk_anim();
    void update_run_anim();
    void load_character_textures();

public:
    // methods
    Character(Sex sex_=Sex::Male);
    int move(DIRECTION, Map, sf::Time);
    int save();
    sf::Sprite& getCurrentSprite();
    void update(sf::RenderWindow&, sf::Time);
    void setName(const std::string);
    bool load();
    Point& getPos();
    Bag* getBag();
};

#endif // DEF_CHARACTER
