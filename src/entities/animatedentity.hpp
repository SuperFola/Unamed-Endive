#ifndef DEF_ANIMATED_ENTITY
#define DEF_ANIMATED_ENTITY

#include <algorithm>
#include <vector>
#include <string>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "../map/map.hpp"
#include "../abstract/container.hpp"
#include "../abstract/point.hpp"
#include "../abstract/functions.hpp"
#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN

class AnimatedEntity
{
protected:
    const int _size = TILE_SIZE * 2;
    Point pos;
    Point opos;
    ChState state;
    MvState anim_cursor;
    DIRECTION direction;
    int speed;
    int defaultspeed;
    std::string path;
    Container<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    sf::Time elapsed_mvt_time;
    sf::Time not_moving_time;
    sf::Time frame_time;

    void update_anim(sf::Time);
    void update_walk_anim();
    void update_run_anim();
    bool has_changed_case();

public:
    AnimatedEntity(int x=32, int y=32);
    bool load();
    virtual void _load() = 0;
    int move(DIRECTION, Map&, sf::Time);
    void simplemove(DIRECTION, sf::Time);
    virtual int chara_move(Map&, std::vector<float>);
    virtual void chara_send_player_touch(Map&);
    virtual bool pass_pnj(Map&, std::vector<float>);
    sf::Sprite& getCurrentSprite();
    void update(sf::RenderWindow&, sf::Time);
    void setDir(DIRECTION);
    Point& getPos();
    void setPos(int, int);
    bool collide(int, int);
    void setSpeed(int);
    void resetSpeed();
    int getSpeed();
};

#endif // DEF_ANIMATED_ENTITY
