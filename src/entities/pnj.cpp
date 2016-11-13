#include <iostream>

#include "pnj.hpp"

PNJ::PNJ(std::string _name, std::string _text, PNJkind _kind) :
    name(_name)
    , text(_text)
    , kind(_kind)
{

}

bool PNJ::load()
{
    const std::vector<std::string> chtexfname = {"up", "down", "left", "right"};

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

    std::string path;
    if (this->kind == PNJkind::me)
        path = "assets/gui/pnj/bubble_me.png";
    else if (this->kind == PNJkind::normal)
        path = "assets/gui/pnj/bubble.png";
    else if (this->kind == PNJkind::special)
        path = "assets/gui/pnj/bubble_special.png";
    else if (this->kind == PNJ::system)
        path = "assets/gui/pnj/bubble_system";

    sf::Texture tex;
    if (!text.loadFromFile(path))
        return false;
    this->textures.addTexture("bubble", tex);
    this->bckg_speak = sf::Sprite(this->textures.getTexture("bubble"));

    if (!this->font.loadFromFile("assets/fonts/pkmn.ttf"))
        return false;

    this->render_text.setFont(this->font);
    this->render_text.setCharacterSize(24);
    this->render_text.setColor(sf::Color::Black);
    this->render_text.setString(this->text);

    return true;
}
