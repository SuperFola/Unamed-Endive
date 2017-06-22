#include <iostream>
#include "../../debug.hpp"

#include "animatedentity.hpp"

#define PLAYER_SPEED 3

void AnimatedEntity::update_anim(sf::Time elapsed)
{
    this->elapsed_mvt_time += elapsed;

    if (this->elapsed_mvt_time.asMilliseconds() >= this->frame_time.asMilliseconds())
    {
        this->elapsed_mvt_time = sf::milliseconds(this->elapsed_mvt_time.asMilliseconds() % this->frame_time.asMilliseconds());

        if (this->state == ChState::walking)
            this->update_walk_anim();
        else if (this->state == ChState::running)
            this->update_run_anim();
    }
}

void AnimatedEntity::update_walk_anim()
{
    if (this->anim_cursor == MvState::idle)
    {
        this->anim_cursor = MvState::walking;
        goto stop;
    }
    else if (this->anim_cursor == MvState::walking)
    {
        this->anim_cursor = MvState::idle2;
        goto stop;
    }
    else if (this->anim_cursor == MvState::idle2)
    {
        this->anim_cursor = MvState::walking2;
        goto stop;
    }
    else if (this->anim_cursor == MvState::walking2)
    {
        this->anim_cursor = MvState::idle;
        goto stop;
    }

    stop:;
}

void AnimatedEntity::update_run_anim()
{
    if (this->anim_cursor == MvState::idle)
    {
        this->anim_cursor = MvState::walking;
        goto stop2;
    }
    else if (this->anim_cursor == MvState::walking)
    {
        this->anim_cursor = MvState::walking2;
        goto stop2;
    }
    else if (this->anim_cursor == MvState::walking2)
    {
        this->anim_cursor = MvState::idle;
        goto stop2;
    }

    stop2:;
}

AnimatedEntity::AnimatedEntity(int x, int y) :
    pos(x, y)
    , opos(x, y)
    , state(ChState::idle)
    , anim_cursor(MvState::idle)
    , direction(DIRECTION::down)
    , speed(PLAYER_SPEED)
    , defaultspeed(PLAYER_SPEED)
    , frame_time(sf::seconds(0.043f))
{
}

bool AnimatedEntity::load()
{
    this->_load();

    const std::vector<std::string> chtexfname = {"up", "down", "left", "right"};
    DebugLog(SH_INFO, "Loading AnimatedEntity at " << this->path);

    for (const auto& dir: chtexfname)
    {
        for (int i=0; i < 4; i++)
        {
            sf::Image image;

            if (!image.loadFromFile(this->path + dir + to_string<int>(i) + ".png"))
            {
                DebugLog(SH_ERR, "Unable to open " << this->path << dir << to_string<int>(i) << ".png");
                return false;
            }

            image.createMaskFromColor(sf::Color(255, 0, 255, 255));
            sf::Texture texture;
            texture.loadFromImage(image);

            this->textures.add(this->path + dir + to_string<int>(i), texture);
            this->sprites.push_back(sf::Sprite(this->textures.get(this->path + dir + to_string<int>(i))));
        }
    }

    // essentially for th pnj, otherwise the static ones won't be at the correct position
    sf::Vector2f _pos {
        float(int(this->pos.getX()))
        , float(int(this->pos.getY()))
    };

    this->getCurrentSprite().setPosition(_pos);

    return true;
}

bool AnimatedEntity::has_changed_case()
{
    if (int(this->opos.getX()) / TILE_SIZE != int(this->pos.getX()) / TILE_SIZE ||
         int(this->opos.getY()) / TILE_SIZE != int(this->pos.getY()) / TILE_SIZE)
             return true;
    return false;
}

