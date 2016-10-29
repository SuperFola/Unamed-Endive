#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "character.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/texturesmanager.hpp"

// private
void Character::update_anim(sf::Time elapsed)
{
    this->elapsed_mvt_time += elapsed;

    if (this->elapsed_mvt_time.asMilliseconds() % 24 < 3)
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

void Character::update_walk_anim()
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

void Character::update_run_anim()
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

void Character::load_character_textures()
{
    const std::vector<std::string> chtexfname = {"haut", "bas", "gauche", "droite"};

    for (const auto& dir: chtexfname)
    {
        for (int i=0; i < 4; i++)
        {
            sf::Image image;

            std::cout << "Loading " << this->path << dir << to_string<int>(i) << ".png" << std::endl;
            if (!image.loadFromFile(path + dir + to_string<int>(i) + ".png"))
            {
                std::cout << "Unable to open " << this->path << dir << to_string<int>(i) << ".png" << std::endl;
            }

            image.createMaskFromColor(sf::Color(255, 0, 255, 255));
            sf::Texture texture;
            texture.loadFromImage(image);

            this->textures.addTexture(this->path + dir + to_string<int>(i), texture);
            this->sprites.push_back(sf::Sprite(this->textures.getTexture(this->path + dir + to_string<int>(i))));
        }
    }
}

// public
Character::Character() :
    name("Someone")
    , pos(16, 16)
    , speed(1.0f / 32.0f)
    , direction(DIR::down)
    , state(ChState::idle)
    , anim_cursor(MvState::idle)
    , path("assets/players/male/")
    , bag()
{
    std::cout << "first chara loader" << std::endl;
    this->load_character_textures();
}

Character::Character(const std::string& name_, const std::string& path) :
    name(name_)
    , pos(16, 16)
    , speed(1.0f / 32.0f)
    , direction(DIR::down)
    , state(ChState::idle)
    , anim_cursor(MvState::idle)
    , bag()
{
    std::cout << "second chara loader" << std::endl;

    this->path = path;

    // load character data from the indicated file
    this->load_character_textures();
}

int Character::move(DIR dir, Map map_, sf::Time elapsed)
{
    // update state
    if (this->state == ChState::idle)
        this->state = ChState::walking;  // default value, we will change it regarding the player equipment in the future

    // set the new direction
    if (this->direction != dir)
        this->anim_cursor = MvState::idle;
        // the player change his direction so we reset the anim cursor
        // to prevent some visual glitches
    this->direction = dir;

    // update anim
    this->not_moving_time = sf::seconds(0.0f);  // reset it
    this->update_anim(elapsed);

    float speed = this->speed * TILE_SIZE ;//* (elapsed.asMilliseconds() / 100.0f);
    std::vector<float> vect {0, 0};
    sf::Vector2u csprite_size = (this->getCurrentSprite().getTexture())->getSize();

    if (dir == DIR::up)
    {
        if (this->pos.getY() - speed >= 0.0f)
            vect[1] = -1 * speed;
    }
    else if (dir == DIR::down)
    {
        if (this->pos.getY() + speed - csprite_size.y < map_.getHeight() * TILE_SIZE)
            vect[1] = 1 * speed;
    }
    else if (dir == DIR::left)
    {
        if (this->pos.getX() - speed >= 0.0f)
            vect[0] = -1 * speed;
    }
    else if (dir == DIR::right)
    {
        if (this->pos.getX() + speed - csprite_size.x < map_.getWidth() * TILE_SIZE)
            vect[0] = 1 * speed;
    }

    bool pass = !map_.colliding_at(
                                   int(vect[0]) / TILE_SIZE + this->pos.getX() / TILE_SIZE
                                   , int(vect[1]) / TILE_SIZE + this->pos.getY() / TILE_SIZE
                                   );

    if (pass)
    {
        // we can set the new position
        this->pos.move(vect[0], vect[1]);
        return 0;
    }
    // we need to recalculate a valid position
    std::cout << "need to recalculate a valid position" << std::endl;
    return 0;
}

int Character::save()
{
    // save to "saves/player_name.umd"
    return 0;
}

sf::Sprite& Character::getCurrentSprite()
{
    return this->sprites[static_cast<int>(this->direction) * 4 + static_cast<int>(this->anim_cursor)];
}


void Character::update(sf::RenderWindow& window, sf::Time elapsed)
{
    sf::Vector2f _pos {
        float(int(this->pos.getX()))
        , float(int(this->pos.getY()))
    };

    if (this->getCurrentSprite().getPosition() != _pos)
        this->getCurrentSprite().setPosition(_pos);
    this->rectangle.setPosition(sf::Vector2f(int(this->pos.getX()) / TILE_SIZE * TILE_SIZE, int(this->pos.getY()) / TILE_SIZE * TILE_SIZE));

    if (this->state != ChState::idle)
        this->not_moving_time += elapsed;
    if (this->not_moving_time.asSeconds() > 1.0f)
        this->state = ChState::idle;
}
