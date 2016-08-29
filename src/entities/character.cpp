#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "character.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/texturesmanager.hpp"

void load_character_textures(std::vector<sf::Sprite>& character_sprites, std::vector<sf::Texture>& textures, const std::string& path)
{
    std::vector<std::string> chtexfname = {"haut", "bas", "gauche", "droite"};

    for (auto direction: chtexfname)
    {
        for (int i=0; i < 4; i++)
        {
            sf::Image image;

            std::cout << "Loading " << path << direction << to_string<int>(i) << ".png" << std::endl;
            if (!image.loadFromFile(path + direction + to_string<int>(i) + ".png"))
            {
                std::cout << "Unable to open " << path << direction << to_string<int>(i) << ".png" << std::endl;
            }

            image.createMaskFromColor(sf::Color(255, 0, 255, 255));
            sf::Texture texture;
            texture.loadFromImage(image);
            textures.push_back(std::move(texture));
            character_sprites.push_back(sf::Sprite(textures[textures.size() - 1]));
        }
    }
}

// private
void Character::update_walk_anim()
{
    if (this->anim_cursor == ChState::idle)
    {
        this->anim_cursor = ChState::walking;
        goto stop;
    }
    else if (this->anim_cursor == ChState::walking)
    {
        this->anim_cursor = ChState::idle2;
        goto stop;
    }
    else if (this->anim_cursor == ChState::idle2)
    {
        this->anim_cursor = ChState::walking2;
        goto stop;
    }
    else if (this->anim_cursor == ChState::walking2)
    {
        this->anim_cursor = ChState::idle;
        goto stop;
    }

    stop:;
}

void Character::update_run_anim()
{
    if (this->anim_cursor == ChState::idle)
    {
        this->anim_cursor = ChState::walking;
        goto stop;
    }
    else if (this->anim_cursor == ChState::walking)
    {
        this->anim_cursor = ChState::walking2;
        goto stop;
    }
    else if (this->anim_cursor == ChState::walking2)
    {
        this->anim_cursor = ChState::idle;
        goto stop;
    }

    stop:;
}

// public
Character::Character() : name("Someone"), speed(2), direction(0), anim_cursor(ChState::idle)
{
    load_character_textures(this->sprites, this->textures, "assets/players/male/");
}

Character::Character(const std::string& name_or_path, bool load) : speed(2), direction(0), anim_cursor(ChState::idle)
{
    if (!load)
    {
        this->name = name_or_path;
    }
    else
    {
        // load character data from the indicated file
        load_character_textures(this->sprites, this->textures, "assets/players/girl/");  // temporary solution. We need to get JsonCpp working to read the saved config file
    }
}

int Character::move(DIR direction, Map map_, sf::Time elapsed)
{
    float speed = this->speed * TILE_SIZE * elapsed.asSeconds() * 10;
    std::vector<float> vect {0, 0};
    sf::Vector2u csprite_size = (this->getCurrentSprite().getTexture())->getSize();

    if (direction == DIR::up)
    {
        if (this->pos.getY() - speed >= 0.0f)
            vect[0] = -1 * speed;
    }
    else if (direction == DIR::down)
    {
        if (this->pos.getY() + speed - csprite_size.y < map_.getHeight() * TILE_SIZE)
            vect[0] = 1 * speed;
    }
    else if (direction == DIR::left)
    {
        if (this->pos.getX() - speed >= 0.0f)
            vect[1] = -1 * speed;
    }
    else if (direction == DIR::right)
    {
        if (this->pos.getX() + speed - csprite_size.x < map_.getWidth() * TILE_SIZE)
            vect[1] = 1 * speed;
    }

    bool pass = map_.colliding_at(0, 0);

    if (pass)
    {
        // we can set the new position
        this->pos.move(int(vect[0]), int(vect[1]));
        return 0;
    }
    else
    {
        // we need to recalculate a valide position
        return 0;
    }
    return 1;
}

int Character::save()
{
    // save to "saves/player_name.umd"
    return 0;
}

sf::Sprite Character::getCurrentSprite()
{
    return this->sprites[this->direction * 4 + static_cast<int>(this->anim_cursor)];
}

void Character::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