int AnimatedEntity::move(DIRECTION dir, Map& map_, sf::Time elapsed)
{
    this->opos = this->pos;

    // update state
    if (this->state == ChState::idle)
        this->state = ChState::walking;  // default value, we will change it regarding the AnimatedEntity equipment in the future

    // set the new direction
    if (this->direction != dir)
        this->anim_cursor = MvState::idle;
        // the AnimatedEntity change his direction so we reset the anim cursor
        // to prevent some visual glitches
    this->direction = dir;

    // update anim
    this->not_moving_time = sf::seconds(0.0f);  // reset it
    this->update_anim(elapsed);

    float speed = ((this->speed / float(this->_size)) * TILE_SIZE * 3.0f);
    sf::Vector2u csprite_size = (this->getCurrentSprite().getTexture())->getSize();

    std::vector<float> vect {0, 0};

    if (dir == DIRECTION::up)
    {
        if (this->pos.getY() - speed >= 0.0f)
            vect[1] = -1 * speed;
    }
    else if (dir == DIRECTION::down)
    {
        if (this->pos.getY() + speed - csprite_size.y < map_.getHeight() * TILE_SIZE)
            vect[1] = 1 * speed;
    }
    else if (dir == DIRECTION::left)
    {
        if (this->pos.getX() - speed >= 0.0f)
            vect[0] = -1 * speed;
    }
    else if (dir == DIRECTION::right)
    {
        if (this->pos.getX() + speed - csprite_size.x < map_.getWidth() * TILE_SIZE)
            vect[0] = 1 * speed;
    }

    bool pass =        !map_.colliding_at(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE,       vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE      )
                        && !map_.colliding_at(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE + 2, vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE      )
                        && !map_.colliding_at(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE,       vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE + 2)
                        && !map_.colliding_at(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE + 2, vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE + 2);
    if (!pass || !this->pass_pnj(map_, vect))
    {
        return 0;
    }

    // check for tp on a new map
    int nrpos = this->chara_move(map_, vect);
    if (nrpos != -1)
    {
        // one the new rpos returned by post_colliding_test_to_check_tp(..) is not -1, our character is on a tp, let's change the map
        this->pos.set(
                      nrpos % map_.getWidth()
                      , nrpos / map_.getWidth()
                      );
    }
    else
    {
        // otherwise we can set the new position, it is not blocking and there is not any tp
        this->pos.move(int(vect[0]), int(vect[1]));
        this->chara_send_player_touch(map_);
    }

    sf::Vector2f _pos {
        float(int(this->pos.getX()))
        , float(int(this->pos.getY()))
    };

    for (int i=0; i < this->sprites.size(); i++)
        this->sprites[i].setPosition(_pos);

    return 0;
}

void AnimatedEntity::simplemove(DIRECTION dir, sf::Time elapsed)
{
    this->opos = this->pos;

    // update state
    if (this->state == ChState::idle)
        this->state = ChState::walking;  // default value, we will change it regarding the AnimatedEntity equipment in the future

    // set the new direction
    if (this->direction != dir)
        this->anim_cursor = MvState::idle;
        // the AnimatedEntity change his direction so we reset the anim cursor
        // to prevent some visual glitches
    this->direction = dir;

    // update anim
    this->not_moving_time = sf::seconds(0.0f);  // reset it
    this->update_anim(elapsed);

    float speed = ((this->speed / 32.0f) * TILE_SIZE * 3.0f);  // * (elapsed.asSeconds() * 100.0f);
    sf::Vector2u csprite_size = (this->getCurrentSprite().getTexture())->getSize();

    std::vector<float> vect {0, 0};

    if (dir == DIRECTION::up)
    {
        if (this->pos.getY() - speed >= 0.0f)
            vect[1] = -1 * speed;
    }
    else if (dir == DIRECTION::down)
    {
        if (this->pos.getY() + speed - csprite_size.y < WIN_H)
            vect[1] = 1 * speed;
    }
    else if (dir == DIRECTION::left)
    {
        if (this->pos.getX() - speed >= 0.0f)
            vect[0] = -1 * speed;
    }
    else if (dir == DIRECTION::right)
    {
        if (this->pos.getX() + speed - csprite_size.x < WIN_W)
            vect[0] = 1 * speed;
    }

     this->pos.move(int(vect[0]), int(vect[1]));

     sf::Vector2f _pos {
        float(int(this->pos.getX()))
        , float(int(this->pos.getY()))
    };

    for (int i=0; i < this->sprites.size(); i++)
        this->sprites[i].setPosition(_pos);
}

void AnimatedEntity::chara_send_player_touch(Map& map_)
{
    // not implemented for basic entities such as PNJ(npc)
}

bool AnimatedEntity::pass_pnj(Map&, std::vector<float>)
{
    // not implemented for basi entities such as PNJ(npc)
    return true;
}

int AnimatedEntity::chara_move(Map&, std::vector<float>)
{
    // not implemented for basic entities such as PNJ(npc)
    return -1;
}

sf::Sprite& AnimatedEntity::getCurrentSprite()
{
    return this->sprites[static_cast<int>(this->direction) * 4 + static_cast<int>(this->anim_cursor)];
}

void AnimatedEntity::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->state != ChState::idle)
        this->not_moving_time += elapsed;
    if (this->not_moving_time.asSeconds() > 1.0f)
        this->state = ChState::idle;
}

void AnimatedEntity::setDir(DIRECTION direc)
{
    this->direction = direc;
}

Point& AnimatedEntity::getPos()
{
    return this->pos;
}

void AnimatedEntity::setPos(int x, int y)
{
    this->pos.set(x, y);
}

bool AnimatedEntity::collide(int x, int y)
{
    if (this->pos.getX() <= x && x <= this->pos.getX() + this->_size &&
         this->pos.getY() <= y && y <= this->pos.getY() + this->_size)
            return true;
    return false;
}

void AnimatedEntity::setSpeed(int new_speed)
{
    this->speed = new_speed;
}

void AnimatedEntity::resetSpeed()
{
    this->speed = this->defaultspeed;
}

int AnimatedEntity::getSpeed()
{
    return this->speed;
}
