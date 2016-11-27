#include <iostream>

#include "pnj.hpp"

namespace Characters
{
    std::vector<std::string> specials = {
        "clemence",
        "darth",
        "elpadrino",
        "euloo",
        "folaefolc",
        "minus",
        "tress",
        "vader"
    };

    bool is_special(const std::string& name)
    {
        if (std::find(specials.begin(), specials.end(), name) != specials.end())
            return true;
        return false;
    }
}

PNJ::PNJ(std::string _name, std::string _text, PNJkind _kind, int x, int y) :
    AnimatedEntity(x, y)
    , name(_name)
    , text(_text)
    , kind(_kind)
    , speaking(false)
{
    if (Characters::is_special(this->name))
        this->path = "assets/pnj/.specials/" + this->name + "/";
    else
        this->path = "assets/pnj/" + this->name + "/";
}

void PNJ::_load()
{
    std::string p;
    if (this->kind == PNJkind::me)
        p = "assets/gui/pnj/bubble_me.png";
    else if (this->kind == PNJkind::normal)
        p = "assets/gui/pnj/bubble.png";
    else if (this->kind == PNJkind::special)
        p = "assets/gui/pnj/bubble_special.png";
    else if (this->kind == PNJkind::system)
        p = "assets/gui/pnj/bubble_system";

    sf::Texture tex;
    tex.loadFromFile(p);
    this->textures.addTexture("bubble", tex);
    this->bckg_speak = sf::Sprite(this->textures.getTexture("bubble"));
    this->bckg_speak.setPosition(WIN_W / 2 - 310, WIN_H - 100 - 10);

    this->font.loadFromFile("assets/fonts/pkmnemn.ttf");

    this->render_text.setFont(this->font);
    this->render_text.setCharacterSize(24);
    this->render_text.setColor(sf::Color::Black);
    this->render_text.setString(this->name + ": " + this->text);
    int w = this->render_text.getGlobalBounds().width;
    int h = this->render_text.getGlobalBounds().height;
    this->render_text.setPosition(310 - w / 2 + this->bckg_speak.getPosition().x, 50 - h / 2 + this->bckg_speak.getPosition().y);
}

void PNJ::setDisplayName(const std::string& _name)
{
    this->name = _name;
}

void PNJ::speak()
{
    this->speaking = !this->speaking;
}

void PNJ::render(sf::RenderWindow& window)
{
    if (this->speaking)
    {
        window.draw(this->bckg_speak);
        window.draw(this->render_text);
    }
}
