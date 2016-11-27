#ifndef DEF_ANIMATED_ENTITY
#define DEF_ANIMATED_ENTITY

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

class AnimatedEntity
{
protected:
    Point pos;
    ChState state;
    MvState anim_cursor;
    DIRECTION direction;
    float speed;
    std::string path;
    TexturesManager textures;
    std::vector<sf::Sprite> sprites;
    sf::Time elapsed_mvt_time;
    sf::Time not_moving_time;

    void update_anim(sf::Time);
    void update_walk_anim();
    void update_run_anim();

public:
    AnimatedEntity(int x=32, int y=32);
    bool load();
    virtual void _load() = 0;
    int move(DIRECTION, Map, sf::Time);
    sf::Sprite& getCurrentSprite();
    void update(sf::RenderWindow&, sf::Time);
    void setDir(DIRECTION);
    Point& getPos();
};

#endif // DEF_ANIMATED_ENTITY
