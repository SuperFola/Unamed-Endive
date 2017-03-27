#include <iostream>
#include "../../debug.hpp"

#include "animatedentity.hpp"

void AnimatedEntity::update_anim(sf::Time elapsed)
{
    this->elapsed_mvt_time += elapsed;

    if (this->elapsed_mvt_time.asMilliseconds() % 24 < 4)
    {
        if (this->state == ChState::walking)
            this->update_walk_anim();
        else if (this->state == ChState::running)
            this->update_run_anim();
    }

    // reset timer to prevent an overflow
    if (this->elapsed_mvt_time.asSeconds() > 3600.0f)
        this->elapsed_mvt_time = sf::seconds((this->elapsed_mvt_time % sf::seconds(3600.0f)).asSeconds());
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
    , state(ChState::idle)
    , anim_cursor(MvState::idle)
    , direction(DIRECTION::down)
    , speed(1.0f / 32.0f)
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

int AnimatedEntity::move(DIRECTION dir, Map& map_, sf::Time elapsed)
{
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

    float speed = (this->speed * TILE_SIZE * 4.0f);  // * (elapsed.asSeconds() * 100.0f);
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
    if (!pass)
        return 0;

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
    }

    sf::Vector2f _pos {
        float(int(this->pos.getX()))
        , float(int(this->pos.getY()))
    };

    this->getCurrentSprite().setPosition(_pos);

    return 0;
}

int AnimatedEntity::chara_move(Map&, std::vector<float>)
{
    // not implemented for basic entity such as PNJ(npc)
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
    DebugLog(SH_INFO, x << " " << y << " " << this->_size << " - " << this->pos.getX() << " " << this->pos.getY());
    if (this->pos.getX() <= x && x <= this->pos.getX() + this->_size &&
         this->pos.getY() <= y && y <= this->pos.getY() + this->_size)
            return true;
    return false;
}
