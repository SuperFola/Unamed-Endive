#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include <string>
#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "../constants.hpp"
#include "../abstract/point.hpp"

class Map;

enum class ChState {
    idle,
    walking,
    idle2,
    walking2
};

class Character {
private:
    // variables
    std::string name;
    Point pos;
    int speed;
    std::vector<std::vector<sf::Sprite>> sprites;
    ChState anim_cursor;
    // methods
    void update_walk_anim();
    void update_run_anim();

public:
    // methods
    Character();
    Character(const std::string&, bool=false);
    int move(DIR, Map, sf::Time);
    int save();
};

#endif // DEF_CHARACTER
