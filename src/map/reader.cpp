#include<iostream>
#include <fstream>

#include "reader.hpp"

MapReader::MapReader() :
    selected(-1)
{

}

bool MapReader::load()
{
    std::cout << "Loading data (map)" << std::endl;
    std::ifstream desc("assets/config/worldmap.json");
    desc >> this->content;
    std::cout << "Loading description (map)" << std::endl;
    std::ifstream file("assets/config/worldmap_desc.json");
    file >> this->root;

    if (!this->t_bg.loadFromFile("assets/worldmap.png"))
        return false;
    this->bg.setTexture(this->t_bg);
    this->bg.setPosition(0, 0);

    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;
    this->text.setFont(this->font);
    this->text.setColor(sf::Color::Black);
    this->text.setCharacterSize(20);
    this->text.setPosition(10, 10);

    if (!this->tileset.loadFromFile("assets/minimap.png"))
        return false;

    for (int i=0; i < this->content.size(); i++)
    {
        for (int j=0; j < this->content[i].size(); j++)
        {
            int k = 0;
            std::string m = this->content[i][j].asString();

            if (this->root["chemin"].get(m, -1) != -1)
                k = 1;
            else if (this->root["chenal"].get(m, -1) != -1)
                k = 2;
            else if (this->root["lieux"].get(m, -1) != -1)
                k = 3;
            else if (this->root["ville"].get(m, -1) != -1)
                k = 4;

            this->tiles.push_back(k);
        }
    }

    // resize vertex array
    this->vertices.setPrimitiveType(sf::Quads);
    this->vertices.resize(40 * 40 * 4);

    // fill vertex array, with one quad per tile
    for (unsigned int i = 0; i < 40; ++i)
        for (unsigned int j = 0; j < 40; ++j)
        {
            int tileNumber = this->tiles[i + j * 40];

            if (tileNumber != 0)
            {
                int tu = (tileNumber - 1) % (this->tileset.getSize().x / TILE_SIZE);
                int tv = 0;

                sf::Vertex* quad = &this->vertices[(i + j * 40) * 4];

                quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
                quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);

                quad[0].texCoords = sf::Vector2f(tu * TILE_SIZE, tv * TILE_SIZE);
                quad[1].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
                quad[2].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, (tv + 1) * TILE_SIZE);
                quad[3].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);
            }
        }

    return true;
}

void MapReader::clic(int x, int y)
{
    this->selected = (x / TILE_SIZE) + (y / TILE_SIZE) * 40;

    if (this->content[y][x].asString() != ".")
    {
        Json::Value node = this->root["descriptions"][this->content[y][x].asString()];
        std::string str = node["name"].asString() + "\n" + node["desc"].asString();
        this->text.setString(str);
    }
    else
        this->selected = -1;
}
