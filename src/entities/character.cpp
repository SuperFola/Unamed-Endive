#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "character.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/texturesmanager.hpp"

// private
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
        goto stop;
    }
    else if (this->anim_cursor == MvState::walking)
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

void Character::load_character_textures()
{
    const std::vector<std::string> chtexfname = {"haut", "bas", "gauche", "droite"};

    for (const auto dir: chtexfname)
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
            this->sprites.push_back(sf::Sprite(textures.getTexture(this->path + dir + to_string<int>(i))));
        }
    }
}

// public
Character::Character() : name("Someone"), speed(2), direction(DIR::down), state(ChState::idle), anim_cursor(MvState::idle), path("assets/players/male/")
{
    this->load_character_textures();
}

Character::Character(const std::string& name, const std::string& path) : speed(2), direction(DIR::down), state(ChState::idle), anim_cursor(MvState::idle)
{
    this->name = name;
    this->path = path;

    // load character data from the indicated file
    this->load_character_textures();
}

int Character::move(DIR dir, Map map_, sf::Time elapsed)
{
    // set the new direction
    this->direction = dir;

    float speed = this->speed * TILE_SIZE * elapsed.asMilliseconds() / 10.0f;
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

    bool pass = !map_.colliding_at(int(vect[0]) / TILE_SIZE, int(vect[1]) / TILE_SIZE);

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
    sf::Vector2f _pos = this->pos.get();

    if (this->getCurrentSprite().getPosition() != _pos)
        this->getCurrentSprite().setPosition(_pos);
}
