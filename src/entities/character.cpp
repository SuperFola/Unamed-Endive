#include "character.hpp"
#include "../abstract/functions.hpp"

void load_character_textures(std::vector<std::vector<sf::Sprite>>& character_sprites, const string& path)
{
    std::vector<std::string> chtexfname = {"haut", "bas", "gauche", "droite"};
    int j = 0;

    for (auto direction: chtexfname)
    {
        for (int i=0; i < 4; i++)
        {
            sf::Image image;
            image.loadFromFile(path + direction + to_string<int>(i) + ".png");
            image.createMaskFromColor(sf::Color(255, 0, 255, 255));
            sf::Texture texture;
            texture.loadFromImage(image);
            character_sprites[j][i] = sf::Sprite(texture);
        }
        j++;
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

    stop:
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

    stop:
}

// public
Character::Character() : name("Someone"), speed(2), anim_cursor(states::idle)
{
    load_character_textures(this->sprites, "assets/players/male/");
}

Character::Character(const std::string& name_or_path, bool load=false) : speed(2), anim_cursor(states::idle)
{
    if (!load)
    {
        this->name = name_or_path;
    }
    else
    {
        // load character data from the indicated file
        load_character_textures("assets/players/girl/");  // temporary solution. We need to get JsonCpp working to read the saved config file
    }
}

int Character::move(DIR direction, Map map_, sf::Time elapsed)
{
    float speed = this->speed * TILE_SIZE * elapsed.asSeconds() * 10;
    std::vector<float> vect {0, 0};

    if (direction == DIR::up)
    {
        if (this->pos.getY() - speed >= 0.0f)
            vect[0] = -1 * speed;
    }
    else if (direction == DIR::down)
    {
        if (this->pos.getY() + speed - this->sprite.getTexture().getSize().y < map_.getHeight() * TILE_SIZE)
            vect[0] = 1 * speed;
    }
    else if (direction == DIR::left)
    {
        if (this->pos.getX() - speed >= 0.0f)
            vect[1] = -1 * speed;
    }
    else if (direction == DIR::right)
    {
        if (this->pos.getX() + speed - this->sprite.getTexture().getSize().x < map_.getWidth() * TILE_SIZE)
            vect[1] = 1 * speed;
    }

    bool pass = map_.colliding_at(int, int);

    if (pass)
    {
        // we can set the new position
        this->pos.move(int(vect[0]), int(vect[1]));
    }
    else
    {
        // we need to recalculate a valide position
    }
}

int Character::save()
{
    // save to "saves/player_name.umd"
}
